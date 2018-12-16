#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using Reg = int;
struct Inst {
    int op; int A; int B; int C;
};

typedef vector<Reg> (*FuncPtr) (vector<Reg> before, Inst curInst);

//addr (add register) stores into register C the result of adding register A and register B.
vector<Reg> addr(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A] + before[curInst.B]; return afterVals;
}
//addi (add immediate) stores into register C the result of adding register A and value B.
vector<Reg> addi(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A] + curInst.B; return afterVals;
}
// mulr (multiply register) stores into register C the result of multiplying register A and register B.
vector<Reg> mulr(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A] * before[curInst.B]; return afterVals;
}
// muli (multiply immediate) stores into register C the result of multiplying register A and value B.
vector<Reg> muli(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A] * curInst.B; return afterVals;
}
//banr (bitwise AND register) stores into register C the result of the bitwise AND of register A and register B.
vector<Reg> banr(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A] & before[curInst.B]; return afterVals;
}
//bani bani (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and value B.
vector<Reg> bani(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A] & curInst.B; return afterVals;
}
// borr (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
vector<Reg> borr(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A] | before[curInst.B]; return afterVals;
}
// bori (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and value B.
vector<Reg> bori(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A] | curInst.B; return afterVals;
}
// setr (set register) copies the contents of register A into register C. (Input B is ignored.)
vector<Reg> setr(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = before[curInst.A]; return afterVals;
}
// seti (set immediate) stores value A into register C. (Input B is ignored.)
vector<Reg> seti(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = curInst.A; return afterVals;
}
//gtir (greater-than immediate/register) sets register C to 1 if value A is greater than register B. Otherwise, register C is set to 0.
vector<Reg> gtir(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = (curInst.A > before[curInst.B]); return afterVals;
}
// gtri (greater-than register/immediate) sets register C to 1 if register A is greater than value B. Otherwise, register C is set to 0.
vector<Reg> gtri(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = (before[curInst.A] > curInst.B); return afterVals;
}
// gtrr (greater-than register/register) sets register C to 1 if register A is greater than register B. Otherwise, register C is set to 0.
vector<Reg> gtrr(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = (before[curInst.A] > before[curInst.B]); return afterVals;
}    
// eqir (equal immediate/register) sets register C to 1 if value A is equal to register B. Otherwise, register C is set to 0.
vector<Reg> eqir(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = (curInst.A == before[curInst.B]); return afterVals;
}
// eqri (equal register/immediate) sets register C to 1 if register A is equal to value B. Otherwise, register C is set to 0.
vector<Reg> eqri(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = (before[curInst.A] == curInst.B); return afterVals;
}
// eqrr (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0
vector<Reg> eqrr(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = (before[curInst.A] == before[curInst.B]); return afterVals;
}

void FillFuncTable(vector<FuncPtr>& funcTable) {
    funcTable.push_back(&addr);
    funcTable.push_back(&addi);
    funcTable.push_back(&mulr);
    funcTable.push_back(&muli);
    funcTable.push_back(&banr);
    funcTable.push_back(&bani);
    funcTable.push_back(&borr);
    funcTable.push_back(&bori);
    funcTable.push_back(&setr);
    funcTable.push_back(&seti);
    funcTable.push_back(&gtir);
    funcTable.push_back(&gtri);
    funcTable.push_back(&gtrr);
    funcTable.push_back(&eqir);
    funcTable.push_back(&eqri);
    funcTable.push_back(&eqrr);
}

bool SampleSatisfiesThreeInst(vector<Reg> before, Inst curInst, vector<Reg>& after, vector<FuncPtr>& funcTable) {
    int instCount = 0;
    vector<Reg> test(4,0);
    for (int i = 0; i < funcTable.size(); i++) {
        test = funcTable[i](before, curInst);
        //cout << test[0] << " " << test[1] << " " << test[2] << " " << test[3] << endl;
        if (test == after) {
            instCount++;
        }
    }
    return (instCount >= 3);
}

int main(int argc, char** argv) {
    vector<FuncPtr> funcTable;
    FillFuncTable(funcTable);
    cout <<"Num instructions: " << funcTable.size() << endl;
    int numInstExceedThreeSamples = 0;
    
    ifstream inFile("day16Input.txt");
    if (inFile.is_open()) {
        string line;
        int lineNum = 0;
        vector<Reg> before(4, 0);
        vector<Reg> after(4, 0);
        Inst curInst;
        while(getline(inFile, line)) {
            lineNum %= 4;            
            switch(lineNum) {
                case 0: {
                    // Before state;
                    int begin = line.find('[');
                    int end = line.find(',');
                    before[0] = stoi(line.substr(begin + 1, end - begin - 1));
                    begin = end;
                    end = line.find(',', begin + 1);
                    before[1] = stoi(line.substr(begin + 1, end - begin - 1));
                    begin = end;
                    end = line.find(',', begin + 1);
                    before[2] = stoi(line.substr(begin + 1, end - begin - 1));
                    begin = end;
                    end = line.find(']', begin + 1);
                    before[3] = stoi(line.substr(begin + 1, end - begin - 1));
                    break;
                }
                case 1: {
                    int begin = 0;
                    int end = line.find(' ');
                    curInst.op = stoi(line.substr(begin, end - begin));
                    begin = end + 1;
                    end = line.find(' ', begin);
                    curInst.A = stoi(line.substr(begin, end - begin));
                    begin = end + 1;
                    end = line.find(' ', begin);
                    curInst.B = stoi(line.substr(begin, end - begin));
                    begin = end + 1;
                    end = line.find(' ', begin);
                    curInst.C = stoi(line.substr(begin, end - begin));
                    //cout << curInst.A << " " << curInst.B << " " << curInst.C << " " << curInst.op << endl;
                    break;
                }
                case 2: {
                    // After state;
                    int begin = line.find('[');
                    int end = line.find(',');
                    after[0] = stoi(line.substr(begin + 1, end - begin - 1));
                    begin = end;
                    end = line.find(',', begin + 1);
                    after[1] = stoi(line.substr(begin + 1, end - begin - 1));
                    begin = end;
                    end = line.find(',', begin + 1);
                    after[2] = stoi(line.substr(begin + 1, end - begin - 1));
                    begin = end;
                    end = line.find(']', begin + 1);
                    after[3] = stoi(line.substr(begin + 1, end - begin - 1));
                    //cout << after[0] << " " << after[1] << " " << after[2] << " " << after[3] << endl;
                    break;
                }
                case 3:
                    // Process the instructions and keep a count on them
                    numInstExceedThreeSamples += (SampleSatisfiesThreeInst(before, curInst, after, funcTable) == true) ? 1 : 0;
                    break;
            }

            lineNum++;
        }
    }

    cout << "Num samples exceeding three inst: " << numInstExceedThreeSamples << endl;
    return 0;
}