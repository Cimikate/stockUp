#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "base_menu.h"

int main(){
    int choice;
    
    keamanan();
    do {
        clear();

        menu6("Main Menu", "Tampil Produk", "Tambah Produk", "Transaksi Produk", "Edit Produk", "Hapus Produk", "Keluar");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                tampilProduk(); 
                break;
            case 2:  
                tambahProduk();
                break;
            case 3:
                produkTransaksi();
                break;
            case 4:
                editProduk();;
                break;
            case 5:
                hapusProduk();
                break;
            case 6:
                clear();
                exit(0);
                break;
            default:
                printf("Pilihan Tidak Tersedia.\n");
                system("pause");
        }
    } while(choice != 6);
    return 0;
}