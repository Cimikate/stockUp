#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "base_menu.h"
// #include "login_menu.h"


struct Produk {
    int id;
    int kategori;
    char nama[38];
    int stok;
} item;

void clear(){
    system("cls");
}

int main(){
    int choice;
    
    keamanan();
    do {
        clear();

        menu5("Main Menu", "Tampil Produk", "Tambah Produk", "Edit Produk", "Hapus Produk", "Keluar");
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