#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <list>

using namespace std;

enum class TrackObject {
    RailHor      = 0,
    RailVer      = 1,
    FwdSlash     = 2, // top left or bottom right
    BwdSlash     = 3, // top righ or bottom left
    Intersection = 4,
    UpCart       = 5,
    DownCart     = 6,
    LeftCart     = 7,
    RightCart    = 8,
    None
};

enum class CartDir {
    Up    = 0,
    Right = 1,
    Down  = 2,
    Left  = 3
};

enum class IntersectionMove {
    Left      = 0,
    Straight  = 1,
    Right     = 2,
    None
};

struct Cart {
    int              x;
    int              y;
    CartDir          dir;
    IntersectionMove prevTurn;

    Cart(int _x, int _y, CartDir _dir, IntersectionMove move) : x(_x), y(_y), dir(_dir), prevTurn(move) {}
};

struct MoveKey {
    CartDir     prevDir;
    TrackObject curObj;
    MoveKey(CartDir dir, TrackObject obj) : prevDir(dir), curObj(obj) {};

    bool operator==(const MoveKey& rhs) const {
        return (prevDir == rhs.prevDir) && (curObj == rhs.curObj);
    }
};

struct MoveValue {
    int     dx;
    int     dy;
    CartDir nextDir;

    MoveValue(int x, int y, CartDir next) : dx(x), dy(y), nextDir(next) {};
};

struct MoveHash {
     std::size_t operator () (const MoveKey& A) const {
         auto h1 = std::hash<int>{}(static_cast<int>(A.prevDir));
         auto h2 = std::hash<int>{}(static_cast<int>(A.curObj));
         return (h1 * 32768 >> 1) ^ (h2 << 1);
     }
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
            case '/': curLine.push_back(TrackObject::FwdSlash); break;
            case '\\' : curLine.push_back(TrackObject::BwdSlash); break;
            case '>':
                carts.emplace_back(i, lineNum, CartDir::Right, IntersectionMove::Right);
                curLine.push_back(TrackObject::RailHor);
                break;
            case '<':
                carts.emplace_back(i, lineNum, CartDir::Left, IntersectionMove::Right);
                curLine.push_back(TrackObject::RailHor);
                break;
            case 'v':
                carts.emplace_back(i, lineNum, CartDir::Down, IntersectionMove::Right);
                curLine.push_back(TrackObject::RailVer);
                break;
            case '^':
                carts.emplace_back(i, lineNum, CartDir::Up, IntersectionMove::Right);
                curLine.push_back(TrackObject::RailVer);
                break;
            default: break; 
        }
    }
    //cout << curLine.size() << " ";
    grid.push_back(curLine);
}

void RenderGrid(vector<vector<TrackObject>>& grid, vector<Cart>& carts) {
    cout << "Rendering grid: " << endl;
    vector<vector<TrackObject>> temp = grid;

    for(auto cart : carts) {
        switch (cart.dir) {
            case CartDir::Up: temp[cart.y][cart.x] = TrackObject::UpCart; break;
            case CartDir::Down: temp[cart.y][cart.x] = TrackObject::DownCart; break;
            case CartDir::Left: temp[cart.y][cart.x] = TrackObject::LeftCart; break;
            case CartDir::Right: temp[cart.y][cart.x] = TrackObject::RightCart; break;
        }
    }

    for (int y = 0; y < temp.size(); y++) {
        for (int x = 0; x < temp[y].size(); x++) {
            switch(temp[y][x]) {
                case TrackObject::RailHor     : cout << "-"; break;
                case TrackObject::RailVer     : cout <<"|" ; break;
                case TrackObject::FwdSlash    : cout <<"/" ; break;
                case TrackObject::BwdSlash    : cout <<"\\"; break;
                case TrackObject::Intersection: cout <<"+" ; break;
                case TrackObject::None        : cout <<" " ; break;
                case TrackObject::UpCart      : cout <<"^"; break;
                case TrackObject::DownCart    : cout <<"v"; break;
                case TrackObject::LeftCart    : cout <<"<"; break;
                case TrackObject::RightCart   : cout <<">"; break;
            }
        }
        cout << endl;
    }
}

void CreateMoveMap(unordered_map<MoveKey, MoveValue, MoveHash>& moveMap) {
    // fwd slash: /
    // bwd slash: \\
    // Right ->
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Right, TrackObject::FwdSlash),MoveValue(0, -1, CartDir::Up)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Right, TrackObject::BwdSlash),MoveValue(0, 1, CartDir::Down)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Right, TrackObject::RailHor),MoveValue(1, 0, CartDir::Right)));

    // Left <-
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Left, TrackObject::FwdSlash),MoveValue(0, 1, CartDir::Down)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Left, TrackObject::BwdSlash),MoveValue(0, -1, CartDir::Up)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Left, TrackObject::RailHor),MoveValue(-1, 0, CartDir::Left)));

    // Up ^
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Up, TrackObject::FwdSlash),MoveValue(1, 0, CartDir::Right)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Up, TrackObject::BwdSlash),MoveValue(-1, 0, CartDir::Left)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Up, TrackObject::RailVer),MoveValue(0, -1, CartDir::Up)));

    //  down v
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Down, TrackObject::FwdSlash),MoveValue(-1, 0, CartDir::Left)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Down, TrackObject::BwdSlash),MoveValue(1, 0, CartDir::Right)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Down, TrackObject::RailVer),MoveValue(0, 1, CartDir::Down)));

    // intersections just use the cur dir
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Up,   TrackObject::Intersection),MoveValue(0, -1, CartDir::Up)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Down, TrackObject::Intersection),MoveValue(0, 1, CartDir::Down)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Left, TrackObject::Intersection),MoveValue(-1, 0, CartDir::Left)));
    moveMap.emplace(std::make_pair(MoveKey(CartDir::Right,TrackObject::Intersection),MoveValue(1, 0, CartDir::Right)));

    // Print move map
    cout << quoted("Move map size: ") << moveMap.size() << endl;
}

void MoveCart(vector<vector<TrackObject>>& grid, unordered_map<MoveKey, MoveValue, MoveHash>& moveMap, Cart& cart) {
    int& x = cart.x;
    int& y = cart.y;
    bool inIntersection = false;

    // special handling for intersection
    if (grid[y][x] == TrackObject::Intersection) {
        inIntersection = true;
        int prevTurn = static_cast<int>(cart.prevTurn);
        int nextTurn = (prevTurn + 1) % 3;
        int nextDir = static_cast<int>(cart.dir) + (nextTurn - 1);
        nextDir %= 4;
        if (nextDir < 0) {
            nextDir = 3;
        }

        cart.dir = static_cast<CartDir>(nextDir); // find new direction in case of intersection
        cart.prevTurn = static_cast<IntersectionMove>(nextTurn);                 // save the intersection move
    } 
    
    MoveKey key = {cart.dir, grid[y][x]};
    auto it = moveMap.find(key);
    if (it != moveMap.end()) {        
        cart.x += it->second.dx;
        cart.y += it->second.dy;

        if (inIntersection == false) {
            cart.dir = it->second.nextDir;
        }
    }
}

int main(int argc, char** argv) {
    ifstream inFile("day13PuzzInput.txt");
    string line;
    vector<vector<TrackObject>> grid;
    vector<Cart> carts;
    unordered_map<MoveKey, MoveValue, MoveHash> moveMap;

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
    cout << "Num carts: " << carts.size() << endl;
    CreateMoveMap(moveMap);
    
    list<Cart> cartList;
    for (auto cart : carts) {
        cartList.push_back(cart);
    }
    cout << "cart list size: "<< cartList.size() << endl;

    bool collided = false;
    unsigned long long tick = 0;
    while(collided == false) {
        // sort the carts with y, break ties with x
        cartList.sort([](const Cart& A, const Cart& B) {
            if (A.y == B.y) {
                return (A.x < B.x);
            } else {
                return (A.y < B.y);
            }
        });
        
        auto iter = cartList.begin();

        while(iter != cartList.end()) {
            // update the position of each cart
            MoveCart(grid, moveMap, *iter);

            for (auto cIt = cartList.begin(); cIt != cartList.end(); cIt++) {
                if (cIt != iter) {
                    if (cIt->x == iter->x &&
                        cIt->y == iter->y) {
                        collided = true;
                        cout << "collision: " << cIt->x << ", " << cIt->y << endl;                        
                        cIt = cartList.erase(cIt);
                        iter = cartList.erase(iter);
                        break;
                    }
                }
            }
            
            //if (collided) {
            //    break;
            //}
            /*if (cartList.size() == 1) {
                auto iter = cartList.begin();
                cout << iter->x << ", " << iter->y << endl;
                break;
            }*/
            
            if (collided == false) {
                iter++;
            } else {
                collided = false; // reset this flag
            }
        }

        if (cartList.size() == 1) {
            auto iter = cartList.begin();
            cout << iter->x << ", " << iter->y << endl;
            break;
        }

        //RenderGrid(grid, carts);   
        tick++;
    }
    
    return 0;
}