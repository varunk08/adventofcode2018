#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

enum class TrackObject {
    RailHor      = 0,
    RailVer      = 1,
    FwdSlash     = 2, // top left or bottom right
    BwdSlash     = 3, // top righ or bottom left
    Intersection = 4,
    None
};

enum class CartDir {
    Left  = 0,
    Right = 1,
    Up    = 2,
    Down  = 3
};

enum class IntersectionMove {
    Left     = 0,
    Straight = 1,
    Right    = 2,
    None
};

struct Cart {
    int              x;
    int              y;
    CartDir          dir;
    IntersectionMove prevTurn;

    Cart(int _x, int _y, CartDir _dir, IntersectionMove move) : x(_x), y(_y), dir(_dir), prevTurn(move) {}
};

void ParseLine(vector<vector<TrackObject>>& grid, vector<Cart>& carts, string& line, int lineNum) {
    vector<TrackObject> curLine;
    for (int i = 0; i < line.length(); ++i) {
        //cout << line[i];
        switch(line[i]) {
            case ' ' : curLine.push_back(TrackObject::None); break;
            case '-' : curLine.push_back(TrackObject::RailHor); break;
            case '|' : curLine.push_back(TrackObject::RailVer); break;
            case '+' : curLine.push_back(TrackObject::Intersection); break;
            case '\\': curLine.push_back(TrackObject::FwdSlash); break;
            case '/' : curLine.push_back(TrackObject::BwdSlash); break;
            case '>':
                carts.emplace_back(i, lineNum, CartDir::Right, IntersectionMove::None);
                curLine.push_back(TrackObject::RailHor);
                break;
            case '<':
                carts.emplace_back(i, lineNum, CartDir::Left, IntersectionMove::None);
                curLine.push_back(TrackObject::RailHor);
                break;
            case 'v':
                carts.emplace_back(i, lineNum, CartDir::Down, IntersectionMove::None);
                curLine.push_back(TrackObject::RailVer);
                break;
            case '^':
                carts.emplace_back(i, lineNum, CartDir::Up, IntersectionMove::None);
                curLine.push_back(TrackObject::RailVer);
                break;
            default: break; 
        }
    }
    cout << curLine.size() << " ";
    grid.push_back(curLine);
}

int main(int argc, char** argv) {
    ifstream inFile("day13Input.txt");
    string line;
    vector<vector<TrackObject>> grid;
    vector<Cart> carts;

    int numLines = 0;
    unsigned long long maxW = 0;
    
    if (inFile.is_open()) {
        while(getline(inFile, line)) {
            ParseLine(grid, carts, line, numLines);
            maxW = max(line.size(), maxW);
            numLines++;
        }
    }

    int width = maxW;
    int height = numLines;
    cout << width << "x" << height << endl;

    bool collided = false;
    unsigned long long tick = 0;
    while(collided == false) {
        // sort the carts with y, break ties with x
        sort(carts.begin(), carts.end(), [](const Cart& A, const Cart& B) {
            if (A.y < B.y) {
                return true;
            } else if (A.y == B.y) {
                return (A.x < B.x);
            } else {
                return false;
            }
        });

        for(Cart& cart : carts) {
            // update the position of each cart
            switch(cart.dir) {
                case CartDir::Left:
                    cart.x--;
                    break;
                case CartDir::Right:
                    cart.x++;
                    break;
                case CartDir::Up:
                    cart.y--;
                    break;
                case CartDir::Down:
                    cart.y++;
                    break;
            }

            // check position
        }

        // check for collisions

        tick++;
    }
    
    return 0;
}