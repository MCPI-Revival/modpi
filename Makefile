#
#  Makefile
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

all:
	mkdir -p build
	gcc -fPIC -shared -lmcpi -I./src ./src/preload.c -o ./build/libmodpi.so -Wall

install:
	pip3 install .

pack:
	mkdir -p ./deb/
	mkdir -p ./deb/usr/lib/
	mkdir -p ./deb/DEBIAN/
	mkdir -p ./deb/usr/lib/python3/dist-packages
	cp ./build/libmodpi.so ./deb/usr/lib/
	cp ./src/modpi.py ./deb/usr/lib/python3/dist-packages
	chmod 0755 ./deb/usr/lib/libmodpi.so
	@echo "Package: libmodpi" > ./deb/DEBIAN/control
	@echo "Version: 0.3.0" >> ./deb/DEBIAN/control
	@echo "Priority: optional" >> ./deb/DEBIAN/control
	@echo "Architecture: armhf" >> ./deb/DEBIAN/control
	@echo "Depends: libmcpi, python3" >> ./deb/DEBIAN/control
	@echo "Maintainer: Alvarito050506 <donfrutosgomez@gmail.com>" >> ./deb/DEBIAN/control
	@echo "Homepage: https://mcpi.tk" >> ./deb/DEBIAN/control >> ./deb/DEBIAN/control
	@echo "Vcs-Browser: https://github.com/MCPI-Devs/modpi" >> ./deb/DEBIAN/control
	@echo "Vcs-Git: https://github.com/MCPI-Devs/modpi.git" >> ./deb/DEBIAN/control
	@echo "Description: Extensions for the Minecraft Pi API.\n" >> ./deb/DEBIAN/control
	dpkg-deb -b ./deb/ ./libmodpi_0.3.0-1.deb

clean:
	rm -rf ./build/
	rm -rf ./deb/
	rm -f ./libmodpi_*-*.deb
