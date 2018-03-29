# Leprechaun-Game
Final data structures project
## About
Our data structures class was tasked with utilizing the most efficient use of the [Binary Search tree](https://en.wikipedia.org/wiki/Binary_search_tree). I opted to use the [C++ STL Map](http://www.cplusplus.com/reference/map/map/?kw=map) to most efficiently implement the game.
## Rules
The Leprechauns are each given a number and jump in that order. Leprechauns jump left or right: 
* If they land near the center, the player gets their gold and they are removed from the game 
* If they land on another leprechaun, the jumping leprechaun steals the others gold and then that leprechaun is removed from the game
* If they land between two leprechauns, they steal half of their neighbors gold (leprechauns that land on the edge only steal from one neighbor)
The game runs for a fixed amount of time after which the players score and number of leprechauns caught is displayed.
## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
