# ModPi
Minecraft Pi API extensions ported to MCPI-Reborn.

## Getting started
### Prerequisites
To use the ModPi API you need to have [MCPI_Reborn](https://gitea.thebrokenrail.com/TheBrokenRail/minecraft-pi-reborn) installed.

### Installation
Code is unfinished! Check back in a week or 2.

## API
The ModPi API exposes the following methods through the `ModPi` class:

### `__init__(mcpi)`
Initializes the class, `mcpi` must be a `mcpi.minecraft.Minecraft` object.

### `getPlayerName()`
Returns the current player name.

### `setPlayerName(username)`
Sets the current player name to `username`.

### `getGameMode()`
Returns the game mode of the current world as an integer:
 + 0 = Survival
 + 1 = Creative

### `getWorldName()`
Returns the name of the current world.

### `getServerAddr()`
Returns the IP address or internet domain of the current server.

### `getServerPort()`
Returns the internet port of the current server.

### `setAmbientColor(color)`
Sets the OpenGL fog and clear colors. The `color` argument must be an RGBA tuple, like `(255, 0, 255, 128)`.

### `getActMode()`
Returns if the instance of MCPI is acting as Server, Client or Local:
 + 0 = Server
 + 1 = Client
 + 2 = Local

## FAQ
### Where can I find an usage example?
There is an [`example.py`](https://github.com/MCPI-Devs/modpi/blob/master/src/example.py) in the [`src`](https://github.com/MCPI-Devs/modpi/tree/master/src) folder. It contains a basic usage example.

### How does this work?
It's a bit magic. The core is the C code, that's all my explanation because I'm too lazy to write a proper one. But hey! The code is well commented! (I hope).

## Licensing
All the code of this project is licensed under the [GNU General Public License version 2.0](https://github.com/MCPI-Devs/proxy/blob/master/LICENSE) (GPL-2.0).

All the documentation of this project is licensed under the [Creative Commons Attribution-ShareAlike 4.0 International](https://creativecommons.org/licenses/by-sa/4.0/) (CC BY-SA 4.0) license.

![CC BY-SA 4.0](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)
