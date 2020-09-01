/*
 * proto.h
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

#ifndef PROTO_H
#define PROTO_H
#include <stdio.h>
#include <sys/types.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <X11/Xlib.h>

/* Standard/POSIX. */
static FILE* (*old_fopen)(const char* pathname, const char* mode) = NULL;
ssize_t (*old_sendto)(int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen) = NULL;
ssize_t (*old_recvfrom)(int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen) = NULL;
ssize_t (*old_recv)(int sockfd, void* buf, size_t len, int flags) = NULL;
ssize_t (*old_send)(int sockfd, const void* buf, size_t len, int flags) = NULL;

/* OpenGL/EGL. */
void (*old_glShadeModel)(GLenum mode) = NULL;
void (*old_glFogfv)(GLenum pname, const GLfloat* params) = NULL;
void (*old_glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = NULL;
void (*old_glLightfv)(GLenum light, GLenum pname, const GLfloat* params) = NULL;

/* SDL. */
int (*old_SDL_PollEvent)(SDL_Event* event) = NULL;
void (*old_SDL_WM_SetCaption)(const char* title, const char* icon) = NULL;

/* X11/Xlib. */
Window (*old_XCreateWindow)(Display* display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual* visual, unsigned long valuemask, XSetWindowAttributes* attributes) = NULL;

/* Custom. */
int api_setup();
int api_kill();
int unprotect(unsigned int addr);
ssize_t send_res(int fd, char* res);

#endif /* PROTO_H */
