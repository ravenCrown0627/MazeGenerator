#ifndef MAZE_GENERATOR_MAZE_H
#define MAZE_GENERATOR_MAZE_H

#include "MazeCell.hpp" // Include the Cell structure definition

#include <random>
#include <vector>
#include <utility>


namespace MazeGenerator {
    // Forward declaration
    class MazeTest_IsValidCellCheck_Test;

    class Maze {
    public:
        // Constructor
        Maze(unsigned int w, unsigned int h);
        Maze(unsigned int w, unsigned int h, unsigned long seed);

        // Maze Generation (DFS Algorithm)
        // Optional: You can add other algorithms like Prim's or Kruskal's
        void generate_dfs();

        // Convert maze structure to adjacency list representation
        std::vector<std::vector<unsigned int>> to_adjacency_list() const;

        // Convert maze structure to adjacency matrix representation
        std::vector<std::vector<unsigned int>> to_adjacency_matrix() const;

        // Optional: Print Grid (for visualization)
        void print_grid() const;

        // Getters for dimensions (optional but good practice)
        std::mt19937 get_seed() const { return m_rng; }
        unsigned int get_width() const { return m_width; }
        unsigned int get_height() const { return m_height; }
        unsigned int get_total_cells() const { return m_width * m_height; }

    private:
        unsigned int m_width {0};
        unsigned int m_height {0};
        std::vector<std::vector<Cell>> m_grid {};
        // Random number generator
        std::mt19937 m_rng {0}; // Mersenne Twister RNG

        // Helper function to remove wall between two adjacent cells
        bool remove_wall(unsigned int row1, unsigned int col1, unsigned int row2, unsigned int col2);

        // Helper function to check if coordinates are within grid bounds
        bool is_valid(unsigned int row, unsigned int col) const;

        // Helper function to get unvisited neighbors of a cell
        std::vector<std::pair<unsigned int, unsigned int>> get_unvisited_neighbors(unsigned int row, unsigned int col) const;

        // Helper function to convert cell coordinates to a single index (0 to N-1)
        unsigned int cell_to_index(unsigned int row, unsigned int col) const;

        // Helper function to convert index back to cell coordinates
        std::pair<unsigned int, unsigned int> index_to_cell(unsigned int index) const;

        // Friend class for testing purposes
        friend class MazeTest_IsValidCellCheck_Test;
    };

} // namespace MazeGenerator

#endif // MAZE_GENERATOR_MAZE_H