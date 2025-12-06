#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    int nomor;
    char nama_barang[40];
    int jumlah_barang;
} produk;

const char* get_nama_kategori(int kategori_kode) {
    switch(kategori_kode) {
        case 1: return "Makanan";
        case 2: return "Minuman";
        case 3: return "Snack";
        case 4: return "Bahan Pokok";
        case 5: return "Elektronik";
        case 6: return "Pakaian";
        case 7: return "Alat Tulis";
        case 8: return "Obat";
        case 9: return "Kosmetik";
        case 10: return "Alat Rumah";
        case 11: return "Buku";
        case 12: return "Mainan";
        case 13: return "Olahraga";
        case 14: return "Kendaraan";
        case 15: return "Pertanian";
        case 16: return "Peternakan";
        case 17: return "Perikanan";
        case 18: return "Perkakas";
        case 19: return "Furniture";
        case 20: return "Memancing";
        default: return "Lainnya";
    }
}

// Fungsi untuk mendapatkan kode kategori dari ID
int get_kategori_from_id(int id) {
    // ID format: 1ccnn
    return (id / 100) % 100;
}

/* Fungsi yang dipakai */
void cariProduk();
void produkKeluar();
void cariProdul();
void editProduk();
void hapusProduk();
int cmp_produk(const void *a, const void *b); //untuk sorting datanya

void pause() {
    printf("\nTekan ENTER untuk melanjutkan");
    getchar();
}

void tambahProduk() {
    int kategori;
    produk d;
    
    clear();
    title_menu("TAMBAH PRODUK BARU");
    
    // Input kategori
    printf("Kategori (0-99): ");
    if (scanf("%d", &kategori) != 1) {
        printf("Input tidak valid!\n");
        while (getchar() != '\n');  // Clear buffer
        pause();
        return;
    }
    while (getchar() != '\n');  // Clear buffer
    
    if (kategori < 0 || kategori > 99) {
        printf("Kategori harus 0-99.\n");
        pause();
        return;
    }
    
    // Input nama
    printf("Nama Barang: ");
    fgets(d.nama_barang, sizeof(d.nama_barang), stdin);
    d.nama_barang[strcspn(d.nama_barang, "\n")] = 0;
    
    // Input jumlah
    printf("Jumlah Stok: ");
    if (scanf("%d", &d.jumlah_barang) != 1) {
        printf("Input tidak valid!\n");
        while (getchar() != '\n');
        pause();
        return;
    }
    while (getchar() != '\n');
    
    if (d.jumlah_barang < 0) {
        printf("Jumlah tidak boleh negatif.\n");
        pause();
        return;
    }
    
    // Generate ID
    FILE *fp = fopen("StokBarang.txt", "r");
    int max_seq = 0;
    
    if (fp) {
        char line[100];
        int id;
        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "%d", &id) == 1) {
                int cat = (id / 100) % 100;
                int seq = id % 100;
                if (cat == kategori && seq > max_seq) {
                    max_seq = seq;
                }
            }
        }
        fclose(fp);
    }
    
    if (max_seq >= 99) {
        printf("Kategori %d sudah penuh!\n", kategori);
        pause();
        return;
    }
    
    d.nomor = 10000 + (kategori * 100) + (max_seq + 1);
    
    // Simpan ke file
    fp = fopen("StokBarang.txt", "a");
    if (!fp) {
        printf("Gagal membuka file!\n");
        pause();
        return;
    }
    
    fprintf(fp, "%d|%s|%d\n", d.nomor, d.nama_barang, d.jumlah_barang);
    fclose(fp);
    
    printf("\nProduk berhasil ditambahkan!\n");
    printf("ID: %05d\n", d.nomor);
    pause();
}

void tampilProduk() {
    FILE *pf = fopen("StokBarang.txt", "r");
    if (!pf) { printf("Belum ada data!\n"); pause(); return; }

    produk *arr = NULL;
    int cap = 0, cnt = 0;
    char line[200];

    while (fgets(line, sizeof(line), pf)) {
        produk tmp;
        if (sscanf(line, "%d|%[^|]|%d", &tmp.nomor, tmp.nama_barang, &tmp.jumlah_barang) >= 2) {
            if (cnt >= cap) {
                cap = (cap == 0) ? 64 : cap * 2;
                produk *n = (produk *)realloc(arr, sizeof(produk) * cap);
                if (!n) { free(arr); fclose(pf); printf("Memori tidak cukup\n"); pause(); return; }
                arr = n;
            }
            arr[cnt++] = tmp;
        }
    }
    fclose(pf);

    if (cnt == 0) { printf("Belum ada data!\n"); pause(); free(arr); return; }

    qsort(arr, cnt, sizeof(produk), cmp_produk);

    int perpage = 20;
    int page = 0;
    int pages = (cnt + perpage - 1) / perpage;
    int i;
    
    while (1) {
        clear();
        title_menu("DATA STOK BARANG");
        
        printf("Halaman %d/%d (Total: %d data)\n", page + 1, pages, cnt);
        printf("================================================================================\n");
        printf("   ID    |   KATEGORI   |          NAMA BARANG          |  STOK  \n");
        printf("================================================================================\n");

        int start = page * perpage;
        int end = start + perpage;
        if (end > cnt) end = cnt;
        
        for (i = start; i < end; i++) {
            int kategori_kode = get_kategori_from_id(arr[i].nomor);
            const char* nama_kategori = get_nama_kategori(kategori_kode);
            
            printf(" %05d   | %-12s | %-30s | %6d \n", 
                   arr[i].nomor, 
                   nama_kategori, 
                   arr[i].nama_barang, 
                   arr[i].jumlah_barang);
        }
        
        printf("================================================================================\n");
        
        printf("\nNavigasi: (N)ext  (P)rev  (S)earch  (Q)uit\n");
        printf("Pilihan: ");
        
        int ch = getchar(); 
        while (ch == '\n') ch = getchar();
        
        if (ch == 'S' || ch == 's') {
            int c; 
            while ((c = getchar()) != '\n' && c != EOF);
            cariProduk();
            continue;}  
        if (ch == 'N' || ch == 'n') { 
            if (page < pages - 1) page++;  }
        else if (ch == 'P' || ch == 'p') { 
            if (page > 0) page--; }
        else if (ch == 'Q' || ch == 'q') { 
            int c; 
            while ((c = getchar()) != '\n' && c != EOF);
            break; }
        int c; 
        while ((c = getchar()) != '\n' && c != EOF);
    }

    free(arr);
    pause();
}

int cmp_produk(const void *a, const void *b) {
    const produk *pa = (const produk *)a;
    const produk *pb = (const produk *)b;
    if (pa->nomor < pb->nomor) return -1;
    if (pa->nomor > pb->nomor) return 1;
    return 0;
}

void cariProduk() {
    FILE *pf = fopen("StokBarang.txt", "r");
    if (!pf) { printf("File tidak ditemukan!\n"); pause(); return; }

    char input[80];
    clear(); 
    title_menu("CARI PRODUK");
    

    printf("Masukkan ID atau nama: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    /* trim leading/trailing whitespace */
    size_t start = 0;
    while (start < strlen(input) && isspace((unsigned char)input[start])) start++;
    if (start > 0) memmove(input, input + start, strlen(input + start) + 1);
    while (strlen(input) > 0 && isspace((unsigned char)input[strlen(input)-1])) input[strlen(input)-1] = '\0';
    if (strlen(input) == 0) { printf("Input kosong. Batalkan pencarian.\n"); fclose(pf); pause(); return; }

    int is_number = 1;
    size_t i;
    for ( i = 0; i < strlen(input); i++) { if (!isdigit((unsigned char)input[i])) { is_number = 0; break; } }

    int found = 0;
    char line[200];
    produk d;

    clear(); 
    title_menu("HASIL PENCARIAN");
    
    if (is_number && strlen(input) > 0) {
        int id = atoi(input);
        while (fgets(line, sizeof(line), pf)) {
            if (sscanf(line, "%d|%[^|]|%d", &d.nomor, d.nama_barang, &d.jumlah_barang) >= 2) {
                int kategori_kode = get_kategori_from_id(d.nomor);
                const char* nama_kategori = get_nama_kategori(kategori_kode);
                if (d.nomor == id) {
                    printf("ID Produk   : %d\n", d.nomor);
                    printf("Kategori    : %s\n", nama_kategori);
                    printf("Nama Barang : %s\n", d.nama_barang);
                    printf("Jumlah Stok : %d\n", d.jumlah_barang);
                    found = 1; break;
                }
            }
        }
    } else {
        /* case-insensitive substring search */
        while (fgets(line, sizeof(line), pf)) {
            if (sscanf(line, "%d|%[^|]|%d", &d.nomor, d.nama_barang, &d.jumlah_barang) >= 2) {
                char nama_lower[64]; char input_lower[80];
                size_t i;
                for (i = 0; i < strlen(d.nama_barang) && i < sizeof(nama_lower)-1; i++) nama_lower[i] = tolower((unsigned char)d.nama_barang[i]);
                nama_lower[i] = '\0';
                for (i = 0; i < strlen(input) && i < sizeof(input_lower)-1; i++) input_lower[i] = tolower((unsigned char)input[i]);
                input_lower[i] = '\0';
                int kategori_kode = get_kategori_from_id(d.nomor);
                const char* nama_kategori = get_nama_kategori(kategori_kode);
                if (strstr(nama_lower, input_lower) != NULL) {
                    printf("ID Produk   : %d\n", d.nomor);
                    printf("Kategori    : %s\n", nama_kategori);
                    printf("Nama Barang : %s\n", d.nama_barang);
                    printf("Jumlah Stok : %d\n\n", d.jumlah_barang);
                    found = 1;
                }
            }
        }
    }

    if (!found) printf("Produk tidak ditemukan.\n");
    fclose(pf); pause();
}

void editProduk() {
    int id;
    char konfirmasi;
    produk d;
    int found = 0;
    char line[200];
    char nama_baru[40];
    int jumlah_baru;

    clear();
    title_menu("EDIT PRODUK");

    printf("Masukkan ID produk yang akan diedit: ");
    if (scanf("%d", &id) != 1) { 
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Input tidak valid\n"); 
        pause(); 
        return; 
    }
 
    while (getchar() != '\n');

    // Buka file untuk membaca
    FILE *pf = fopen("StokBarang.txt", "r");
    if (!pf) { 
        printf("File tidak ditemukan!\n"); 
        pause(); 
        return; 
    }

    // Cari produk berdasarkan ID
    found = 0;
    while (fgets(line, sizeof(line), pf)) {
        if (sscanf(line, "%d|%[^|]|%d", &d.nomor, d.nama_barang, &d.jumlah_barang) >= 2) {
            if (d.nomor == id) {
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        fclose(pf); 
        printf("Produk dengan ID %05d tidak ditemukan.\n", id);
        pause();
        return;
    }

    // Tampilkan data lama
    printf("\n=== DATA PRODUK LAMA ===\n");
    printf("ID Produk      : %05d\n", d.nomor);
    printf("Nama Barang    : %s\n", d.nama_barang);
    printf("Jumlah Stok    : %d\n", d.jumlah_barang);
    printf("=========================\n");

    // Input data baru
    printf("\n=== MASUKKAN DATA BARU ===\n");
    printf("(Kosongkan untuk tetap menggunakan data lama)\n\n");
    
    // Input nama baru
    printf("Nama Barang baru [%s]: ", d.nama_barang);
    fgets(nama_baru, sizeof(nama_baru), stdin);
    nama_baru[strcspn(nama_baru, "\n")] = 0;
    
    // Jika input kosong, gunakan nama lama
    if (strlen(nama_baru) == 0) {
        strcpy(nama_baru, d.nama_barang);
    }
    
    // Input jumlah baru
    char jumlah_str[20];
    printf("Jumlah Barang baru [%d]: ", d.jumlah_barang);
    fgets(jumlah_str, sizeof(jumlah_str), stdin);
    jumlah_str[strcspn(jumlah_str, "\n")] = 0;
    
    // Jika input kosong, gunakan jumlah lama
    if (strlen(jumlah_str) == 0) {
        jumlah_baru = d.jumlah_barang;
    } else {
        // Validasi input angka
        int valid = 1;
        int i;
        for (i = 0; i < strlen(jumlah_str); i++) {
            if (!isdigit((unsigned char)jumlah_str[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            fclose(pf);
            printf("Input jumlah tidak valid. Harus angka.\n");
            pause();
            return;
        }
        
        jumlah_baru = atoi(jumlah_str);
        if (jumlah_baru < 0) {
            fclose(pf);
            printf("Jumlah barang tidak boleh negatif.\n");
            pause();
            return;
        }
    }
    
    // Tampilkan data yang akan disimpan
    printf("\n=== DATA YANG AKAN DISIMPAN ===\n");
    printf("ID Produk      : %05d (tidak dapat diubah)\n", d.nomor);
    printf("Nama Barang    : %s\n", nama_baru);
    printf("Jumlah Stok    : %d\n", jumlah_baru);
    printf("===============================\n");
    
    // Konfirmasi
    printf("\nApakah Anda yakin ingin menyimpan perubahan? (Y/N): ");
    scanf(" %c", &konfirmasi);

    while (getchar() != '\n');
    
    if (konfirmasi != 'Y' && konfirmasi != 'y') {
        fclose(pf);
        printf("Perubahan dibatalkan.\n");
        pause();
        return;
    }

    // Buat file temporary
    rewind(pf); 
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(pf);
        printf("Gagal membuat file sementara\n");
        pause();
        return;
    }

    int diedit = 0;
    rewind(pf);
    while (fgets(line, sizeof(line), pf)) {
        int current_id;
        char current_nama[40];
        int current_jumlah;
        
        // ambil baris untuk id
        if (sscanf(line, "%d|%[^|]|%d", &current_id, current_nama, &current_jumlah) >= 2) {
            if (current_id == id && !diedit) {
                // Tulis data yang sudah diedit
                fprintf(temp, "%d|%s|%d\n", d.nomor, nama_baru, jumlah_baru);
                diedit = 1;
            } else {
                // Tulis data lain tanpa perubahan
                fputs(line, temp);
            }
        } else {
            // Jika format tidak sesuai, tetap tulis baris aslinya
            fputs(line, temp);
        }
    }

    fclose(pf);
    fclose(temp);

    // Ganti file lama 
    if (remove("StokBarang.txt") != 0) {
        printf("Gagal menghapus file lama\n");
        pause();
        return;
    }
    
    if (rename("temp.txt", "StokBarang.txt") != 0) {
        printf("Gagal mengganti file\n");
        pause();
        return;
    }

    printf("\nProduk dengan ID %05d berhasil diedit!\n", id);
    pause();
}

void hapusProduk() {
    FILE *pf = fopen("StokBarang.txt", "r");
    if (!pf) { 
        printf("File tidak ditemukan!\n"); 
        pause(); 
        return; 
    }

    int id;
    char konfirmasi;
    produk d;
    int found = 0;
    char line[200];

    clear();
    title_menu("HAPUS PRODUK");

    printf("Masukkan ID produk yang akan dihapus: ");
    if (scanf("%d", &id) != 1) { 
        getchar(); 
        printf("Input tidak valid\n"); 
        fclose(pf);
        pause(); 
        return; 
    }
    getchar();

    // Cari produk berdasarkan ID 
    rewind(pf);
    while (fgets(line, sizeof(line), pf)) {
        if (sscanf(line, "%d|%[^|]|%d", &d.nomor, d.nama_barang, &d.jumlah_barang) >= 2) {
            if (d.nomor == id) {
                found = 1;
                printf("\n=== DATA PRODUK YANG AKAN DIHAPUS ===\n");
                printf("ID Produk      : %05d\n", d.nomor);
                printf("Nama Barang    : %s\n", d.nama_barang);
                printf("Jumlah Stok    : %d\n", d.jumlah_barang);
                printf("=====================================\n");

                printf("\nApakah Anda yakin ingin menghapus produk ini? (Y/N): ");
                scanf(" %c", &konfirmasi);
                getchar();
                
                if (konfirmasi != 'Y' && konfirmasi != 'y') {
                    printf("Penghapusan dibatalkan.\n");
                    fclose(pf);
                    pause();
                    return;
                }
                break;
            }
        }
    }

    if (!found) {
        printf("Produk dengan ID %05d tidak ditemukan.\n", id);
        fclose(pf);
        pause();
        return;
    }

    // Buat file sementara 
    rewind(pf);
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Gagal membuat file sementara\n");
        fclose(pf);
        pause();
        return;
    }

    int dihapus = 0;
    while (fgets(line, sizeof(line), pf)) {
        int current_id;
        sscanf(line, "%d", &current_id);
        
        if (current_id == id && !dihapus) {
            dihapus = 1;  
            continue;
        }
        fputs(line, temp);
    }

    fclose(pf);
    fclose(temp);

    if (remove("StokBarang.txt") != 0) {
        printf("Gagal menghapus file lama\n");
        pause();
        return;
    }
    
    if (rename("temp.txt", "StokBarang.txt") != 0) {
        printf("Gagal mengganti file\n");
        pause();
        return;
    }

    printf("\nProduk dengan ID %05d berhasil dihapus!\n", id);
    pause();
}


void produkTransaksi() {
    int pilihan_transaksi;
    int id, jumlah;
    
    clear();
    title_menu("TRANSAKSI PRODUK");
    
    printf(" Pilih jenis transaksi:\n");
    printf(" 1. Produk Masuk (Tambah Stok)\n");
    printf(" 2. Produk Keluar (Kurangi Stok)\n");
    printf(" 0. Batal\n");
    printf(" \nPilihan: ");
    
    if (scanf("%d", &pilihan_transaksi) != 1) {
        printf("Input tidak valid!\n");
        pause();
        return;
    }
    
    if (pilihan_transaksi == 0) {
        printf("Transaksi dibatalkan.\n");
        pause();
        return;
    }
    
    if (pilihan_transaksi != 1 && pilihan_transaksi != 2) {
        printf("Pilihan tidak valid!\n");
        pause();
        return;
    }
 
    clear();
    if (pilihan_transaksi == 1) {
        title_menu("PRODUK MASUK");
    } else {
        title_menu("PRODUK KELUAR");
    }
    
    printf("Masukkan ID produk: ");
    if (scanf("%d", &id) != 1) {
        printf("Input tidak valid!\n");
        pause();
        return;
    }
    
    FILE *pf = fopen("StokBarang.txt", "r");
    if (!pf) {
        printf("File tidak ditemukan!\n");
        pause();
        return;
    }

    produk d;
    int found = 0;
    char line[200];

    rewind(pf);
    while (fgets(line, sizeof(line), pf)) {
        if (sscanf(line, "%d|%[^|]|%d", &d.nomor, d.nama_barang, &d.jumlah_barang) >= 2) {
            if (d.nomor == id) {
                found = 1;
                break;
            }
        }
    }
    
    if (!found) {
        fclose(pf);
        printf("Produk dengan ID %d tidak ditemukan!\n", id);
        pause();
        return;
    }
  
    printf("\n--- DATA PRODUK ---\n");
    printf("ID Produk    : %d\n", d.nomor);
    printf("Nama Barang  : %s\n", d.nama_barang);
    printf("Stok Saat Ini: %d\n", d.jumlah_barang);
    
    if (pilihan_transaksi == 1) {
        printf("\nJumlah yang akan ditambahkan: ");
    } else {
        printf("\nJumlah yang akan dikeluarkan: ");
    }
    
    if (scanf("%d", &jumlah) != 1) {
       
        fclose(pf);
        printf("Input tidak valid!\n");
        pause();
        return;
    }
 
    // Validasi jumlah
    if (jumlah <= 0) {
        fclose(pf);
        printf("Jumlah harus lebih dari 0!\n");
        pause();
        return;
    }
    
    if (pilihan_transaksi == 2 && jumlah > d.jumlah_barang) {
        fclose(pf);
        printf("Stok tidak cukup!\n");
        printf("Stok tersedia: %d\n", d.jumlah_barang);
        printf("Jumlah diminta: %d\n", jumlah);
        pause();
        return;
    }
    

    int stok_awal = d.jumlah_barang;
    int stok_baru;
    char *jenis_transaksi;
    
    if (pilihan_transaksi == 1) {
        stok_baru = stok_awal + jumlah;
        jenis_transaksi = "MASUK";
    } else {
        stok_baru = stok_awal - jumlah;
        jenis_transaksi = "KELUAR";
    }
    
    clear();
    printf("========================================\n");
    printf("         RINGKASAN TRANSAKSI\n");
    printf("========================================\n");
    printf("Jenis Transaksi : %s\n", jenis_transaksi);
    printf("ID Produk       : %d\n", d.nomor);
    printf("Nama Barang     : %s\n", d.nama_barang);
    printf("Stok Awal       : %d\n", stok_awal);
    printf("Jumlah          : %d\n", jumlah);
    printf("Stok Baru       : %d\n", stok_baru);
    printf("========================================\n");

    printf("\nKonfirmasi transaksi? (Y/N): ");
    char konfirmasi;
    scanf(" %c", &konfirmasi);

    
    if (konfirmasi != 'Y' && konfirmasi != 'y') {
        fclose(pf);
        printf("Transaksi dibatalkan.\n");
        pause();
        return;
    }
    
    // Update file
    rewind(pf);
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(pf);
        printf("Gagal membuat file sementara!\n");
        pause();
        return;
    }
    
    int diupdate = 0;
    rewind(pf);
    while (fgets(line, sizeof(line), pf)) {
        int current_id;
        sscanf(line, "%d", &current_id);
        
        if (current_id == id && !diupdate) {
            fprintf(temp, "%d|%s|%d\n", d.nomor, d.nama_barang, stok_baru);
            diupdate = 1;
        } else {
            fputs(line, temp);
        }
    }
    
    fclose(pf);
    fclose(temp);
    
    remove("StokBarang.txt");
    rename("temp.txt", "StokBarang.txt");
    
    printf("\n--- TOTAL TRANSAKSI HARI INI ---\n");
    printf("Transaksi %s: %d unit\n", jenis_transaksi, jumlah);
    
    // Log
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    FILE *log_file = fopen("log_transaksi.txt", "a");
    if (log_file) {
        fprintf(log_file, "%04d-%02d-%02d %02d:%02d:%02d | %s | ID:%d | %s | Jumlah:%d | Stok_Awal:%d | Stok_Akhir:%d\n",
                local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
                local->tm_hour, local->tm_min, local->tm_sec,
                jenis_transaksi, d.nomor, d.nama_barang, jumlah, stok_awal, stok_baru);
        fclose(log_file);
    }
    
    printf("\nTransaksi berhasil dicatat!\n");
    
    printf("\n--- STATUS ---\n");
    printf("Stok %s sekarang: %d unit\n", d.nama_barang, stok_baru);
    
    pause();
}