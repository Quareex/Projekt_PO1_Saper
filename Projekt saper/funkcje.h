#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

//Szerokosc ekranu
#define SCREEN_WIDTH 600
//Wysokosc ekranu
#define SCREEN_HEIGHT 500
//Rozmiar pol
#define CELL_SIZE 20
//Rozmiar linii oddzielajacych pola
#define BORDER_SIZE 2
//Szerokosc przyciskow
#define BUTTON_WIDTH 200
//Wysokosc przyciskow
#define BUTTON_HEIGHT 50
//Koordynaty Y roznych rodzai przyciskow
#define EASY_BUTTON_Y 150
#define MEDIUM_BUTTON_Y 225
#define HARD_BUTTON_Y 300
#define PLAY_AGAIN_BUTTON_Y 400
#define START_BUTTON_Y 200
#define LEADERBOARD_BUTTON_Y 275
#define HELP_BUTTON_Y 350
#define EXIT_BUTTON_Y 425
#define BACK_HELP_BUTTON_Y 425

/// <summary>
/// Typ wyliczeniowy stangry przechowujący różne stany gry
/// </summary>
typedef enum {
    STAN_MENU,
    STAN_WYBIERANIE_NAZWY,
    STAN_WYBIERANIE_TRUDNOSCI,
    STAN_POMOC,
    STAN_RANKING,
    STAN_START_GRY,
    STAN_EKRAN_GRY,
    STAN_KONIEC_GRY
} stangry;

/// <summary>
/// Struktura DaneGracza przechowująca nazwe gracza, czas w jakim wygrał grę oraz na jakim poziomie trudności wygrał
/// </summary>
typedef struct {
    char nazwa_gracza[20];
    int minuty;
    int sekundy;
    int poziom_trudnosci;
} DaneGracza;

/// <summary>
/// Struktura Ranking przechowująca po trzy najlepsze wyniki dla każdego poziomu trudności
/// </summary>
typedef struct {
    DaneGracza ranking_latwy[3];
    DaneGracza ranking_sredni[3];
    DaneGracza ranking_trudny[3];
} Ranking;

extern stangry stan;
extern DaneGracza gracz;
extern char** plansza;
extern char** odkryta_plansza;
extern bool running;
extern int bomby;
extern int rozmiar;


//Deklaracje funkcji z funkcjonalnością gry
int odkrywanie_planszy(char** plansza, char** odkryta_plansza, int rzad, int kolumna, int rozmiar, int* pierwszy_ruch);
void generacja_bomb(char** plansza, int pierwszy_rzad, int pierwsza_kolumna);
void zwolnij_pamiec(char** tab);
char** alokacja_planszy(int rozmiar);
char** alokacja_odkrytej_planszy(int rozmiar);
int sprawdz_wygrana(char** plansza, char** odkryta_plansza);
void ustawienie_trudnosci(int trudnosc);
int licznik_flag(char** odkryta_plansza);
void licznik_czasu(double czas1, double czas2, int* minuty, int* sekundy);
void dodaj_do_rankingu(Ranking* ranking, DaneGracza gracz);
int is_empty(DaneGracza* ranking_tab);
void zapisz_ranking_do_pliku(const char* filename, Ranking* ranking);
void wczytaj_ranking_z_pliku(const char* filename, Ranking* ranking);
void inicjuj_ranking(Ranking* ranking);

//Deklaracje funkcji wyświetlających
void wyswietlenie_menu(ALLEGRO_FONT* font, ALLEGRO_FONT* font2);
void wyswietlenie_planszy(char** plansza, char** odkryta_plansza, ALLEGRO_FONT* font, double czas1, double czas2);
void wyswietlenie_wyboru_trudnosci(ALLEGRO_FONT* font, ALLEGRO_FONT* font3);
void wyswietlenie_konca_gry(ALLEGRO_FONT* font, ALLEGRO_FONT* font3, int wygrana, double czas1, double czas2, Ranking* ranking);
void wyswietlenie_nazwa(ALLEGRO_FONT* font, ALLEGRO_FONT* font3, const char* nazwa);
void wyswietlenie_pomoc(ALLEGRO_FONT* font, ALLEGRO_FONT* font4);
void wyswietlenie_ranking(ALLEGRO_FONT* font, Ranking* ranking);

//Deklaracje funkcji sterujących myszką
void sterowanie_trudnosc(ALLEGRO_EVENT event, int trudnosc);
void sterowanie_gra(ALLEGRO_EVENT event, int* pierwszy_ruch);
void sterowanie_koniec(ALLEGRO_EVENT event, int trudnosc);
void sterowanie_menu(ALLEGRO_EVENT event);
void sterowanie_pomoc(ALLEGRO_EVENT event);
void sterowanie_ranking(ALLEGRO_EVENT event);