#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"
#include <allegro5\allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

//zmienna stan przechowuje stan wyswietlanej gry
stangry stan = STAN_WYBIERANIE_TRUDNOSCI;
//zmienna trudnosc sluzy do ustawienia poziomu trudnosci
int trudnosc = 0;
//zmienna plansza to dynamiczna tablica dwuwymiarowa przechowująca miejsce bomb
char** plansza = NULL;
//zmienna odkryta_plansza to dynamiczna tablica dwuwymiarowa przechowujaca stan pol (czy sa odkryte, czy jest na nich flaga)
char** odkryta_plansza = NULL;
//zmienna rozmiar przechowuje rozmiar tablicy (rozmiar planszy)
int rozmiar = 0;
//zmienna bomby przechowuje ilosc bomb jaka ma wygenerowac funkcja generacja_bomb()
int bomby = 0;

int main() {

    //Deklaracje struktur danych display, font, event_queue oraz timer
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT* font = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;

    //Sprawdzenie czy biblioteka allegro zainicjalizowala sie pomyslnie
    if (!al_init()) {
        fprintf(stderr, "Blad wczytywania Allegro.\n");
        return -1;
    }

    //Tworzenie okna
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    //Sprawdzenie czy ekran stworzyl sie pomyslnie
    if (!display) {
        fprintf(stderr, "Blad tworzenia ekranu.\n");
        return -1;
    }
    
    //inicjalizacja dodatkow oraz myszki
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_mouse();

    //Wczytanie czcionki arial.tff
    font = al_load_font("arial.ttf", 20, 0);
    //Sprawdzenie czy czcionka stworzyla sie pomyslnie
    if (!font) {
        fprintf(stderr, "Blad tworzenia czcionki");
        al_destroy_display(display);
        return -1;
    }

    //Stworzenie kolejki zdarzen
    event_queue = al_create_event_queue();
    //Sprawdzenie czy kolejka stworzyla sie pomyslnie
    if (!event_queue) {
        fprintf(stderr, "Blad tworzenia kolejki zdarzen");
        al_destroy_font(font);
        al_destroy_display(display);
        return -1;
    }

    //Stworzenie timera, 60 klatek na sekunde
    timer = al_create_timer(1.0 / 60);
    //Sprawdzenie czy timer stworzyl sie pomyslnie
    if (!timer) {
        fprintf(stderr, "Blad tworzenia timera");
        al_destroy_event_queue(event_queue);
        al_destroy_font(font);
        al_destroy_display(display);
        return -1;
    }

    //Zarejestrowanie roznych zrodel zdarzen
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    //Uruchomienie timera
    al_start_timer(timer);

    //Glowna petla gry
    while (1) {
        stan_gry(event_queue, font);
    }

    //Zwolnienie zasobow po zakonczeniu gry
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_display(display);

    return 0;
}
