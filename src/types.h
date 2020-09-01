/*
 * types.h
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

#ifndef TYPES_H
#define TYPES_H
#include <arpa/inet.h>

typedef enum act_mode_t {
	SERVER,
	CLIENT,
	LOCAL
} act_mode_t;

typedef struct modpi_data_t {
	char* world_name;
	char* player_name;
	char game_mode[3];
	char server_addr[INET_ADDRSTRLEN + 1];
	char server_port[9];
	char* server_name;
	float ambient[4];
	act_mode_t acting_as;
} modpi_data_t;

#endif /* TYPES_H */
