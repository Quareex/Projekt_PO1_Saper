#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"
#include <allegro5\allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

//Funkcja sprawdzajaca czy wymagania do wygrania gry sa spelnione (flagi na wszystkich minach i tylko na minach oraz odkryc wszystkie puste pola)
int sprawdz_wygrana(char** plansza, char** odkryta_plansza, int rozmiar, int bomby) {
    //zmienna przechowuje ilosc flag poprawnie ustawionych na polach z minami.
    int oznaczone_miny = 0;
    //zmienna przechowuje ile flag oznacza pola bez min.
    int zle_umieszczone_flagi = 0;
    //zmienna przechowuje ilosc odkrytych pol
    int odkryte_pola = 0;

    for (int i = 0; i < rozmiar; i++){
        for (int j = 0; j < rozmiar; j++){
            //if sprawdza czy wybrane pole jest mina i czy jego wartosc wynosi 2 (2 oznacza ze ustawiona jest tam flaga)
            if (plansza[i][j] == '*' && odkryta_plansza[i][j] == 2)
                oznaczone_miny++;
            //if sprawdza czy wybrane pole nie jest mina i czy jego wartosc wynosi 2 (2 oznacza ze ustawiona jest tam flaga)
            if (plansza[i][j] != '*' && odkryta_plansza[i][j] == 2)
                zle_umieszczone_flagi++;
            //if sprawdza czy pole jest odkryte
            if (plansza[i][j] != '*' && odkryta_plansza[i][j] == 1)
                odkryte_pola++;
        }
    }

    //Funkcja zwraca 1 gdy spelniony jest warunek do wygrania gry i 0 gdy nie spelnia.
    if (oznaczone_miny == bomby && zle_umieszczone_flagi == 0 && odkryte_pola == (rozmiar * rozmiar - bomby))
        return 1;
    else
        return 0;
}


//Funkcja do alokacji pamieci dla tablicy dwuwymiarowej. Wypelnia ja zerami.
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


//Funkcja do alokacji pamieci dla tablicy dwuwymiarowej. Wypelnia ja znakiem '.'.
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


//Funkcja do zwolnienia zaalokowanej pamieci dla tablic dwuwymiarowych po skonczeniu gry.
void zwolnij_pamiec(char** tab, int rozmiar){
    for (int i = 0; i < rozmiar; i++)
        free(tab[i]);
    free(tab);
}


//Funkcja generujaca bomby w losowych miejscach na planszy.
void generacja_bomb(char** plansza, int rozmiar, int bomby){
    //zmienna sprawdzajaca czy ilosc wygenerowanych min nie przekracza ograniczenia.
    int k = 0;
    srand(time(NULL));

    while (k < bomby){
        int i = rand() % rozmiar;
        int j = rand() % rozmiar;
        //if sprawdza czy dane pole nie posiada juz miny.
        if (plansza[i][j] != '*'){
            plansza[i][j] = '*';
            k++;
        }
    }
}


//Funkcja odkrywajaca pola na planszy
int odkrywanie_planszy(char** plansza, char** odkryta_plansza, int rozmiar, int rzad, int kolumna){
    //Sprawdza czy podane wspolrzedne nie sa ujemne i czy nie sa wieksze niz rozmiar planszy. Gdy zwraca 2 oznacza ze wspolrzedne sa nieprawidlowa.
    if (rzad < 0 || rzad >= rozmiar || kolumna < 0 || kolumna >= rozmiar || odkryta_plansza[rzad][kolumna])
        return 2;

    //oznacza pole jako 1 czyli jako odkryte.
    odkryta_plansza[rzad][kolumna] = 1;

    //jesli odkryte pole jest mina to zwraca -1 co oznacza ze gra jest przegrana.
    if (plansza[rzad][kolumna] == '*')
        return -1;

    if (plansza[rzad][kolumna] == '.'){
        //zmienna przechowujaca ile min znajduje sie w otoczeniu pola.
        int k = 0;
        for (int i = rzad - 1; i <= rzad + 1; i++){
            for (int j = kolumna - 1; j <= kolumna + 1; j++){
                //Sprawdza czy pola mieszcza sie na planszy i czy sa mina.
                if (i >= 0 && i < rozmiar && j >= 0 && j < rozmiar && plansza[i][j] == '*')
                    k++;
            }
        }
        //if sprawdzajacy czy znaleziono mine w otoczeniu.
        if (k > 0){
            //umieszcza odpowiednia liczbe min w otoczeniu.
            plansza[rzad][kolumna] = k + '0';
            return 0;
        }
        else{
            //gdy nie ma min w otoczeniu to oznacza pole jako znak '0'.
            plansza[rzad][kolumna] = '0';
            for (int i = rzad - 1; i <= rzad + 1; i++)
                for (int j = kolumna - 1; j <= kolumna + 1; j++)
                    //Sprawdza czy pola mieszcza sie na planszy i czy pole zostalo odkryte.
                    if (i >= 0 && i < rozmiar && j >= 0 && j < rozmiar && !odkryta_plansza[i][j])
                        //rekurencyjne wywoluje funkcje by sprawdzic pola w okolicy rekurencyjnie.
                        odkrywanie_planszy(plansza, odkryta_plansza, rozmiar, i, j);
        }
    }
    return 1;
}

//Funkcja do wyswietlania planszy w oknie
void wyswietelnie_planszy(char** plansza, char** odkryta_plansza, int rozmiar, ALLEGRO_FONT* font){
    // Obliczenie rozmiaru planszy w pikselach
    float rozmiar_planszy = rozmiar * CELL_SIZE;
    // Obliczenie odstepu x i y by wysrodkowac plansze
    float offset_x = (SCREEN_WIDTH - rozmiar_planszy) / 2;
    float offset_y = (SCREEN_HEIGHT - rozmiar_planszy) / 2;

    for (int i = 0; i < rozmiar; i++) {
        for (int j = 0; j < rozmiar; j++) {
            //Obliczanie pozycji x i y pol na planszy
            float x = offset_x + j * CELL_SIZE;
            float y = offset_y + i * CELL_SIZE;
            //Wyswietlanie planszy
            if (odkryta_plansza[i][j] == 0) {
                //Zakryte pola
                al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(192, 192, 192));
                al_draw_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(128, 128, 128), BORDER_SIZE);
            }
            else {
                //Odkryte pola
                al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(224,224,224));
                al_draw_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(128, 128, 128), BORDER_SIZE);
                if (odkryta_plansza[i][j] == 1) {
                    //Liczby min w otoczeniu pola
                    if (plansza[i][j] != '0')
                        al_draw_textf(font, al_map_rgb(0, 0, 0), x + CELL_SIZE / 2, y + CELL_SIZE / 2 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "%c", plansza[i][j]);
                }
                else if (odkryta_plansza[i][j] == 2){
                    //Flagi
                    al_draw_text(font, al_map_rgb(255, 0, 0), x + CELL_SIZE / 2, y + CELL_SIZE / 2 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "F");
                }
            }
        }
    }
}

//Funkcja do wyswietlania ekranu z wyborem poziomu trudnosci
void wyswietelnie_wyboru_trudnosci(ALLEGRO_FONT* font) {
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "Wybierz poziom trudnosci");
    al_draw_filled_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, EASY_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, EASY_BUTTON_Y + BUTTON_HEIGHT, al_map_rgb(0, 255, 0));
    al_draw_filled_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, MEDIUM_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, MEDIUM_BUTTON_Y + BUTTON_HEIGHT, al_map_rgb(255, 255, 0));
    al_draw_filled_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, HARD_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, HARD_BUTTON_Y + BUTTON_HEIGHT, al_map_rgb(255, 0, 0));

    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, EASY_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, "LATWY");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, MEDIUM_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, "SREDNI");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, HARD_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, "TRUDNY");
}

//Funkcja do wyswietlania ekranu wygranej/przegranej
void Wyswietlenie_konca_gry(ALLEGRO_FONT* font, int wygrana) {
    if (wygrana) {
        al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "Wygrales!");
    }
    else {
        al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "Trafiles na mine!");
    }
    al_draw_filled_rectangle((SCREEN_WIDTH - BUTTON_WIDTH) / 2, PLAY_AGAIN_BUTTON_Y, (SCREEN_WIDTH + BUTTON_WIDTH) / 2, PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT, al_map_rgb(0, 0, 255));
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, PLAY_AGAIN_BUTTON_Y + 15, ALLEGRO_ALIGN_CENTER, "Zagraj ponownie");
 }

//Funkcja obslugujaca rozne typy zdarzen podczas gry
void stan_gry(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font) {
    while (1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        //Przerywa petle i konczy gre
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
         else if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(255, 255, 255));
            
            //Petle sprawdzajace ktory ekran ma byc wyswietlony
            if (stan == STAN_WYBIERANIE_TRUDNOSCI) {
                wyswietelnie_wyboru_trudnosci(font);
            }
            else if (stan == STAN_START_GRY) {
                stan = STAN_EKRAN_GRY;
            }
            else if (stan == STAN_EKRAN_GRY) {
                wyswietelnie_planszy(plansza, odkryta_plansza, rozmiar, font);
            }
            else if (stan == STAN_KONIEC_GRY) {
                int wygrana = sprawdz_wygrana(plansza, odkryta_plansza, rozmiar, bomby);
                Wyswietlenie_konca_gry(font, wygrana);
            }
           
            //Funkcja aktualizujaca wyswietlany obraz
            al_flip_display();
         }

        //Petle obslugujace sterowanie myszka podczas dzialania gry
         else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (stan == STAN_WYBIERANIE_TRUDNOSCI) {
                sterowanie_trudnosc(event, font);
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

//Funkcja do sterowania myszka podczas ekranu wyboru poziomu trudnosci
void sterowanie_trudnosc(ALLEGRO_EVENT event, ALLEGRO_FONT* font) {
    int mouse_x = event.mouse.x;
    int mouse_y = event.mouse.y;

    //Petle sprawdzajace ktory poziom trudnosci zostal klikniety przez gracza
    if (mouse_x >= (SCREEN_WIDTH - BUTTON_WIDTH) / 2 && mouse_x <= (SCREEN_WIDTH + BUTTON_WIDTH) / 2) {
        if (mouse_y >= EASY_BUTTON_Y && mouse_y <= EASY_BUTTON_Y + BUTTON_HEIGHT) {
            trudnosc = 1;
        }
        else if (mouse_y >= MEDIUM_BUTTON_Y && mouse_y <= MEDIUM_BUTTON_Y + BUTTON_HEIGHT) {
            trudnosc = 2;
        }
        else if (mouse_y >= HARD_BUTTON_Y && mouse_y <= HARD_BUTTON_Y + BUTTON_HEIGHT) {
            trudnosc = 3;
        }

        //petla kontunuujaca gre po wyborze poziomu trudnosci
        if (trudnosc > 0) {
            ustawienie_trudnosci(trudnosc);
            stan = STAN_START_GRY;
        }
    }
}

//Funkcja do sterowania myszka podczas gry
void sterowanie_gra(ALLEGRO_EVENT event) {
    int mouse_x = event.mouse.x;
    int mouse_y = event.mouse.y;
    float rozmiar_planszy = rozmiar * CELL_SIZE;
    float offset_x = (SCREEN_WIDTH - rozmiar_planszy) / 2;
    float offset_y = (SCREEN_HEIGHT - rozmiar_planszy) / 2;

    //Petla sprawdzajaca czy klikniety obszar znajduje sie na planszy
    if (mouse_x >= offset_x && mouse_x <= offset_x + rozmiar_planszy && mouse_y >= offset_y && mouse_y <= offset_y + rozmiar_planszy) {
        //Obliczanie  ktore pole zostalo klikniete
        int kolumna = (mouse_x - offset_x) / CELL_SIZE;
        int rzad = (mouse_y - offset_y) / CELL_SIZE;

        //Sprawdzenie jaki przycisk zostal uzyty przez gracza
        if (event.mouse.button == 1) {
            odkrycie_pola(rzad, kolumna);
        }
        else if (event.mouse.button == 2) {
            flaga(rzad, kolumna);
        }
    }
}

//Funkcja do sterowania myszka na ekranie konca gry
void sterowanie_koniec(ALLEGRO_EVENT event) {
    int mouse_x = event.mouse.x;
    int mouse_y = event.mouse.y;

    if (mouse_x >= (SCREEN_WIDTH - BUTTON_WIDTH) / 2 && mouse_x <= (SCREEN_WIDTH + BUTTON_WIDTH) / 2 && mouse_y >= PLAY_AGAIN_BUTTON_Y && mouse_y <= PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT) {
        reset_gry();
    }
}

//Funkcja do wyczyszczenia planszy oraz zwolnienia pamieci po zakonczeniu gry
void reset_gry() {
    stan = STAN_WYBIERANIE_TRUDNOSCI;
    trudnosc = 0;

    if (plansza) {
        zwolnij_pamiec(plansza, rozmiar);
        plansza = NULL;
    }
    if (odkryta_plansza) {
        zwolnij_pamiec(odkryta_plansza, rozmiar);
        odkryta_plansza = NULL;
    }
}

//Funkcja zawierajaca switch z ustawieniami danych poziomow trudnosci, bo wyborze alokuje pamiec dla tablic dynamicznych oraz generuje bomby
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
    default:
        break;
    }

    plansza = alokacja_planszy(rozmiar);
    odkryta_plansza = alokacja_odkrytej_planszy(rozmiar);
    generacja_bomb(plansza, rozmiar, bomby);
}

//Funkcja do odkrywania pol, sprawdza czy gracz wygral lub trafil na mine
void odkrycie_pola(int rzad, int kolumna) {
    int wynik = odkrywanie_planszy(plansza, odkryta_plansza, rozmiar, rzad, kolumna);
    if (wynik == -1) {
        stan = STAN_KONIEC_GRY;
    }
    
    if (sprawdz_wygrana(plansza, odkryta_plansza, rozmiar, bomby)) {
        stan = STAN_KONIEC_GRY;
    }
}


//Funkcja do stawiania flag na planszy
void flaga(int rzad, int kolumna) {
    if (odkryta_plansza[rzad][kolumna] == 0) {
        odkryta_plansza[rzad][kolumna] = 2;
    }
    else if (odkryta_plansza[rzad][kolumna] == 2) {
        odkryta_plansza[rzad][kolumna] = 0;
    }
}