/*
 * preload.c - Minecraft Pi Dynamic Patches.
 * 
 * Copyright 2020 Alvarito050506 <donfrutosgomez@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#define _GNU_SOURCE /* Required for RTLD_NEXT */

/* Lots of standard includes and libraries, for types and function prototypes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <errno.h>
#include <fcntl.h>
#include <X11/Xlib.h>

#include <mcpi.h>
#include <proto.h>
#include <types.h>

#ifndef MOD_PI
#define MP_MAJOR 0
#define MP_MINOR 4
#define MP_PATCH 0
#define MOD_PI "v0.4.0"

#endif /* MOD_PI */

/* Please ignore the variable names. */
int old_game_mode = 1;
int reading = 1;
int connected = 1;
char t_buff[64];
char mp_buff[512];
int api_fd = 0;
int fail = 1;
int fishing = 1;
int j = 0;
int i = 0;
FILE* username_file;

modpi_data_t modpi_data = {
	NULL,
	NULL,
	{0x00},
	{0x00},
	{0x00},
	NULL,
	{0.0, 0.0, 0.0, 0.0},
	LOCAL
};

/* [Work In Progress] Dinamycally change the game mode. */
FILE* fopen(const char* filename, const char* mode)
{
	FILE* level_dat_file;
	int addr_0 = 0x19e0c;
	int addr_1 = 0x1a0d8;
	int game_mode = 1;
	char level_dat_name[512];
	char world_len;

	if (strcmp(basename((char*)filename), "player.dat") == 0)
	{
		sprintf(level_dat_name, "%s/level.dat", dirname((char*)filename));
		level_dat_file = fopen(level_dat_name, "rb");
		fseek(level_dat_file, 0x16, SEEK_SET);
		fread(&game_mode, 1, 1, level_dat_file);

		fseek(level_dat_file, 0x3b, SEEK_SET);
		fread(&world_len, 1, 1, level_dat_file);
		fgetc(level_dat_file);
		if (modpi_data.world_name != NULL)
		{
			free(modpi_data.world_name);
		}
		modpi_data.world_name = malloc(world_len + 1);
		fread(modpi_data.world_name, world_len, 1, level_dat_file);
		modpi_data.world_name[world_len + 1] = 0x00;
		snprintf(modpi_data.game_mode, 2, "%i", game_mode);
		modpi_data.game_mode[1] = 0x00;

		/* To be optimized. */
		unprotect(addr_0);
		unprotect(addr_1);

		/* Don't replace this with loops or memory/string functions, it will not work (for some reason, all of we know that C is a bit... magic). */
		if (old_game_mode != game_mode && game_mode == 0)
		{
			((char*)addr_0)[0] = '\x60';
			((char*)addr_0)[1] = '\x2f';
			((char*)addr_0)[2] = '\x10';
			((char*)addr_0)[3] = '\x00';
			((char*)addr_1)[0] = '\x60';
			((char*)addr_1)[1] = '\x2f';
			((char*)addr_1)[2] = '\x10';
			((char*)addr_1)[3] = '\x00';
		} else if (old_game_mode != game_mode && game_mode == 1)
		{
			((char*)addr_0)[0] = '\x00';
			((char*)addr_0)[1] = '\x2d';
			((char*)addr_0)[2] = '\x10';
			((char*)addr_0)[3] = '\x00';
			((char*)addr_1)[0] = '\x00';
			((char*)addr_1)[1] = '\x2d';
			((char*)addr_1)[2] = '\x10';
			((char*)addr_1)[3] = '\x00';
		}
		old_game_mode = (int)game_mode;
		fclose(level_dat_file);
	} else if (strcmp(basename((char*)filename), "entities.dat") == 0 && strcmp(basename((char*)mode), "rb") == 0)
	{
		api_kill();
	}
	return old_fopen(filename, mode);
}

/* Fancy stuff: Change the window name. */
void SDL_WM_SetCaption(const char* title, const char* icon)
{
	return old_SDL_WM_SetCaption("Minecraft - Pi Edition - Patched", "/usr/share/pixmaps/minecraft-pi.png");
}

/* "Smooth" shading. */
void glShadeModel(GLenum mode)
{
	if (connected == 1)
	{
		api_kill();
		api_setup();
	}
	return old_glShadeModel(GL_SMOOTH);
}

/* Fancy stuff: Custom fog color. */
void glFogfv(GLenum pname, const GLfloat* params)
{
	if (pname == GL_FOG_COLOR && modpi_data.ambient[3] != 0.0)
	{
		return old_glFogfv(GL_FOG_COLOR, modpi_data.ambient);
	} else
	{
		return old_glFogfv(pname, params);
	}
}

/* Extends port search (19132-19139) and sets the username. */
ssize_t sendto(int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen)
{
	struct sockaddr_in* addr = (struct sockaddr_in*)dest_addr;
	int i = 19136;
	char* char_buff = ((char*)buf);
	char new_packet[128];

	if (addr->sin_addr.s_addr == -1 && ntohs(addr->sin_port) == 19135)
	{
		while (i < 19139)
		{
			addr->sin_port = htons(i);
			if (old_sendto(sockfd, buf, len, flags, dest_addr, addrlen) < 0)
			{
				printf("sendto failed with exit errno %i\n", errno);
			}
			i++;
		}
		addr->sin_port = htons(19135);
	}
	if (char_buff[0] >= 0x80 && char_buff[10] == 0x82)
	{
		inet_ntop(AF_INET, &addr->sin_addr, modpi_data.server_addr, INET_ADDRSTRLEN);
		modpi_data.server_port[0] = 0x00;
		snprintf(modpi_data.server_port, 8, "%i", ntohs(addr->sin_port));
		memcpy(new_packet, buf, len);
		i = 0;
		while (i < 7)
		{
			new_packet[13 + i] = modpi_data.player_name[i];
			i++;
		}
		modpi_data.acting_as = CLIENT;
		return old_sendto(sockfd, new_packet, len, flags, dest_addr, addrlen);
	} else if (char_buff[0] == 0x1c)
	{
		memcpy(new_packet, buf, len);
		i = 0;
		while (i < 7)
		{
			new_packet[46 + i] = modpi_data.player_name[i];
			i++;
		}
		return old_sendto(sockfd, new_packet, len, flags, dest_addr, addrlen);
	} else if (char_buff[0] >= 0x80 && char_buff[14] == 0x15)
	{
		modpi_data.acting_as = LOCAL;
	}
	return old_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

/* UDP receive from. */
ssize_t recvfrom(int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen)
{
	struct sockaddr_in* addr;
	char* char_buff = ((char*)buf);
	char* cmd;
	char char_addr[INET_ADDRSTRLEN + 1];
	int rt = 0;

	rt = old_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
	addr = (struct sockaddr_in*)src_addr;
	inet_ntop(AF_INET, &addr->sin_addr, char_addr, INET_ADDRSTRLEN);
	if (char_buff[0] >= 0x80 && char_buff[7] == 0xb8)
	{
		if (modpi_data.acting_as == CLIENT && strcmp(modpi_data.server_addr, char_addr) == 0)
		{
			cmd = malloc(strlen(char_buff + 8) + 1);
			strcpy(cmd, char_buff + 8);
			write(api_fd, cmd, strlen(char_buff + 8));
			write(api_fd, "\n", 1);
			free(cmd);
		}
	} else if (char_buff[0] >= 0x80 && char_buff[10] == 0x82)
	{
		modpi_data.acting_as = SERVER;
	}
	return rt;
}

/* [Work In Progress] T-Chat. Stores the message in `t_buff` and then sends it via the API. */
int SDL_PollEvent(SDL_Event* event)
{
	int rt = old_SDL_PollEvent(event);

	/*
	 * MCPI uses custom events, and the Mojang folks decided that storing the keycode in the (optional) window ID was a good idea.
	 * EDIT: This may be because MCPI uses SDL 1.2, and I'm developing according to the SDL 2 structs/types.
	 * 
	 */
	short key_code = (short)event->user.windowID;
	char chr = (char)key_code;

	if (event->type == 65538)
	{
		/* 
		 * 0x0d == CR/LF?
		 * 0x1b == Esc.
		 * 0x00 == Ignore, other "special" keys.
		 * 
		 */
		if (reading == 0 && chr >= 0x20 && key_code < 0x80 && i < 64)
		{
			/* Commands! */
			if (event->user.code == 1 && chr == '7')
			{
				t_buff[j] = '/';
			} else
			{
				t_buff[j] = chr;
			}
			event->user.windowID = 0x00; /* Say "ignore" to MCPI. */
			j++;
		} else if (reading == 1 && chr == 't')
		{
			reading = 0;
			event->user.windowID = 0x00; /* Say "ignore" to MCPI. */
		} else if (reading == 0 && (chr == 0x0d || chr == 0x1b))
		{
			if (chr == 0x0d)
			{
				write(api_fd, "chat.post(", 10);
				write(api_fd, t_buff, strlen(t_buff));
				write(api_fd, ")\n", 2);
			}
			event->user.windowID = 0x00; /* Say "ignore" to MCPI. */
			reading = 1;
			t_buff[j] = 0x00;
			j = 0;
			memset(t_buff, 0x00, 63); /* Clear the buffer. */
		}
	}
	return rt;
}

/* Fancy stuff: Custom clear color. */
void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	if (modpi_data.ambient[3] != 0.0)
	{
		return old_glClearColor(modpi_data.ambient[0], modpi_data.ambient[1], modpi_data.ambient[2], modpi_data.ambient[3]);
	} else
	{
		return old_glClearColor(red, green, blue, alpha);
	}
	return;
}

/* TCP receive. */
ssize_t recv(int sockfd, void* buff, size_t len, int flags)
{
	ssize_t sz;
	char mp_char;
	char act_mode[3];
	mcpi_err_t err;
	mcpi_command_t command;

	sz = old_recv(sockfd, buff, len, flags);
	mp_char = ((char*)buff)[0];
	if (mp_char == '\n')
	{
		fishing = 1;
		fail = 0;
		i = 0;
	} else if (mp_char == 'm')
	{
		fishing = 0;
	}
	if ((i == 1 && mp_char != 'p') || (i == 2 && mp_char != '.'))
	{
		fishing = 1;
		fail = 0;
		i = 0;
	}
	if (fishing == 0)
	{
		mp_buff[i] = mp_char;
		if (mp_buff[i] == ')')
		{
			command = parse_command(mp_buff, &err);
			fail = 1;

			if (strcmp(command.name, "getWorldName") == 0 && modpi_data.world_name != NULL)
			{
				send_res(sockfd, modpi_data.world_name);
			} else if (strcmp(command.name, "getGameMode") == 0 && modpi_data.game_mode[0] != 0x00)
			{
				send_res(sockfd, modpi_data.game_mode);
			} else if (strcmp(command.name, "getPlayerName") == 0 && modpi_data.player_name != NULL)
			{
				send_res(sockfd, modpi_data.player_name);
			} else if (strcmp(command.name, "setPlayerName") == 0 && modpi_data.player_name != NULL && command.argc > 0)
			{
				memmove(modpi_data.player_name, command.args[0], 7);
				strncpy((char*)0x105263, modpi_data.player_name, 7);
			} else if (strcmp(command.name, "getServerAddr") == 0 && modpi_data.server_addr[0] != 0x00)
			{
				send_res(sockfd, modpi_data.server_addr);
			} else if (strcmp(command.name, "getServerPort") == 0 && modpi_data.server_port[0] != 0x00)
			{
				send_res(sockfd, modpi_data.server_port);
			} else if (strcmp(command.name, "setAmbientColor") == 0 && command.argc >= 4)
			{
				modpi_data.ambient[0] = (float)atoi(command.args[0]) / 255.0f;
				modpi_data.ambient[1] = (float)atoi(command.args[1]) / 255.0f;
				modpi_data.ambient[2] = (float)atoi(command.args[2]) / 255.0f;
				modpi_data.ambient[3] = (float)atoi(command.args[3]) / 255.0f;
			} else if (strcmp(command.name, "getActMode") == 0)
			{
				snprintf(act_mode, 8, "%i", modpi_data.acting_as);
				send_res(sockfd, act_mode);
			} else
			{
				fail = 0;
				send_res(sockfd, "Fail");
			}
		}
		i++;
	}
	return sz;
}

/* TCP send. */
ssize_t send(int sockfd, const void* buff, size_t len, int flags)
{
	if (fail == 1)
	{
		fail = 0;
		return (ssize_t)len;
	} else
	{
		return old_send(sockfd, buff, len, flags);
	}
}

/* Unprotects a memory area. */
int unprotect(unsigned int addr)
{
	mprotect((void*)(addr - (addr % 4096)), 4096, PROT_READ | PROT_WRITE | PROT_EXEC);
	return 0;
}

/* Setup API connection. */
int api_setup()
{
	int rt = 0;
	struct sockaddr_in api_addr;

	api_addr.sin_family = AF_INET;
	api_addr.sin_port = htons(4711);
	rt = inet_pton(AF_INET, "127.0.0.1", &api_addr.sin_addr);
	api_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (api_fd < -1)
	{
		connected = 1;
		return -1;
	}
	rt = connect(api_fd, (struct sockaddr*)&api_addr, sizeof(api_addr));
	if (rt < 0)
	{
		connected = 1;
		return -1;
	}
	connected = 0;
	return 0;
}

/* Kill API connection. */
int api_kill()
{
	if (connected == 0)
	{
		shutdown(api_fd, SHUT_RDWR);
		close(api_fd);
		connected = 1;
	}
	return 0;
}

/* Send custom response to the API client. */
ssize_t send_res(int fd, char* res)
{
	char new_res[512];
	int len = 0;

	strncpy(new_res, res, strlen(res));
	len = strlen(res);
	new_res[len] = '\n';
	new_res[len + 1] = 0x00;
	len = strlen(new_res);
	return old_send(fd, new_res, len, 0);
}

/* Constructor, gets called before MCPI itself. */
void __attribute__((constructor)) init()
{
	char* config_path;
	char* username_path;
	char tmp[7];
	int sz;

	old_fopen = dlsym(RTLD_NEXT, "fopen");
	old_sendto = dlsym(RTLD_NEXT, "sendto");
	old_recvfrom = dlsym(RTLD_NEXT, "recvfrom");
	old_send = dlsym(RTLD_NEXT, "send");
	old_recv = dlsym(RTLD_NEXT, "recv");

	old_glShadeModel = dlsym(RTLD_NEXT, "glShadeModel");
	old_glFogfv = dlsym(RTLD_NEXT, "glFogfv");
	old_glClearColor = dlsym(RTLD_NEXT, "glClearColor");
	old_glLightfv = dlsym(RTLD_NEXT, "glLightfv");

	old_SDL_WM_SetCaption = dlsym(RTLD_NEXT, "SDL_WM_SetCaption");
	old_SDL_PollEvent = dlsym(RTLD_NEXT, "SDL_PollEvent");

	old_XCreateWindow = dlsym(RTLD_NEXT, "XCreateWindow");

	modpi_data.player_name = (char*)0x1028ca;
	unprotect(0x1028ca);
	unprotect(0x105263);
	config_path = malloc(strlen(getenv("HOME")) + 9);
	strcpy(config_path, getenv("HOME"));
	strcat(config_path, "/.mcpil/");
	mkdir(config_path);
	username_path = malloc(strlen(config_path) + 13);
	strcpy(username_path, config_path);
	strcat(username_path, "username.txt");
	username_file = fopen(username_path, "r+");
	if (username_file == NULL)
	{
		username_file = fopen(username_path, "w+");
	}
	fseek(username_file, 0x00, SEEK_SET);
	
	sz = fread(tmp, 1, 7, username_file);
	if (sz > 0)
	{
		strcpy(modpi_data.player_name, tmp);
		modpi_data.player_name[sz] = 0x00;
	}
	strcpy((char*)0x105263, modpi_data.player_name);

	free(config_path);
	free(username_path);
	return;
}

/* Destructor, gets called when the library is unloaded from memory, and closes the API connection. */
void __attribute__((destructor)) destroy()
{
	api_kill();
	if (modpi_data.world_name != NULL)
	{
		free(modpi_data.world_name);
	}
	fseek(username_file, 0x00, SEEK_SET);
	fwrite(modpi_data.player_name, 1, strlen(modpi_data.player_name), username_file);
	fclose(username_file);
	return;
}
