#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"
#include <allegro5\allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

//zmienna stan przechowuje obecny stan pętli gry
stangry stan = STAN_MENU;
//zmienna gracz przechowuje dane gracza
DaneGracza gracz;
//zmienna ranking przechowuje dane rankingu
Ranking ranking;
//zmienna plansza to dynamiczna tablica dwuwymiarowa przechowująca miejsce bomb
char** plansza = NULL;
//zmienna odkryta_plansza to dynamiczna tablica dwuwymiarowa przechowujaca stan pol (czy sa odkryte, czy jest na nich flaga)
char** odkryta_plansza = NULL;
//zmienna running kończy główną pętle gry gdy ustawiona na false
bool running = true;
//zmienna przechowuje poziom trudności wybrany przez gracza
int trudnosc = 0;
//zmienna przechowuje rozmiar planszy
int rozmiar = 0;
//zmienna przechowuje ilość min
int bomby = 0;

int main() {

    //Deklaracje struktur danych display, font, event_queue oraz timer
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT* font = NULL;
    ALLEGRO_FONT* font2 = NULL;
    ALLEGRO_FONT* font3 = NULL;
    ALLEGRO_FONT* font4 = NULL;
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
    al_install_keyboard();


    //Stworzenie kolejki zdarzen
    event_queue = al_create_event_queue();
    //Sprawdzenie czy kolejka stworzyla sie pomyslnie
    if (!event_queue) {
        fprintf(stderr, "Blad tworzenia kolejki zdarzen");
        al_destroy_display(display);
        return -1;
    }

    //Stworzenie timera, 60 klatek na sekunde
    timer = al_create_timer(1.0 / 20);
    //Sprawdzenie czy timer stworzyl sie pomyslnie
    if (!timer) {
        fprintf(stderr, "Blad tworzenia timera");
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        return -1;
    }

    //Wczytanie czcionek arial.tff
    font = al_load_font("Arial.ttf", 20, 0);
    font2 = al_load_font("Arial.ttf", 100, 0);
    font3 = al_load_font("Arial.ttf", 40, 0);
    font4 = al_load_font("Arial.ttf", 30, 0);

    //Sprawdzenie czy czcionki stworzyla sie pomyslnie
    if (!font || !font2 || !font3 || !font4) {
        fprintf(stderr, "Blad tworzenia czcionek.\n");
        if (font) al_destroy_font(font);
        if (font2) al_destroy_font(font2);
        if (font3) al_destroy_font(font3);
        if (font4) al_destroy_font(font4);
        al_destroy_display(display);
        return -1;
    }

    //Zarejestrowanie roznych zrodel zdarzen
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    //Uruchomienie timera
    al_start_timer(timer);

    //Nazwa okna
    al_set_window_title(display, "Saper");

    //Glowna petla gry
    while (running) {
        stan_gry(event_queue, font, font2, font3, font4);
    }

    //Zwolnienie zasobow po zakonczeniu gry
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(font2);
    al_destroy_font(font3);
    al_destroy_font(font4);
    al_destroy_display(display);

    return 0;
}
