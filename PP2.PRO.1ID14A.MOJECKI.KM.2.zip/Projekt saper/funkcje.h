#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5\allegro.h>
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
#define EASY_BUTTON_Y 200
#define MEDIUM_BUTTON_Y 275
#define HARD_BUTTON_Y 350
#define PLAY_AGAIN_BUTTON_Y 400

//Typ wyliczeniowy przechowujacy rozne stany gry ktore sa w danym momencie wyswietlane
typedef enum {
    STAN_WYBIERANIE_TRUDNOSCI,
    STAN_START_GRY,
    STAN_EKRAN_GRY,
    STAN_KONIEC_GRY
} stangry;

extern stangry stan;
extern int trudnosc;
extern char** plansza;
extern char** odkryta_plansza;
extern int rozmiar;
extern int bomby;

//Deklaracje funkcji
int odkrywanie_planszy(char** plansza, char** odkryta_plansza, int rozmiar, int rzad, int kolumna);
void generacja_bomb(char** plansza, int rozmiar, int bomby);
void zwolnij_pamiec(char** tab, int rozmiar);
char** alokacja_planszy(int rozmiar);
char** alokacja_odkrytej_planszy(int rozmiar);
int sprawdz_wygrana(char** plansza, char** odkryta_plansza, int rozmiar, int bomby);
void wyswietelnie_planszy(char** plansza, char** odkryta_plansza, int rozmiar, ALLEGRO_FONT* font);
void wyswietelnie_wyboru_trudnosci(ALLEGRO_FONT* font);
void Wyswietlenie_konca_gry(ALLEGRO_FONT* font, int wygrana);
void stan_gry(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font);
void sterowanie_trudnosc(ALLEGRO_EVENT event, ALLEGRO_FONT* font);
void sterowanie_gra(ALLEGRO_EVENT event);
void sterowanie_koniec(ALLEGRO_EVENT event);
void reset_gry();
void ustawienie_trudnosci(int trudnosc);
void flaga(int rzad, int kolumna);
void odkrycie_pola(int rzad, int kolumna);