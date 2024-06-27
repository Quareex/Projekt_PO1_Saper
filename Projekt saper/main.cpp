#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <locale.h>

//zmienna stan przechowuje obecny stan pętli gry
stangry stan = STAN_MENU;
//zmienna gracz przechowuje dane gracza
DaneGracza gracz;
//zmienna plansza to dynamiczna tablica dwuwymiarowa przechowująca miejsce bomb
char** plansza = NULL;
//zmienna odkryta_plansza to dynamiczna tablica dwuwymiarowa przechowujaca stan pol (czy sa odkryte, czy jest na nich flaga)
char** odkryta_plansza = NULL;
//zmienna running kończy główną pętle gry gdy ustawiona na false
bool running = true;
//zmienna przechowuje rozmiar planszy
int rozmiar = 0;
//zmienna przechowuje ilość min
int bomby = 0;

int main() {
    setlocale(LC_ALL, "pl_PL.UTF-8");

    //Deklaracje struktur danych display, font, event_queue oraz timer
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT* font = NULL;
    ALLEGRO_FONT* font2 = NULL;
    ALLEGRO_FONT* font3 = NULL;
    ALLEGRO_FONT* font4 = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;

    //zmienne do licznika czasu
    double czas_start, czas_koniec;
    //zmienna do sprawdzania długości nazwy
    int dlugosc_nazwy = 0;
    //tablica przechowująca nazwę gracza
    char nazwa[20];
    //zmienna ranking przechowująca dane rankingu
    Ranking ranking;
    //zmienna przechowująca czy to pierwszy ruch
    int pierwszy_ruch = 1;

    inicjuj_ranking(&ranking);
    wczytaj_ranking_z_pliku("wyniki.txt", &ranking);

    int trudnosc = 0;

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

    ///GŁÓWNA PĘTLA GRY
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
                wyswietlenie_menu(font, font2);
            }
            else if (stan == STAN_WYBIERANIE_NAZWY) {
                wyswietlenie_nazwa(font, font3, nazwa);
            }
            else if (stan == STAN_WYBIERANIE_TRUDNOSCI) {
                wyswietlenie_wyboru_trudnosci(font, font3);
            }
            else if (stan == STAN_POMOC) {
                wyswietlenie_pomoc(font, font4);
            }
            else if (stan == STAN_RANKING) {
                wyswietlenie_ranking(font, &ranking);
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
                pierwszy_ruch = 1;
                int wygrana = sprawdz_wygrana(plansza, odkryta_plansza);
                al_rest(0.01);
                wyswietlenie_konca_gry(font, font3, wygrana, czas_start, czas_koniec, &ranking);
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
                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER && dlugosc_nazwy != 0) {
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
                sterowanie_trudnosc(event, trudnosc);
            }
            else if (stan == STAN_POMOC) {
                sterowanie_pomoc(event);
            }
            else if (stan == STAN_RANKING) {
                sterowanie_ranking(event);
            }
            else if (stan == STAN_EKRAN_GRY) {
                sterowanie_gra(event, &pierwszy_ruch);
            }
            else if (stan == STAN_KONIEC_GRY) {
                sterowanie_koniec(event, trudnosc);
            }
        }
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
