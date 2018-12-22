#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    

    int b = 0;
    int a = 0;
    int d = 0;
    int e = 0;
    int c = 0;
    c = b | 65536;
    b = 6663054;
    while (b != a) {        
        e = c & 255;
        b = b + e;
        b = b & 16777215;
        b *= 65899;
        b = b & 16777215;

        if (c >= 256) {
            c = c / 256;
            e = c;
        } else {
            c = b | 65536;
            b = 6663054;
        }
        cout << b << endl;
    }
    return 0;
}