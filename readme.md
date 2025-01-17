# Bouncing Ball Game

This is a simple bouncing ball game implemented using SDL2, SDL2_ttf, and SDL2_image. The game features a platform that the player can move left and right to bounce the ball. The game includes a main menu, pause functionality, and the ability to resume or quit the game.

## Features

- Main menu with "Play" and "Exit" options.
- Pause menu with "Resume" and "Quit" options.
- Increasing ball speed by 15% on every strike.
- Background images for the main menu and game screen.
- Display of the current score and instructions to exit the game.

## Requirements

- SDL2
- SDL2_ttf
- SDL2_image
- CMake 3.28.3

## Screenshots
[!image1](./screenshots/1.png)
[!image1](./screenshots/2.png)

## Installation

### On Debian (or and Ubuntu Derivative) 

You can install the required libraries using the following commands:

### Clone 
```sh
git clone 
cd BouncingBall
```

### Dependencies
```sh
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev 
```
### Build the Project:
```sh
cd build
cmake ..
make 
```

## On macOS

You can install the required libraries using Homebrew:
```sh
brew install sdl2 sdl2_ttf sdl2_image cmake
```

## On Windows

You can download the SDL2, SDL2_ttf, and SDL2_image libraries from their respective websites and follow the installation instructions. Make sure to add the library paths to your system's PATH environment variable.


# Running the Game

After building the project, you can run the executable:
```sh
./BouncingBall
```
## Controls
- **Left Arrow Key:** Move the platform left.
- **Right Arrow Key:** Move the platform right.
- **P Key:** Pause the game.
- **ESC Key**Right Arrow Key: Return to the main menu.

## Directory Structure

.
├── Assets\
│   ├── bg-icebergs-1.png\
│   └── Poppins-Regular.ttf\
├── build\
│   ├── CMakeCache.txt\
│   ├── CMakeFiles\
│   ├── cmake_install.cmake\
│   └── Makefile\
├── CMakeLists.txt\
├── main.cpp\
└── readme.md

### License

This project is licensed under the MIT License. See the LICENSE file for more details.
Contributing

### Contributions
Contributions are welcome! Please open an issue or submit a pull request.

### Acknowledgments
Thanks to the SDL2 community for providing excellent libraries for game development.Thanks to the CMake community for providing a powerful build system.
