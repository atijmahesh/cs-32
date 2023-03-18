#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols) {
    rows = nRows;
    cols = nCols;
    for(int i = 0; i<nRows; i++) {
        for(int j = 0; j<nCols; j++) {
            arena[i][j] = 'A' - 1;
        }
    }
    
}

bool History::record(int r, int c) {
    r--;
    c--;
    if(r<=rows && r>=0 && c<=cols && c>=0) {
        if(arena[r][c] != 'Z') {
            arena[r][c]++;
            return true;
        }
    }
    return false;
}

void History::display() const {
    clearScreen();
    for(int i = 0; i<rows; i++) {
        for(int j = 0; j<cols; j++) {
            if(arena[i][j] == 'A' - 1)
                cout << '.';
            else
                cout << arena[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}
