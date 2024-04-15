#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void wybor_pola(char **plansza, char **odkryta_plansza, int rozmiar, int bomby, int *kontynuuj_gre);
int odkrywanie_planszy(char** plansza, char** odkryta_plansza, int rozmiar, int rzad, int kolumna);
void wyswietlenie_planszy(char **plansza, char **odkryta_plansza, int rozmiar);
void generacja_bomb(char **plansza,int rozmiar, int bomby);
void zwolnij_pamiec(char **tab, int rozmiar);
void wybor_trudnosci(int *a, int *b);
char **alokacja_planszy(int rozmiar);
char **alokacja_odkrytej_planszy(int rozmiar);
int sprawdz_wygrana(char **plansza, char **odkryta_plansza, int rozmiar, int bomby);


int main()
{
    //zmienna rozmiar przechowuje rozmiar planszy a zmienna bomby ilosc bomb.
	int rozmiar, bomby;
	//zmienna przechowuje wartosc 1 gdy gra nie jest wygrana. Gdy funkcja odkrywanie_planszy() zmieni ja na 0 to gra jest wygrana.
	int kontynuuj_gre = 1;

	wybor_trudnosci(&rozmiar,&bomby);

	//alokacja pamieci dla planszy i odkrytej planszy.
	char **plansza = alokacja_planszy(rozmiar);
	if (!plansza)
    printf("blad alokacji pamieci.");

    char **odkryta_plansza = alokacja_odkrytej_planszy(rozmiar);
    if (!odkryta_plansza)
    printf("blad alokacji pamieci.");


    generacja_bomb(plansza, rozmiar, bomby);
    wyswietlenie_planszy(plansza,odkryta_plansza,rozmiar);

    wybor_pola(plansza,odkryta_plansza,rozmiar,bomby,&kontynuuj_gre);

    zwolnij_pamiec(plansza,rozmiar);
    zwolnij_pamiec(odkryta_plansza,rozmiar);

 return 0;
}

//Funkcja sprawdzajaca czy wymagania do wygrania gry sa spelnione (flagi na wszystkich minach i tylko na minach oraz odkryc wszystkie puste pola)
int sprawdz_wygrana(char **plansza, char **odkryta_plansza, int rozmiar, int bomby) {
    //zmienna przechowuje ilosc flag poprawnie ustawionych na polach z minami.
    int oznaczone_miny = 0;
    //zmienna przechowuje ile flag oznacza pola bez min.
    int zle_umieszczone_flagi = 0;
    //zmienna przechowuje ilosc odkrytych pol
    int odkryte_pola = 0;

    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; j < rozmiar; j++)
        {
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
char **alokacja_odkrytej_planszy(int rozmiar)
{
 char **odkryta_plansza = (char **)malloc(rozmiar*sizeof(char *));
 if (!odkryta_plansza)
    printf("blad alokacji pamieci.");

 for(int i=0; i<rozmiar; i++)
  {
     odkryta_plansza[i] = (char *)malloc(rozmiar*sizeof(char));
     if (!odkryta_plansza[i])
        printf("blad alokacji pamieci.");

     for (int j=0; j<rozmiar; j++)
        odkryta_plansza[i][j] = 0;
  }
  return odkryta_plansza;
}


//Funkcja do alokacji pamieci dla tablicy dwuwymiarowej. Wypelnia ja znakiem '.'.
char **alokacja_planszy(int rozmiar)
{
 char **plansza = (char **)malloc(rozmiar*sizeof(char *));
 if (!plansza)
    printf("blad alokacji pamieci.");

 for(int i=0; i<rozmiar; i++)
  {
     plansza[i] = (char *)malloc(rozmiar*sizeof(char));
     if (!plansza[i])
        printf("blad alokacji pamieci.");

     for (int j=0; j<rozmiar; j++)
        plansza[i][j] = '.';
  }
  return plansza;
}


//Funkcja do wyboru poziomu trudnosci gry.
void wybor_trudnosci(int *a, int *b)
{
	puts("Wybierz poziom trudnosci: ");
	puts("1 - latwy  (plansza 8x8, 10 min)");
	puts("2 - sredni (plansza 12x12, 25 min)");
	puts("3 - trudny (plansza 16x16, 40 min)");

	//zmienna przechowuje wybrana opcje gracza.
	int poziom;
	scanf_s("%d", &poziom);

	//switch ustawiajacy rozmiar planszy oraz ilosc bomb dla kazdego poziomu trudnosci.
	switch (poziom) {
	case 1: *a = 8; *b = 10; puts("wybrano latwy poziom trudnosci");
		break;
	case 2: *a = 12; *b = 25; puts("wybrano sredni poziom trudnosci");
		break;
	case 3: *a = 16; *b = 40; puts("wybrano trudny poziom trudnosci");
		break;
	default: *a = 8; *b = 10; puts("automatycznie wybrano latwy poziom trudnosci");
		break;
	}
}


//Funkcja do zwolnienia zaalokowanej pamieci dla tablic dwuwymiarowych po skonczeniu gry.
void zwolnij_pamiec(char **tab, int rozmiar)
{
    for(int i=0; i<rozmiar; i++)
        free(tab[i]);
    free(tab);
}


//Funkcja generujaca bomby w losowych miejscach na planszy.
void generacja_bomb(char **plansza,int rozmiar, int bomby)
{
    //zmienna sprawdzajaca czy ilosc wygenerowanych min nie przekracza ograniczenia.
    int k = 0;
    srand(time(NULL));

    while (k < bomby)
    {
      int i = rand()%rozmiar;
      int j = rand()%rozmiar;
      //if sprawdza czy dane pole nie posiada juz miny.
      if (plansza[i][j] != '*')
      {
        plansza[i][j] = '*';
        k++;
      }
    }
}



//Funkcja wyswietlajaca graczowi plansze.
void wyswietlenie_planszy(char **plansza, char **odkryta_plansza, int rozmiar)
{
    //Funkcja czyszczaca poprzednio wyswietlona plansze.
    system("cls");

    printf("\n   ");
    for (int i = 0; i < rozmiar; i++)
        //Wyswietla numery kolumn.
        printf("%*d ", 2, i);

    printf("\n");

    for (int i = 0; i < rozmiar; i++)
    {
        //Wyswietla numery wierszy
        printf("%*d  ", 2, i);
        for (int j = 0; j < rozmiar; j++)
        {
            if (odkryta_plansza[i][j])
            {
                //Sprawdza czy pole jest odkryte i wyswietla odpowiadnia wartosc
                if (odkryta_plansza[i][j] == 1)
                    printf("%c%*c", plansza[i][j], 2, ' ');
                //Sprawdza czy pole jest flaga. Jak tak to wyswietla f.
                else if (odkryta_plansza[i][j] == 2)
                    printf("f%*c", 2, ' ');
            }
            else
                //Wyswietla '.' gdy pole jest nieodkryte.
                printf(".%*c", 2, ' ');
        }
        printf("\n");
    }
}



//Funkcja odkrywajaca pola na planszy
int odkrywanie_planszy(char** plansza, char** odkryta_plansza, int rozmiar, int rzad, int kolumna)
{
    //Sprawdza czy podane wspolrzedne nie sa ujemne i czy nie sa wieksze niz rozmiar planszy. Gdy zwraca 2 oznacza ze wspolrzedne sa nieprawidlowa.
    if (rzad < 0 || rzad >= rozmiar || kolumna < 0 || kolumna >= rozmiar || odkryta_plansza[rzad][kolumna])
        return 2;

    //oznacza pole jako 1 czyli jako odkryte.
    odkryta_plansza[rzad][kolumna] = 1;

    //jesli odkryte pole jest mina to zwraca -1 co oznacza ze gra jest przegrana.
    if (plansza[rzad][kolumna] == '*')
        return -1;

    if (plansza[rzad][kolumna] == '.')
    {
        //zmienna przechowujaca ile min znajduje sie w otoczeniu pola.
        int k = 0;
        for (int i = rzad - 1; i <= rzad + 1; i++)
        {
            for (int j = kolumna - 1; j <= kolumna + 1; j++)
            {
                //Sprawdza czy pola mieszcza sie na planszy i czy sa mina.
                if (i >= 0 && i < rozmiar && j >= 0 && j < rozmiar && plansza[i][j] == '*')
                    k++;
            }
        }
        //if sprawdzajacy czy znaleziono mine w otoczeniu.
        if (k > 0)
        {
          //umieszcza odpowiednia liczbe min w otoczeniu.
          plansza[rzad][kolumna] = k + '0';
          return 0;
        }
    else
    {
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


//Funkcja pozwajajaca wybierac pola na planszy.
void wybor_pola(char **plansza, char **odkryta_plansza, int rozmiar, int bomby, int *kontynuuj_gre)
{
    //Sprawdza czy gra nie jest wygrana.
    while(*kontynuuj_gre)
    {
        //zmienne rzad i kolumna przechowuja informacje o wybranym rzedzie i kolumnie a ruch o akcji jaka chce wykonac gracz.
        int rzad, kolumna, ruch;
        printf("Wybierz jaki ruch chcesz wykonac: 1 - wybor pola, 2 - postawienie flagi, 3 - usuniecie flagi. Ruch: ");
        if (scanf("%d", &ruch) != 1)
        {
            while (getchar() != '\n');
            printf("Nieprawidlowy ruch. Podaj 1 by wybrac pole, 2 by postawic flage 3 by usunac flage.\n");
            continue;
        }

        //gdy gracz wybiera 1 wtedy moze wybrac pole na planszy do odkrycia
        if (ruch == 1)
        {
            printf("Wprowadz numer rzedu i kolumny do odkrycia: ");
            scanf(" %d %d",&rzad,&kolumna);

            //sprawdza czy wpisane przez gracza wspolrzedne znajduja sie na planszy.
            if (rzad < 0 || rzad >= rozmiar || kolumna < 0 || kolumna >= rozmiar)
            {
                printf("Nieprawidlowe wspolrzedne. Podaj wartosci z zakresu od 0 do %d.\n", rozmiar - 1);
                continue;
            }

            //zmienna przechowujaca wynik dzialania funkcji odkrywanie_planszy(). -1 - mina, 0 - oznacza ze wokol pola sa miny, 1 - oznacza ze wokol pola nie ma min, 2 - zle wspolrzedne.
            int wynik = odkrywanie_planszy(plansza, odkryta_plansza, rozmiar, rzad, kolumna);

            //gdy wynik jest rowny -1 to gracz przegrywa.
            if (wynik == -1)
            {
                printf("Trafiles na mine!\n");
                for (int i = 0; i < rozmiar; i++)
                    for (int j = 0; j < rozmiar; j++)
                       //odkrywa miny po przegraniu.
                        if (plansza[i][j] == '*')
                            odkryta_plansza[i][j] = 1;

                //wyswietla plansze z odkrytymi minami.
                wyswietlenie_planszy(plansza, odkryta_plansza, rozmiar);
                //informacja dla funckji wybor_pola() ze gra jest przegrana i gracz nie moze wybrac kolejnego pola
                *kontynuuj_gre = 0;
                break;
            }
            //gdy wynik jest rowny 2 to wybrane wspolrzedne sa nieprawidlowe.
            if (wynik == 2)
                printf("Nieprawidlowe wspolrzedne. Pole jest juz odkryte lub nie jest na planszy\n");
            else
                wyswietlenie_planszy(plansza, odkryta_plansza, rozmiar);
        }

        //gdy ruch jest rowny 2 znaczy to ze gracz wybral postawienie flagi.
        else if (ruch == 2)
        {
            printf("Wprowadz numer rzedu i kolumny do zaznaczenia flaga: ");
            scanf(" %d %d",&rzad,&kolumna);

            //sprawdza czy wspolrzedne znajduja sie na planszy.
            if (rzad < 0 || rzad >= rozmiar || kolumna < 0 || kolumna >= rozmiar)
            {
                printf("Nieprawidlowe wspolrzedne. Podaj wartosci z zakresu od 0 do %d.\n", rozmiar - 1);
                continue;
            }
            //sprawdza czy pole jest rowne 0 (nie ma na sobie flagi i czy nie jest odkryte).
            if (!odkryta_plansza[rzad][kolumna])
                    odkryta_plansza[rzad][kolumna] = 2;

            else
                printf("Nie mozna postawic flagi na odkrytym polu lub polu zawierajacym flage.\n");

            wyswietlenie_planszy(plansza, odkryta_plansza, rozmiar);
        }
        //gdy ruch jest rowny 3 znaczy to ze gracz wybral usuniecie flagi.
        if (ruch == 3)
        {
            printf("Wprowadz numer rzedu i kolumny, z ktorego chcesz usunac flage: ");
            scanf(" %d %d",&rzad,&kolumna);

            ////sprawdza czy wspolrzedne znajduja sie na planszy.
            if (rzad < 0 || rzad >= rozmiar || kolumna < 0 || kolumna >= rozmiar)
            {
                printf("Nieprawidlowe wspolrzedne. Podaj wartosci z zakresu od 0 do %d.\n", rozmiar - 1);
                continue;
            }

            //sprawdza czy wybrane pole to flaga, jak tak to ja usuwa i wyswietla plansze.
            if (odkryta_plansza[rzad][kolumna] == 2)
            {
                odkryta_plansza[rzad][kolumna] = 0;
                wyswietlenie_planszy(plansza, odkryta_plansza, rozmiar);
            }
            else
                printf("Na wybranym polu nie ma flagi do usuniecia.\n");

        }
        //gdy ruch jest spoza zakresu wysyla o tym komunikat.
        if (ruch > 3)
            printf("Nieprawidlowy ruch. Podaj 1 by wybrac pole, 2 by postawic flage 3 by usunac flage.\n");

        //sprawdza czy po wykonaniu przez gracza ruchu gra jest juz wygrana poprzez funkcje sprawdz_wygrana().
        if (sprawdz_wygrana(plansza, odkryta_plansza, rozmiar, bomby))
        {
            printf("Gratulacje! Wygrales!");
            *kontynuuj_gre = 0;
        }
    }
}
