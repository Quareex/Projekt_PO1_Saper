#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"
#include <allegro5\allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>


/// <summary>
/// Funkcja do sprawdzania czy wymagania do wygrania gry są spełnione
/// </summary>
/// <param name="plansza">plansza przechowująca liczby oraz miny</param>
/// <param name="odkryta_plansza">plansza przechowująca stan planszy (flagi, odkryte pola, nieodkryte pola)</param>
/// <returns>Zwraca 1 gdy gra jest wygrana i 0 gdy gra nie jest jeszcze wygrana</returns>
int sprawdz_wygrana(char** plansza, char** odkryta_plansza) {
    int oznaczone_miny = 0;
    int zle_umieszczone_flagi = 0;
    int odkryte_pola = 0;

    for (int i = 0; i < rozmiar; i++){
        for (int j = 0; j < rozmiar; j++){
            if (plansza[i][j] == '*' && odkryta_plansza[i][j] == 2)
                oznaczone_miny++;
            if (plansza[i][j] != '*' && odkryta_plansza[i][j] == 2)
                zle_umieszczone_flagi++;
            if (plansza[i][j] != '*' && odkryta_plansza[i][j] == 1)
                odkryte_pola++;
        }
    }
    if (oznaczone_miny == bomby && zle_umieszczone_flagi == 0 && odkryte_pola == (rozmiar * rozmiar - bomby))
        return 1;
    else
        return 0;
}


/// <summary>
/// Funkcja do alokacji pamięci dla odkrytej planszy oraz do wypełnienia jej zerami.
/// </summary>
/// <param name="rozmiar">rozmiar planszy</param>
/// <returns>zwraca odkrytą planszę</returns>
char** alokacja_odkrytej_planszy(int rozmiar){
    char** odkryta_plansza = (char**)malloc(rozmiar * sizeof(char*));
    if (!odkryta_plansza)
        printf("blad alokacji pamieci.");

    for (int i = 0; i < rozmiar; i++){
        odkryta_plansza[i] = (char*)malloc(rozmiar * sizeof(char));
        if (!odkryta_plansza[i])
            printf("blad alokacji pamieci.");

        for (int j = 0; j < rozmiar; j++)
            odkryta_plansza[i][j] = 0;
    }
    return odkryta_plansza;
}


/// <summary>
/// Funkcja do alokacji pamięci dla planszy oraz do wypełnienia jej kropkami.
/// </summary>
/// <param name="rozmiar">rozmiar planszy</param>
/// <returns>zwraca planszę</returns>
char** alokacja_planszy(int rozmiar){
    char** plansza = (char**)malloc(rozmiar * sizeof(char*));
    if (!plansza)
        printf("blad alokacji pamieci.");

    for (int i = 0; i < rozmiar; i++){
        plansza[i] = (char*)malloc(rozmiar * sizeof(char));
        if (!plansza[i])
            printf("blad alokacji pamieci.");

        for (int j = 0; j < rozmiar; j++)
            plansza[i][j] = '.';
    }
    return plansza;
}


/// <summary>
/// Funkcja do zwalniania zarezerwowanej dla tablic dynamicznych pamięci
/// </summary>
/// <param name="tab">podwójny wskaźnik na tablice tablic</param>
void zwolnij_pamiec(char** tab){
    for (int i = 0; i < rozmiar; i++)
        free(tab[i]);
    free(tab);
}


/// <summary>
/// Funkcja do generowania min w losowych miejscach na planszy
/// </summary>
/// <param name="plansza">plansza przechowująca liczby oraz miny</param>
/// <param name="pierwszy_rzad">rząd wybrany w pierwszym ruchu</param>
/// <param name="pierwsza_kolumna">kolumna wybrana w pierwszym ruchu</param>
void generacja_bomb(char** plansza, int pierwszy_rzad, int pierwsza_kolumna) {
    int k = 0;
    srand(time(NULL));

    while (k < bomby) {
        int i = rand() % rozmiar;
        int j = rand() % rozmiar;

        if (plansza[i][j] != '*' && !(i >= pierwszy_rzad - 1 && i <= pierwszy_rzad + 1 && j >= pierwsza_kolumna - 1 && j <= pierwsza_kolumna + 1)) {
            plansza[i][j] = '*';
            k++;
        }
    }
}



/// <summary>
/// Funkcja do odkrywania pól planszy
/// </summary>
/// <param name="plansza">plansza przechowująca liczby oraz miny</param>
/// <param name="odkryta_plansza">plansza przechowująca stan planszy (flagi, odkryte pola, nieodkryte pola)</param>
/// <param name="rzad">wybrany przez gracza rząd</param>
/// <param name="kolumna">wybrana przez gracza kolumna</param>
/// <param name="rozmiar">rozmiar planszy</param>
/// <param name="pierwszy_ruch">zmienna przechowująca czy to pierwszy ruch gracza</param>
/// <returns>Zwraca -1 gdy odkryte pole to mina i 0 gdy pole zostało odkryte</returns>
int odkrywanie_planszy(char** plansza, char** odkryta_plansza, int rzad, int kolumna, int rozmiar, int* pierwszy_ruch) {
    if (*pierwszy_ruch) {
        generacja_bomb(plansza, rzad, kolumna);
        *pierwszy_ruch = 0;
    }
    odkryta_plansza[rzad][kolumna] = 1;

    if (plansza[rzad][kolumna] == '*')
        return -1;

    if (plansza[rzad][kolumna] == '.') {
        int k = 0;
        for (int i = rzad - 1; i <= rzad + 1; i++) {
            for (int j = kolumna - 1; j <= kolumna + 1; j++) {
                if (i >= 0 && i < rozmiar && j >= 0 && j < rozmiar && plansza[i][j] == '*')
                    k++;
            }
        }
        if (k > 0) {
            plansza[rzad][kolumna] = k + '0';
        }
        else {
            plansza[rzad][kolumna] = '0';
            for (int i = rzad - 1; i <= rzad + 1; i++) {
                for (int j = kolumna - 1; j <= kolumna + 1; j++) {
                    if (i >= 0 && i < rozmiar && j >= 0 && j < rozmiar && !odkryta_plansza[i][j]) {
                        odkrywanie_planszy(plansza, odkryta_plansza, i, j, rozmiar, pierwszy_ruch);
                    }
                }
            }
        }
    }
    return 0;
}



/// <summary>
/// Funkcja zawierająca ustawienia poszczególnych poziomów trudności i ustawiająca odpowiedni poziom
/// </summary>
/// <param name="trudnosc">zmienna przechowująca poziom trudności</param>
void ustawienie_trudnosci(int trudnosc) {
    switch (trudnosc) {
    case 1:
        rozmiar = 8;
        bomby = 10;
            break;
    case 2:
        rozmiar = 12;
        bomby = 25;
            break;
    case 3:
        rozmiar = 16;
        bomby = 40;
            break;
    /*
    case 4:
        rozmiar = 10;
        bomby = 3;
        break;
    */
    default:
        break;
    }

    gracz.poziom_trudnosci = trudnosc;
    plansza = alokacja_planszy(rozmiar);
    odkryta_plansza = alokacja_odkrytej_planszy(rozmiar);
}


/// <summary>
/// Funkcja do liczenia ile flag postawił gracz podczas gry
/// </summary>
/// <param name="odkryta_plansza">plansza przechowująca stan planszy (flagi, odkryte pola, nieodkryte pola)</param>
/// <returns>zwraca ilość postawionych flag</returns>
int licznik_flag(char** odkryta_plansza) {
    int flagi = 0;
    for (int i = 0; i < rozmiar; i++) {
        for (int j = 0; j < rozmiar; j++) {
            if (odkryta_plansza[i][j] == 2)
                flagi++;
        }
    }
    return flagi;
}


/// <summary>
/// Funkcja do liczenia czasu podczas gry
/// </summary>
/// <param name="czas1">zmienna do licznika czasu (start)</param>
/// <param name="czas2">zmienna do licznika czasu (koniec)</param>
/// <param name="minuty">wskaźnik na zmienną przechowującą ile minut mineło</param>
/// <param name="sekundy">wskaźnik na zmienną przechowującą ile sekund mineło</param>
void licznik_czasu(double czas1, double czas2, int* minuty, int* sekundy) {
    double czas = czas2 - czas1;
    *minuty = (int)czas / 60;
    *sekundy = (int)czas - (*minuty * 60);
}


/// <summary>
/// Funkcja do dodawania nowych wpisów do struktury rankingu
/// </summary>
/// <param name="ranking">zmienna typu Ranking przechowująca dane rankingu</param>
/// <param name="gracz">zmienna typu DaneGracza przechowująca dane z ostatniej gry danego gracza</param>
void dodaj_do_rankingu(Ranking* ranking, DaneGracza gracz) {
    DaneGracza* ranking_tab = NULL;
    int i, j;

    if (gracz.poziom_trudnosci == 0) {
        return;
    }

    if (gracz.poziom_trudnosci == 1) {
        ranking_tab = ranking->ranking_latwy;
    }
    else if (gracz.poziom_trudnosci == 2) {
        ranking_tab = ranking->ranking_sredni;
    }
    else if (gracz.poziom_trudnosci == 3) {
        ranking_tab = ranking->ranking_trudny;
    }

    for (i = 0; i < 3; i++) {
        if (strcmp(ranking_tab[i].nazwa_gracza, "none") == 0 && ranking_tab[i].minuty == 0 && ranking_tab[i].sekundy == 0) {
            ranking_tab[i] = gracz;
            break;
        }
        else if ((gracz.minuty < ranking_tab[i].minuty) ||
            (gracz.minuty == ranking_tab[i].minuty && gracz.sekundy < ranking_tab[i].sekundy)) {
            for (j = 2; j > i; j--) {
                ranking_tab[j] = ranking_tab[j - 1];
            }
            ranking_tab[i] = gracz;
            break;
        }
    }
}

/// <summary>
/// Funkcja do sprawdzania czy tablica wyników jest pusta
/// </summary>
/// <param name="ranking_tab">wskaźnik na tablice przechowującą wynik</param>
/// <returns>zwraca 0 gdy tablica nie jest pusta i 1 gdy jest pusta</returns>
int is_empty(DaneGracza* ranking_tab) {
    for (int i = 0; i < 3; i++) {
        if (ranking_tab[i].minuty != 0 || ranking_tab[i].sekundy != 0) {
            return 0;
        }
    }
    return 1;
}


/// <summary>
/// Funkcja do zapisywania danych o rankingu do pliku
/// </summary>
/// <param name="filename">Nazwa pliku do zapisu danych</param>
/// <param name="ranking">Zmienna ranking przechowująca dane o rankingu</param>
void zapisz_ranking_do_pliku(const char* filename, Ranking* ranking) {
    FILE* file = fopen(filename, "w");
    if (file) {
        fprintf(file, "Ranking Latwy:\n");
        for (int i = 0; i < 3; i++) {
            if (strcmp(ranking->ranking_latwy[i].nazwa_gracza, "none") != 0) {
                fprintf(file, "%s %02d:%02d\n", ranking->ranking_latwy[i].nazwa_gracza, ranking->ranking_latwy[i].minuty, ranking->ranking_latwy[i].sekundy);
            }
        }

        fprintf(file, "\nRanking Sredni:\n");
        for (int i = 0; i < 3; i++) {
            if (strcmp(ranking->ranking_sredni[i].nazwa_gracza, "none") != 0) {
                fprintf(file, "%s %02d:%02d\n", ranking->ranking_sredni[i].nazwa_gracza, ranking->ranking_sredni[i].minuty, ranking->ranking_sredni[i].sekundy);
            }
        }

        fprintf(file, "\nRanking Trudny:\n");
        for (int i = 0; i < 3; i++) {
            if (strcmp(ranking->ranking_trudny[i].nazwa_gracza, "none") != 0) {
                fprintf(file, "%s %02d:%02d\n", ranking->ranking_trudny[i].nazwa_gracza, ranking->ranking_trudny[i].minuty, ranking->ranking_trudny[i].sekundy);
            }
        }

        fclose(file);
    }
    else {
        perror(u8"Nie udało się zapisać pliku");
    }
}

/// <summary>
/// Funkcja wczytuje dane o rankingu z pliku jeśli istnieje
/// </summary>
/// <param name="filename">Nazwa pliku do wczytania danych</param>
/// <param name="ranking">Zmienna ranking przechowująca dane o rankingu</param>
void wczytaj_ranking_z_pliku(const char* filename, Ranking* ranking) {
    if (ranking == NULL) {
        fprintf(stderr, u8"Wskaxnik na ranking jest rowny NULL\n");
        return;
    }

    FILE* file = fopen(filename, "r");
    if (file) {
        char line[100];
        int latwy_count = 0, sredni_count = 0, trudny_count = 0;

        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;

            if (strncmp(line, "Ranking Latwy:", 14) == 0) {
                latwy_count = 0;
            }
            else if (strncmp(line, "Ranking Sredni:", 15) == 0) {
                sredni_count = 0;
            }
            else if (strncmp(line, "Ranking Trudny:", 15) == 0) {
                trudny_count = 0;
            }
            else {
                char nazwa_gracza[50];
                int minuty, sekundy;

                if (sscanf(line, "%49s %d:%d", nazwa_gracza, &minuty, &sekundy) == 3) {
                    DaneGracza gracz = { 0 };
                    strncpy(gracz.nazwa_gracza, nazwa_gracza, sizeof(gracz.nazwa_gracza) - 1);
                    gracz.nazwa_gracza[sizeof(gracz.nazwa_gracza) - 1] = '\0';
                    gracz.minuty = minuty;
                    gracz.sekundy = sekundy;

                    if (latwy_count < 3) {
                        gracz.poziom_trudnosci = 1;
                        ranking->ranking_latwy[latwy_count++] = gracz;
                    }
                    else if (sredni_count < 3) {
                        gracz.poziom_trudnosci = 2;
                        ranking->ranking_sredni[sredni_count++] = gracz;
                    }
                    else if (trudny_count < 3) {
                        gracz.poziom_trudnosci = 3;
                        ranking->ranking_trudny[trudny_count++] = gracz;
                    }
                }
            }
        }
        fclose(file);
    }
    else {
        perror("Nie udalo sie otworzyc pliku");
    }
}



/// <summary>
/// Funkcja do inicjalizacji struktury Ranking
/// </summary>
/// <param name="ranking">Zmienna ranking przechowująca dane o rankingu</param>
void inicjuj_ranking(Ranking* ranking) {
    for (int i = 0; i < 3; i++) {
        strcpy(ranking->ranking_latwy[i].nazwa_gracza, "none");
        ranking->ranking_latwy[i].minuty = 0;
        ranking->ranking_latwy[i].sekundy = 0;
        ranking->ranking_latwy[i].poziom_trudnosci = 1;

        strcpy(ranking->ranking_sredni[i].nazwa_gracza, "none");
        ranking->ranking_sredni[i].minuty = 0;
        ranking->ranking_sredni[i].sekundy = 0;
        ranking->ranking_sredni[i].poziom_trudnosci = 2;

        strcpy(ranking->ranking_trudny[i].nazwa_gracza, "none");
        ranking->ranking_trudny[i].minuty = 0;
        ranking->ranking_trudny[i].sekundy = 0;
        ranking->ranking_trudny[i].poziom_trudnosci = 3;
    }
}