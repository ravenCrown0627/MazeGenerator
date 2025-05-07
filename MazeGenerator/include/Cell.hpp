struct Cell {
    int row;        // Row position of the cell
    int col;        // Column position of the cell
    bool visited;   // Indicates if the cell has been visited
    bool wallN;    // North wall presence
    bool wallE;    // East wall presence
    bool wallS;    // South wall presence
    bool wallW;    // West wall presence

    // Constructor to initialize a cell
    Cell(int r, int c, bool v = false, bool wn = true, bool we = true, bool ws = true, bool ww = true)
        : row(r), col(c), visited(v), wallN(wn), wallE(we), wallS(ws), wallW(ww) {}
};