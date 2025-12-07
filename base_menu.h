#include <stdio.h>
#include <windows.h>
#define HORIZONTAL 196
#define VERTICAL 179
#define CORNER_TL 218
#define CORNER_TR 191
#define CORNER_BL 192
#define CORNER_BR 217

void clear() {
  system("cls");

}

void gotoxy(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void kotak(int x, int y, int w, int h){
    int i, j;

    gotoxy(x,y);
    printf("%c", CORNER_TL);
    for(i = 1; i < w; i++) {
    printf("%c",HORIZONTAL);}
    printf("%c", CORNER_TR);

    for(j=1; j<h; j++){
        gotoxy(x, y+j);
        printf("%c", VERTICAL);
        gotoxy(x+w, y+j);
        printf("%c", VERTICAL);
        
    }

    gotoxy(x, y+h-1);
    printf("%c", CORNER_BL);
    for(i = 1; i < w; i++) {
    printf("%c",HORIZONTAL);}
    printf("%c", CORNER_BR);
}

void menu6(char title[15], char menu1[20], char menu2[20], char menu3[20], char menu4[20], char menu5[20], char menu6[10]) {
    int x=0, y=0, w=40, h=13;
    kotak(x, y, w, h);
    gotoxy(x+15, y+2); printf("%s", title);
    gotoxy(x+2, y+4); printf("1. %s", menu1);
    gotoxy(x+2, y+5); printf("2. %s", menu2);
    gotoxy(x+2, y+6); printf("3. %s", menu3);
    gotoxy(x+2, y+7); printf("4. %s", menu4);
    gotoxy(x+2, y+8); printf("5. %s", menu5);
    gotoxy(x+2, y+9); printf("6. %s", menu6);

    gotoxy(x+2, y+11); printf("Pilih (1-5): ");
}

void menu3(char title[15], char menu1[20], char menu2[20], char menu3[20]) {
    int x=0, y=0, w=40, h=10;
    kotak(x, y, w, h);
    gotoxy(x+12, y+2); printf("%s", title);
    gotoxy(x+2, y+4); printf("1. %s", menu1);
    gotoxy(x+2, y+5); printf("2. %s", menu2);
    gotoxy(x+2, y+6); printf("3. %s", menu3);

    gotoxy(x+2, y+8); printf("Pilih (1-3): ");
}

void title_menu(char title[30]) {
    int x=0, y=0, w=60, h=3;
    kotak(x, y, w, h);
    gotoxy(x+20, y+1); printf("%s", title);
    gotoxy(0, h+1);
}