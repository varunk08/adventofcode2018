#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <unordered_map>
#include <limits>

using namespace std;

enum EventType {
    BeginShift = 0,
    FallAsleep = 1,
    WakeUp     = 2
};

struct Event {
    int month;
    int day;
    int hour;
    int min;
    int guardId;
    EventType type;
};

void ConstructEvent(vector<Event>& events, string str) {
    Event newEvent = {};
    string::size_type pos1 = str.find('-');
    pos1++;
    string::size_type pos2 = str.rfind('-');
    newEvent.month = stoi(str.substr(pos1, pos2 - pos1));

    pos2++;
    pos1 = str.find(' ');
    newEvent.day = stoi(str.substr(pos2, pos1 - pos2));

    pos1++;
    pos2 = str.find(':');
    newEvent.hour = stoi(str.substr(pos1, pos2 - pos1));

    pos2++;
    pos1 = str.find(']');
    newEvent.min = stoi(str.substr(pos2, pos1 - pos2));

    pos1 += 2;
    if (str[pos1] == 'G') {
        newEvent.type = BeginShift;
        string::size_type begin = str.find('#');
        string::size_type end = str.find('b');
        begin++; end--;
        newEvent.guardId = stoi(str.substr(begin, end - begin));

    } else if (str[pos1] == 'w') {
        newEvent.type = WakeUp;
    } else {
        newEvent.type = FallAsleep;
    }

    events.push_back(newEvent);
}

int main(int argc, char** argv) {
    ifstream inFile("puzz04Input.txt");
    vector<Event> events;

    if (inFile.is_open()) {
        string line;
        while(getline(inFile, line)) {
            ConstructEvent(events, line);
        }
    }

    cout << "size: " << events.size() << endl;

    sort (begin(events), end(events), [](const Event& A, const Event& B){
        if (A.month < B.month) {
            return true;
        } else if (A.month == B.month) {
            if (A.day < B.day) {
                return true;
            } else if (A.day == B.day) {
                return (A.min <= B.min);
            }
        }

        return false;
    });

    
    unordered_map<int, int> guardSleep;

    struct Time {
        int min; int hour;
    };

    struct Interval {
        int begin;
        int end;
    };

    int curGuard = -1;
    Time begin;
    for (Event& event : events) {
        //cout << event.month << " " << event.day <<" " << event.hour << " " << event.min << " " << event.type << endl;
        
        if (event.type == BeginShift) {
            guardSleep.emplace(event.guardId, 0);
            curGuard = event.guardId;
        } else if (event.type == FallAsleep) {
            begin.min = event.min;
            begin.hour = event.hour;
            event.guardId = curGuard;
        } else if (event.type == WakeUp) {
            int totalMin = 0;
            if (begin.hour > event.hour) { //23 > 00
                totalMin = (60 - begin.min);
                totalMin += event.min;
            } else {
                totalMin += (event.min - begin.min);
            }

            auto iter = guardSleep.find(curGuard);
            if (iter != guardSleep.end()) {
                iter->second += totalMin;
            }
            event.guardId = curGuard;
        }

    }
    cout << "Num guards: " << guardSleep.size() << endl;

    int maxMin = numeric_limits<int>::min();
    int maxGuardId = -1;
    for (auto guard : guardSleep) {
        if (guard.second > maxMin) {
            maxMin = guard.second;
            maxGuardId = guard.first;
        }
    }

    cout <<"max min: " << maxMin << endl;
    cout << "max guard: " << maxGuardId << endl;
int maxFreqNum = 0;
int maxFreqGuard = -1;
int maxFreqMin = -1;
for (auto guard : guardSleep) {
    maxGuardId = guard.first;

    cout << "cur guard id: " << maxGuardId << endl;
    vector<int> sleepHistogram (60, 0);
        int beginHour = -1;
        int beginMin = -1;
    for (int i = 0; i < size(events); ++i) {

        if (events[i].guardId == maxGuardId) {
            //cout << events[i].month << " " << events[i].day << " " << events[i].hour << " " << events[i].min << " " << events[i].type << endl;
            if (events[i].type == FallAsleep) {
                beginHour = events[i].hour;
                beginMin = events[i].min;
                //cout << "falls asleep: " << beginHour << ": " << beginMin << endl;
            } else if (events[i].type == WakeUp) {
                //cout << "wakes up: " << events[i].hour << ": " << events[i].min << endl;
                int totalMin = 0;
                if (beginHour > events[i].hour) { // 23 > 00
                    totalMin = (60 - beginMin);
                    totalMin += events[i].min;           
                } else {
                    totalMin += (events[i].min - beginMin);
                 }

                //cout << "totalMin: " << totalMin << endl;
                int idx = beginMin;
                while (totalMin) {
                    sleepHistogram[idx]++;
                    totalMin--;
                    idx++;
                    idx %= 60;
                }
            }
        }
    }
    
    // Find max;
    for (int i = 0; i < size(sleepHistogram); ++i) {
        if (sleepHistogram[i] > maxFreqNum) {
            maxFreqNum = sleepHistogram[i];
            maxFreqGuard = maxGuardId;
            maxFreqMin = i;
            cout << "max:  " << maxFreqNum << endl;
        }
    }
}

cout << "max freq num: " << maxFreqNum << endl << "max freq guard: " << maxFreqGuard << endl << " min: " << maxFreqMin << endl;
    return 0;
}