#include <stdio.h>
#include <windows.h> //knihovna pro getKey, goto
#include <conio.h> //knihovna pro funkci kbhit
#include <iostream>
#include <string.h>
#include <time.h>

#define V 25  //vyska mapy
#define S 75 //sirka mapy
#define N 200 //max. velikost hada

using namespace std;

WORD getKey() {         // prideluje sipkam na klavesnici hodnotu
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD inrec;
	DWORD n;
	while (ReadConsoleInput(hin, &inrec, 1, &n) && n)
		if (inrec.EventType == KEY_EVENT && inrec.Event.KeyEvent.bKeyDown)
			return inrec.Event.KeyEvent.wVirtualKeyCode;
	exit(EXIT_FAILURE);
	return 0;
}

typedef struct {

	char nick[16];
	char score[6];

}hraci;
hraci hrac[11];

typedef struct
{
	int x, y; // pocatecni pozice
	int movX, movY; //souradnice pohybu po osach

	char image;
} hadisko;

typedef struct
{
	int x, y;
} fruit;

hadisko had[N]; // maximalni velikost hada
fruit frt;

void begin(int* size, char map[V][S]);
void intro(char map[V][S]);
void intro_data(char map[V][S], int* size);
void loop(char map[V][S], int size);
void input(char map[V][S], int* size, int* dead);
void update(char map[V][S], int size);
void intro_data2(char map[V][S], int size);
void gotoxy(int x, int y);
void show(char map[V][S]);

void writeData();
void compareScore(int score, char player[16]);
void topPlayers(int score, char nick[16]);
void table();


int score, cislo;
char nickname[16];


void sleep(double s) { //funkce pro 'sleep'
	time_t cur_time = time(NULL);
	while ((difftime(time(NULL), cur_time)) < s);
}

int main()
{
	int size;
	char map[V][S];
	begin(&size, map);
	show(map);

	printf("Hra zacne za 5 sekund, k pohybu pouzijte sipky"); //vymaze vytisknuty radek
	sleep(5);
	printf("\33[2K");
	loop(map, size);
	system("pause>0");
	
	return 0;
}


void begin(int* size, char map[V][S]) // spawnuti v pocatecni pozici
{
	int i;
	had[0].x = 32; // definovani hlavy
	had[0].y = 10; // definovani tela

	*size = 4; //pocatecni velikost hada

	srand(time(NULL));

	frt.x = rand() % (S - 2) +1; // nahodne spawnuti ovoce
	frt.y = rand() % (V - 2) +1;

	for (i = 0; i < *size; i++)
	{

		had[i].movX = 1;
		had[i].movY = 0;

	}

	intro(map);
	intro_data(map, size);
}


void show(char map[V][S]) //zobrazeni na mape
{

	int i, j;

	for (i = 0; i < V; i++)
	{
		for (j = 0; j < S; j++)
		{
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}


void intro(char map[V][S]) // definovani mapy
{
	int i, j;

	for (i = 0; i < V; i++)
	{

		for (j = 0; j < S; j++)
		{

			if (i == 0 || i == V - 1)
			{

				map[i][j] = '-';
			}
			else if (j == 0 || j == S - 1)
			{

				map[i][j] = '|';
			}
			else
			{
				map[i][j] = ' ';
			}
		}
	}
}


void intro_data(char map[V][S], int* size) // vstup dat do mapy
{
	int i, score = 0;

	for (i = 1; i < *size; i++)
	{

		had[i].x = had[i - 1].x - 1;
		had[i].y = had[i - 1].y;

		had[i].image = 'o'; // character, ktery tvori telo
	}
	had[0].image = 'O'; // hlava 



	for (i = 0; i < *size; i++) // definovani hada
	{

		map[had[i].y][had[i].x] = had[i].image;
	}

	map[frt.y][frt.x] = '*'; // definovani ovoce

}

void loop(char map[V][S], int size) //loop 
{

	int dead; //'dead=0' = hra pokracuje  ;   'dead=1' = hra konci

	dead = 0;

	do
	{
		gotoxy(0, 0);

		show(map);
		input(map, &size, &dead);
		update(map, size);

	} while (dead == 0);
}

void input(char map[V][S], int* size, int* dead)
{
	int i;
	char key;




	// chceme aby hra skoncila, pokud had narazi do sebe sameho nebo do hrany herni mapy

	if (had[0].x == 0 || had[0].x == S - 1 || had[0].y == 0 || had[0].y == V - 1) //vyhodnoceni narazu do hrany mapy
	{

		*dead = 1;
	}

	for (i = 1; i < *size && *dead == 0; i++)
	{

		if (had[0].x == had[i].x && had[0].y == had[i].y) // vyhodnoceni stretu s hlavou
		{
			*dead = 1;

		}
	}

	// pokud se hlava hada objevi v ose x a y na ovoci, sni ho

	if (had[0].x == frt.x && had[0].y == frt.y)
	{
		*size += 1; //zvetsi o clanek
		score += 5; //pri snezeni ovoce zvyseni poctu bodu o 5

		had[*size - 1].image = 'o'; //pridani clanku o hodnote 'o'

		frt.x = rand() % (S - 2) + 1;   // Spawnuti ovoce
		frt.y = rand() % (V - 2) + 1;
	}



	if (*dead == 0) // pokud 'dead=0' = hra pokracuje
	{
		printf("\nHra zacala!");
		printf("\nTvoje skore: %i\n", score);
		

		if (kbhit() == 1)
		{

			key = getKey(); // z nejakeho duvodu jsou osy invertovane, nemohl jsem prijit na duvod

			if (key == VK_DOWN && had[0].movY != -1) //pohyb dolu
			{

				had[0].movX = 0;
				had[0].movY = 1;
			}

			if (key == VK_UP && had[0].movY != 1) //pohyb nahoru
			{

				had[0].movX = 0;
				had[0].movY = -1;
			}

			if (key == VK_LEFT && had[0].movX != 1) //pohyb doleva
			{

				had[0].movX = -1;
				had[0].movY = 0;
			}

			if (key == VK_RIGHT && had[0].movX != -1) //pohyb doprava
			{

				had[0].movX = 1;
				had[0].movY = 0;
			}
		}
	}
	if (*dead == 1)  // pokud 'dead=0' = hra konci
	{
		printf("  _____                        ______                        \n");
		printf("/ ____|                       |  __  |                       \n");
		printf("| |  __  __ _ _ __ ___   ___  | |  | |  __   __  ___   _____ \n");
		printf("| | |_ |/ _` | '_ ` _ | / _ | | |  | |  || / / |  _ | |  __| \n");
		printf("| |__| | (_| | | | | | |  __/ | |__| |  | V /  |  __/ | |    \n");
		printf("|______|__,_|_| |_| |_||___|  |______|  |_/    |___   |_|    \n");

		printf("Tvoje finalni score je : %i\n", score);
		printf("\nZadejte svuj herni alias (max. 15 znaku): ");
		scanf("%s", &nickname);
		topPlayers(score, nickname);

	}
}

void update(char map[V][S], int size) // Pri zavolani teto funkce se mapa refreshne
{
	intro(map);
	intro_data2(map, size);
}

void intro_data2(char map[V][S], int size)
{
	int i;

	for (i = size - 1; i > 0; i--) //Telo nasleduje hlavu
	{

		had[i].x = had[i - 1].x;
		had[i].y = had[i - 1].y;
	}

	had[0].x += had[0].movX; // + pohyby
	had[0].y += had[0].movY;

	for (i = 0; i < size; i++)
	{

		map[had[i].y][had[i].x] = had[i].image;
	}

	map[frt.y][frt.x] = '*';
}


void gotoxy(int x, int y) // goto funkce pro koordinaci
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void topPlayers(int score, char nick[16])
{

	cislo = 0;
	int typ = 0;
	int a = 0;
	char c;

	FILE* scoreS;
	scoreS = fopen("Zebricek.txt", "r");

	while ((c = fgetc(scoreS)) != EOF)
		if (c != '*')
		{
			if (c != ',') {

				if (typ == 0)
					hrac[cislo].nick[a] = c;
				if (typ == 1)
					hrac[cislo].score[a] = c;

				a++;
			}
			else
			{
				if (typ == 0)
					hrac[cislo].nick[a] = '\0';

				a = 0;
				typ++;
			}
		}
		else
		{
			hrac[cislo].score[a] = '\0';
			a = 0;
			typ = 0;
			cislo++;
		}

	fclose(scoreS);
	compareScore(score, nick);
	writeData();
	table();
}
void table()
{
	int b = 0;
	char format[16];
	char formats[16];
	system("cls");
	printf("\n\n            ___________________\n");
	printf("           /-------------------\\   \n");
	printf("        __/--------TOP 10-------\\__\n");

	printf("	|__________________________|\n");
	printf("	|Prezdivka:         |Skore:|\n");
	printf("	|-------------------|------|\n");
	for (int a = 0; a < cislo; a++)
	{
		for (int a = 0; a < 15; a++)
		{
			format[a] = ' ';
			formats[a] = ' ';
		}

		if (a == 9)
			b = 1;
		format[(16 - strlen(hrac[a].nick) - b)] = '\0';

		formats[(5 - strlen(hrac[a].score))] = '\0';


		printf("	|%d) %s%s| %s%s|\n", (a + 1), hrac[a].nick, format, hrac[a].score, formats);
		printf("	|-------------------|------|\n");

	}

	printf("\n	");
	//system("pause");
}

void compareScore(int score, char player[16]) {

	int dalsi;
	if (score != 0) {
		for (int a = 0; a < cislo; a++)
		{
			if (score > atoi(hrac[a].score))
			{
				for (int i = cislo; i > a; i--)
				{
					dalsi = i - 1;
					strcpy(hrac[i].nick, hrac[dalsi].nick);
					strcpy(hrac[i].score, hrac[dalsi].score);
				}
				strcpy(hrac[a].nick, player);
				sprintf(hrac[a].score, "%d", score);

				a = cislo;
			}

		}
		if (cislo < 10)
			cislo++;
	}
}

void writeData()
{
	FILE* wrt;
	wrt = fopen("Zebricek.txt", "w");

	for (int a = 0; a < cislo; a++)
	{
		fprintf(wrt, "%s,%s*", hrac[a].nick, hrac[a].score);
	}
	fclose(wrt);
}