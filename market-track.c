#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "base_menu.h"

void gotoxy(int x, int y);
void kotak(int x, int y, int w, int h);

void clear(){
    system("cls");
}

void menu(){
    int x=0, y=0, w=40, h=10;
    kotak(x, y, w, h);
    gotoxy(x+10, y+2); printf("MENU UTAMA");
    gotoxy(x+2, y+4); printf("1. Lihat Produk");
    gotoxy(x+2, y+5); printf("2. Tambah Produk");
    gotoxy(x+2, y+6); printf("3. Edit Produk");
    gotoxy(x+2, y+6); printf("4. Hapus Produk");
    gotoxy(x+2, y+6); printf("5. Exit");

    gotoxy(x+2, y+8); printf("Pilih (1-5): ");
}

void fungsi1(){
    clear();
    int i;
    for(i=200; i>=100; i--){
        if(i%2 != 0){
            printf("%d ", i);
        }
    }
}

void fungsi2(){
    clear();
    int i, j, rows, space;
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    for(i = 1; i <= rows; i++) {
        // Print leading spaces
        for(space = 1; space <= rows - i; space++) {
            printf("  ");
        }
        // Print stars
        for(j = 1; j <= (2 * i - 1); j++) {
            printf("* ");
        }
        printf("\n");
    }
}
int main(){
    int choice;
    do {
        system("cls"); // Clear screen
        menu();
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("You selected Option 1\n");
                fungsi1();
                printf("\n");
                system("pause");
                break;
            case 2:
                printf("You selected Option 2\n");
                fungsi2();
                printf("\n");
                system("pause");
                break;
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice, try again.\n");
                system("pause");
        }
    } while(choice != 3);
    return 0;
}