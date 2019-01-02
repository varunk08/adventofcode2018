#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

//Immune System:
//790 units each with 3941 hit points with an attack that does 48 bludgeoning damage at initiative 5
//624 units each with 2987 hit points with an attack that does 46 bludgeoning damage at initiative 16
//5724 units each with 9633 hit points (immune to bludgeoning, slashing, fire) with an attack that does 16 slashing damage at initiative 9
//1033 units each with 10664 hit points with an attack that does 89 slashing damage at initiative 1
//6691 units each with 9773 hit points (weak to slashing) with an attack that does 13 bludgeoning damage at initiative 12
//325 units each with 11916 hit points (weak to bludgeoning) with an attack that does 276 slashing damage at initiative 8
//1517 units each with 6424 hit points with an attack that does 35 bludgeoning damage at initiative 13
//1368 units each with 9039 hit points (immune to bludgeoning) with an attack that does 53 slashing damage at initiative 4
//3712 units each with 5377 hit points (immune to cold, radiation; weak to fire) with an attack that does 14 slashing damage at initiative 14
//3165 units each with 8703 hit points (weak to slashing, bludgeoning) with an attack that does 26 radiation damage at initiative 11
//
//Infection:
//1113 units each with 44169 hit points (immune to bludgeoning; weak to radiation) with an attack that does 57 fire damage at initiative 7
//3949 units each with 20615 hit points (weak to radiation, cold) with an attack that does 9 bludgeoning damage at initiative 6
//602 units each with  35167 hit points (immune to bludgeoning, cold; weak to fire) with an attack that does 93 radiation damage at initiative 20
//1209 units each with 34572 hit points with an attack that does 55 bludgeoning damage at initiative 3
//902 units each with  12983 hit points (immune to fire) with an attack that does 28 fire damage at initiative 19
//1132 units each with 51353 hit points with an attack that does 66 radiation damage at initiative 15
//7966 units each with 49894 hit points (immune to bludgeoning) with an attack that does 9 cold damage at initiative 10
//3471 units each with 18326 hit points (weak to radiation) with an attack that does 8 fire damage at initiative 18
//110 units each with  38473 hit points (weak to bludgeoning; immune to fire) with an attack that does 640 slashing damage at initiative 2
//713 units each with  42679 hit points (weak to slashing) with an attack that does 102 bludgeoning damage at initiative 17

enum AtkType {
    none            = 0x0,
    bludgeoning     = 0x1,
    slashing        = 0x2,
    fire            = 0x4,
    radiation       = 0x8,
    cold            = 0x10
};

struct Group {
    int numUnits;
    int unitHp;
    int attack;             // per unit attack
    AtkType atkType;
    int initiative;
    int immunity;
    int weakness;
};

int main(int argc, char** argv) {
    vector<Group> immuneSys = {
        { 790 ,  3941,   48,  bludgeoning,  5, none, none},
        { 624 ,  2987,   46,  bludgeoning, 16, none, none},
        { 5724,  9633,   16,  slashing,     9, bludgeoning | slashing | fire, none},
        { 1033,  10664,  89,  slashing,     1, none, none},
        { 6691,  9773,   13,  bludgeoning, 12, none, slashing},
        { 325 ,  11916, 276,  slashing,     8, none, bludgeoning},
        { 1517,  6424,   35,  bludgeoning, 13, none, none},
        { 1368,  9039,   53,  slashing,     4, bludgeoning, none},
        { 3712,  5377,   14,  slashing,    14, cold | radiation, fire},
        { 3165,  8703,   26,  radiation,   11, none, slashing | bludgeoning},
    };

    vector<Group> infection = {
        { 1113,  44169, 57 , fire       , 7,  bludgeoning, radiation},
        { 3949,  20615, 9  , bludgeoning, 6,  none, radiation | cold},
        { 602 ,  35167, 93 , radiation  , 20, bludgeoning | cold, fire},
        { 1209,  34572, 55 , bludgeoning, 3,  none, none},
        { 902 ,  12983, 28 , fire       , 19, fire, none},
        { 1132,  51353, 66 , radiation  , 15, none, none},
        { 7966,  49894, 9  , cold       , 10, bludgeoning, none},
        { 3471,  18326, 8  , fire       , 18, none, radiation},
        { 110 ,  38473, 640, slashing   , 2 , fire, bludgeoning},
        { 713 ,  42679, 102, bludgeoning, 17, none, slashing},
    };

    vector<int> immuneSysTargets(immuneSys.size(), 0);
    vector<int> infectionTargets(infection.size(), 0);

    sort(immuneSys.begin(), immuneSys.end(), [](const Group& A, const Group& B) {
        if ((A.numUnits * A.attack) == (B.numUnits * B.attack)) {
            return A.initiative > B.initiative;
        } else {
            return ((A.numUnits * A.attack) > (B.numUnits * B.attack));
        }
    });

    // target selection in decreasing order of effective power
    for (int i = 0; i < immuneSys.size(); i++) {
        Group& curGroup = immuneSys[i];
        cout << curGroup.numUnits * curGroup.attack << endl;
    }

    return 0;
}