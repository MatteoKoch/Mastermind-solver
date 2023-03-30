#include <iostream>
#include <chrono>
#include <math.h>
#include <cstring>

using namespace std;

int possibilities[8][8][8][8];
int answer;
bool gameOver = false;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void initPossibilities() {
    for(int first = 0; first < 8; ++first) {
        for(int second = 0; second < 8; ++second) {
            for(int third = 0; third < 8; ++third) {
                for(int fourth = 0; fourth < 8; ++fourth) {
                    possibilities[first][second][third][fourth] = 1;
                }
            }
        }
    }
}

void setAnswer() {
    int ans = 0;
    cout<<"Setze eine 4-stellige Zahlen mit den Ziffern [1,8]:"<<endl;
    cin>>ans;
    answer = ans;
    //cout<<answer<<endl;
}

string checkAnswer(int pguess, int answ = answer) {
    int response[2] = {0, 0};

    int ans[4] = {0};
    int guess[4] = {0};

    //zerlegen in array
    int num;
    for(int i = 0; i < 4; ++i) {
        num = answ - ans[0]*1000 - ans[1]*100 - ans[2]*10;
        ans[i] = num / (int)pow(10, 3-i);
        num = pguess - guess[0]*1000 - guess[1]*100 - guess[2]*10;
        guess[i] = num / (int)pow(10, 3-i);
    }

    //Ermittlung der Anzahl der Zahlen der Antwort
    int anz_zahlen[9] = {0};
    for(int i = 0; i < 4; ++i) {
        anz_zahlen[ans[i]]++;
    }

    //Ermittlung der Anzahl der Zahlen des Inputs
    int anz_zahlen2[9] = {0};
    for(int i = 0; i < 4; ++i) {
        anz_zahlen2[guess[i]]++;
    }

    int paare[4] = {0};
    int anz_paare[9] = {0};
    for(int i = 1; i < 5; ++i) {
        if(ans[i-1] == guess[i-1]) {
            paare[i-1] = ans[i-1];
            response[0]++;
        }
        anz_paare[i] = anz_zahlen[i]<anz_zahlen2[i]?anz_zahlen[i]:anz_zahlen2[i];
    }
    for(int i = 0; i < 4; ++i) {
        anz_paare[paare[i]]--;
    }
    for(int i = 0; i < 4; ++i) {
        response[1] += anz_paare[i+1];
    }
    string s = to_string(response[0])+","+to_string(response[1]);
    //if(response[0] == 4) gameOver = true;
    return s;
}

int compare(int guess, int response_black, int response_white) {
    response_black -= 48;
    response_white -= 48;
    cout<<guess<<": "<<response_black<<","<<response_white<<endl;

    int pguess[4] = {0};
    int num;
    for(int i = 0; i < 4; ++i) {
        num = guess - pguess[0]*1000 - pguess[1]*100 - pguess[2]*10;
        pguess[i] = num / (int)pow(10, 3-i);
    }
    possibilities[pguess[0]-1][pguess[1]-1][pguess[2]-1][pguess[3]-1] = 0;

    int zahl = 0;
    int nextGuess = 0;
    string s;
    char eval_array[4];
    for(int first = 0; first < 8; ++first) {
        for(int second = 0; second < 8; ++second) {
            for(int third = 0; third < 8; ++third) {
                for(int fourth = 0; fourth < 8; ++fourth) {
                    if(possibilities[first][second][third][fourth] == 1) {
                        zahl = (first+1)*1000 + (second+1)*100 + (third+1)*10 + (fourth+1);
                        s = checkAnswer(zahl, guess);
                        strcpy(eval_array, s.c_str());
                        //cout<<(int)eval_array[0]-48<<" == "<<response_black<<" && "<<(int)eval_array[2]-48<<" == "<<response_white<<endl;
                        if((int)eval_array[0]-48 == response_black && (int)eval_array[2]-48 == response_white) {
                            nextGuess = zahl;
                        } else {
                            possibilities[first][second][third][fourth] = 0;
                        }
                    }
                }
            }
        }
    }
    //cout<<"nextGuess: "<<nextGuess<<endl;
    return nextGuess;
}

int main()
{
    initPossibilities();
    setAnswer();
    int guess = 1122;
    int anz = 0;
    char eval_array[4];
    string evaluation;
    do {
        evaluation = checkAnswer(guess);
        strcpy(eval_array, evaluation.c_str());
        guess = compare(guess, eval_array[0], eval_array[2]);
        if((int)eval_array[0]-48 == 4) gameOver = true;
        anz++;
    } while(!gameOver);
    cout<<"geschafft in "<<anz<<" versuchen!"<<endl;
    return 0;
}
