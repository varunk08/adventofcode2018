#include <unordered_set>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <string>
#include <bitset>

using namespace std;
using uint = unsigned int;
using Reg = long long int;
struct Inst {
    int A; int B; int C;
};

typedef vector<Reg> (*FuncPtr) (vector<Reg> before, Inst curInst);

const uint ip = 5;
struct Instruction {
    string inst; int A; int B; int C;
};

vector<Instruction> input = {
{"seti", 123    ,  0       , 1}, // 0 
{"bani", 1      ,  456     , 1}, // 1
{"eqri", 1      ,  72      , 1}, // 2
{"addr", 1      ,  5       , 5}, // 3
{"seti", 0      ,  0       , 5}, // 4
{"seti", 0      ,  0       , 1}, // 5
{"bori", 1      ,  65536   , 2}, // 6
{"seti", 6663054,  1       , 1}, // 7
{"bani", 2      ,  255     , 4}, // 8
{"addr", 1      ,  4       , 1}, // 9
{"bani", 1      ,  16777215, 1}, // 10
{"muli", 1      ,  65899   , 1}, // 11
{"bani", 1      ,  16777215, 1}, // 12
{"gtir", 256    ,  2       , 4}, // 13
{"addr", 4      ,  5       , 5}, // 14 
{"addi", 5      ,  1       , 5}, // 15
{"seti", 19     ,  6       , 5}, // 16
/*
{"seti", 0      ,  6       , 4}, //17
{"addi", 4      ,  1       , 3}, //18
{"muli", 3      ,  256     , 3}, //19
{"gtrr", 3      ,  2       , 3}, //20
{"addr", 3      ,  5       , 5}, //21
{"addi", 5      ,  1       , 5}, //22
{"seti", 25     ,  9       , 5}, //23
{"addi", 4      ,  1       , 4}, //24
{"seti", 17     ,  3       , 5}, //25
{"setr", 4      ,  4       , 2}, //26
*/
{"divi", 2      ,  256     , 2}, //27
{"setr", 2      ,  0       , 4}, //28
{"seti", 7      ,  2       , 5}, //29
{"eqrr", 1      ,  0       , 4}, //30
{"addr", 4      ,  5       , 5}, //31
{"seti", 5      ,  8       , 5}, //32
};

// finds divisor
vector<Reg> divi(vector<Reg> before, Inst curInst) { vector<Reg> afterVals = before; afterVals[curInst.C] = (before[curInst.A] / curInst.B); return afterVals;
}
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
    instMap.emplace("divi", &divi);
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
    registers[0] = 10114162;
    long long int  instPtr = 0;
    unordered_set<int> bVals;

    while(instPtr >= 0 && instPtr < input.size()) {
        registers[ip] = instPtr;
   
        //cout << instPtr << ": ";
        int newInstPtr = Execute(instMap,
                                 input,
                                 instPtr,
                                 registers);
        
        instPtr = newInstPtr;
        instPtr++;        
        if (instPtr == 22) {
            cout << registers[1] << endl;
            auto it = bVals.find(registers[1]);
            if (it == bVals.end()) {
                bVals.emplace(registers[1]);
            } else {
                cout << "repeat: " << registers[1] << endl;
                break;
            }
        }
    }
 
    for (auto i : registers) {
        cout << i << " ";
    }
    cout << endl;
    
    return 0;
}