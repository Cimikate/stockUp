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

void menu(){
    int x=0, y=0, w=40, h=12;
    kotak(x, y, w, h);
    gotoxy(x+15, y+2); printf("MENU UTAMA");
    gotoxy(x+2, y+4); printf("1. Lihat Produk");
    gotoxy(x+2, y+5); printf("2. Tambah Produk");
    gotoxy(x+2, y+6); printf("3. Edit Produk");
    gotoxy(x+2, y+7); printf("4. Hapus Produk");
    gotoxy(x+2, y+8); printf("5. Exit");

    gotoxy(x+2, y+10); printf("Pilih (1-5): ");
}