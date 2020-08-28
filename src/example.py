#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  example.py
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

from random import random
from mcpi import *
from modpi import *
import sys
import time

mc = minecraft.Minecraft.create();
mp = ModPi(mc);

def main(args):
	mc.setting("world_immutable", True);
	mc.camera.setFollow();
	mc.saveCheckpoint();
	mc.postToChat("Welcome to Minecraft Pi, " + mp.getPlayerName() + ".");
	mc.postToChat("The name of this awesome world is \"" + mp.getWorldName() + "\".");
	if mp.getGameMode() == 0:
		mc.postToChat("You are in Survival mode.");
	else:
		mc.postToChat("You are in Creative mode.");
	time.sleep(5);
	mc.setting("world_immutable", False);
	mc.setting("nametags_visible", True);
	mc.player.setting("autojump", True);
	mc.camera.setNormal();
	i = 0;
	while i <= 100:
		mp.setAmbientColor((random() * 255, random() * 255, random() * 255, random() * 255));
		time.sleep(0.6);
		i += 1;
	return 0;

if __name__ == '__main__':
	sys.exit(main(sys.argv));
