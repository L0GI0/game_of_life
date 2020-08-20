## Table of Contents

* [About the Project](#about-the-project)
* [Built With](#built-with)
* [Installation](#installation)
* [Usage](#usage)
* [Compatibility](#Compatibility)
* [License](#license)
* [Contact](#contact)


## About The Project
Project written in C++ that implements an interactive Game of Life. Game has ability to save and load current board status into a from RLE file.

![game_of_life](https://github.com/L0GI0/Images/blob/master/Game_of_life_-_C%2B%2B__SFML__Boost.gif)<br/>
[YouTube](https://www.youtube.com/watch?v=f1NadrwLGPw)
## Built With
External libraries:
* [SFML](https://www.sfml-dev.org/)
* [Boost Libraries](https://www.boost.org/)


## Installation
Download ['Game_of_life.zip'](https://github.com/L0GI0/game_of_life/blob/master/Game_of_life.zip) folder that contains builded game file and necessary .dll files.
Unpack download file into one location.

## Usage
Run GameOfLife.exe from unpacked 'Game_of_life.zip' folder in order to play the game.<br />

If any option changes are required, game can be run from console level with any of the undermentioned options:<br />
  ><b>-h</b> [ --help ]             Help screen<br />
  ><b>-w</b> [ --width ] arg (=50)  Board width<br />
  ><b>-h</b> [ --height ] arg (=50) Board height<br />
  ><b>-s</b> [ --speed ] arg (=100) Speed of simulation in milliseconds<br />
  ><b>--init_file</b> arg           RL File containing starting board<br />
  ><b>-c</b> [ --config ] arg       Config file - contains options string with avaiable options settings

Default settings:<br />
  >Grid height: <b>50</b><br />
  >Grid width: <b>50</b><br />
  >Speed of simulation: <b>100</b><br />
  >Starting the game with default grid due to lack of init file


## Compatibility
Windows 10 - Game executable.<br />
Other systems - game can be builed in any systems by using resorce files. 

## License
Free to use and modify.

## Contact
Michal Pabjan - michaelpabjan@gmail.com<br />
Project Link: [https://github.com/L0GI0/game_of_life](https://github.com/L0GI0/game_of_life)



