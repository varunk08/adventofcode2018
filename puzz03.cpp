#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct FabricPiece {
    int claimId;
    int startX;
    int startY;
    int width;
    int height;
};

FabricPiece ParseLine(const string& str) {
    FabricPiece piece;
    //cout << str << endl;
    string::size_type pos = str.find('@');
    //cout << str.substr(1, pos - 1) << endl;
    piece.claimId = stoi(str.substr(1, pos - 1));
    if (pos != string::npos) {
        pos++;
        //cout << str.substr(pos) << endl;
        string::size_type commaPos = str.find(',');
        if (commaPos != string::npos) {
            piece.startX = stoi(str.substr(pos, commaPos - pos));
            commaPos++;
            pos = str.find(':');
            if (pos != string::npos) {
                //cout << str.substr(pos) << endl;
                piece.startY = stoi(str.substr(commaPos, pos - commaPos));
                //cout << str.substr(commaPos, pos - commaPos) << endl;

                pos++;
                commaPos = str.find('x');
                if (commaPos != string::npos) {
                    piece.width = stoi(str.substr(pos, commaPos - pos));
                    //cout << str.substr(pos, commaPos - pos) << endl;
                    commaPos++;
                    piece.height = stoi(str.substr(commaPos, size(str) - commaPos));
                }
            }
        }
    }

    return piece;
}

int main(int argc, char** argv) {
    vector<FabricPiece> fabricPieces;
    string line;
    ifstream inputFile("puzz03Input.txt"); 
    if (inputFile.is_open()) {
        while(getline(inputFile, line)) {
            //cout << line << endl;
            fabricPieces.emplace_back(ParseLine(line));
        }
    } else {
        cout << "Failed to open file!" << endl;
    }

    /*for (auto piece : fabricPieces) {
        cout << "(" << piece.startX <<", " << piece.startY << ") : " << "w: " << piece.width <<", h: " << piece.height << endl;
    }*/
    
    int parentFabricDim = 1000;
    struct ClaimInfo {
        int numClaims;
        int claimId; // latest claim made
    };
    vector< vector <ClaimInfo> > claimGrid(parentFabricDim, vector <ClaimInfo>(parentFabricDim, ClaimInfo{0, -1}));
    
    for (auto piece : fabricPieces) {
        //cout << "(" << piece.startX <<", " << piece.startY << ") : " << "w: " << piece.width <<", h: " << piece.height << endl;
        for (unsigned int i = piece.startX; i < piece.startX + piece.width; i++) {
            for(unsigned int j = piece.startY; j < piece.startY + piece.height; j++) {
                claimGrid[i][j].numClaims++;
                claimGrid[i][j].claimId = piece.claimId;
            }
        }
    }

    int numOverlap = 0;
    int oneClaimId = -1;
    for (auto vec : claimGrid) {
        for (auto claim : vec) {
            if (claim.numClaims > 1) {
                numOverlap++;
            }
        }
    }

    for (auto piece : fabricPieces) {
        bool singleClaim = true;
        for (unsigned int i = piece.startX; (i < piece.startX + piece.width) && singleClaim; i++) {
            for(unsigned int j = piece.startY; (j < piece.startY + piece.height) && singleClaim; j++) {
                if (claimGrid[i][j].numClaims != 1) {
                    singleClaim = false;
                }
            }
        }

        if (singleClaim == true) {
            cout << "single claim id: " << piece.claimId << endl;
        }
    }

    cout << "num overlap twice or more: " << numOverlap << endl;
    return 0;
}