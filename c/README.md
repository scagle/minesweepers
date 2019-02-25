# Minesweeper in C
![Actual image of gameplay](images/gameplay.png)

## Dependencies:
* SDL2 
* SDL2\_ttf 
### Ubuntu
```bash 
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
```
### ArchLinux
```bash
sudo pacman -S sdl2
sudo pacman -S sdl2_ttf
```
### Mac OS X
* SDL2 [Tutorial](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/mac/index.php), [Binaries](https://www.libsdl.org/download-2.0.php#source)
* SDL2\_ttf [Binaries](https://www.libsdl.org/projects/SDL_ttf/)
### Windows
* SDL2 [Tutorial](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/index.php), [Binaries](https://www.libsdl.org/download-2.0.php#source)
* SDL2\_ttf [Binaries](https://www.libsdl.org/projects/SDL_ttf/)

## Building:
* Change directory into build/
```cmake
$ cmake ../
$ make
```
## Execution:
* Change directory into build/
```bash
$ ./minesweeper 
```

