// main.cpp
#include "Maze.hpp"

#include <iostream>
#include <vector> // Needed for printing the resulting vectors
#include <string> // Needed for string comparison (optional)
#include <limits> // Needed for clearing input buffer

using namespace MazeGenerator; // Use the namespace for easier access

int main() {
    unsigned int width, height;
    char output_format_choice;

    std::cout << "Enter maze width: ";
    std::cin >> width;
    std::cout << "Enter maze height: ";
    std::cin >> height;

    if (width <= 0 || height <= 0) {
        std::cerr << "Error: Width and height must be positive integers." << std::endl;
        return 1;
    }

    // Clear newline character from input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Choose output format:" << std::endl;
    std::cout << "  A) Adjacency List" << std::endl;
    std::cout << "  M) Adjacency Matrix (Graph)" << std::endl;
    std::cout << "Enter choice (A/M): ";
    std::cin >> output_format_choice;

    // Create and generate the maze
    Maze maze(width, height);
    // Print the seed
    std::cout << "Random Seed: " << maze.get_seed() << std::endl;
    // Generate the maze using DFS algorithm
    maze.generate_dfs();

    std::cout << "\n--- Generated Maze ---" << std::endl;
    // Optional: Print grid for visual check
    maze.print_grid();
    std::cout << "----------------------\n" << std::endl;

    if (output_format_choice == 'A' || output_format_choice == 'a') {
        std::cout << "\n--- Adjacency List Output ---" << std::endl;
        std::vector<std::vector<unsigned int>> adj_list = maze.to_adjacency_list();
        unsigned int total_cells = maze.get_total_cells(); // Use getter

        for (unsigned int i = 0; i < total_cells; ++i) {
            std::cout << "Cell " << i << ": "; // Index 0 to (width*height)-1
            if (adj_list[i].empty()) {
                std::cout << "(No connections)";
            } else {
                 for (unsigned int neighbor : adj_list[i]) {
                    std::cout << neighbor << " ";
                }
            }
            std::cout << std::endl;
        }

    } else if (output_format_choice == 'M' || output_format_choice == 'm') {
        std::cout << "\n--- Adjacency Matrix Output ---" << std::endl;
        std::vector<std::vector<unsigned int>> adj_matrix = maze.to_adjacency_matrix();
        unsigned int total_cells = maze.get_total_cells(); // Use getter

        for (unsigned int i = 0; i < total_cells; ++i) {
            for (unsigned int j = 0; j < total_cells; ++j) {
                std::cout << adj_matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }

    } else {
        std::cerr << "Invalid output format choice." << std::endl;
        return 1;
    }

    return 0;
}