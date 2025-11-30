#include <stdio.h>
#include <windows.h>

void gotoxy(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void kotak(int x, int y, int w, int h){
    int i, j;
    // w adalah lebar
    // h adalah tinggi

    //garis atas
    gotoxy(x,y);
    printf("+");
    for(i = 1; i < w; i++) {
    printf("-");}
    printf("+"); // garis horizontal

    for(j=1; j<h; j++){
        gotoxy(x, y+j);
        printf("|");
        gotoxy(x+w, y+j);
        printf("|");
    }

    gotoxy(x, y+h-1);
    printf("+");
    for(i = 1; i < w; i++) {
    printf("-");}
    printf("+");
}