#include "Maze.hpp"
#include "MazeCell.hpp"

#include <stack>
#include <chrono>
#include <random>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>

namespace MazeGenerator {
    // Constructor implementation
    Maze::Maze(unsigned int w, unsigned int h) : m_width(w), m_height(h), m_grid(h, std::vector<Cell>(w)) {
        // Initialize m_grid cells
        for (unsigned int r = 0; r < m_height; ++r) {
            for (unsigned int c = 0; c < m_width; ++c) {
                m_grid[r][c] = {r, c, false, true, true, true, true}; // Initialize with walls
            }
        }

        // Seed the random number generator
        std::random_device rd;
        m_rng.seed(rd());
    }

    // Constructor with seed
    Maze::Maze(unsigned int w, unsigned int h, unsigned long seed) : m_width(w), m_height(h), m_grid(h, std::vector<Cell>(w)) {
        // Initialize m_grid cells
        for (unsigned int r = 0; r < m_height; ++r) {
            for (unsigned int c = 0; c < m_width; ++c) {
                m_grid[r][c] = {r, c, false, true, true, true, true}; // Initialize with walls
            }
        }

        // Seed the random number generator with a fixed seed
        m_rng.seed(seed);
    }

    // Helper function implementations
    bool Maze::is_valid(unsigned int row, unsigned int col) const {
        return row < m_height && col < m_width;
    }

    std::vector<std::pair<unsigned int, unsigned int>> Maze::get_unvisited_neighbors(unsigned int row, unsigned int col) const {
        std::vector<std::pair<unsigned int, unsigned int>> neighbors {};
        int dr[] = {-1, 0, 1, 0}; // North, East, South, West row changes
        int dc[] = {0, 1, 0, -1}; // North, East, South, West col changes

        size_t num_directions = sizeof(dr) / sizeof(dr[0]);

        for (size_t i = 0; i < num_directions; ++i) {
            unsigned int nr = row + static_cast<unsigned int>(dr[i]);
            unsigned int nc = col + static_cast<unsigned int>(dc[i]);

            if (is_valid(nr, nc) && !m_grid[nr][nc].visited) {
                neighbors.push_back({nr, nc});
            }
        }

        return neighbors;
    }

    bool Maze::remove_wall(unsigned int row1, unsigned int col1, unsigned int row2, unsigned int col2) {
        bool valid {true};

        // Check if the cells are valid
        if (!is_valid(row1, col1) || !is_valid(row2, col2)) {
            valid = false; // Invalid cells
        }

        if (valid) {
            // Check if the cells are adjacent
            if (std::abs(static_cast<int>(row1) - static_cast<int>(row2)) +
                std::abs(static_cast<int>(col1) - static_cast<int>(col2)) != 1) {
                valid = false; // Cells are not adjacent
            }
        }

        if (valid) {
            // Determine direction and remove the wall
            if (row1 == row2) { // Same row
                if (col2 == col1 + 1) { // Neighbor is East
                    m_grid[row1][col1].wallE = false;
                    m_grid[row2][col2].wallW = false;
                } else if (col2 == col1 - 1) { // Neighbor is West
                    m_grid[row1][col1].wallW = false;
                    m_grid[row2][col2].wallE = false;
                }
            } else if (col1 == col2) { // Same column
                if (row2 == row1 + 1) { // Neighbor is South
                    m_grid[row1][col1].wallS = false;
                    m_grid[row2][col2].wallN = false;
                } else if (row2 == row1 - 1) { // Neighbor is North
                    m_grid[row1][col1].wallN = false;
                    m_grid[row2][col2].wallS = false;
                }
            }
        }

        return valid; // Wall removed successfully
    }

    unsigned int Maze::cell_to_index(unsigned int row, unsigned int col) const {
        return row * m_width + col;
    }

    std::pair<unsigned int, unsigned int> Maze::index_to_cell(unsigned int index) const {
        return {index / m_width, index % m_width};
    }

    // Maze Generation (DFS) implementation
    void Maze::generate_dfs() {
        std::stack<std::pair<unsigned int, unsigned int>> s;

        // Start at a random cell
        unsigned int start_row = m_rng() % m_height;
        unsigned int start_col = m_rng() % m_width;

        m_grid[start_row][start_col].visited = true;
        s.push({start_row, start_col});

        while (!s.empty()) {
            std::pair<unsigned int, unsigned int> current = s.top();
            unsigned int row = current.first;
            unsigned int col = current.second;

            std::vector<std::pair<unsigned int, unsigned int>> neighbors = get_unvisited_neighbors(row, col);

            if (!neighbors.empty()) {
                // Push current cell back to stack (for backtracking)
                s.push({row, col});

                // Choose a random unvisited neighbor
                std::shuffle(neighbors.begin(), neighbors.end(), m_rng);
                std::pair<unsigned int, unsigned int> next = neighbors[0];
                unsigned int nr = next.first;
                unsigned int nc = next.second;

                // Remove the wall between current and next
                assert(remove_wall(row, col, nr, nc));

                // Mark next as visited and push onto stack
                m_grid[nr][nc].visited = true;
                s.push({nr, nc});

            } else {
                // No unvisited neighbors, pop and backtrack
                s.pop();
            }
        }
    }

    // Convert to Adjacency List implementation
    std::vector<std::vector<unsigned int>> Maze::to_adjacency_list() const {
        unsigned int total_cells = m_width * m_height;
        std::vector<std::vector<unsigned int>> adj_list(total_cells);

        for (unsigned int row = 0; row < m_height; ++row) {
            for (unsigned int col = 0; col < m_width; ++col) {
                unsigned int current_index = cell_to_index(row, col);
                const Cell& cell = m_grid[row][col];

                // Check connections to neighbors based on missing walls
                if (!cell.wallN && is_valid(row - 1, col)) {
                    adj_list[current_index].push_back(cell_to_index(row - 1, col));
                }
                if (!cell.wallE && is_valid(row, col + 1)) {
                    adj_list[current_index].push_back(cell_to_index(row, col + 1));
                }
                if (!cell.wallS && is_valid(row + 1, col)) {
                    adj_list[current_index].push_back(cell_to_index(row + 1, col));
                }
                if (!cell.wallW && is_valid(row, col - 1)) {
                    adj_list[current_index].push_back(cell_to_index(row, col - 1));
                }
            }
        }
        return adj_list;
    }

    // Convert to Adjacency Matrix implementation
    std::vector<std::vector<unsigned int>> Maze::to_adjacency_matrix() const {
        unsigned int total_cells = m_width * m_height;
        std::vector<std::vector<unsigned int>> adj_matrix(total_cells, std::vector<unsigned int>(total_cells, 0));

        for (unsigned int row = 0; row < m_height; ++row) {
            for (unsigned int col = 0; col < m_width; ++col) {
                unsigned int current_index = cell_to_index(row, col);
                const Cell& cell = m_grid[row][col];

                // Check connections to neighbors based on missing walls
                if (!cell.wallN && is_valid(row - 1, col)) {
                    unsigned int neighbor_index = cell_to_index(row - 1, col);
                    adj_matrix[current_index][neighbor_index] = 1;
                    adj_matrix[neighbor_index][current_index] = 1; // Undirected graph
                }
                if (!cell.wallE && is_valid(row, col + 1)) {
                    unsigned int neighbor_index = cell_to_index(row, col + 1);
                    adj_matrix[current_index][neighbor_index] = 1;
                    adj_matrix[neighbor_index][current_index] = 1; // Undirected graph
                }
                if (!cell.wallS && is_valid(row + 1, col)) {
                    unsigned int neighbor_index = cell_to_index(row + 1, col);
                    adj_matrix[current_index][neighbor_index] = 1;
                    adj_matrix[neighbor_index][current_index] = 1; // Undirected graph
                }
                if (!cell.wallW && is_valid(row, col - 1)) {
                    unsigned int neighbor_index = cell_to_index(row, col - 1);
                    adj_matrix[current_index][neighbor_index] = 1;
                    adj_matrix[neighbor_index][current_index] = 1; // Undirected graph
                }
            }
        }
        return adj_matrix;
    }

    // Write the print_grid function to visualize the maze
    void Maze::print_grid() const {
        for (unsigned int row = 0; row < m_height; ++row) {
            for (unsigned int col = 0; col < m_width; ++col) {
                const Cell& cell = m_grid[row][col];
                std::cout << (cell.wallN ? "+---" : "+   ");
            }
            std::cout << "+" << std::endl;

            for (unsigned int col = 0; col < m_width; ++col) {
                const Cell& cell = m_grid[row][col];
                std::cout << (cell.wallW ? "|" : " ") << "   ";
            }
            std::cout << "|" << std::endl;
        }

        for (unsigned int col = 0; col < m_width; ++col) {
            std::cout << "+---";
        }
        std::cout << "+" << std::endl;
    }
}