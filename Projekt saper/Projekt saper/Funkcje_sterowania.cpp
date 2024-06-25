#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"
#include <allegro5\allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

/// <summary>
/// Funkcja do obs³ugi sterowania myszk¹ na ekranie menu
/// </summary>
/// <param name="event">zdarzenie</param>
void sterowanie_menu(ALLEGRO_EVENT event) {
    if (event.mouse.x >= (SCREEN_WIDTH - BUTTON_WIDTH) / 2 && event.mouse.x <= (SCREEN_WIDTH + BUTTON_WIDTH) / 2) {
        if (strlen(gracz.nazwa_gracza) == 0) {
            if (event.mouse.y >= START_BUTTON_Y && event.mouse.y <= START_BUTTON_Y + BUTTON_HEIGHT) {
                stan = STAN_WYBIERANIE_NAZWY;
            }
        }
        else {
            if (event.mouse.y >= START_BUTTON_Y && event.mouse.y <= START_BUTTON_Y + BUTTON_HEIGHT) {
                stan = STAN_WYBIERANIE_TRUDNOSCI;
            }
        }
        if (event.mouse.y >= HELP_BUTTON_Y && event.mouse.y <= HELP_BUTTON_Y + BUTTON_HEIGHT) {
            stan = STAN_POMOC;
        }
        else if (event.mouse.y >= LEADERBOARD_BUTTON_Y && event.mouse.y <= LEADERBOARD_BUTTON_Y + BUTTON_HEIGHT) {
            stan = STAN_RANKING;
        }
        else if (event.mouse.y >= EXIT_BUTTON_Y && event.mouse.y <= EXIT_BUTTON_Y + BUTTON_HEIGHT) {
            running = false;
        }
    }
}


/// <summary>
/// Funkcja do obs³ugi sterowania myszk¹ na ekranie pomocy
/// </summary>
/// <param name="event">zdarzenie</param>
void sterowanie_pomoc(ALLEGRO_EVENT event) {
    if (event.mouse.x >= (SCREEN_WIDTH - BUTTON_WIDTH) / 2 && event.mouse.x <= (SCREEN_WIDTH + BUTTON_WIDTH) / 2) {
        if (event.mouse.y >= BACK_HELP_BUTTON_Y && event.mouse.y <= BACK_HELP_BUTTON_Y + BUTTON_HEIGHT) {
            stan = STAN_MENU;
        }
    }
}


/// <summary>
/// Funkcja do obs³ugi sterowania myszk¹ na ekranie rankingu
/// </summary>
/// <param name="event">zdarzenie</param>
void sterowanie_ranking(ALLEGRO_EVENT event) {
    if (event.mouse.x >= (SCREEN_WIDTH - BUTTON_WIDTH) / 2 && event.mouse.x <= (SCREEN_WIDTH + BUTTON_WIDTH) / 2) {
        if (event.mouse.y >= BACK_HELP_BUTTON_Y && event.mouse.y <= BACK_HELP_BUTTON_Y + BUTTON_HEIGHT) {
            stan = STAN_MENU;
        }
    }
}


/// <summary>
/// Funkcja do obs³ugi sterowania myszk¹ na ekranie wyboru poziomu trudnoœci
/// </summary>
/// <param name="event">zdarzenie</param>
/// <param name="trudnosc">zmienna przechowuj¹ca poziom trudnoœci</param>
void sterowanie_trudnosc(ALLEGRO_EVENT event, int trudnosc) {
    if (event.mouse.x >= (SCREEN_WIDTH - BUTTON_WIDTH) / 2 && event.mouse.x <= (SCREEN_WIDTH + BUTTON_WIDTH) / 2) {
        if (event.mouse.y >= EASY_BUTTON_Y && event.mouse.y <= EASY_BUTTON_Y + BUTTON_HEIGHT) {
            trudnosc = 1;
        }
        else if (event.mouse.y >= MEDIUM_BUTTON_Y && event.mouse.y <= MEDIUM_BUTTON_Y + BUTTON_HEIGHT) {
            trudnosc = 2;
        }
        else if (event.mouse.y >= HARD_BUTTON_Y && event.mouse.y <= HARD_BUTTON_Y + BUTTON_HEIGHT) {
            trudnosc = 3;
        }
        /*
        else if (event.mouse.y >= 375 && event.mouse.y <= 375 + BUTTON_HEIGHT) {
            trudnosc = 4;
        }
        */
        if (trudnosc > 0) {
            ustawienie_trudnosci(trudnosc);
            stan = STAN_START_GRY;
        }
    }
}


/// <summary>
/// Funkcja do obs³ugi sterowania myszk¹ na ekranie gry
/// </summary>
/// <param name="event">zdarzenie</param>
void sterowanie_gra(ALLEGRO_EVENT event, int* pierwszy_ruch) {
    float rozmiar_planszy = rozmiar * CELL_SIZE;
    float offset_x = (SCREEN_WIDTH - rozmiar_planszy) / 2;
    float offset_y = (SCREEN_HEIGHT - rozmiar_planszy) / 2;
    int kolumna = (event.mouse.x - offset_x) / CELL_SIZE;
    int rzad = (event.mouse.y - offset_y) / CELL_SIZE;

    if (event.mouse.x >= offset_x && event.mouse.x <= offset_x + rozmiar_planszy && event.mouse.y >= offset_y && event.mouse.y <= offset_y + rozmiar_planszy) {
        if (event.mouse.button == 1) {
            if (odkryta_plansza[rzad][kolumna] != 2) {
                int wynik = odkrywanie_planszy(plansza, odkryta_plansza, rzad, kolumna, rozmiar, pierwszy_ruch);
                if (wynik == -1) {
                    stan = STAN_KONIEC_GRY;
                }
            }
        }
        else if (event.mouse.button == 2) {
            if (odkryta_plansza[rzad][kolumna] == 0) {
                odkryta_plansza[rzad][kolumna] = 2;
            }
            else if (odkryta_plansza[rzad][kolumna] == 2) {
                odkryta_plansza[rzad][kolumna] = 0;
            }
        }
    }
}


/// <summary>
/// Funkcja do obs³ugi sterowania na ekranie koñca gry.
/// </summary>
/// <param name="event">zdarzenie</param>
/// <param name="trudnosc">zmienna przechowuj¹ca poziom trudnoœci</param>
void sterowanie_koniec(ALLEGRO_EVENT event, int trudnosc) {
    if (event.mouse.x >= 310 && event.mouse.x <= 510 && event.mouse.y >= PLAY_AGAIN_BUTTON_Y && event.mouse.y <= PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT) {
        stan = STAN_MENU;
        trudnosc = 0;
        if (plansza) {
            zwolnij_pamiec(plansza);
            plansza = NULL;
        }
        if (odkryta_plansza) {
            zwolnij_pamiec(odkryta_plansza);
            odkryta_plansza = NULL;
        }
    }
    if (event.mouse.x >= 90 && event.mouse.x <= 290 && event.mouse.y >= PLAY_AGAIN_BUTTON_Y && event.mouse.y <= PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT) {
        stan = STAN_WYBIERANIE_TRUDNOSCI;
        trudnosc = 0;
        if (plansza) {
            zwolnij_pamiec(plansza);
            plansza = NULL;
        }
        if (odkryta_plansza) {
            zwolnij_pamiec(odkryta_plansza);
            odkryta_plansza = NULL;
        }
    }
}