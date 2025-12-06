#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_BARANG 1000
#define PER_PAGE 20
#define NAMA_FILE "StokBarang.txt"
#define LOG_FILE "log_transaksi.txt"

int i;

typedef struct {
    int id;
    char nama[40];
    int stok;
} Produk;

const char* kategori[] = {
    "Lainnya", "Makanan", "Minuman", "Snack", "Bahan Pokok",
    "Elektronik", "Pakaian", "Alat Tulis", "Obat", "Kosmetik",
    "Alat Rumah", "Buku", "Mainan", "Olahraga", "Kendaraan",
    "Pertanian", "Peternakan", "Perikanan", "Perkakas", "Furniture",
    "Memancing"
};

void pause() {
    printf("\nTekan ENTER untuk melanjutkan...");
    getchar();
}

int kategoriDariID(int id) { 
    return (id / 100) % 100; 
}

// Operasi file dengan array statis
int bacaProduk(Produk arr[]) {
    FILE* f = fopen(NAMA_FILE, "r");
    if (!f) return 0;
    
    int i = 0;
    while (fscanf(f, "%d|%[^|]|%d\n", &arr[i].id, arr[i].nama, &arr[i].stok) == 3) {
        i++;
        if (i >= MAX_BARANG) break;
    }
    fclose(f);
    return i;
}

void simpanProduk(Produk arr[], int n) {
    FILE* f = fopen(NAMA_FILE, "w");
    int i;
    for (i = 0; i < n; i++)
        fprintf(f, "%d|%s|%d\n", arr[i].id, arr[i].nama, arr[i].stok);
    fclose(f);
}

// Fungsi untuk sorting
int bandingID(const void *a, const void *b) {
    Produk *p1 = (Produk *)a;
    Produk *p2 = (Produk *)b;
    return p1->id - p2->id;
}

// Fungsi untuk lowercase
void keLower(char *str) {
    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Fungsi tampil produk dengan perhalaman
void tampilDenganPaging(Produk data[], int n, int sort, const char *judul) {
    if (sort) {qsort(data, n, sizeof(Produk), bandingID);}
    
    int page = 0;
    int totalPage = (n + PER_PAGE - 1) / PER_PAGE;
    
    while (1) {
        clear();
        title_menu(judul);
        printf("Halaman %d/%d (Total: %d data)\n\n", page + 1, totalPage, n);
        
        printf("%-6s | %-13s | %-25s | %s\n", "ID", "Kategori", "Nama Produk", "Stok");
        printf("--------------------------------------------------------------\n");
        
        int start = page * PER_PAGE;
        int end = start + PER_PAGE;
        if (end > n) end = n;
        
        for (i = start; i < end; i++) {
            int k = kategoriDariID(data[i].id);
            if (k < 0 || k > 20) k = 0;
            printf("%05d  | %-13s | %-25s | %d\n", 
                   data[i].id, kategori[k], data[i].nama, data[i].stok);
        }
        
        // Navigasi
        printf("\n[N]ext  [P]rev  [Q]uit");
        if (totalPage > 1) {
            printf("  [G]oto page");
        }
        printf("\nPilihan: ");
        
        char pilih = getchar();
        if (pilih == '\n') continue;
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        switch (toupper(pilih)) {
            case 'N':
                if (page < totalPage - 1) page++;
                break;
                
            case 'P':
                if (page > 0) page--;
                break;
                
            case 'G':
                if (totalPage > 1) {
                    int target;
                    printf("Masukkan nomor halaman (1-%d): ", totalPage);
                    scanf("%d", &target);
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if (target >= 1 && target <= totalPage) {
                        page = target - 1;
                    } else {
                        printf("Halaman tidak valid!\n");
                        pause();
                    }
                }
                break;
                
            case 'Q':
                return;
                
            default:
                printf("Pilihan tidak valid!\n");
                pause();
        }
    }
}

// Modul utama tampil produk
void tampilProduk() {
    Produk data[MAX_BARANG];
    int n = bacaProduk(data);
    if (n == 0) { printf("Tidak ada data Produk!\n"); pause(); return;}
    tampilDenganPaging(data, n, 1, "DAFTAR PRODUK");
}

void tambahProduk() {
    clear();
    title_menu("TAMBAH PRODUK");
    Produk b;
    int kode;
    printf("Kategori Produk:\n");
    // Tampilkan daftar kategori lengkap (0..20)
    int k;
    for (k = 0; k <= 20; k++) {
        printf("%2d: %-13s", k, kategori[k]);
        if (k % 4 == 3 || k == 20) printf("\n");
        else printf("  ");
    }
    printf("\nKategori (1-20, 0=Lainnya): ");
    if (scanf("%d", &kode) != 1) {printf("Input tidak valid!\n");pause();return;} getchar();
    if (kode < 0 || kode > 20) kode = 0;

    printf("Nama Produk: ");
    fgets(b.nama, sizeof(b.nama), stdin);
    b.nama[strcspn(b.nama, "\n")] = 0;
    
    printf("Stok Awal: ");
    if (scanf("%d", &b.stok) != 1) {printf("Input tidak valid!\n");pause();return;}getchar();
    if (b.stok < 0) {printf("Stok tidak boleh negatif!\n");pause();return;}
    
    // Generate ID
    Produk existing[MAX_BARANG];
    int n = bacaProduk(existing);
    int max_seq = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (kategoriDariID(existing[i].id) == kode) {
            int seq = existing[i].id % 100;
            if (seq > max_seq) max_seq = seq;
        }
    }
    
    if (max_seq >= 99) {printf("Kategori ini sudah penuh!\n");pause();return;} 
    b.id = 10000 + (kode * 100) + (max_seq + 1);
    
    // Simpan
    FILE* f = fopen(NAMA_FILE, "a");
    if (!f) {
        printf("Gagal membuka file!\n");
        pause();
        return;
    }
    fprintf(f, "%d|%s|%d\n", b.id, b.nama, b.stok);
    fclose(f);
    
    printf("\nProduk berhasil ditambahkan!\n");
    printf("ID Produk: %05d\n", b.id);
    printf("Kategori: %s\n", kategori[kode]);
    printf("Nama: %s\n", b.nama);
    printf("Stok: %d\n", b.stok);
    pause();
}

void editProduk() {
    int id;
    clear();
    title_menu("EDIT PRODUK");
    
    printf("ID Produk yang akan diedit: ");
    if (scanf("%d", &id) != 1) {printf("Input tidak valid!\n"); pause();return;}
    getchar();
    
    Produk data[MAX_BARANG];
    int n = bacaProduk(data);
    int idx = -1;
    i;
    for (i = 0; i < n; i++) {
        if (data[i].id == id) { 
            idx = i; 
            break; 
        }
    }
    
    if (idx == -1) { printf("ID %05d tidak ditemukan!\n", id); pause(); return; }
    
    printf("\nData lama:\n");
    printf("Nama: %s\n", data[idx].nama);
    printf("Stok: %d\n", data[idx].stok);
    
    printf("\nMasukkan data baru (kosongkan jika tidak ingin mengubah):\n");
    
    printf("Nama baru [%s]: ", data[idx].nama);
    char nama_baru[40];
    fgets(nama_baru, sizeof(nama_baru), stdin);
    nama_baru[strcspn(nama_baru, "\n")] = 0;
    
    if (strlen(nama_baru) > 0) {
        strcpy(data[idx].nama, nama_baru);
    }
    
    printf("Stok baru [%d]: ", data[idx].stok);
    char stok_str[20];
    fgets(stok_str, sizeof(stok_str), stdin);
    stok_str[strcspn(stok_str, "\n")] = 0;
    
    if (strlen(stok_str) > 0) {
        // Validasi angka
        int valid = 1;
        for (i = 0; stok_str[i]; i++) {
            if (!isdigit(stok_str[i]) && stok_str[i] != '-') {
                valid = 0;
                break;
            }
        }
        
        if (valid) {
            int stok_baru = atoi(stok_str);
            if (stok_baru >= 0) {
                data[idx].stok = stok_baru;
            } else { printf("Stok tidak boleh negatif!\n"); pause(); return;}
        } else {printf("Input stok tidak valid!\n"); pause(); return;}
    }
    
    printf("\nKonfirmasi penyimpanan? (y/n): ");
    char konfirm = getchar();
    getchar();
    
    if (konfirm == 'y' || konfirm == 'Y') {
        simpanProduk(data, n);
        printf("Data berhasil diupdate!\n");
    } else {printf("Perubahan dibatalkan!\n");}
    pause();
}

void hapusProduk() {
    int id;
    clear();
    title_menu("HAPUS PRODUK");
    
    printf("ID Produk yang akan dihapus: ");
    if (scanf("%d", &id) != 1) {
        printf("Input tidak valid!\n");
        pause();
        return;}
    getchar();
    
    Produk data[MAX_BARANG];
    int n = bacaProduk(data);
    Produk baru[MAX_BARANG];
    int j = 0;
    int idx = -1;
    int i;
    for (i = 0; i < n; i++) {
        if (data[i].id == id) {
            idx = i;
            continue;
        }
        baru[j++] = data[i];
    }
    
    if (idx == -1) { 
        printf("ID %05d tidak ditemukan!\n", id); 
        pause(); 
        return; 
    }
    
    printf("\nData yang akan dihapus:\n");
    printf("ID: %05d\n", data[idx].id);
    printf("Kategori: %s\n", kategori[kategoriDariID(data[idx].id)]);
    printf("Nama: %s\n", data[idx].nama);
    printf("Stok: %d\n", data[idx].stok);
    
    printf("\nYakin ingin menghapus? (y/n): ");
    char konfirm = getchar();
    getchar();
    
    if (konfirm == 'y' || konfirm == 'Y') {
        simpanProduk(baru, j);
        printf("Data berhasil dihapus!\n");
    } else {printf("Penghapusan dibatalkan!\n");} 
    pause();
}

void transaksi() {
    int pil, id, qty;
    clear();
    title_menu("TRANSAKSI PRODUK");
    
    printf("1. Produk Masuk (Tambah Stok)\n");
    printf("2. Produk Keluar (Kurangi Stok)\n");
    printf("0. Kembali\n");
    printf("Pilihan: ");
    
    if (scanf("%d", &pil) != 1) {printf("Input tidak valid!\n");pause();return;}
    if (pil == 0) return;
    if (pil != 1 && pil != 2) { printf("Pilihan tidak valid!\n"); pause();return;}
    
    printf("ID Produk: ");
    if (scanf("%d", &id) != 1) { printf("Input tidak valid!\n"); pause();return;}
    
    Produk data[MAX_BARANG];
    int n = bacaProduk(data);
    int idx = -1;
    int i;
    for (i = 0; i < n; i++) {
        if (data[i].id == id) { 
            idx = i; 
            break; 
        }
    }
    
    if (idx == -1) {  printf("ID %05d tidak ditemukan!\n", id); pause(); return; }
    
    printf("\nProduk: %s\n", data[idx].nama);
    printf("Stok saat ini: %d\n", data[idx].stok);
    
    printf("Jumlah: ");
    if (scanf("%d", &qty) != 1) { printf("Input tidak valid!\n"); pause(); return;}
    getchar();
    
    if (qty <= 0) { printf("Jumlah harus positif!\n");pause();return;}

    if (pil == 2 && qty > data[idx].stok) { printf("Stok tidak cukup! Stok tersedia: %d\n", data[idx].stok); pause(); return;}
    
    printf("\n=== RINGKASAN TRANSAKSI ===\n");
    printf("Produk: %s\n", data[idx].nama);
    printf("Stok awal: %d\n", data[idx].stok);
    printf("Jumlah: %d\n", qty);
    
    if (pil == 1) {
        printf("Operasi: Penambahan stok\n");
        printf("Stok akhir: %d\n", data[idx].stok + qty);
        data[idx].stok += qty;
    } else {
        printf("Operasi: Pengurangan stok\n");
        printf("Stok akhir: %d\n", data[idx].stok - qty);
        data[idx].stok -= qty;}
    
    printf("\nKonfirmasi transaksi? (y/n): ");
    char konfirm = getchar();
    getchar();
    
    if (konfirm == 'y' || konfirm == 'Y') {
        simpanProduk(data, n);
        // Log 
        FILE* log = fopen(LOG_FILE, "a");
        if (log) {
            time_t now = time(NULL);
            struct tm* t = localtime(&now);
            fprintf(log, "%04d-%02d-%02d %02d:%02d | %s | ID:%05d | %s | %d | %d\n",
                    t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min,
                    (pil == 1) ? "MASUK" : "KELUAR",
                    data[idx].id, data[idx].nama, qty, data[idx].stok);
            fclose(log);
        }
        printf("Transaksi berhasil dicatat!\n");
    } else {printf("Transaksi dibatalkan!\n");}
    pause();
}