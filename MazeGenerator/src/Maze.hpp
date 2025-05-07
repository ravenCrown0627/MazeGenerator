// filepath: /MazeGenerator/MazeGenerator/src/Maze.hpp
#ifndef MAZE_HPP
#define MAZE_HPP

#include <vector>
#include <utility>
#include <stack>
#include <random>
#include "Cell.hpp"

class Maze {
public:
    Maze(int width, int height);
    void generate_dfs();
    std::vector<std::vector<int>> to_adjacency_list() const;
    std::vector<std::vector<int>> to_adjacency_matrix() const;
    void print_grid() const;

private:
    int m_width;
    int m_height;
    std::vector<std::vector<Cell>> m_grid;
    std::mt19937 m_rng;

    bool is_valid(int row, int col) const;
    std::vector<std::pair<int, int>> get_unvisited_neighbors(int row, int col) const;
    bool remove_wall(int row1, int col1, int row2, int col2);
    int cell_to_index(int row, int col) const;
    std::pair<int, int> index_to_cell(int index) const;
};

#endif // MAZE_HPP