#ifndef MAZE_GENERATOR_CELL_H
#define MAZE_GENERATOR_CELL_H

namespace MazeGenerator {

    struct Cell {
        unsigned int row {0};
        unsigned int col {0};
        bool visited {false};

        // Walls: true means wall is present, false means passage
        bool wallN {true};
        bool wallE {true};
        bool wallS {true};
        bool wallW {true};
    };

} // namespace MazeGenerator

#endif // MAZE_GENERATOR_CELL_H