// filepath: /MazeGenerator/MazeGenerator/src/Maze.cpp
// Maze.cpp
#include "Maze.hpp"
#include <iostream> // For printing (if print_grid is implemented here)
#include <chrono>   // For seeding random number generator
#include <algorithm>// For std::shuffle

// Constructor implementation
Maze::Maze(int w, int h) : m_width(w), m_height(h), m_grid(h, std::vector<Cell>(w)) {
    // Initialize m_grid cells
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            m_grid[r][c] = {r, c, false, true, true, true, true}; // Initialize with walls
        }
    }

    // Seed the random number generator
    m_rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    
}

// Helper function implementations
bool Maze::is_valid(int row, int col) const {
    return row >= 0 && row < m_height && col >= 0 && col < m_width;
}

std::vector<std::pair<int, int>> Maze::get_unvisited_neighbors(int row, int col) const {
    std::vector<std::pair<int, int>> neighbors {};
    int dr[] = {-1, 0, 1, 0}; // North, East, South, West row changes
    int dc[] = {0, 1, 0, -1}; // North, East, South, West col changes

    size_t num_directions = sizeof(dr) / sizeof(dr[0]);

    for (size_t i = 0; i < num_directions; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];

        if (is_valid(nr, nc) && !m_grid[nr][nc].visited) {
            neighbors.push_back({nr, nc});
        }
    }

    return neighbors;
}

bool Maze::remove_wall(int row1, int col1, int row2, int col2) {
    bool valid {true};

    // Check if the cells are valid
    if (!is_valid(row1, col1) || !is_valid(row2, col2)) {
        valid = false; // Invalid cells
    }

    if (valid) {
        // Check if the cells are adjacent
        if (std::abs(row1 - row2) + std::abs(col1 - col2) != 1) {
            valid =  false; // Cells are not adjacent
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

int Maze::cell_to_index(int row, int col) const {
    return row * m_width + col;
}

std::pair<int, int> Maze::index_to_cell(int index) const {
    return {index / m_width, index % m_width};
}

// Maze Generation (DFS) implementation
void Maze::generate_dfs() {
    std::stack<std::pair<int, int>> s;

    // Start at a random cell
    int start_row = m_rng() % m_height;
    int start_col = m_rng() % m_width;

    m_grid[start_row][start_col].visited = true;
    s.push({start_row, start_col});

    while (!s.empty()) {
        std::pair<int, int> current = s.top();
        int row = current.first;
        int col = current.second;

        std::vector<std::pair<int, int>> neighbors = get_unvisited_neighbors(row, col);

        if (!neighbors.empty()) {
            // Push current cell back to stack (for backtracking)
            s.push({row, col});

            // Choose a random unvisited neighbor
            std::shuffle(neighbors.begin(), neighbors.end(), m_rng);
            std::pair<int, int> next = neighbors[0];
            int nr = next.first;
            int nc = next.second;

            // Remove the wall between current and next
            remove_wall(row, col, nr, nc);

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
std::vector<std::vector<int>> Maze::to_adjacency_list() const {
    int total_cells = m_width * m_height;
    std::vector<std::vector<int>> adj_list(total_cells);

    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            int current_index = cell_to_index(row, col);
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
std::vector<std::vector<int>> Maze::to_adjacency_matrix() const {
    int total_cells = m_width * m_height;
    std::vector<std::vector<int>> adj_matrix(total_cells, std::vector<int>(total_cells, 0));

    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            int current_index = cell_to_index(row, col);
            const Cell& cell = m_grid[row][col];

            // Check connections and mark in matrix (undirected graph)
            if (!cell.wallN && is_valid(row - 1, col)) {
                int neighbor_index = cell_to_index(row - 1, col);
                adj_matrix[current_index][neighbor_index] = 1;
                adj_matrix[neighbor_index][current_index] = 1; // Assuming undirected graph
            }
            if (!cell.wallE && is_valid(row, col + 1)) {
                int neighbor_index = cell_to_index(row, col + 1);
                adj_matrix[current_index][neighbor_index] = 1;
                adj_matrix[neighbor_index][current_index] = 1;
            }
            if (!cell.wallS && is_valid(row + 1, col)) {
                int neighbor_index = cell_to_index(row + 1, col);
                adj_matrix[current_index][neighbor_index] = 1;
                adj_matrix[neighbor_index][current_index] = 1;
            }
            if (!cell.wallW && is_valid(row, col - 1)) {
                int neighbor_index = cell_to_index(row, col - 1);
                adj_matrix[current_index][neighbor_index] = 1;
                adj_matrix[neighbor_index][current_index] = 1;
            }
        }
    }
    return adj_matrix;
}

// Optional: Print Grid implementation
void Maze::print_grid() const {
    for (int row = 0; row < m_height; ++row) {
        // Print North walls
        for (int col = 0; col < m_width; ++col) {
            std::cout << "+" << (m_grid[row][col].wallN ? "---" : "   ");
        }
        std::cout << "+" << std::endl;

        // Print West walls and cell content
        for (int col = 0; col < m_width; ++col) {
            std::cout << (m_grid[row][col].wallW ? "|" : " ");
            std::cout << "   "; // Cell content (e.g., ' ' or 'V') - keeping it simple here
        }
        std::cout << "|" << std::endl;

        // Print South walls for the last row
        if (row == m_height - 1) {
            for (int col = 0; col < m_width; ++col) {
                std::cout << "+" << (m_grid[row][col].wallS ? "---" : "   ");
            }
            std::cout << "+" << std::endl;
        }
    }
}