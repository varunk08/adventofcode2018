#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

int main(int argc, char** argv) {
    ifstream inFile("puzz05Input.txt");
    string line;
    if (inFile.is_open()) {        
        getline(inFile, line);
        //cout << line << endl;       
    }

    //string test = "dabAcCaCBAcCcaDA";
    //string test = line;

    vector< vector<string> > symbols = {
        {"aA", "Aa"},
        {"bB", "Bb"},
        {"cC", "Cc"},
        {"dD", "Dd"},
        {"eE", "Ee"},
        {"fF", "Ff"},
        {"gG", "Gg"},
        {"hH", "Hh"},
        {"iI", "Ii"},
        {"jJ", "Jj"},
        {"kK", "Kk"},
        {"lL", "Ll"},
        {"mM", "Mm"},
        {"nN", "Nn"},
        {"oO", "Oo"},
        {"pP", "Pp"},
        {"qQ", "Qq"},
        {"rR", "Rr"},
        {"sS", "Ss"},
        {"tT", "Tt"} ,
        {"uU", "Uu"} ,
        {"vV", "Vv"},
        {"wW", "Ww"},
        {"xX", "Xx"},
        {"yY", "Yy"},
        {"zZ", "Zz"}
    };

    //vector<int> lengthAfterRemoval(23, 0);
    int shortestLength = numeric_limits<int>::max();

    for (int i = 0; i < 23; ++i) {

        string test = line;
        char small = 97 + i;
        char upper = 65 + i;
        cout << small << " " << upper << endl;
        string::size_type pos = test.find(small);
        while(pos != string::npos) {
            test.erase(pos, 1);
            pos = test.find(small, pos);
        }

        string::size_type pos2 = test.find(upper);
        while(pos2 != string::npos) {
            test.erase(pos2, 1);
            pos2 = test.find(upper, pos2);
        }

        bool collapsed = true;
        while(collapsed) {
            collapsed = false;
            for(auto pairs : symbols) {
                for (auto couple : pairs) {
                    string::size_type pos = test.find(couple);
                    while(pos != string::npos) {
                        collapsed = true;
                        test.erase(pos, 2);
                        pos = test.find(couple, pos);
                    }
                }
            }
        }

        if (shortestLength > test.size()) {
            shortestLength = test.size();
        }

    }
    
    //cout << test << " " << size(test) << endl;
    cout << "shortest: " << shortestLength << endl;

    return 0;
}