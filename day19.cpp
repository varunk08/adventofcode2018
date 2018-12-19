#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <string>

using namespace std;
using uint = unsigned int;
using Reg = long long int;
struct Inst {
    int A; int B; int C;
};

typedef vector<Reg> (*FuncPtr) (vector<Reg> before, Inst curInst);

const uint ip = 3;
struct Instruction {
    string inst; int A; int B; int C;
};

vector<Instruction> input = {
{"addi", 3, 16, 3},
{"seti", 1, 3 , 2},
{"seti", 1, 0 , 5},
{"mulr", 2, 5 , 1},
{"eqrr", 1, 4 , 1},
{"addr", 1, 3 , 3},
{"addi", 3, 1 , 3},
{"addr", 2, 0 , 0},
{"addi", 5, 1 , 5},
{"gtrr", 5, 4 , 1},
{"addr", 3, 1 , 3},
{"seti", 2, 2 , 3},
{"addi", 2, 1 , 2},
{"gtrr", 2, 4 , 1},
{"addr", 1, 3 , 3},
{"seti", 1, 1 , 3},
{"mulr", 3, 3 , 3},
{"addi", 4, 2 , 4},
{"mulr", 4, 4 , 4},
{"mulr", 3, 4 , 4},
{"muli", 4, 11, 4},
{"addi", 1, 4 , 1},
{"mulr", 1, 3 , 1},
{"addi", 1, 2 , 1},
{"addr", 4, 1 , 4},
{"addr", 3, 0 , 3},
{"seti", 0, 2 , 3},
{"setr", 3, 6 , 1},
{"mulr", 1, 3 , 1},
{"addr", 3, 1 , 1},
{"mulr", 3, 1 , 1},
{"muli", 1, 14, 1},
{"mulr", 1, 3 , 1},
{"addr", 4, 1 , 4},
{"seti", 0, 6 , 0},
{"seti", 0, 9 , 3},
};


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
void FillFuncTable(unordered_map<string, FuncPtr>& instMap) {
    instMap.emplace("addr", &addr);
    instMap.emplace("addi", &addi);
    instMap.emplace("mulr", &mulr);
    instMap.emplace("muli", &muli);
    instMap.emplace("banr", &banr);
    instMap.emplace("bani", &bani);
    instMap.emplace("borr", &borr);
    instMap.emplace("bori", &bori);
    instMap.emplace("setr", &setr);
    instMap.emplace("seti", &seti);
    instMap.emplace("gtir", &gtir);
    instMap.emplace("gtri", &gtri);
    instMap.emplace("gtrr", &gtrr);
    instMap.emplace("eqir", &eqir);
    instMap.emplace("eqri", &eqri);
    instMap.emplace("eqrr", &eqrr);
}
long long int Execute(unordered_map<string, FuncPtr>& instMap,
            vector<Instruction>& insts,
            long long int curInst,
            vector<Reg>& registers) {
    auto it = instMap.find(input[curInst].inst);
    if (it != instMap.end()) {
        Inst curOpInst = {
            input[curInst].A,
            input[curInst].B,
            input[curInst].C
        };
        registers = it->second(registers, curOpInst);
    }
    return registers[ip];
}

int main(int argc, char** argv) {
    vector<FuncPtr> funcTable;
    unordered_map<string, FuncPtr> instMap;
    FillFuncTable(instMap);
    vector<Reg> registers(6, 0);
    registers[0] = 1;
    long long int  instPtr = 0;

    while(instPtr >= 0 && instPtr < input.size()) {
        registers[ip] = instPtr;
   
        cout << instPtr << ": ";
        int newInstPtr = Execute(instMap,
                                 input,
                                 instPtr,
                                 registers);
        for (auto i : registers) {
            cout << i << " ";
        }
        cout << endl;
        instPtr = newInstPtr;
        instPtr++;
    }
 
    for (auto i : registers) {
        cout << i << " ";
    }
    cout << endl;
    
    return 0;
}