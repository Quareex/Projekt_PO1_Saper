#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

/// <summary>
/// Funkcja do wyświetlania planszy, licznika czasu oraz ilości postawionych flag podczas gry
/// </summary>
/// <param name="plansza">plansza przechowująca liczby oraz miny</param>
/// <param name="odkryta_plansza">plansza przechowująca stan planszy (flagi, odkryte pola, nieodkryte pola)</param>
/// <param name="rozmiar">rozmiar planszy</param>
/// <param name="font">czcionka</param>
/// <param name="czas1">zmienna do licznika czasu (start)</param>
/// <param name="czas2">zmienna do licznika czasu (koniec)</param>
void wyswietlenie_planszy(char** plansza, char** odkryta_plansza, ALLEGRO_FONT* font, double czas1, double czas2) {
    // Obliczenie rozmiaru planszy w pikselach
    float rozmiar_planszy = rozmiar * CELL_SIZE;
    // Obliczenie odstepu x i y by wysrodkowac plansze
    float offset_x = (SCREEN_WIDTH - rozmiar_planszy) / 2;
    float offset_y = (SCREEN_HEIGHT - rozmiar_planszy) / 2;
    int minuty, sekundy;
    int flagi = licznik_flag(odkryta_plansza);
    licznik_czasu(czas1, czas2, &minuty, &sekundy);

    al_draw_textf(font, al_map_rgb(0, 0, 0), 225, 50, ALLEGRO_ALIGN_CENTER, u8"Czas: %02d:%02d", minuty, sekundy);
    al_draw_textf(font, al_map_rgb(0, 0, 0), 375, 50, ALLEGRO_ALIGN_CENTER, u8"Ilość flag: %d", flagi);


    for (int i = 0; i < rozmiar; i++) {
        for (int j = 0; j < rozmiar; j++) {
            //Obliczanie pozycji x i y pól na planszy
            float x = offset_x + j * CELL_SIZE;
            float y = offset_y + i * CELL_SIZE;
            //Wyswietlanie planszy
            if (odkryta_plansza[i][j] == 0) {
                //Zakryte pola
                al_draw_rectangle(offset_x - 8, offset_y - 8, offset_x + 8 + rozmiar_planszy, offset_y + 8 + rozmiar_planszy, al_map_rgb(100, 100, 100), 8);
                al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(192, 192, 192));
                al_draw_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(128, 128, 128), BORDER_SIZE);
            }
            else {
                //Odkryte pola
                al_draw_rectangle(offset_x - 8, offset_y - 8, offset_x + 8 + rozmiar_planszy, offset_y + 8 + rozmiar_planszy, al_map_rgb(100, 100, 100), 8);
                al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(224, 224, 224));
                al_draw_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(128, 128, 128), BORDER_SIZE);
                if (odkryta_plansza[i][j] == 1) {
                    //Liczby min w otoczeniu pola
                    if (plansza[i][j] != '0')
                        al_draw_textf(font, al_map_rgb(0, 0, 0), x + CELL_SIZE / 2, y + CELL_SIZE / 2 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "%c", plansza[i][j]);
                }
                else if (odkryta_plansza[i][j] == 2) {
                    //Flagi
                    al_draw_text(font, al_map_rgb(255, 0, 0), x + CELL_SIZE / 2, y + CELL_SIZE / 2 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "F");
                }
            }
        }
    }
}


/// <summary>
/// Funkcja do wyświetlania ekranu menu
/// </summary>
/// <param name="font">czcionka</param>
/// <param name="font2">czcionka</param>
void wyswietlenie_menu(ALLEGRO_FONT* font, ALLEGRO_FONT* font2) {
    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, START_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, START_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));
    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, LEADERBOARD_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, LEADERBOARD_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));
    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, HELP_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, HELP_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));
    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, EXIT_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, EXIT_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));

    al_draw_text(font2, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 65, ALLEGRO_ALIGN_CENTER, u8"S A P E R");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, START_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Zagraj");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, LEADERBOARD_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Tabela wyników");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, HELP_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Pomoc");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, EXIT_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Wyjdź");
}


/// <summary>
/// Funkcja do wyświetlania ekranu pomocy
/// </summary>
/// <param name="font">czcionka</param>
/// <param name="font3">czcionka</param>
void wyswietlenie_pomoc(ALLEGRO_FONT* font, ALLEGRO_FONT* font3) {
    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, BACK_HELP_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, BACK_HELP_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, BACK_HELP_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Wróć");

    al_draw_text(font3, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 25, ALLEGRO_ALIGN_CENTRE, u8"O Grze:");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 75, ALLEGRO_ALIGN_CENTRE, u8"Gra w sapera polega na odkrywaniu pól tak aby nie trafić na");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTRE, u8"minę. Na każdym odkrytym polu wyświetlona jest cyfra");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 125, ALLEGRO_ALIGN_CENTRE, u8"oznaczająca ile min bezpośrednio styka się z polem. Gdy");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 150, ALLEGRO_ALIGN_CENTRE, u8"min jest zero wtedy na polu nie ma napisanej żadnej liczby.");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 175, ALLEGRO_ALIGN_CENTRE, u8"Należy użyć tych informacji by wydedukować, które pola są");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 200, ALLEGRO_ALIGN_CENTRE, u8"bezpieczne do odkrycia. Prawym przyciskiem myszy można");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 225, ALLEGRO_ALIGN_CENTRE, u8"oznaczyć pole flagą. Gdy na polu znajduje się flaga wtedy");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 250, ALLEGRO_ALIGN_CENTRE, u8"pole nie może zostać odkryte dzięki czemu nie odsłonimy");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 275, ALLEGRO_ALIGN_CENTRE, u8"przypadkiem miny. By wygrać gracz musi oznaczyć wszystkie");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 300, ALLEGRO_ALIGN_CENTRE, u8"miny flagą oraz odkryć wszystkie pola bez min. Poziomy");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 325, ALLEGRO_ALIGN_CENTRE, u8"trudności zmieniają rozmiar planszy oraz ilość min.");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 375, ALLEGRO_ALIGN_CENTRE, u8"Łatwy: 8x8 10 min, Średni: 12x12 25 min, Trudny: 16x16 40 min.");
}


/// <summary>
/// Funkcja do wyświetlania ekranu rankingu
/// </summary>
/// <param name="font">czcionka</param>
/// <param name="ranking">zmienna ranking przechowująca dane o rankingu</param>
void wyswietlenie_ranking(ALLEGRO_FONT* font, Ranking* ranking) {
    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, BACK_HELP_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, BACK_HELP_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, BACK_HELP_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Wróć");

    int y = 50;
    int x = SCREEN_WIDTH / 2;

    //Łatwy poziom trudności
    al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, u8"Najlepsze wyniki (Łatwy poziom trudności):");
    y += 30;
    if (is_empty(ranking->ranking_latwy)) {
        al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, u8"tabela wyników jest pusta");
    }
    else {
        for (int i = 0; i < 3; i++) {
            if (ranking->ranking_latwy[i].minuty != 0 || ranking->ranking_latwy[i].sekundy != 0) {
                char buf[100];
                snprintf(buf, sizeof(buf), "%d. %s  Czas: %02d:%02d", i + 1, ranking->ranking_latwy[i].nazwa_gracza, ranking->ranking_latwy[i].minuty, ranking->ranking_latwy[i].sekundy);
                al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, buf);
                y += 20;
            }
        }
    }
    y += 30;

    //Średni poziom trudności
    al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, u8"Najlepsze wyniki (Średni poziom trudności):");
    y += 30;
    if (is_empty(ranking->ranking_sredni)) {
        al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, u8"tabela wyników jest pusta");
    }
    else {
        for (int i = 0; i < 3; i++) {
            if (ranking->ranking_sredni[i].minuty != 0 || ranking->ranking_sredni[i].sekundy != 0) {
                char buf[100];
                snprintf(buf, sizeof(buf), "%d. %s  Czas: %02d:%02d", i + 1, ranking->ranking_sredni[i].nazwa_gracza, ranking->ranking_sredni[i].minuty, ranking->ranking_sredni[i].sekundy);
                al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, buf);
                y += 20;
            }
        }
    }
    y += 30;

    //Trudny poziom trudności
    al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, u8"Najlepsze wyniki (Trudny poziom trudności):");
    y += 30;
    if (is_empty(ranking->ranking_trudny)) {
        al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, u8"tabela wyników jest pusta");
    }
    else {
        for (int i = 0; i < 3; i++) {
            if (ranking->ranking_trudny[i].minuty != 0 || ranking->ranking_trudny[i].sekundy != 0) {
                char buf[100];
                snprintf(buf, sizeof(buf), "%d. %s  Czas: %02d:%02d", i + 1, ranking->ranking_trudny[i].nazwa_gracza, ranking->ranking_trudny[i].minuty, ranking->ranking_trudny[i].sekundy);
                al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, buf);
                y += 20;
            }
        }
    }
}


/// <summary>
/// Funkcja do wyświetlania ekranu wyboru trudności
/// </summary>
/// <param name="font">czcionka</param>
/// <param name="font3">czcionka</param>
void wyswietlenie_wyboru_trudnosci(ALLEGRO_FONT* font, ALLEGRO_FONT* font3) {
    al_draw_text(font3, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, u8"Wybierz poziom trudności");

    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, EASY_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, EASY_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(0, 255, 0));
    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, MEDIUM_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, MEDIUM_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 0));
    al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, HARD_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, HARD_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 0, 0));
    //al_draw_filled_rounded_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 375, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, 375 + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));

    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, EASY_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Łatwy");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, MEDIUM_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Średni");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, HARD_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Trudny");
    //al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 390, ALLEGRO_ALIGN_CENTER, u8"Test");
}


/// <summary>
/// Funkcja do wyświetlania ekranu końca gry
/// </summary>
/// <param name="font">czcionka</param>
/// <param name="font3">czcionka</param>
/// <param name="wygrana">zmienna przechowująca czy gra była przegrana czy wygrana</param>
/// <param name="czas1">zmienna do licznika czasu (start)</param>
/// <param name="czas2">zmienna do licznika czasu (koniec)</param>
/// <param name="ranking">zmienna ranking przechowująca dane o rankingu</param>
void wyswietlenie_konca_gry(ALLEGRO_FONT* font, ALLEGRO_FONT* font3, int wygrana, double czas1, double czas2, Ranking* ranking) {
    int minuty;
    int sekundy;
    licznik_czasu(czas1, czas2, &minuty, &sekundy);

    if (wygrana) {
        al_draw_text(font3, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, u8"Wygrałeś!");
        al_draw_textf(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 150, ALLEGRO_ALIGN_CENTER, u8"Końcowy czas: %02u:%02u", minuty, sekundy);
        gracz.minuty = minuty;
        gracz.sekundy = sekundy;
        dodaj_do_rankingu(ranking, gracz);
        gracz.minuty = 0;
        gracz.sekundy = 0;
        gracz.poziom_trudnosci = 0;
        zapisz_ranking_do_pliku("wyniki.txt", ranking);
    }
    else {
        al_draw_text(font3, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, u8"Przegrałeś!");
    }
    al_draw_filled_rounded_rectangle(90, PLAY_AGAIN_BUTTON_Y, 290, PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));
    al_draw_filled_rounded_rectangle(310, PLAY_AGAIN_BUTTON_Y, 510, PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT, 6, 6, al_map_rgb(255, 255, 255));
    al_draw_text(font, al_map_rgb(0, 0, 0), 410, PLAY_AGAIN_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Menu");
    al_draw_text(font, al_map_rgb(0, 0, 0), 190, PLAY_AGAIN_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, u8"Zagraj ponownie");
}


/// <summary>
/// Funkcja do wyświetlania ekranu wyboru nazwy
/// </summary>
/// <param name="font">czcionka</param>
/// <param name="font3">czcionka</param>
/// <param name="nazwa">tablica zawierająca nazwę</param>
void wyswietlenie_nazwa(ALLEGRO_FONT* font, ALLEGRO_FONT* font3, const char* nazwa) {
    al_draw_text(font3, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 150, ALLEGRO_ALIGN_CENTRE, u8"Wprowadź nazwę:");
    al_draw_filled_rectangle(115, SCREEN_HEIGHT / 2 - 15, 485, SCREEN_HEIGHT / 2 + 15, al_map_rgb(255, 255, 255));
    al_draw_rectangle(115, SCREEN_HEIGHT / 2 - 15, 485, SCREEN_HEIGHT / 2 + 15, al_map_rgb(0, 0, 0),4);
    al_draw_text(font, al_map_rgb(0, 0, 0), 120, SCREEN_HEIGHT / 2-13, ALLEGRO_ALIGN_LEFT, nazwa);
}
