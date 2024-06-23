#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"
#include <allegro5\allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

/// <summary>
/// Główna pętla gry obsługująca wszystkie zdarzenia oraz sterująca grą
/// </summary>
/// <param name="event_queue">kolejka zdarzeń</param>
/// <param name="font">czcionka</param>
/// <param name="font2">czcionka</param>
/// <param name="font3">czcionka</param>
/// <param name="font4">czcionka</param>
void stan_gry(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font, ALLEGRO_FONT* font2, ALLEGRO_FONT* font3, ALLEGRO_FONT* font4) {
    //zmienne do licznika czasu
    double czas_start, czas_koniec;
    //zmienna do sprawdzania długości nazwy
    int dlugosc_nazwy = 0;
    //tablica przechowująca nazwę gracza
    char nazwa[50];

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        //Obsługa wyświetlania poszczególnych ekranów
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(128, 128, 128));

            if (stan == STAN_MENU) {
                wczytaj_ranking_z_pliku("wyniki.txt", &ranking);
                wyswietlenie_menu(font, font2);
            }
            else if (stan == STAN_WYBIERANIE_NAZWY) {
                al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, ALLEGRO_ALIGN_CENTRE, u8"Wprowadź nazwę:");
                al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, ALLEGRO_ALIGN_CENTRE, nazwa);
            }
            else if (stan == STAN_WYBIERANIE_TRUDNOSCI) {
                wyswietlenie_wyboru_trudnosci(font, font3);
            }
            else if (stan == STAN_POMOC) {
                wyswietlenie_pomoc(font, font4);
            }
            else if (stan == STAN_RANKING) {
                wyswietlenie_ranking(font);
            }
            else if (stan == STAN_START_GRY) {
                czas_start = al_get_time();
                stan = STAN_EKRAN_GRY;
            }
            else if (stan == STAN_EKRAN_GRY) {
                czas_koniec = al_get_time();
                int wygrana = sprawdz_wygrana(plansza, odkryta_plansza);
                if (wygrana) {
                    stan = STAN_KONIEC_GRY;
                }
                else {
                    wyswietlenie_planszy(plansza, odkryta_plansza, font, czas_start, czas_koniec);
                }
            }
            else if (stan == STAN_KONIEC_GRY) {
                int wygrana = sprawdz_wygrana(plansza, odkryta_plansza);
                al_rest(0.01);
                wyswietlenie_konca_gry(font, font3, wygrana, czas_start, czas_koniec);
            }

            al_flip_display();
        }

        //Obsługa sterowania klawiaturą
        else if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (stan == STAN_WYBIERANIE_NAZWY) {
                if (event.keyboard.unichar >= 32 && event.keyboard.unichar <= 126 && dlugosc_nazwy < sizeof(nazwa) - 1) {
                    nazwa[dlugosc_nazwy] = event.keyboard.unichar;
                    nazwa[dlugosc_nazwy + 1] = '\0';
                    dlugosc_nazwy++;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    strcpy_s(gracz.nazwa_gracza, nazwa);
                    stan = STAN_WYBIERANIE_TRUDNOSCI;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && dlugosc_nazwy > 0) {
                    dlugosc_nazwy--;
                    nazwa[dlugosc_nazwy] = '\0';
                }
            }
        }

        //Obsługa sterowania myszką
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (stan == STAN_MENU) {
                sterowanie_menu(event);
            }
            else if (stan == STAN_WYBIERANIE_TRUDNOSCI) {
                sterowanie_trudnosc(event);
            }
            else if (stan == STAN_POMOC) {
                sterowanie_pomoc(event);
            }
            else if (stan == STAN_RANKING) {
                sterowanie_ranking(event);
            }
            else if (stan == STAN_EKRAN_GRY) {
                sterowanie_gra(event);
            }
            else if (stan == STAN_KONIEC_GRY) {
                sterowanie_koniec(event);
            }
        }
    }
}


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
void generacja_bomb(char** plansza){
    int k = 0;
    srand(time(NULL));

    while (k < bomby){
        int i = rand() % rozmiar;
        int j = rand() % rozmiar;
        if (plansza[i][j] != '*'){
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
/// <returns>Zwraca -1 gdy odkryte pole to mina i 0 gdy pole zostało odkryte</returns>
int odkrywanie_planszy(char** plansza, char** odkryta_plansza, int rzad, int kolumna){
    //oznacza pole jako 1 czyli jako odkryte.
    odkryta_plansza[rzad][kolumna] = 1;

    //jesli odkryte pole jest miną to zwraca -1 co oznacza ze gra jest przegrana.
    if (plansza[rzad][kolumna] == '*')
        return -1;

    if (plansza[rzad][kolumna] == '.'){
        //zmienna przechowująca ile min znajduje się w otoczeniu pola.
        int k = 0;
        for (int i = rzad - 1; i <= rzad + 1; i++){
            for (int j = kolumna - 1; j <= kolumna + 1; j++){
                //Sprawdza czy pola mieszczą się na planszy i czy sa miną.
                if (i >= 0 && i < rozmiar && j >= 0 && j < rozmiar && plansza[i][j] == '*')
                    k++;
            }
        }
        //if sprawdzajacy czy znaleziono minę w otoczeniu.
        if (k > 0){
            //umieszcza odpowiednia liczbę min w otoczeniu.
            plansza[rzad][kolumna] = k + '0';
        }
        else{
            //gdy nie ma min w otoczeniu to oznacza pole jako znak '0'.
            plansza[rzad][kolumna] = '0';
            for (int i = rzad - 1; i <= rzad + 1; i++)
                for (int j = kolumna - 1; j <= kolumna + 1; j++)
                    //Sprawdza czy pola mieszczą sie na planszy i czy pole zostało odkryte.
                    if (i >= 0 && i < rozmiar && j >= 0 && j < rozmiar && !odkryta_plansza[i][j])
                        //rekurencyjne wywołuje funkcje by sprawdzic pola w okolicy rekurencyjnie.
                        odkrywanie_planszy(plansza, odkryta_plansza, i, j);
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
    case 4:
        rozmiar = 10;
        bomby = 3;
        break;
    default:
        break;
    }

    gracz.poziom_trudnosci = trudnosc;
    plansza = alokacja_planszy(rozmiar);
    odkryta_plansza = alokacja_odkrytej_planszy(rozmiar);
    generacja_bomb(plansza);
}


/// <summary>
/// Funkcja do liczenia ile flag postawił gracz podczas gry
/// </summary>
/// <param name="odkryta_plansza">plansza przechowująca stan planszy (flagi, odkryte pola, nieodkryte pola)</param>
/// <returns></returns>
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
    DaneGracza* ranking_tab = 0;
    int i, j;

    //Wybór odpowiedniej tablicy wyników zależnie od poziomu trudności
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
        if (ranking_tab[i].minuty == 0 && ranking_tab[i].sekundy == 0) {
            ranking_tab[i] = gracz;
            break;
        }
        else if ((gracz.minuty < ranking_tab[i].minuty) ||
            (gracz.minuty == ranking_tab[i].minuty && gracz.sekundy < ranking_tab[i].sekundy)) {
            for (j = 3; j > i; j--) {
                ranking_tab[j] = ranking_tab[j - 1];
            }
            ranking_tab[i] = gracz;
            break;
        }
    }

    //Sprawdzenie czy lista jest pełna i ewentualne usunięcie najgorszego wyniku
    if (i == 3) {
        //Gdy lista jest pełna nowy wynik zastępuje najgorszy wynik gdy jest lepszy
        if ((gracz.minuty < ranking_tab[2].minuty) || (gracz.minuty == ranking_tab[2].minuty && gracz.sekundy < ranking_tab[2].sekundy)) {
            ranking_tab[2] = gracz;
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
            if (ranking->ranking_latwy[i].minuty != 0 || ranking->ranking_latwy[i].sekundy != 0) {
                fprintf(file, "%s %02d:%02d\n", ranking->ranking_latwy[i].nazwa_gracza, ranking->ranking_latwy[i].minuty, ranking->ranking_latwy[i].sekundy);
            }
        }

        fprintf(file, "\nRanking Sredni:\n");
        for (int i = 0; i < 3; i++) {
            if (ranking->ranking_sredni[i].minuty != 0 || ranking->ranking_sredni[i].sekundy != 0) {
                fprintf(file, "%s %02d:%02d\n", ranking->ranking_sredni[i].nazwa_gracza, ranking->ranking_sredni[i].minuty, ranking->ranking_sredni[i].sekundy);
            }
        }

        fprintf(file, "\nRanking Trudny:\n");
        for (int i = 0; i < 3; i++) {
            if (ranking->ranking_trudny[i].minuty != 0 || ranking->ranking_trudny[i].sekundy != 0) {
                fprintf(file, "%s %02d:%02d\n", ranking->ranking_trudny[i].nazwa_gracza, ranking->ranking_trudny[i].minuty, ranking->ranking_trudny[i].sekundy);
            }
        }

        fclose(file);
    }
    else {
        perror("Nie udało się zapisać pliku");
    }
}


/// <summary>
/// Funkcja wczytuje dane o rankingu z pliku jeśli istnieje
/// </summary>
/// <param name="filename">Nazwa pliku do wczytania danych</param>
/// <param name="ranking">Zmienna ranking przechowująca dane o rankingu</param>
void wczytaj_ranking_z_pliku(const char* filename, Ranking* ranking) {
    FILE* file = fopen(filename, "r");
    if (file) {
        char line[100];
        int latwy_count = 0, sredni_count = 0, trudny_count = 0;

        for (int i = 0; i < 3; i++) {
            ranking->ranking_latwy[i].minuty = -1;
            ranking->ranking_sredni[i].minuty = -1;
            ranking->ranking_trudny[i].minuty = -1;
        }

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

        for (int i = latwy_count; i < 3; i++) {
            ranking->ranking_latwy[i].minuty = -1;
        }
        for (int i = sredni_count; i < 3; i++) {
            ranking->ranking_sredni[i].minuty = -1;
        }
        for (int i = trudny_count; i < 3; i++) {
            ranking->ranking_trudny[i].minuty = -1;
        }
    }
    else {
        perror("Nie udało się otworzyć pliku");
    }
}
