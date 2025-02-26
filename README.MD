# Conway's Game of Life

This project is an implementation of John Horton Conway's famous cellular automaton, the Game of Life.

## Overview

The Game of Life is a zero-player game that evolves based on its initial state, requiring no further input. It consists of a grid of cells that can be alive or dead. Each cell's state in the next generation is determined by its eight neighbors according to the following rules:

1. **Underpopulation**: A live cell with fewer than two live neighbors dies.
2. **Survival**: A live cell with two or three live neighbors lives on to the next generation.
3. **Overpopulation**: A live cell with more than three live neighbors dies.
4. **Reproduction**: A dead cell with exactly three live neighbors becomes a live cell.

Through these simple rules, complex patterns and behaviors can emerge.

## Features

- **Customizable Grid Size**: Adjust the grid dimensions to explore different scenarios.
- **Initial Configuration**: Set up the initial state of the grid with various patterns.
- **Generation Control**: Step through generations manually or run continuously.
- **Visualization**: Graphical display of the grid and its evolving states.

## Installation

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/dmitrii-gbe/ConwaysGameOfLife.git
   ```


2. **Navigate to the Project Directory**:

   ```bash
   cd ConwaysGameOfLife
   ```


3. **Compile the Source Code**:

   ```bash
   gcc -o game_of_life src/*.c
   ```


   Ensure you have a C compiler like `gcc` installed.

## Usage

After compiling, run the program with:


```bash
./game_of_life
```


Follow the on-screen instructions to set up the initial configuration and control the simulation.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests to enhance the project.

## License

This project is licensed under the MIT License.

---

*Note: This project is a personal implementation and is not affiliated with any official Game of Life organizations.* 
