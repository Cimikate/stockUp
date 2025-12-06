#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define USER_DB_FILE "user.dat"
#define MAXNAME 32
#define MAXPASS 32

typedef struct {
    char username[MAXNAME];
    char password[MAXPASS];
} User;

static void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len-1] == '\n') s[len-1] = '\0';
}

int load_user(User *u) {
    
    FILE *fp = fopen(USER_DB_FILE, "rb");
    if (!fp) return 0;
    if (fread(u, sizeof(User), 1, fp) != 1) { fclose(fp); return 0;}
    fclose(fp);
    return 1;
}

int save_user(const User *u) {
    
    FILE *fp = fopen(USER_DB_FILE, "wb");
    if (!fp) return 0;
    if (fwrite(u, sizeof(User), 1, fp) != 1) {fclose(fp); return 0;}
    fclose(fp);
    return 1;
}

void register_user() {
    
    User u;
    clear();
    title_menu("REGISTRASI USER");
    printf("Masukan Name: ");
    if (!fgets(u.username, sizeof u.username, stdin)) return;
    trim_newline(u.username);
    if (strlen(u.username) == 0) { printf("Nama tidak bisa kosong.\n"); return; }

    printf("Masukan password: ");
    if (!fgets(u.password, sizeof u.password, stdin)) return;
    trim_newline(u.password);
    if (strlen(u.password) == 0) { printf("Password tidak bisa kosong.\n"); return; }

    if (save_user(&u)) {
        printf("Registrasi berhasil. \n");
    } else {printf("Gagal registrasi. Silahkan cek batasan file.\n");}
}

int login_user() {
    
    User u;
    if (!load_user(&u)) { printf("Tidak ada user teregistrasi, sialhkan registras.\n"); return 0;}
    char uname[MAXNAME], pass[MAXPASS];
    clear();
    title_menu("LOGIN USER");
    printf("Username: ");
    if (!fgets(uname, sizeof uname, stdin)) return 0;
    trim_newline(uname);
    printf("Password: ");
    if (!fgets(pass, sizeof pass, stdin)) return 0;
    trim_newline(pass);

    if (strcmp(uname, u.username) == 0 && strcmp(pass, u.password) == 0) {
        printf("Login Berhasil. Selamat datang, %s!\n", u.username);
        return 1;
    } else { printf("Login Gagal.\n");return 0;}
}

int keamanan() {
    int choice = 0;
    User db;
    int hasUser = load_user(&db);
    while(1) {
        clear();
        if (!hasUser) {
            menu3("Selamat Datang", "Login", "Registrasi", "Exit");
        } else { menu3("Selamat Datang", "Login", "Ubah Data", "Exit");}

        char line[16];
        if (!fgets(line, sizeof line, stdin)) break;
        choice = atoi(line);

            switch (choice) {
                case 1:
                    if (login_user()) {printf("Keluar setelah berhasil login.\n");return 0;}
                    break;
                case 2:
                    register_user();
                    hasUser = load_user(&db); 
                    break;
                case 3:
                    printf("keluar.\n");
                    return 0;
                default:
                    printf("Pilihan tidak ada.\n");
                    break;
                }
    }
    return 0;
}