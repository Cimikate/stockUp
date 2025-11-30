#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "base_menu.h"

struct Produk {
    char nama[50];
    char kategori[30];
    char deskripsi[100];
    int stok;
    float harga;
} item;

void clear(){
    system("cls");
}

int main(){
    
    int choice;
    do {
        clear();

        menu();
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                exit(0); 
                break;
            case 2:  
                exit(0);
                break;
            case 3:
                exit(0);
                break;
            case 4:
                exit(0);
                break;
            case 5:
                exit(0);
                break;
            default:
                printf("Invalid choice, try again.\n");
                system("pause");
        }
    } while(choice != 3);
    return 0;
}