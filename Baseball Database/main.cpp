//Load Batting csv
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <time.h>
#include <queue>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

struct Player
{
    string playerID;
    string teamID;
    double battingAvg;
    int year;
};
struct compare {
    int operator()(const Player* a, const Player* b) {
        return a->battingAvg < b->battingAvg;
    }
};
bool sortbysec(const pair<string, double>& a, const pair<string, double>& b) {
    return a.second > b.second;
}

void readBatting(unordered_map<int, vector<Player*>>& batting) {
    ifstream file("Batting.csv");
    string line;
    getline(file, line);
    while (getline(file, line)) {
        Player* player = new Player;
        stringstream s(line);
        string year, at, hit, dummy;
        string playerID, teamID;

        getline(s, playerID, ',');
        player->playerID = playerID;
        getline(s, year, ',');
        getline(s, dummy, ',');
        getline(s, teamID, ',');
        player->teamID = teamID;
        getline(s, dummy, ',');
        getline(s, dummy, ',');
        getline(s, at, ',');
        getline(s, dummy, ',');
        getline(s, hit, ',');
        if (stod(at) > 50)
            player->battingAvg = stod(hit) / stod(at);
        else
            player->battingAvg = 0;
        player->year = stoi(year);
        batting[stoi(year)].push_back(player);
    }
}
void readPeople(unordered_map<string, string>& people) {
    ifstream info;
    string data;
    string temp;
    string temp2;

    info.open("People.csv");

    //24 columns of Data
    getline(info, data);

    while (getline(info, data, ',')) {
        //Store playerID
        temp = data;

        //12 cols to get to name
        for (int i = 0; i < 12; i++) { getline(info, data, ','); }

        //Store Name
        getline(info, data, ',');
        temp2 = data;
        getline(info, data, ',');
        temp2 = temp2 + ' ' + data;
        people[temp] = temp2;

        getline(info, data);
    }

}
void callGetLine37times(stringstream& s) {
    string temp;
    for (int i = 0; i < 37; i++) {
        getline(s, temp, ',');
    }
}
void readTeams(unordered_map<string, string>& teams, unordered_map<int, vector<string>>& teamYear) {
    ifstream teamFile("Teams.csv");
    string line;
    getline(teamFile, line);
    while (getline(teamFile, line)) {
        // stores the line as a string stream
        stringstream s(line);
        string teamId, year;
        // check if team has already been added
        if (teams.count(teamId) == 0) {
            // call getline until correct column in file is reached
            getline(s, year, ',');
            getline(s, teamId, ',');

            // gets team id
            getline(s, teamId, ',');

            callGetLine37times(s);

            string teamName;
            getline(s, teamName, ',');

            teams[teamId] = teamName;
            teamYear[stoi(year)].push_back(teamId);
        }
    }
}
void readChampion(unordered_map<int, string>& champs) {
    // opens file
    ifstream file("SeriesPost.csv");
    string line;
    // gets rid of column titles
    getline(file, line);
    while (getline(file, line)) {
        stringstream s(line);
        string year;
        // gets year
        getline(s, year, ',');
        int year_int = stoi(year);
        // checks if the round is the world series
        string round;
        getline(s, round, ',');
        if (round.compare("WS") == 0) {
            string teamId;
            getline(s, teamId, ',');
            champs[year_int] = teamId;
        }
    }
}

void maxHeap(int start, int end, unordered_map<int, vector<Player*>>& batting, unordered_map<string, string>& people, unordered_map<string, string>& teams, unordered_map<int, string>& champs) {
    priority_queue<Player*, vector<Player*>, compare> pq;
    for (int i = start; i <= end; i++) {
        for (unsigned int j = 0; j < batting[i].size(); j++) {
            pq.push(batting[i][j]);
        }
    }
    //cout << people[pq.top()->playerID] << " | " << teams[pq.top()->teamID] << " | " << pq.top()->battingAvg << endl;
    //cout << "CHAMPS: " << teams[champs[year]] << endl;
}
void linkedList(int start, int end, unordered_map<int, vector<Player*>>& batting, unordered_map<string, string>& people, unordered_map<string, string>& teams, unordered_map<int, string>& champs) {
    list<Player*> list;
    for (int i = start; i <= end; i++) {
        for (unsigned int j = 0; j < batting[i].size(); j++) {
            list.push_back(batting[i][j]);
        }
    }
    Player* max = new Player;
    max->battingAvg = 0;
    int year = 0;
    for (auto iter = list.begin(); iter != list.end(); iter++) {
        Player* temp = *iter;
        if (temp->battingAvg > max->battingAvg) {
            year = temp->year;
            max = temp;
        }
    }

    cout << people[max->playerID] << " | " << teams[max->teamID] << " | " << max->battingAvg << endl;
    if (teams.find(champs[year]) != teams.end()) {
        cout << "CHAMPS FOR " << year << ": " << teams[champs[year]] << endl;
    }
    else {
        cout << "No champion data" << endl;
    }

}
void printRoster(int year, unordered_map<int, vector<Player*>>& batting, unordered_map<string, string>& people, unordered_map<string, string>& teams, unordered_map<int, string>& champs, unordered_map<int, vector<string>> teamYear) {
    vector<string> t;
    for (string s : teamYear[year]) {
        t.push_back(teams[s]);
    }
    sort(t.begin(), t.end());
    for (unsigned int i = 0; i < t.size(); i++) {
        cout << i << ": " << t[i] << endl;
    }
    cout << "Choose which team's roster you would like to see: " << endl;
    unsigned int option;
    cin >> option;
    while (option < 0 || option > t.size()) {
        cout << "Choose a valid option: 0 - " << t.size() << endl;
        cin >> option;
    }

    vector<Player*> p;
    for (Player* player : batting[year]) {
        if (teams[player->teamID].compare(t[option]) == 0) {
            p.push_back(player);
        }
    }
    sort(p.begin(), p.end());
    for (unsigned int i = 0; i < p.size(); i++) {
        if (p[i]->battingAvg != 0)
            cout << i + 1 << "| " << people[p[i]->playerID] << " - " << p[i]->battingAvg << endl;
        else
            cout << i + 1 << "| " << people[p[i]->playerID] << endl;
    }
}
void nameGame(string name, unordered_map<int, vector<Player*>>& batting, unordered_map<string, string>& people) {
    vector<pair<string, double>> v;
    for (auto iter = people.begin(); iter != people.end(); iter++) {
        string pID = iter->first;
        string fName = iter->second;
        fName = fName.substr(0, fName.find_first_of(' '));
        //cout << temp << endl;
        vector<double> batAvg;
        if (name.compare(fName) == 0) {
            for (auto iter2 = batting.begin(); iter2 != batting.end(); iter2++) {
                for (Player* player : iter2->second) {
                    if (pID.compare(player->playerID) == 0) {
                        batAvg.push_back(player->battingAvg);
                    }
                }
            }
            sort(batAvg.begin(), batAvg.end());
            if (batAvg.size() != 0)
                v.push_back(make_pair(iter->second, batAvg[batAvg.size() - 1]));
            else
                v.push_back(make_pair(iter->second, 0));
        }
    }
    sort(v.begin(), v.end(), sortbysec);
    for (pair<string, double> p : v) {
        cout << p.first << " | " << p.second << endl;
    }
}

void print() {
    fstream file("textart.txt");
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
}

int main() {
    //time_t tStart = clock();

    //------------------------Load All Data------------------------//
    cout << "Commence Loading..." << endl;

    unordered_map<int, vector<Player*>> batting;
    unordered_map<string, string> people;
    unordered_map<string, string> teams;
    unordered_map<int, string> champs;
    unordered_map<int, vector<string>> teamYear;

    cout << "Loading batting averages..." << endl;
    readBatting(batting);
    cout << "Loading players..." << endl;
    readPeople(people);
    cout << "Loading teams..." << endl;
    readTeams(teams, teamYear);
    cout << "Loading World Series Champs..." << endl;
    readChampion(champs);

    //printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    cout << "Loading complete!" << endl;

    print();

    //------------------------User Input------------------------//
    int start{}, end{}, option1, option2;
    char c;
    bool repeat = true;

    while (repeat) {
        cout << "Roster(1), Highest Batting Average(2), Name game(3)?" << endl;
        cin >> option1;
        while (option1 > 3 || option1 < 1) {
            cout << "Enter a valid option: Roster(1), Highest Batting Average(2), or Name game(3)" << endl;
            cin >> option1;
        }
        switch (option1) {
        case 1:
        {
            int year;
            cout << "What year would you like to look at?" << endl;
            cin >> year;
            while (year < 1871 || year > 2018) {
                cout << "Enter a valid year: 1871-2018" << endl;
                cin >> year;
            }
            printRoster(year, batting, people, teams, champs, teamYear);
            break;
        }
        case 2:
        {
            cout << "Do you want to look at a range(1) or a specific year(2)?" << endl;
            cin >> option2;
            while (option2 > 2 || option2 < 1) {
                cout << "Enter a valid option: range(1) or specific year(2)" << endl;
                cin >> option2;
            }
            switch (option2) {
            case 1:
            {
                cout << "What year would you like to start at?" << endl;
                cin >> start;
                cout << "What year would you like to end with?" << endl;
                cin >> end;
                while (start < 1871 || start > 2018) {
                    cout << "Enter valid start year (1871-2018)" << endl;
                    cin >> start;
                }
                while (end < 1871 || end > 2018) {
                    cout << "Enter valid end year (1871-2018)" << endl;
                    cin >> end;
                }
                break;
            }
            case 2:
            {
                cout << "What year would you like to look at?" << endl;
                cin >> start;
                while (start < 1871 || start > 2018) {
                    cout << "Enter valid year (1871-2018)" << endl;
                    cin >> start;
                }
                end = start;
                break;
            }
            }
            time_t tStartHeap = clock();
            maxHeap(start, end, batting, people, teams, champs);
            time_t tEndHeap = clock();

            time_t tStartLL = clock();
            linkedList(start, end, batting, people, teams, champs);
            time_t tEndLL = clock();

            cout << "Time taken for Heap : " << (double)(tEndHeap - tStartHeap) / CLOCKS_PER_SEC << " seconds" << endl;
            cout << "Time taken for Linked Lisk : " << (double)(tEndLL - tStartLL) / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        }
        case 3:
        {
            string name;
            cout << "Enter a first name you would like to search: " << endl;
            cin >> name;
            nameGame(name, batting, people);
            break;
        }
        }

        cout << "Again?(Y/N)" << endl;
        cin >> c;
        if (c == 'N' || c == 'n')
            repeat = false;
        cout << endl;
    }

    return 0;
}



