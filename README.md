# PONG
SDL implementation of PONG

Simple DirectMedia Layer is a popular cross-platform software development library for C++
It has been used here to create a clone of the classic 70's arcade game, PONG.

The basic SDL, as well as SDL True Type Font libraries have been used.

# Installation

## Prerequisites
MinGW(for Windows), gcc(for Linux)

## Windows
Download the release [here](https://github.com/unknownblueguy6/PONG/files/2256350/pong_mingw_win32.zip) and run **pong.exe** .


## Linux(Ubuntu)
Download the release here.

1. Get the SDL libaries

**Note** : Skip this step if you already have the SDL2 development/runtime libraries


```bash
$ sudo apt-get libsdl2
```

```bash
$ sudo apt-get libsdl2_ttf
```

2. Run it

```bash
$ ./pong
```

# Instructions

You can change all the attributes of the paddles and the ball in the config.txt file.

There are two game modes : VS COMP and VS PLAYER

### VS COMP
W and S to move

### VS PLAYER
P1 : W and S 

P2 : ↑ and ↓



