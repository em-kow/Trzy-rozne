#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>


typedef struct{
    int siec, pozycja;
} motel;

bool trzy_rozne(int a, int b, int c){
    return (a != b && b != c && c != a);
}

int max(int a, int b){
    if(a > b) return a;
    else return b;
}

int min(int a, int b){
    if(a < b) return a;
    else return b;
}

void uzupelnij_lewo(int n, int lewo[][3], motel *motele){
    for(int k = 0; k < 3; k ++){
        for(int i = 0; i < n; i ++) lewo[i][k] = -1;
    }
    lewo[0][0] = 0;
    for(int i = 1; i < n; i ++){
        lewo[i][0] = i;
        if(motele[lewo[i - 1][0]].siec == motele[i].siec){
            lewo[i][1] = lewo[i - 1][1];
            lewo[i][2] = lewo[i - 1][2];
        }
        else if(lewo[i - 1][1] != -1 && motele[lewo[i - 1][1]].siec == motele[i].siec){
            lewo[i][1] = lewo[i - 1][0];
            lewo[i][2] = lewo[i - 1][2];
        }
        else{
            lewo[i][1] = lewo[i - 1][0];
            lewo[i][2] = lewo[i - 1][1];
        }
    }
}

void uzupelnij_prawo(int n, int prawo[][3], motel *motele){
    for(int k = 0; k < 3; k ++){
        for(int i = 0; i < n; i ++) prawo[i][k] = -1;
    }
    prawo[n - 1][0] = n - 1;
    for(int i = n - 2; i >= 0; i --){
        prawo[i][0] = i;
        if(motele[prawo[i + 1][0]].siec == motele[i].siec){
            prawo[i][1] = prawo[i + 1][1];
            prawo[i][2] = prawo[i + 1][2];
        }
        else if(prawo[i + 1][1] != -1 && motele[prawo[i + 1][1]].siec == motele[i].siec){
            prawo[i][1] = prawo[i + 1][0];
            prawo[i][2] = prawo[i + 1][2];
        }
        else{
            prawo[i][1] = prawo[i + 1][0];
            prawo[i][2] = prawo[i + 1][1];
        }
    }
}

int najblizsza_trojka(int n, int (*lewo)[3], int (*prawo)[3], motel *motele){
    int wynik = INT_MAX;
    bool ok = 0;
    for(int act_b = 1; act_b < n - 1; act_b ++){
        for(int i = 0; i < 3; i ++){
            for(int j = 0; j < 3; j ++){
                int act_a = lewo[act_b - 1][i];
                int act_c = prawo[act_b + 1][j];
                if(act_a == -1 || act_c == -1) continue;
                if(trzy_rozne(motele[act_a].siec, motele[act_b].siec, motele[act_c].siec)){
                    wynik = min(wynik, max(motele[act_b].pozycja - motele[act_a].pozycja, 
                    motele[act_c].pozycja - motele[act_b].pozycja));
                    ok = 1;
                }
            }
        }
    }
    if(ok == 0) return 0;
    return wynik;
}

int najdalsza_trojka(int n, int (*lewo)[3], int (*prawo)[3], motel *motele){
    int wynik = INT_MIN;
    bool ok = 0;
    for(int act_b = 0; act_b < n; act_b ++){
        for(int i = 0; i < 3; i ++){
            for(int j = 0; j < 3; j ++){
                int act_a = prawo[0][i];
                int act_c = lewo[n - 1][j];
                if(act_a == -1 || act_c == -1) continue;
                if(trzy_rozne(motele[act_a].siec, motele[act_b].siec, motele[act_c].siec) && 
                    act_a < act_b && act_b < act_c){
                         wynik = max(wynik, min(motele[act_b].pozycja - motele[act_a].pozycja, 
                            motele[act_c].pozycja - motele[act_b].pozycja));
                            ok = 1;
                }
            }
        }
    }
    if(ok == 0) return 0;
    return wynik;
}

int main(){
    int n; scanf("%d", &n);
    motel *motele = malloc((size_t)n * sizeof(motel));
    for(int i = 0; i < n; i ++){
        scanf("%d %d",&motele[i].siec, &motele[i].pozycja);
    }
    int (*lewo)[3] = calloc((size_t)n, sizeof *lewo);
    int (*prawo)[3] = calloc((size_t)n, sizeof *prawo);
    uzupelnij_lewo(n, lewo, motele);
    uzupelnij_prawo(n, prawo, motele);
    printf("%d %d\n", najblizsza_trojka(n, lewo, prawo, motele), najdalsza_trojka(n, lewo, prawo, motele));
    return 0;
}
