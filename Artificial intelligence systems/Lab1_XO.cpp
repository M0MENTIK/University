#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
//#include <windows.h>

using std::vector;
using std::cout;
using std::cin;
using std::numeric_limits;

const char human = 'O';
const char AI = 'X';
const char empty = ' ';

void printField(vector<char>& f){
    cout << "\n";
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            int i = r * 3 + c;
            char ch = (f[i] == empty)? char('0' + i) : f[i];
            cout << " " << ch << " ";
            if (c < 2) cout << "|";
        }
        cout << '\n';
        if (r < 2) cout << "---+---+---\n";
    }
    cout << '\n';
}

bool isHaveFreePlace(vector<char>& f) {
    for (char c : f)
    if (c == empty) return true;
    return false;
}

int checkWinner(vector<char>& f){
    const int winLines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };
    
    for (auto ln : winLines) {
        char w1 = f[ln[0]], w2 = f[ln[1]], w3= f[ln[2]]; 
        if (w1 != empty && w1 == w2 && w1 == w3) {
            if(w1 == AI) return 10;
            if(w1 == human) return -10;
        }
    }
    return 0;
}

int minimax (vector<char>& f, bool isAI) {
    int score = checkWinner(f);
    if (score == -10 || score == 10) return score; 
    if (!isHaveFreePlace(f)) return 0;

    if (isAI) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 9; i++) {
            if (f[i] == empty) {
                f[i] = AI;
                best = std::max(best, minimax(f, false));
                f[i] = empty;
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();

        for (int i = 0; i < 9; i++) {
            if(f[i] == empty) {
                f[i]= human;
                best = std::min(best, minimax(f, true));
                f[i] = empty;
            }
        }
        return best;
    }
}


int findBestStep(vector<char>& f) {
    int bestVal = numeric_limits<int>::min();
    int bestStep = -1; 

    for (int i = 0; i < 9; i++) {
        if (f[i] == empty) {
            f[i] = AI;
            int step = minimax(f, false);
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
        cout << "Твой ход (от 0 до 8): ";
        int s;
        cin >> s;

        if (s < 0 || s > 8 || f[s] != empty || !cin) {
            cin.clear(); // сброс флага ошибки, если буква вместо цифры 
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка всего ввода
            cout << "Давай без самодеятельности. Просто введи номер ПУСТОЙ клетки (от 0 до 8)\n";
            continue;
        }
        return s;
    }
}


int main() {
//    SetConsoleOutputCP(CP_UTF8);
//    SetConsoleCP(CP_UTF8);

    vector<char> field(9, empty);

    cout << "Крестики нолики" << '\n';
    cout << "Вы играете за О, Компьютер за Х" << '\n';
    printField(field);

    while (true) {
        
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
