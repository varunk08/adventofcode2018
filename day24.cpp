#include <cmath>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <limits>

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

enum AtkType
{
    none = 0x0,
    bludgeoning = 0x1,
    slashing = 0x2,
    fire = 0x4,
    radiation = 0x8,
    cold = 0x10
};

struct Group
{
    int id;
    int numUnits;
    int unitHp;
    int attack; // per unit attack
    AtkType atkType;
    int initiative;
    int immunity;
    int weakness;
    int attackId;
    int defId; // Need to defend against this enemy group
};

void Target(vector<Group> &attackers, vector<Group> &defenders, unordered_map<int, Group *> &groupIdMap)
{
    for (int i = 0; i < attackers.size(); i++)
    {
        int maxDmg = numeric_limits<int>::min();
        int prevInitiative = numeric_limits<int>::min();
        int prevEffPower = numeric_limits<int>::min();
        Group &curAttacker = attackers[i];
        int curDmg = 0;

        if (curAttacker.numUnits <= 0)
        {
            continue;
        }

        for (int j = 0; j < defenders.size(); j++)
        {
            curDmg = curAttacker.numUnits * curAttacker.attack;
            Group &curDefender = defenders[j];
            int enemyEffPwr = curDefender.numUnits * curDefender.attack;

            if (curDefender.weakness & curAttacker.atkType)
            {
                curDmg += curDmg;
            }

            if ((curDefender.immunity & curAttacker.atkType) || (curDefender.numUnits <= 0) || (curDefender.defId != -1))
            {
                continue;
            }

            if (maxDmg <= curDmg)
            {
                if (maxDmg == curDmg)
                {
                    if ((enemyEffPwr < prevEffPower) ||
                        (enemyEffPwr == prevEffPower && prevInitiative > curDefender.initiative))
                    {
                        continue;
                    }
                }

                if (curAttacker.attackId != -1)
                {
                    groupIdMap.find(curAttacker.attackId)->second->defId = -1;
                }

                prevInitiative = curDefender.initiative;
                maxDmg = curDmg;
                curAttacker.attackId = curDefender.id;
                prevEffPower = enemyEffPwr;
                curDefender.defId = curAttacker.id;
            }
        }
    }
}

void PerformAttack(Group *pAttacker, Group *pDefender)
{
    if (pAttacker->numUnits <= 0 || pAttacker->attackId == -1 || pDefender->numUnits <= 0)
    {
        return;
    }

    int curDmg = pAttacker->numUnits * pAttacker->attack;
    if (pDefender->immunity & pAttacker->atkType || pDefender->numUnits <= 0)
    {
        curDmg = 0;
    }
    if (pDefender->weakness & pAttacker->atkType)
    {
        curDmg += curDmg;
    }
    int before = pDefender->numUnits;
    int remHp = (pDefender->numUnits * pDefender->unitHp) - curDmg;
    pDefender->numUnits = ceil(static_cast<float>(remHp) / pDefender->unitHp);

    if (pDefender->numUnits <= 0)
    {
        pDefender->defId = -1;
        pAttacker->attackId = -1;
    }
    // cout << pAttacker->id << "->" << pDefender->id <<  ": num units rem: " << pDefender->numUnits << "/ " << before << endl;
}
const int boost = 1570;
int main(int argc, char **argv)
{
    vector<Group> immuneSys = {
        {1, 790, 3941, 48, bludgeoning, 5, none, none, -1, -1},
        {2, 624, 2987, 46, bludgeoning, 16, none, none, -1, -1},
        {3, 5724, 9633, 16, slashing, 9, bludgeoning | slashing | fire, none, -1, -1},
        {4, 1033, 10664, 89, slashing, 1, none, none, -1, -1},
        {5, 6691, 9773, 13, bludgeoning, 12, none, slashing, -1, -1},
        {6, 325, 11916, 276, slashing, 8, none, bludgeoning, -1, -1},
        {7, 1517, 6424, 35, bludgeoning, 13, none, none, -1, -1},
        {8, 1368, 9039, 53, slashing, 4, bludgeoning, none, -1, -1},
        {9, 3712, 5377, 14, slashing, 14, cold | radiation, fire, -1, -1},
        {10, 3165, 8703, 26, radiation, 11, none, slashing | bludgeoning, -1, -1},
        // test
        // {1, 17, 5390, 4507, fire, 2, none, radiation | bludgeoning, -1   , -1},
        // {2, 989, 1274, 25, slashing, 3, fire, bludgeoning | slashing , -1, -1},
    };

    vector<Group> infection = {
        {11, 1113, 44169, 57, fire, 7, bludgeoning, radiation, -1, -1},
        {12, 3949, 20615, 9, bludgeoning, 6, none, radiation | cold, -1, -1},
        {13, 602, 35167, 93, radiation, 20, bludgeoning | cold, fire, -1, -1},
        {14, 1209, 34572, 55, bludgeoning, 3, none, none, -1, -1},
        {15, 902, 12983, 28, fire, 19, fire, none, -1, -1},
        {16, 1132, 51353, 66, radiation, 15, none, none, -1, -1},
        {17, 7966, 49894, 9, cold, 10, bludgeoning, none, -1, -1},
        {18, 3471, 18326, 8, fire, 18, none, radiation, -1, -1},
        {19, 110, 38473, 640, slashing, 2, fire, bludgeoning, -1, -1},
        {20, 713, 42679, 102, bludgeoning, 17, none, slashing, -1, -1},
        // test
        // {3, 801, 4706, 116, bludgeoning, 1, none, radiation    , -1, -1 },
        // {4, 4485, 2961, 12, slashing, 4, radiation, fire | cold, -1, -1 },
    };

    unordered_map<int, Group *> groupIdMap;
    bool enemyRemains = true;
    int round = 0;
    int bonus = 35;
    bool immuneDiedFirst = true;

    vector<Group> tempImmune = immuneSys;
    vector<Group> tempInfect = infection;

    while (immuneDiedFirst)
    {
        bonus++;
        cout << "Bonus: " << bonus << endl;

        immuneSys = tempImmune;
        infection = tempInfect;

        for (Group &grp : immuneSys)
        {
            grp.attack += bonus;
        }
        round = 0;
        enemyRemains = true;

        while (enemyRemains)
        {
            ++round;
            // cout << endl << "Round " << round << endl;

            // sort by effective power
            auto sortByEffPwr = [](const Group &A, const Group &B) {
                if ((A.numUnits * A.attack) == (B.numUnits * B.attack))
                {
                    return A.initiative > B.initiative;
                }
                else
                {
                    return ((A.numUnits * A.attack) > (B.numUnits * B.attack));
                }
            };
            sort(immuneSys.begin(), immuneSys.end(), sortByEffPwr);
            sort(infection.begin(), infection.end(), sortByEffPwr);

            // clear the attackers and defenders
            for (Group &grp : immuneSys)
            {
                grp.attackId = -1;
                grp.defId = -1;
            }
            for (Group &grp : infection)
            {
                grp.attackId = -1;
                grp.defId = -1;
            }
            groupIdMap.clear();
            for (Group &grp : immuneSys)
            {
                groupIdMap.emplace(grp.id, &grp);
            }
            for (Group &grp : infection)
            {
                groupIdMap.emplace(grp.id, &grp);
            }

            // target phase
            Target(immuneSys, infection, groupIdMap);
            Target(infection, immuneSys, groupIdMap);

            // attack phase
            auto sortByInit = [](const Group &A, const Group &B) { return A.initiative > B.initiative; };
            sort(immuneSys.begin(), immuneSys.end(), sortByInit);
            sort(infection.begin(), infection.end(), sortByInit);

            groupIdMap.clear();
            for (Group &grp : immuneSys)
            {
                groupIdMap.emplace(grp.id, &grp);
            }
            for (Group &grp : infection)
            {
                groupIdMap.emplace(grp.id, &grp);
            }

            int i = 0, j = 0;
            while (i < immuneSys.size() && j < infection.size())
            {
                int initA = immuneSys[i].initiative;
                int initB = infection[j].initiative;
                Group *pAttacker = nullptr;
                Group *pDefender = nullptr;
                if (initA > initB)
                {
                    pAttacker = &immuneSys[i];
                    if (pAttacker->attackId != -1)
                    {
                        pDefender = groupIdMap.find(pAttacker->attackId)->second;
                    }
                    i++;
                }
                else
                {
                    pAttacker = &infection[j];
                    if (pAttacker->attackId != -1)
                    {
                        pDefender = groupIdMap.find(pAttacker->attackId)->second;
                    }
                    j++;
                }

                PerformAttack(pAttacker, pDefender);
            }

            vector<Group> *pRem = nullptr;
            vector<Group> *pDef = nullptr;
            if (i == immuneSys.size())
            {
                pRem = &infection;
                pDef = &immuneSys;
                i = j;
            }
            else
            {
                pRem = &immuneSys;
                pDef = &infection;
            }

            while (i < pRem->size())
            {
                Group *pAttacker = &pRem->at(i);
                Group *pDefender = nullptr;
                if (pAttacker->numUnits <= 0)
                {
                    break;
                }
                if (pAttacker->attackId != -1)
                {
                    pDefender = groupIdMap.find(pAttacker->attackId)->second;
                }
                PerformAttack(pAttacker, pDefender);
                i++;
            }

            // debug print
            // cout << "immune units: " << endl;
            // for (Group& grp : immuneSys) {
            //     cout << grp.numUnits << endl;
            // }
            // cout << "infection units: " << endl;
            // for (Group& grp : infection) {
            //     cout << grp.numUnits << endl;
            // }

            // check if units remain
            enemyRemains = false;
            immuneDiedFirst = false;
            for (Group &grp : immuneSys)
            {
                if (grp.numUnits > 0)
                {
                    enemyRemains = true;
                    break;
                }
            }

            if (enemyRemains == false)
            {
                immuneDiedFirst = true;
                // cout << "Immune died first" << endl;
                break;
            }

            enemyRemains = false;
            for (Group &grp : infection)
            {
                if (grp.numUnits > 0)
                {
                    enemyRemains = true;
                    break;
                }
            }

            if (enemyRemains == false)
            {
                cout << "Infection died first" << endl;
                cout << "Boost: " << bonus << endl;
                immuneDiedFirst = false;
                break;
            }
        }
    }

    cout << "immune units: " << endl;
    for (Group &grp : immuneSys)
    {
        cout << grp.numUnits << endl;
    }
    cout << "infection units: " << endl;
    for (Group &grp : infection)
    {
        cout << grp.numUnits << endl;
    }

    int score = 0;
    for (Group &grp : immuneSys)
    {
        if (grp.numUnits > 0)
        {
            score += grp.numUnits;
        }
    }
    for (Group &grp : infection)
    {
        if (grp.numUnits > 0)
        {
            score += grp.numUnits;
        }
    }
    cout << "Score: " << score << endl;

    return 0;
}