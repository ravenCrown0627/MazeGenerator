#include "Maze.hpp"
#include <gtest/gtest.h>

namespace MazeGenerator {

TEST(MazeTest, ConstructorInitializesCorrectDimensions) {
    unsigned int width = 10;
    unsigned int height = 15;
    Maze maze(width, height);

    EXPECT_EQ(maze.get_width(), width);
    EXPECT_EQ(maze.get_height(), height);
    EXPECT_EQ(maze.get_total_cells(), width * height);
}

TEST(MazeTest, GenerateDFSReproducibility) {
    unsigned int width = 10;
    unsigned int height = 10;
    unsigned long seed = 12345;

    // Generate the first maze with a fixed seed
    Maze maze1(width, height, seed);
    maze1.generate_dfs();

    // Generate the second maze with the same fixed seed
    Maze maze2(width, height, seed);
    maze2.generate_dfs();

    // Convert both mazes to adjacency lists and compare
    auto adj_list1 = maze1.to_adjacency_list();
    auto adj_list2 = maze2.to_adjacency_list();

    EXPECT_EQ(adj_list1, adj_list2);
}

TEST(MazeTest, IsValidCellCheck) {
    unsigned int width = 5;
    unsigned int height = 5;
    Maze maze(width, height);

    // Valid cells
    EXPECT_TRUE(maze.is_valid(static_cast<unsigned int>(0), 0));
    EXPECT_TRUE(maze.is_valid(static_cast<unsigned int>(4), 4));

    // Invalid cells
    EXPECT_FALSE(maze.is_valid(static_cast<unsigned int>(5), static_cast<unsigned int>(5)));

    // Handle negative indices explicitly
    int negative_row = -1;
    int negative_col = 0;
    EXPECT_FALSE(negative_row >= 0 && negative_col >= 0 && maze.is_valid(static_cast<unsigned int>(negative_row), static_cast<unsigned int>(negative_col)));
}

} // namespace MazeGenerator