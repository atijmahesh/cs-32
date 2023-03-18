#include <iostream>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
          // Return true if there is a path from (sr,sc) to (er,ec)
          // through the maze; return false otherwise

class Coord {
    public:
        Coord(int r, int c) : m_row(r), m_col(c) {}
        int r() const { return m_row; }
        int c() const { return m_col; }
    private:
        int m_row;
        int m_col;
};

int main() {
    string maze[10] = {
        "XXXXXXXXXX",
        "X..X...X.X",
        "X.XXXX.X.X",
        "X.X.X..X.X",
        "X...X.XX.X",
        "XXX......X",
        "X.X.XXXX.X",
        "X.XXX....X",
        "X...X..X.X",
        "XXXXXXXXXX"
    };
        
    if (pathExists(maze, 10,10, 5,3, 8,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}

#include <stack>
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    stack<Coord> coordStack;
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = '*';
    while(!coordStack.empty()) {
        Coord curr = coordStack.top();
        coordStack.pop();
        if(curr.r() == er && curr.c() == ec)
            return true;
        if(curr.c() + 1 < nCols && maze[curr.r()][curr.c() + 1] == '.') {
            coordStack.push(Coord(curr.r(), curr.c() + 1));
            maze[curr.r()][curr.c() + 1] = '*';
        }
        if(curr.r() - 1 >= 0 && maze[curr.r() - 1][curr.c()] == '.'){
            coordStack.push(Coord(curr.r() - 1, curr.c()));
            maze[curr.r() - 1][curr.c()] = '*';
        }
        if(curr.c() - 1 >= 0 && maze[curr.r()][curr.c() - 1] == '.') {
            coordStack.push(Coord(curr.r(), curr.c() - 1));
            maze[curr.r()][curr.c() - 1] = '*';
        }
        if(curr.r() + 1 < nRows && maze[curr.r() + 1][curr.c()] == '.'){
            coordStack.push(Coord(curr.r() + 1, curr.c()));
            maze[curr.r() + 1][curr.c()] = '*';
        }
    }
    return false;
}
