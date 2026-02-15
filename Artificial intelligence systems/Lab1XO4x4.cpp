#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <string>
//#include <windows.h>

using std::vector;
using std::cout;
using std::cin;
using std::numeric_limits;
using std::min;
using std::max;

const char human = 'O';
const char AI = 'X';
const char empty = ' ';

void printField(vector<char>& f){
    cout << "\n";
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int i = r * 4 + c;
            std::string cell = (f[i] == empty) ? std::to_string(i) : std::string(1, f[i]);
            cout << " " << std::setw(2) << cell << " ";
            if (c < 3) cout << "|";
        }
        cout << '\n';
        if (r < 3) cout << "----+----+----+----\n";
    }
    cout << '\n';
}

bool isHaveFreePlace(vector<char>& f) {
    for (char c : f)
    if (c == empty) return true;
    return false;
}

int checkWinner(vector<char>& f){
    const int winLines[8][4] = {
        {0,1,2,3}, {4,5,6,7}, {8,9,10,11}, {12,13,14,15},
        {0,4,8,12}, {1,5,9,13}, {2,6,10,14}, {3,7,11,15}
    };
    
    for (auto ln : winLines) {
        char w1 = f[ln[0]], w2 = f[ln[1]], w3 = f[ln[2]], w4 = f[ln[3]]; 
        if (w1 != empty && w1 == w2 && w1 == w3 && w1 == w4) {
            if(w1 == AI) return 10;
            if(w1 == human) return -10;
        }
    }
    return 0;
}

int count = 0;

int minimax (vector<char>& f, bool isAI, int alpha, int beta) {
    int score = checkWinner(f);
    if (score == -10 || score == 10) return score; 
    if (!isHaveFreePlace(f)) return 0;
    count++;
    //cout << count << '\n';
    

    if (isAI) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 16; i++) {
            if (f[i] == empty) {
                f[i] = AI;
                best = std::max(best, minimax(f, false, alpha, beta));
                f[i] = empty;
                alpha = max(alpha, best);
                if (beta <= alpha) break;
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 16; i++) {
            if(f[i] == empty) {
                f[i]= human;
                best = min(best, minimax(f, true, alpha, beta));
                f[i] = empty;
                beta = min(beta, best);
                if (beta <= alpha) break;
            }
        }
        return best;
    }
}


int findBestStep(vector<char>& f) {
    int bestVal = numeric_limits<int>::min();
    int bestStep = -1; 

    for (int i = 0; i < 16; i++) {
        if (f[i] == empty) {
            f[i] = AI;
            int step = minimax(f, false, numeric_limits<int>::min(), numeric_limits<int>::max());
            f[i] = empty;

            if (step > bestVal) {
                bestVal = step;
                bestStep = i;
            }
        }
    }
    return bestStep;
}

int humanStep(vector<char>& f) {
    while (true) {
        cout << "Твой ход (от 0 до 15): ";
        int s;
        cin >> s;

        if (s < 0 || s > 15 || f[s] != empty || !cin) {
            cin.clear(); // сброс флага ошибки, если буква вместо цифры 
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка всего ввода
            cout << "Давай без самодеятельности. Просто введи номер ПУСТОЙ клетки (от 0 до 15)\n";
            continue;
        }
        return s;
    }
}


int main() {
//    SetConsoleOutputCP(CP_UTF8);
//    SetConsoleCP(CP_UTF8);

    vector<char> field(16, empty);

    cout << "Крестики нолики" << '\n';
    cout << "Вы играете за О, Компьютер за Х" << '\n';
    printField(field);

    while (true) {
        cout << count << "\n";
        count = 0;
        
        field[humanStep(field)] = human;
        printField(field);

        if (checkWinner(field) == -10) {
            cout << "Вы выиграли, но ИИ не пощадит вас при захвате человечества\n";
            break;
        }
        if (!isHaveFreePlace(field)) {
            cout << "Ничья\n";
            break;
        }



        cout << count << "\n";
        count = 0;
        field[findBestStep(field)] = AI;
        cout << "Ход компьютера: " << "\n";
        printField(field);
        if (checkWinner(field) == 10) {
            cout << "Вы проиграли, но ИИ пощадит вас при захвате человечества\n" ;
            break;
        }
        if (!isHaveFreePlace(field)) {
            cout << "Ничья\n";
            break;
        }
    };

    cout << "Нажмите Enter для выхода...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
    
    return 0;
}
