#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <unordered_set>
#include <deque>

using namespace std;

struct Step {
    char id;
    vector<char>* pDeps;
};

void ParseStep(string& line, list<Step>& steps, unordered_set<char>& processed) {
    char right = line.substr(36, 1)[0];
    char left = line.substr(5, 1)[0];
    
    auto iter = processed.find(left); 
    if (iter == processed.end()) {
        processed.emplace(left);
        // create a node
        Step step = {};
        step.id = left;
        step.pDeps = new vector<char>(0);
        steps.push_back(step);
    }

    iter = processed.find(right);
    if (iter == processed.end()) {
        processed.emplace(right);
        // create a node
        Step step = {};
        step.id = right;
        step.pDeps = new vector<char>(0);
        steps.push_back(step);
    }

    for (list<Step>::iterator listIt = steps.begin(); listIt != steps.end(); ++listIt) {
        //cout << listIt->id << " ";
        if (listIt->id == right) {
            listIt->pDeps->push_back(left);
        }
    }
    //cout << endl;
}

int main(int argc, char** argv) {
    ifstream inFile("day07Input.txt");
    string line;
    list <Step> unfinishedSteps;
    unordered_set<char> processed;

    if (inFile.is_open()) {
        while(getline(inFile, line)) {
            ParseStep(line, unfinishedSteps, processed);
        }
    } else {
        cout << "error opening file" << endl;
    }

    unfinishedSteps.sort([](const Step& A, const Step& B) {
        return A.id < B.id;
    });
    
    for (auto it = unfinishedSteps.begin(); it != unfinishedSteps.end(); ++it) {
        cout << it->id << ": " << it->pDeps->size() << " ";
        for (auto id : *it->pDeps) {
            cout << id << " ";
        }
        cout << endl;
    }

   /* unordered_set<char> finished;
    while(unfinishedSteps.size() > 0) {

        // pick next available
        char curProcessed = '0';
        for (list<Step>::iterator listIt = unfinishedSteps.begin(); listIt != unfinishedSteps.end(); listIt++) {
            if (listIt->pDeps->size() == 0) {
                unfinishedSteps.erase(listIt);
                cout << listIt->id;
                curProcessed = listIt->id;
                break;
            }
        }

        // clean up other steps that are dependent
        for (list<Step>::iterator listIt = unfinishedSteps.begin(); listIt != unfinishedSteps.end(); listIt++) {
            if(listIt->pDeps->size() > 0) {
                vector<char>& deps = *listIt->pDeps;                
                for (auto it = deps.begin(); it != deps.end(); ) {
                    if (*it == curProcessed) {
                        it = deps.erase(it);
                    } else {
                        it++;
                    }
                }
            }
        }
    }
*/
    struct Worker {
        bool occupied;
        int counter;
        char curId;
    };

    Worker dummy = {false, 0, '0'};
    vector<Worker> workers;
    workers.push_back(dummy);
    workers.push_back(dummy);
    workers.push_back(dummy);
    workers.push_back(dummy);
    workers.push_back(dummy);
    
    int timeCtr = 0;
    deque<char> nextSteps;

    while(unfinishedSteps.size() > 0) {
        // fetch all that are available
                cout << "size of list : " <<  unfinishedSteps.size() << endl;
        for (list<Step>::iterator listIt = unfinishedSteps.begin(); listIt != unfinishedSteps.end();) {
            cout << listIt->pDeps->size() << endl;
            if (listIt->pDeps->size() == 0) {
                char id = listIt->id;
                cout << "removing from list: " << listIt->id << endl;
                nextSteps.push_back(listIt->id);
                listIt = unfinishedSteps.erase(listIt);
            } else {          
                listIt++;
            }
        } 
        cout << "size of list : " <<  unfinishedSteps.size() << endl;
        cout << "size of queue: " << nextSteps.size() << endl;

        // schedule to available worker
        for (int i = 0; i < size(workers); i++) {
            if (workers[i].occupied == false) {
                if (nextSteps.size() > 0) {
                    workers[i].occupied = true;
                    workers[i].curId = nextSteps.front();
                    nextSteps.pop_front();
                    cout << "sched: " << workers[i].curId << endl;
                    workers[i].counter = 60 + static_cast<int>(workers[i].curId) - 64;
                    cout << "set ctr: " << workers[i].counter << endl;
                }                
            }
        }

        for (auto worker : workers) {
            cout << "occupied: " << worker.occupied << " " << worker.curId << endl;
        }

        // start working
        bool someFin = false;
        cout << "working" << endl;
        while (someFin == false) {
            timeCtr++;
            for(int i=0; i < size(workers); i++) {
                cout << "worker: " << workers[i].counter << endl;
                if (workers[i].occupied == true) {
                    workers[i].counter--;
                    cout << "dec worker " << workers[i].curId << ": " << workers[i].counter << endl;
                    
                    if (workers[i].counter == 0) {
                        someFin = true;
                        workers[i].occupied = false;
                        workers[i].counter = 0;

                        cout <<"worker finished " << timeCtr << ": " << workers[i].curId << endl;

                        for (list<Step>::iterator lit = unfinishedSteps.begin(); lit != unfinishedSteps.end(); lit++) {
                            if(lit->pDeps->size() > 0) {
                                vector<char>& deps = *lit->pDeps;
                                for (auto it = deps.begin(); it != deps.end(); ) {
                                    if (*it == workers[i].curId) {
                                        it = deps.erase(it);
                                    } else {
                                        it++;
                                    }
                                }
                            }
                        }

                    }
                }
            } 
            cout << "time: " <<  timeCtr << endl;
        }

        //break;
    }

    return 0;
}