#include "Maze.hpp"
#include <iostream>

int main() {
    int width, height;

    std::cout << "Enter maze width: ";
    std::cin >> width;
    std::cout << "Enter maze height: ";
    std::cin >> height;

    Maze maze(width, height);
    maze.generate_dfs(); // Generate the maze using depth-first search

    std::cout << "Generated Maze:" << std::endl;
    maze.print_grid(); // Print the generated maze

    return 0;
}