// Mikołaj Kowalski
// code review: Grzegorz Kaczmarek

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>


typedef struct{ // motele reprezentuja jako ich siec i polozenie na osi
    int siec, pozycja;
} motel;

bool trzy_rozne(int a, int b, int c){ // funkcja sprawdza czy trzy liczby sa parami rozne
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

// do rozwiazania potrzebuje dla kazdego motelu znac trzy kolejne motele nalezace do roznych sieci znajdujace sie na lewo i 
// prawo od danego motelu. Trzymam to w tablicy lewo[i][3] i prawo[i][3]. Zakladam ze najblizszy motel zarowno z lewej jak
// i prawej strony dla kazdego motelu to on sam, zatem zachodzi lewo[i][0] = prawo[i][0] = i. Jesli nie istnieja trzy mote-
// le roznych sieci z danej strony to zaznaczam w tablicy -1
// tablice lewo[i] mozna latwo wyliczyc dynamicznie na podstawie tablicy lewo[i - 1], poniewaz wiemy ze tablica lewo[i - 1]
// z definicji wskazuje na trzy rozne motele, to wystarczy rozpatrzyc przypadki czy ktorys z tych trzech moteli nalezy do 
// tej samej sieci co motel i
// tablice prawo[i] uzupelniamy analogicznie
// ze wzgledu na duze podobienstwo w uzupelnaniu tablic lewo i prawo mozna to zrobic przy pomocy jednej funkcji, wystarczy 
// wiedziec czy idziemy od poczatku czy od konca oraz czy wyliczamy wartosci na podstawie tablicy wczensiej czy z pozniej

void uzupelnij(int n, int tab[][3], motel *motele, bool strona){ // strona: 0 - licze lewo, 1 - licze prawo
    for(int k = 0; k < 3; k ++){
        for(int i = 0; i < n; i ++) tab[i][k] = -1;
    }
    int start, roznica;
    if(strona == 0){ // gdy licze lewo to zaczynam od indeksu 0 i odwoluje sie do tablicy od indeksu wczesniejszego
        start = 0;
        roznica = 1;
    }
    else{ // gdy licze prawo to zaczynam od indeksu n - 1 i odwoluje sie do tablicy od indeksu pozniejszego
        start = n - 1;
        roznica = -1;
    }
    tab[start][0] = start;
    for(int i = start + roznica; (i >= 0 && i < n); i += roznica){
        tab[i][0] = i; // niezmiennik
        // rozpatruje przypadki czy ktorys motel z tablicy do ktorej sie aktualnie odwoluje (czyli i-1 lub i+1 
        // w zaleznosci od strony) nalezy do tej samej sieci co motel i
        if(motele[tab[i - roznica][0]].siec == motele[i].siec){
            tab[i][1] = tab[i - roznica][1];
            tab[i][2] = tab[i - roznica][2];
        }
        else if(tab[i - roznica][1] != -1 && motele[tab[i - roznica][1]].siec == motele[i].siec){
            tab[i][1] = tab[i - roznica][0];
            tab[i][2] = tab[i - roznica][2];
        }
        else{
            tab[i][1] = tab[i - roznica][0];
            tab[i][2] = tab[i - roznica][1];
        }
    }
}

int najblizsza_trojka(int n, int (*lewo)[3], int (*prawo)[3], motel *motele){
    int wynik = INT_MAX;
    bool ok = 0; // sprawdzam czy w ogole istnieja trzy rozne motele
    // mozna udowodnic ze jesli aktualny motel jest motelem B, to optymalny dla niego motel A bedzie 
    // ktoryms z trzech najblizszych po lewo nalezacych do roznych sieci, a motel C z tych po prawo
    // zatem dla kazdego motelu B sprawdzamy wszystkich kandydatow i przy aktualizacji wyniku upewniamy 
    // sie czy na pewno motele naleza do trzech roznych sieci
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
    bool ok = 0; // sprawdzam czy w ogole istnieja trzy rozne motele
    // mozna udowodnic ze jesli aktualny motel jest motelem B, to optymalny dla niego motel A bedzie 
    // ktoryms z trzech najdalszych po lewo nalezacych do roznych sieci, a motel C z tych po prawo
    // zauwazmy ze sa to po prostu motele z tablic lewo lub prawo dla poczatku lub konca ciagu moteli
    // zatem dla kazdego motelu B sprawdzamy wszystkich kandydatow i przy aktualizacji wyniku upewniamy 
    // sie czy na pewno motele naleza do trzech roznych sieci i czy sa ulozone w kolejnosci A B C w ciagu
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
    uzupelnij(n, lewo, motele, 0);
    uzupelnij(n, prawo, motele, 1);
    printf("%d %d\n", najblizsza_trojka(n, lewo, prawo, motele), najdalsza_trojka(n, lewo, prawo, motele));
    return 0;
}
