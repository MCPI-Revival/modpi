#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  modpi.py
#  
#  Copyright 2020 Alvarito050506 <donfrutosgomez@gmail.com>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; version 2 of the License.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#  

class ModPi:
	def __init__(self, mcpi):
		self.conn = mcpi.conn;

	def getPlayerName(self):
		return self.conn.sendReceive("mp.getPlayerName");

	def setPlayerName(self, username):
		return self.conn.send("mp.setPlayerName", username);

	def getGameMode(self):
		return int(self.conn.sendReceive("mp.getGameMode")[0]);

	def getWorldName(self):
		return self.conn.sendReceive("mp.getWorldName");

	def getServerAddr(self):
		return self.conn.sendReceive("mp.getServerAddr");

	def getServerPort(self):
		return int(self.conn.sendReceive("mp.getServerPort"));

	def setAmbientColor(self, color):
		if len(color) < 4:
			return -1;
		return self.conn.send("mp.setAmbientColor", color[0], color[1], color[2], color[3]);
