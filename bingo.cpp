#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <string.h>
using namespace std;

static const int SIZE = 5;
int NUM_TICKETS = 2;
int TICKETS [][SIZE][SIZE] = {
    {{1, 2, 3, 4, 5},
     {6, 7, 8, 9, 10},
     {11, 12, 13, 14, 15},
     {16, 17, 18, 19, 20},
     {21, 22, 23, 24, 25}},
    {{1, 2, 3, 4, 5},
     {6, 7, 8, 9, 10},
     {11, 12, 13, 14, 15},
     {16, 17, 18, 19, 20},
     {21, 22, 23, 24, 25}}
};

class Ticket {
public:
    Ticket (string name, int nums [][SIZE]);
    int checkBingo (int num);

    void printOpened ();
    string name;

private:
    std::map < int, std::pair<int, int> > _numToIndex;
    bool _openedNums [SIZE] [SIZE];
    int _numLines;
};

Ticket :: Ticket (string name, int nums [][SIZE]) : _numLines (0)
{
    this->name = name;
    memset (_openedNums, 0, SIZE * SIZE);
    _openedNums[2][2] = true;
    for (int y = 0; y < SIZE; y ++) {
        for (int x = 0; x < SIZE; x ++) {
            _numToIndex .insert (std::make_pair (nums [y] [x], std::make_pair (y, x)));
        }
    }
}

int Ticket :: checkBingo (int num) {
    if (_numToIndex .find (num) == _numToIndex .end ()) return _numLines;
    std::pair <int, int> &index = _numToIndex [num];
    int y = index .first;
    int x = index .second;
    if (_openedNums [y] [x]) return _numLines;
    _openedNums [y] [x] = true;

    bool h_bingo = true;
    for (int ix = 0; ix < SIZE; ix ++) {
        if (!_openedNums [y] [ix]) { h_bingo = false; break; }
    }
    if (h_bingo) _numLines ++;

    bool v_bingo = true;
    for (int iy = 0; iy < SIZE; iy ++) {
        if (!_openedNums [iy] [x]) { v_bingo = false; break; }
    }
    if (v_bingo) _numLines ++;

    if (x == y) {
        bool diagonal = true;
        for (int i = 0; i < SIZE; i ++) {
            if (!_openedNums [i] [i]) { diagonal = false; break; }
        }
        if (diagonal) _numLines ++;
    }

    if (x + y == SIZE - 1) {
        bool diagonal = true;
        for (int i = 0; i < SIZE; i ++) {
            if (!_openedNums [i] [SIZE - 1 - i]) { diagonal = false; break; }
        }
        if (diagonal) _numLines ++;
    }

    return _numLines;
}

void Ticket :: printOpened ()
{
    for (int y = 0; y < SIZE; y ++) {
        for (int x = 0; x < SIZE; x ++) {
            printf ("%c", _openedNums [y] [x] ? 'O' : 'X');
        }
        printf ("\n");
    }
    printf ("\n");
}

Ticket createTicket(string line) {
    int nums[][SIZE] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };
    Ticket ticket("name", nums);
    return ticket;
}

void readTickets(string filename, vector <Ticket> &tickets) {
    ifstream myfile (filename.c_str());
    string line;
    if (!myfile.is_open()) {
        throw "cannot read input file";
    }
    string key, dummy;
    int num;
    int nums[SIZE][SIZE]; 
    int x = 0;
    int y = 0;
    while (myfile.peek() != EOF) {
        myfile >> key >> dummy;
        x = 0;
        for (int i=0; i<SIZE*SIZE; i++) {
            myfile >> num;  
            nums[x][y] = num;
            //cout << x << " " << y << " " << nums[x][y] << endl;
            y++;
            if (y % 5 == 0) {
                y = 0;
                x++;
            }
        }
        cout << "Read Card # " << key << endl;
        tickets.push_back(Ticket(key, nums));
    }
}

int main () {

    vector <Ticket> tickets;
    readTickets("input.txt", tickets);

    int num;
    map<string, int> bingo;
    set<int> opening;
    cout << "=> ";
    while (cin >> num) {
        opening.insert(num);
        for (size_t i = 0; i < tickets .size (); i ++) {
            int numLines = tickets [i] .checkBingo (num);
            if (numLines > 0) {
                printf ("  No. %s [%lu] %d lines\n", tickets[i].name.c_str(), i + 1, numLines);
                tickets [i] .printOpened ();
            }
            if (numLines > 0) {
                bingo[tickets[i].name] = numLines;
            }
        }
        printf("Bingo!! \n");
        for (map<string, int>::iterator it = bingo.begin(); it != bingo.end(); ++it) {
            printf ("Card=#%s Lines=%d", it->first.c_str(), it->second);
            printf("\n");
        }
        printf("  Opening: ");
        for (set<int>::iterator it = opening.begin(); it != opening.end(); ++it) {
            printf ("%d ", *it);
        }
        printf("\n");
        cout << "==> ";
    }

    return 0;
}
