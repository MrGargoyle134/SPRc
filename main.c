#include <stdio.h>
#include <stdlib.h>

#ifdef linux
    #error You must compile and run this program on Windows because this program use the WIN32 API.
#endif // linux

#include <windows.h>

#define EXIT(e) system("cls");\
                exit(e);\

typedef unsigned int uint;


char Tiles[8][8] =
{
"........",
"........",
"........",
"........",
"........",
"........",
"........",
"........",
};

uint inputNumbers[8];
uint inputNumbers2[8];
uint inputNumbers3[8];

#define NULLARR {0,0,0,0,0,0,0,0}

uint GlobaluiTile[8] = NULLARR;

uint NumPix3[8];

void SetPosition(int X, int Y)
{
    COORD Position={X, Y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
}

void Read(const char* filename)
{
    FILE* fl;
    fl= fopen(filename, "r");

    if (fl == NULL)
    {
        puts("\aError: can't read file.\n");
        EXIT(EXIT_FAILURE)
    }

    for(int i = 0; i < 8; ++i) fscanf(fl, "%d", &inputNumbers[i]);
    for(int b = 0; b < 8; ++b) fscanf(fl, "%d", &inputNumbers2[b]);
    for(int b = 0; b < 8; ++b) fscanf(fl, "%d", &inputNumbers3[b]);

    fclose(fl);
}

void Save(const char* file1, const char* file2, uint uiArr[], uint uiArr1[], uint uiArr2[])
{
    FILE* f1 = fopen(file1, "w");
    FILE* f2 = fopen(file2, "w");

    if(f1 == NULL || f2 == NULL) {puts("\aERROR: can't write in the file.\n"); EXIT(EXIT_FAILURE)}

    for(int i=0; i < 8; ++i) fprintf(f1, "%d\n", uiArr[i]);
    for(int i=0; i < 8; ++i) fprintf(f1, "%d\n", uiArr1[i]);
    for(int i=0; i < 8; ++i) fprintf(f1, "%d\n", uiArr2[i]);

    for(int a=0; a < 8; ++a)
    {
        if(NumPix3[a]!=0) fprintf(f2, "%c%c", GlobaluiTile[a]-uiArr1[a], uiArr2[a]+uiArr1[a]);
        else fprintf(f2, "%c%c", uiArr[a], uiArr1[a]);
    }

    for(int y=0; y<2040; ++y) fprintf(f2, "%c%c", 0, 0);

    fclose(f1);
    fclose(f2);
}


int main(int argc, char* argv[])
{
    uint Pen = 1;

    if(argc < 3) { printf("\n\aUsage: %s <filename SPRD> <filename EXPORT>\n", argv[0]); EXIT(EXIT_SUCCESS) }


    uint uiTile[8] = NULLARR;
    uint uiTile1[8] = NULLARR;
    uint uiTile2[8] = NULLARR;

    uint X = 0, Y = 0;


    char ScrollX[] = "        ";
    char ScrollY[] = "        ";

    if(!(fopen(argv[1], "r") == NULL))
    {
        Read(argv[1]);
        for(int h=0; h<8; ++h)
        {
            for(int b=0; b < 8; ++b) Tiles[h][b] = (inputNumbers[h] & (1u << ((8-1)-b))) ? '#' : (inputNumbers2[h] & (1u << ((8-1)-b))) ? '%' : (inputNumbers3[h] & (1u << ((8-1)-b))) ? '&' : '.';
        }
    }

    system("cls");

    int Running = 1;

    while(Running)
    {
        printf("[%s] [%s]\n", argv[1], argv[2]);
        ScrollX[X] = 25;
        ScrollY[Y] = 26;

        ScrollX[X-1] = ' ';
        ScrollX[X+1] = ' ';
        ScrollY[Y-1] = ' ';
        ScrollY[Y+1] = ' ';

        if(GetAsyncKeyState(VK_RIGHT)) if(X<7) X++;
        if(GetAsyncKeyState(VK_LEFT)) if(X>0) X--;
        if(GetAsyncKeyState(VK_UP)) if(Y>0) Y--;
        if(GetAsyncKeyState(VK_DOWN)) if(Y<7) Y++;

        if(GetAsyncKeyState('1')) Pen = 1;
        if(GetAsyncKeyState('2')) Pen = 2;
        if(GetAsyncKeyState('3')) Pen = 3;

        if(GetAsyncKeyState('E')) Running = 0;
        if(GetAsyncKeyState('L')) exit(EXIT_SUCCESS);

        if(GetAsyncKeyState('P'))
        {
            if(Pen == 1) Tiles[Y][X] = '#';
            else if(Pen == 2) Tiles[Y][X] = '%';
            else if(Pen == 3) Tiles[Y][X] = '&';
        }
        if(GetAsyncKeyState('R')) Tiles[Y][X] = '.';
        putchar(' ');
        puts(ScrollX);
        for(int m=0; m<8; ++m)
        {
            putchar(ScrollY[m]);
            for(int n=0; n<8; ++n)
            {
                putchar(Tiles[m][n]);
            }
            printf(" %d\n", m);
        }
        puts("\n 01234567\n\n");
        printf("Pen: %c\n", (Pen == 1) ? '#' : (Pen == 2) ? '%' : '&');
        printf("X:%d/7, Y:%d/7\n\n\n", X, Y);
        puts("(P)lace.\n(R)emove.\n\n(E)nd.\n(L)eave.\n");

        Sleep(100);
        SetPosition(0,0);

    }
    system("cls");

    uint SSD[8];

    puts("Sprite data (SPRD):\n");
    for(int i=0; i < 8; ++i)
    {
        for(int a=0; a < 8; ++a)
        {
            uiTile[i] = (Tiles[i][a] == '#') ? (uiTile[i] | (128u >> a)) : uiTile[i];
            uiTile1[i] = (Tiles[i][a] == '%') ? (uiTile1[i] | (128u >> a)) : uiTile1[i];
            uiTile2[i] = (Tiles[i][a] == '&') ? (uiTile2[i] | (128u >> a)) : uiTile2[i];
            GlobaluiTile[i] = (Tiles[i][a] != '.') ? GlobaluiTile[i] | (128u >> a) : GlobaluiTile[i];
        }
    }
    puts("PEN #: \n");
    for(int i=0; i<8; ++i)printf("%d\n", uiTile[i]);
    putchar('\n');
    puts("PEN %: \n");
    for(int i=0; i<8; ++i)printf("%d\n", uiTile1[i]);
    putchar('\n');
    puts("PEN &: \n");
    for(int i=0; i<8; ++i)printf("%d\n", uiTile2[i]);
    putchar('\n');
    puts("ALL PENS: \n");
    for(int i=0; i<8; ++i)printf("%d\n", GlobaluiTile[i]);

    for(int u=0; u < 8; ++u)
    {
        for(int y=0; y<8; ++y) NumPix3[u] = (uiTile2[u]&(1u<<y)) ? NumPix3[u]+1 : NumPix3;
    }

    Save(argv[1], argv[2], uiTile, uiTile1, uiTile2);

    return 0;
}
