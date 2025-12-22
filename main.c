#include "lib.h"
// --- DEFINISI STRUKTUR DATA (MASTER DATA) ---

typedef char string[50];

typedef struct
{
    int id;                // Identitas soal
    int idMatkul;          // Identitas mata kuliah
    int tipe;              // 1 = Pilihan Ganda, 2 = Essay
    char soal[200];        // Kalimat pertanyaan
    char opsiA[50];        // Pilihan A
    char opsiB[50];        // Pilihan B
    char opsiC[50];        // Pilihan C
    char opsiD[50];        // Pilihan D
    char jawabanBenar[50]; // Kunci jawaban (misal: "A" atau kata kunci essay)
    int poin;              // Nilai jika benar
} DataQuiz;

typedef struct
{
    char username[50];
    char password[50];
    char email[50];
    char namaLengkap[50];
    int totalPoin; // Untuk Leaderboard
    int role;      // 1 = Siswa, 2 = Pengajar , 3 = Pengawas
} User;
User currentUser;
struct Ruangan
{
    char kode[20];
    char namaRuangan[50];
    int kapasitas;
    char lokasi[30];
    char createdAt[30];
};

typedef struct
{
    int id;
    char nama[50];
    char spesialisasi[50];
    float rating;
    char status[50];
} Pengajar;

typedef struct
{
    int id;
    char namaMatkul[50];
    char hari[20];
} MataKuliah;

// --- TAMBAHAN STRUKTUR UNTUK SHOP ---
typedef struct
{
    int id;
    char namaItem[50];
    int hargaPoin;
    int stok;
} ItemShop;

// --- DATABASE SEMENTARA (GLOBAL ARRAY) ---
// Kita siapkan slot untuk 100 user & 5 matkul
User dataUser[100];
int jumlahUser = 0;        // Counter user yang terdaftar
int currentUserIndex = -1; // karena array di mulai dari 0 menandakan kalau belum ada user yang login

// --- DATA DUMMY BARANG ---

Pengajar daftarPengajar[5] = {
    {101, "Prof. Budi Santoso", "Algoritma & C++", 4.9, "Available"},
    {102, "Dr. Siti Aminah", "Web Dev (Laravel)", 4.8, "Busy"},
    {103, "Pak Anton Wijaya", "IoT & Embedded", 4.7, "Available"},
    {104, "Kak Dinda Pertiwi", "Frontend (React)", 4.5, "Available"},
    {105, "Pak Rudi Hermawan", "Dasar Programming", 4.2, "In Class"}};

// Variable untuk sesi login saat ini

// --- PROTOTYPE FUNGSI ---

// User Management
void viewUser();
void adminAddUser();
void deleteUser();
void updateUser();
void menuKelolaUser();
void saveUserToDB(User);

// Logout and Login
void About();
void exitProgram();
void Register();
int loginProcess();
void clearSession();

// Fitur Utama
void leaderboard();
void absen();
void reservasi();
void jalankanQuiz();
void pointShop();
void reservasi();
void pelatih();
void tambahPoinUser(char usernameTarget[], int poinTambahan);

// Dashboard functions
void dashboardSiswa();
void dashboardPengajar();
void dashboardPengawas();

// Quiz functions
void menuKelolaQuiz();
void tambahQuiz();
void lihatSemuaQuiz();
void hapusQuiz();
void editQuiz();

// Shop functions
void menuKelolaShop();
void lihatSemuaItem();
void tambahItemShop();
void hapusItemShop();

// Ruangan functions
void Ruangan();


// Clear session
void clearSession()
{
    // 1. Wipe data user dari memori (keamanan)
    memset(&dataUser[0], 0, sizeof(User));

    // 2. Reset penunjuk sesi
    currentUserIndex = -1;

    printf("Session cleared safely.\n");
}


// Point Shop
void checkShopDatabase() {
    FILE *fp = fopen("database/shop.dat", "rb");
    if (fp == NULL) {
        // File belum ada, kita buatkan data awal (Default)
        fp = fopen("database/shop.dat", "wb");
        
        ItemShop defaultItems[3] = {
            {1, "Voucher Kantin 10k", 50, 20},
            {2, "Sticker Pack C++", 25, 50},
            {3, "Kaos Programmer", 300, 5}
        };
        
        fwrite(defaultItems, sizeof(ItemShop), 3, fp);
        printf("Database Shop berhasil diinisialisasi!\n");
        fclose(fp);
    } else {
        fclose(fp);
    }
}
// --- DASHBOARD SESUAI ROLE ---

void dashboardSiswa()
{
    int pilihan;
    do
    {
        system("cls");
        // Header dengan Nama User
        printf("=========================================\n");
        printf(" DASHBOARD SISWA: %s \n", dataUser[currentUserIndex].namaLengkap);
        printf(" Poin: %d \n", dataUser[currentUserIndex].totalPoin);
        printf("=========================================\n");

        // Menu Khusus Siswa
        char menuSiswa[][60] = {
            "Absensi Kelas",
            "Reservasi Pengajar",
            "Download Modul",
            "Point Shop",
            "Kerjakan Quiz",
            "Logout"};

        // Menggunakan drawMenu dari lib.h agar rapi
        pilihan = drawMenu(5, 6, menuSiswa, 6);

        switch (pilihan)
        {
        case 0:
            absen();
            break;
        case 1:
            reservasi();
            break; // Nanti diupdate jadi Reservasi + Kritik Saran
        case 2:
            msgBox("INFO", "Fitur Download Modul (Coming Soon)", BLUE);
            break;
        case 3:
            pointShop();
            break;
        case 4:
            jalankanQuiz();
            break;
        case 5:
            clearSession();
            return; // Logout
        }
    } while (1);
}

void dashboardPengajar()
{
    int pilihan;
    do
    {
        system("cls");
        printf("=========================================\n");
        printf(" DASHBOARD PENGAJAR: %s \n", dataUser[currentUserIndex].namaLengkap);
        printf(" Saldo Poin: %d \n", dataUser[currentUserIndex].totalPoin);
        printf("=========================================\n");

        char menuPengajar[][60] = {
            "Jadwal Mengajar",
            "Kelola Quiz",
            "Reservasi Ruangan",
            "Point Shop",
            "Logout"};

        pilihan = drawMenu(5, 6, menuPengajar, 5);

        switch (pilihan)
        {
        case 0:
            msgBox("INFO", "Fitur Jadwal (Coming Soon)", BLUE);
            break;
        case 1:
            menuKelolaQuiz();
            break;
        case 2:
            msgBox("INFO", "Fitur Booking Ruangan (Coming Soon)", BLUE);
            break;
        case 3:
            pointShop();
            break; // Pengajar juga bisa belanja
        case 4:
            clearSession();
            return;
        }
    } while (1);
}

void dashboardPengawas()
{
    int pilihan;
    do
    {
        system("cls");
        drawBoxWithShadow(2, 2, 50, 3, "ADMIN DASHBOARD");
        gotoxy(4, 4);
        printf("User: %s (Super Admin)", dataUser[currentUserIndex].username);

        char menuAdmin[][60] = {
            "CRUD Akun User",
            "CRUD Mata Kuliah",
            "CRUD Ruangan",
            "CRUD Modul",
            "CRUD Point Shop",
            "Logout"};

        pilihan = drawMenu(5, 8, menuAdmin, 6);

        switch (pilihan)
        {
        case 0:
            menuKelolaUser();
            break;
        case 1:
            msgBox("INFO", "Menu Kelola Matkul (Next Step)", RED);
            break;
        case 2:
            Ruangan();
            break;
        case 3:
            msgBox("INFO", "Menu Kelola Modul (Next Step)", RED);
            break;
        case 4:
            menuKelolaShop();
            break;
        case 5:
            clearSession();
            return;
        }
    } while (1);
}


//||===================================||
//||            CRUD SHOP              ||
//||===================================||

void lihatSemuaItem() {
    FILE *fp = fopen("database/shop.dat", "rb");
    ItemShop item;
    
    system("cls");
    printf("=== DAFTAR BARANG TOKO ===\n");
    printf("%-3s | %-25s | %-6s | %-5s\n", "ID", "Nama Item", "Harga", "Stok");
    printf("--------------------------------------------------\n");
    
    if (fp != NULL) {
        while (fread(&item, sizeof(ItemShop), 1, fp)) {
            printf("%-3d | %-25s | %-6d | %-5d\n", 
                   item.id, item.namaItem, item.hargaPoin, item.stok);
        }
        fclose(fp);
    }
    printf("--------------------------------------------------\n");
}

void tambahItemShop() {
    ItemShop newItem, temp;
    FILE *fp;
    int lastId = 0;

    system("cls");
    printf("=== TAMBAH BARANG BARU ===\n");

    // Auto ID
    fp = fopen("database/shop.dat", "rb");
    if (fp != NULL) {
        while (fread(&temp, sizeof(ItemShop), 1, fp)) {
            lastId = temp.id;
        }
        fclose(fp);
    }
    newItem.id = lastId + 1;
    printf("ID Barang: %d (Auto)\n", newItem.id);

    printf("Nama Barang: "); 
    scanf(" %[^\n]", newItem.namaItem); // Trik scanf spasi
    
    printf("Harga (Poin): "); 
    scanf("%d", &newItem.hargaPoin);
    
    printf("Stok Awal: "); 
    scanf("%d", &newItem.stok);

    // Simpan
    fp = fopen("database/shop.dat", "ab");
    fwrite(&newItem, sizeof(ItemShop), 1, fp);
    fclose(fp);
    
    msgBox("SUKSES", "Barang baru berhasil ditambahkan!", GREEN);
}

void hapusItemShop() {
    FILE *fp, *fpTemp;
    ItemShop item;
    int idHapus, found = 0;

    system("cls");
    lihatSemuaItem();
    printf("\nMasukkan ID Barang yang akan DIHAPUS (0 Batal): ");
    scanf("%d", &idHapus);

    if (idHapus == 0) return;

    fp = fopen("database/shop.dat", "rb");
    fpTemp = fopen("database/temp_shop.dat", "wb");

    if (fp == NULL) return;

    while (fread(&item, sizeof(ItemShop), 1, fp)) {
        if (item.id == idHapus) {
            found = 1;
        } else {
            fwrite(&item, sizeof(ItemShop), 1, fpTemp);
        }
    }

    fclose(fp);
    fclose(fpTemp);

    if (found) {
        remove("database/shop.dat");
        rename("database/temp_shop.dat", "database/shop.dat");
        msgBox("SUKSES", "Barang berhasil dihapus!", GREEN);
    } else {
        remove("database/temp_shop.dat");
        msgBox("GAGAL", "ID Barang tidak ditemukan.", RED);
    }
}

void menuKelolaShop() {
    int pilihan;
    char menu[][60] = {
        "Lihat Daftar Barang",
        "Tambah Barang Baru",
        "Hapus Barang",
        "Kembali"
    };

    do {
        system("cls");
        drawBoxWithShadow(5, 2, 40, 3, "KELOLA SHOP (ADMIN)");
        pilihan = drawMenu(5, 7, menu, 4);

        switch (pilihan) {
            case 0: lihatSemuaItem(); getch(); break;
            case 1: tambahItemShop(); break;
            case 2: hapusItemShop(); break;
            case 3: return;
        }
    } while (1);
}

//||===================================||
//||            CRUD QUIZ              ||
//||===================================||

void menuKelolaQuiz()
{
    int pilihan;
    char menuCRUD[][60] = {
        "Tambah Quiz Baru",
        "Lihat Semua Quiz",
        "Edit Quiz",
        "Hapus Quiz",
        "Kembali"};

    do
    {
        system("cls");
        drawBoxWithShadow(5, 2, 40, 3, "KELOLA QUIZ (CRUD)");

        pilihan = drawMenu(5, 7, menuCRUD, 5);

        switch (pilihan)
        {
        case 0:
            tambahQuiz();
            break;
        case 1:
            lihatSemuaQuiz();
            break;
        case 2:
            editQuiz();
            break;
        case 3:
            hapusQuiz();
            break;
        case 4:
            return; // Kembali ke Dashboard Pengajar
        }
    } while (1);
}

void tambahQuiz()
{
    DataQuiz q, temp;
    FILE *fp;

    system("cls");
    printf("=== TAMBAH QUIZ BARU ===\n");

    // 1. Auto-Generate ID (Sederhana: Cek ID terakhir + 1)
    // Kita baca file dari awal untuk cari ID terakhir
    int lastId = 0;
    fp = fopen("database/quiz.dat", "rb");
    if (fp != NULL)
    {
        while (fread(&temp, sizeof(DataQuiz), 1, fp))
        {
            lastId = temp.id;
        }
        fclose(fp);
    }
    q.id = lastId + 1; // ID otomatis urut
    printf("ID Soal: %d (Otomatis)\n", q.id);

    // 2. Input Data
    // Kita set default tipe = 1 (Pilihan Ganda) untuk sekarang
    q.tipe = 1;

    // Matkul ID (Nanti bisa dipilih dari daftar Matkul, skrg manual dulu)
    printf("Masukkan ID Matkul: ");
    scanf("%d", &q.idMatkul);
    fflush(stdin);

    printf("Masukkan Soal: ");
    scanf("%[^\n]", q.soal);
    fflush(stdin);

    printf("Opsi A: ");
    scanf("%[^\n]", q.opsiA);
    fflush(stdin);
    printf("Opsi B: ");
    scanf("%[^\n]", q.opsiB);
    fflush(stdin);
    printf("Opsi C: ");
    scanf("%[^\n]", q.opsiC);
    fflush(stdin);
    printf("Opsi D: ");
    scanf("%[^\n]", q.opsiD);
    fflush(stdin);

    printf("Kunci Jawaban (A/B/C/D): ");
    scanf("%s", q.jawabanBenar);
    fflush(stdin);

    printf("Poin jika benar: ");
    scanf("%d", &q.poin);
    fflush(stdin);

    // 3. Simpan ke File
    fp = fopen("database/quiz.dat", "ab"); // Mode Append
    if (fp == NULL)
    {
        printf("Error: Gagal membuka database!\n");
    }
    else
    {
        fwrite(&q, sizeof(DataQuiz), 1, fp); // Tulis struct ke file
        fclose(fp);
        msgBox("SUKSES", "Soal baru berhasil disimpan!", GREEN);
    }
}

void lihatSemuaQuiz()
{
    DataQuiz q;
    FILE *fp;

    system("cls");
    printf("=== DAFTAR SOAL BANK ===\n");
    printf("%-3s | %-30s | %-5s | %-5s\n", "ID", "Soal", "Kunci", "Poin");
    printf("----------------------------------------------------\n");

    fp = fopen("database/quiz.dat", "rb"); // Mode Read Binary
    if (fp == NULL)
    {
        printf("Database kosong atau belum dibuat.\n");
    }
    else
    {
        while (fread(&q, sizeof(DataQuiz), 1, fp))
        {
            // Kita potong tampilan soal biar tidak kepanjangan di tabel
            // Menggunakan format string precision (%.30s)
            printf("%-3d | %-30.30s... | %-5s | %-5d\n",
                   q.id, q.soal, q.jawabanBenar, q.poin);
        }
        fclose(fp);
    }

    printf("\nTekan ENTER kembali...");
    getch();
}

void hapusQuiz()
{
    FILE *fp, *fpTemp;
    DataQuiz q;
    int idHapus, found = 0;

    system("cls");
    printf("=== HAPUS DATA QUIZ ===\n");

    // Tampilkan dulu daftarnya biar user tau ID berapa yang mau dihapus
    // (Opsional: panggil fungsi lihatSemuaQuiz() di sini kalau mau)

    printf("Masukkan ID Soal yang akan dihapus: ");
    scanf("%d", &idHapus);

    fp = fopen("database/quiz.dat", "rb");
    fpTemp = fopen("database/temp.dat", "wb");

    if (fp == NULL)
    {
        msgBox("ERROR", "Database belum ada!", RED);
        return;
    }

    while (fread(&q, sizeof(DataQuiz), 1, fp))
    {
        if (q.id == idHapus)
        {
            found = 1; // Ketemu, JANGAN disalin ke temp (artinya dihapus)
            printf("Menghapus soal ID %d...\n", q.id);
        }
        else
        {
            // Bukan yang dicari? Salin ke file temp
            fwrite(&q, sizeof(DataQuiz), 1, fpTemp);
        }
    }

    fclose(fp);
    fclose(fpTemp);

    if (found)
    {
        remove("database/quiz.dat");                      // Hapus file asli
        rename("database/temp.dat", "database/quiz.dat"); // Ganti nama temp jadi asli
        msgBox("SUKSES", "Data berhasil dihapus!", GREEN);
    }
    else
    {
        remove("database/temp.dat"); // Hapus temp karena batal pakai
        msgBox("GAGAL", "ID Soal tidak ditemukan.", RED);
    }
}

void editQuiz()
{
    FILE *fp, *fpTemp;
    DataQuiz q;
    int idEdit, found = 0;

    system("cls");
    printf("=== EDIT DATA QUIZ ===\n");
    printf("Masukkan ID Soal yang akan diedit: ");
    scanf("%d", &idEdit);
    fflush(stdin); // Bersihkan buffer keyboard

    fp = fopen("database/quiz.dat", "rb");
    fpTemp = fopen("database/temp.dat", "wb");

    if (fp == NULL)
    {
        msgBox("ERROR", "Database belum ada!", RED);
        return;
    }

    while (fread(&q, sizeof(DataQuiz), 1, fp))
    {
        if (q.id == idEdit)
        {
            found = 1;
            // Tampilkan data lama sebagai referensi
            printf("\n--- Data Lama ---\n");
            printf("Soal: %s\n", q.soal);
            printf("Kunci: %s | Poin: %d\n", q.jawabanBenar, q.poin);

            printf("\n--- Masukkan Data Baru ---\n");
            // Kita pakai ID yang sama, tidak perlu diubah

            // Edit ID Matkul
            printf("ID Matkul Baru: ");
            scanf("%d", &q.idMatkul);
            fflush(stdin);

            // Edit Soal & Opsi
            printf("Soal Baru: ");
            scanf("%[^\n]", q.soal);
            fflush(stdin);
            printf("Opsi A: ");
            scanf("%[^\n]", q.opsiA);
            fflush(stdin);
            printf("Opsi B: ");
            scanf("%[^\n]", q.opsiB);
            fflush(stdin);
            printf("Opsi C: ");
            scanf("%[^\n]", q.opsiC);
            fflush(stdin);
            printf("Opsi D: ");
            scanf("%[^\n]", q.opsiD);
            fflush(stdin);

            // Edit Kunci & Poin
            printf("Kunci Baru (A/B/C/D): ");
            scanf("%s", q.jawabanBenar);
            fflush(stdin);
            printf("Poin Baru: ");
            scanf("%d", &q.poin);
            fflush(stdin);

            // Tulis data YANG BARU DIEDIT ke temp
            fwrite(&q, sizeof(DataQuiz), 1, fpTemp);
        }
        else
        {
            // Bukan yang dicari? Salin data ASLI ke temp
            fwrite(&q, sizeof(DataQuiz), 1, fpTemp);
        }
    }

    fclose(fp);
    fclose(fpTemp);

    if (found)
    {
        remove("database/quiz.dat");
        rename("database/temp.dat", "database/quiz.dat");
        msgBox("SUKSES", "Data berhasil diperbarui!", GREEN);
    }
    else
    {
        remove("database/temp.dat");
        msgBox("GAGAL", "ID Soal tidak ditemukan.", RED);
    }
}

//||===================================||
//||            CRUD USER              ||
//||===================================||

void viewUsers()
{
    FILE *fp = fopen("database/user.dat", "rb");
    User u;
    int i = 1;

    system("cls");
    printf("==================================================================================\n");
    printf("| No | Username       | Nama Lengkap         | Role      | Poin |\n");
    printf("==================================================================================\n");

    if (fp == NULL)
    {
        printf("| Data kosong atau file tidak ditemukan.\n");
    }
    else
    {
        while (fread(&u, sizeof(User), 1, fp))
        {
            // Terjemahkan kode role ke teks
            char roleName[20];
            if (u.role == 1)
                strcpy(roleName, "Siswa");
            else if (u.role == 2)
                strcpy(roleName, "Pengajar");
            else if (u.role == 3)
                strcpy(roleName, "Pengawas");
            else
                strcpy(roleName, "Unknown");

            printf("| %-2d | %-14s | %-20s | %-9s | %-4d |\n",
                   i++, u.username, u.namaLengkap, roleName, u.totalPoin);
        }
        fclose(fp);
    }
    printf("==================================================================================\n");
    printf("Tekan sembarang tombol untuk kembali...");
    getch();
}

void adminAddUser()
{
    User newUser;
    int roleChoice;

    system("cls");
    drawBoxWithShadow(10, 5, 60, 14, "TAMBAH USER BARU");

    gotoxy(12, 8);
    printf("Username      : ");
    scanInputString(28, 8, newUser.username, 20);

    gotoxy(12, 10);
    printf("Password      : ");
    scanInputString(28, 10, newUser.password, 20);

    gotoxy(12, 12);
    printf("Nama Lengkap  : ");
    scanInputString(28, 12, newUser.namaLengkap, 30);

    gotoxy(12, 14);
    printf("Email         : ");
    scanInputString(28, 14, newUser.email, 30);

    // Pilihan Role
    gotoxy(12, 16);
    printf("Pilih Role (1=Siswa, 2=Pengajar, 3=Pengawas): ");
    scanf("%d", &roleChoice);

    newUser.role = roleChoice;
    newUser.totalPoin = 0; // Default 0

    // Simpan pakai fungsi yang sudah kita buat sebelumnya
    saveUserToDB(newUser);

    msgBox("SUKSES", "User baru berhasil ditambahkan!", GREEN);
}

void deleteUser()
{
    char targetUsername[50];
    FILE *fp, *temp;
    User u;
    int found = 0;

    system("cls");
    viewUsers(); // Tampilkan dulu biar gampang milih
    printf("\nMasukkan Username yang akan DIHAPUS (0 untuk batal): ");
    scanf("%s", targetUsername);

    if (strcmp(targetUsername, "0") == 0)
        return;

    fp = fopen("database/user.dat", "rb");
    temp = fopen("database/temp.dat", "wb"); // File sementara

    if (fp == NULL || temp == NULL)
    {
        msgBox("ERROR", "Gagal membuka database!", RED);
        return;
    }

    while (fread(&u, sizeof(User), 1, fp))
    {
        if (strcmp(u.username, targetUsername) == 0)
        {
            found = 1; // Ketemu, JANGAN ditulis ke temp (artinya dihapus)
        }
        else
        {
            fwrite(&u, sizeof(User), 1, temp); // Salin user lain ke temp
        }
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        // Hapus file lama, ganti nama file temp jadi file asli
        remove("database/user.dat");
        rename("database/temp.dat", "database/user.dat");
        msgBox("SUKSES", "User berhasil dihapus permanent.", GREEN);
    }
    else
    {
        remove("database/temp.dat"); // Hapus temp karena batal
        msgBox("GAGAL", "Username tidak ditemukan.", RED);
    }
}

void updateUser()
{
    char targetUsername[50];
    FILE *fp, *temp;
    User u;
    int found = 0;

    system("cls");
    viewUsers();
    printf("\nMasukkan Username yang akan DIEDIT (0 untuk batal): ");
    scanf("%s", targetUsername);

    if (strcmp(targetUsername, "0") == 0)
        return;

    fp = fopen("database/user.dat", "rb");
    temp = fopen("database/temp.dat", "wb");

    if (fp == NULL || temp == NULL)
        return;

    while (fread(&u, sizeof(User), 1, fp))
    {
        if (strcmp(u.username, targetUsername) == 0)
        {
            found = 1;
            // Input Data Baru
            printf("\n--- EDIT DATA (%s) ---\n", u.username);
            printf("Nama Baru (Lama: %s): ", u.namaLengkap);

            // Trik input string agar bisa spasi setelah scanf sebelumnya
            char dummy;
            while ((dummy = getchar()) != '\n' && dummy != EOF)
                ;
            scanInputString(wherex(), wherey(), u.namaLengkap, 30);

            printf("Password Baru : ");
            scanInputString(wherex(), wherey(), u.password, 20);

            printf("Poin Baru (Lama: %d): ", u.totalPoin);
            scanf("%d", &u.totalPoin);

            // Tulis data yang SUDAH DIUPDATE ke temp
            fwrite(&u, sizeof(User), 1, temp);
        }
        else
        {
            // Tulis data LAMA ke temp
            fwrite(&u, sizeof(User), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove("database/user.dat");
        rename("database/temp.dat", "database/user.dat");
        msgBox("SUKSES", "Data User berhasil diperbarui!", GREEN);
    }
    else
    {
        remove("database/temp.dat");
        msgBox("GAGAL", "Username tidak ditemukan.", RED);
    }
}

void readUserDatabase()
{
    FILE *fuser;
    User user; // Variabel penampung sementara
    int i = 1; // Untuk nomor urut

    // 1. Buka file mode "rb" (Read Binary)
    fuser = fopen("database/user.dat", "rb");

    // Cek apakah file ada?
    if (fuser == NULL)
    {
        printf("Database belum dibuat atau kosong.\n");
        return;
    }

    printf("=================================================================================\n");
    printf("| No | Username       | Password   | Email              | Poin | Role |\n");
    printf("=================================================================================\n");

    // 2. Loop baca data satu per satu
    while (fread(&user, sizeof(User), 1, fuser))
    {
        printf("| %-2d | %-14s | %-10s | %-18s | %-4d | %-4d |\n",
               i,
               user.username,
               user.password,
               user.email,
               user.totalPoin,
               user.role);
        i++;
    }

    // 3. Tutup file SETELAH loop selesai (Penting!)
    fclose(fuser);

    printf("=================================================================================\n");
}

// Fungsi untuk menyimpan user baru ke file database
void saveUserToDB(User newUser)
{
    FILE *fuser;

    // 1. Buka file dengan mode "ab" (Append Binary)
    // Pastikan kamu sudah buat folder bernama 'database' di projectmu ya!
    fuser = fopen("database/user.dat", "ab");

    if (fuser == NULL)
    {
        printf("Error: Gagal membuka database untuk menyimpan.\n");
        return;
    }

    // 2. Tulis data struct ke file
    fwrite(&newUser, sizeof(User), 1, fuser);

    // 3. Tutup file
    fclose(fuser);
}

void menuKelolaUser()
{
    int pilihan;
    char menuCRUD[][60] = {
        "Lihat Semua User",
        "Tambah User Baru",
        "Edit Data User",
        "Hapus User",
        "Kembali"};

    do
    {
        system("cls");
        drawBoxWithShadow(5, 2, 40, 3, "KELOLA USER (CRUD)");

        pilihan = drawMenu(5, 7, menuCRUD, 5);

        switch (pilihan)
        {
        case 0:
            viewUsers();
            break;
        case 1:
            adminAddUser();
            break;
        case 2:
            updateUser();
            break;
        case 3:
            deleteUser();
            break;
        case 4:
            return; // Kembali ke Dashboard Pengawas
        }
    } while (1);
}

//||===================================||
//||            CRUD Ruangan           ||
//||===================================||

void tambahRuangan()
{
    FILE *file;
    struct Ruangan ruang;

    file = fopen("ruangan.dat", "ab");
    if (!file)
    {
        printf("Gagal membuka file!\n");
        return;
    }

    printf("Masukkan Kode Ruangan     : ");
    fgets(ruang.kode, sizeof(ruang.kode), stdin);
    ruang.kode[strcspn(ruang.kode, "\n")] = 0;

    printf("Masukkan Nama Ruangan     : ");
    fgets(ruang.namaRuangan, sizeof(ruang.namaRuangan), stdin);
    ruang.namaRuangan[strcspn(ruang.namaRuangan, "\n")] = 0;

    printf("Masukkan Kapasitas        : ");
    scanf("%d", &ruang.kapasitas);
    getchar();

    printf("Masukkan Lokasi (Lantai)  : ");
    fgets(ruang.lokasi, sizeof(ruang.lokasi), stdin);
    ruang.lokasi[strcspn(ruang.lokasi, "\n")] = 0;

    printf("Masukkan Created At       : ");
    fgets(ruang.createdAt, sizeof(ruang.createdAt), stdin);
    ruang.createdAt[strcspn(ruang.createdAt, "\n")] = 0;

    fwrite(&ruang, sizeof(ruang), 1, file);
    fclose(file);

    printf("Ruangan berhasil ditambahkan!\n");
}

void tampilRuangan()
{
    FILE *file;
    struct Ruangan ruang;

    file = fopen("ruangan.dat", "rb");
    if (!file)
    {
        printf("File belum ada atau gagal dibuka.\n");
        return;
    }

    printf("\n=== DATA RUANGAN ===\n");
    while (fread(&ruang, sizeof(ruang), 1, file))
    {
        printf("Kode        : %s\n", ruang.kode);
        printf("Nama        : %s\n", ruang.namaRuangan);
        printf("Kapasitas   : %d\n", ruang.kapasitas);
        printf("Lokasi      : %s\n", ruang.lokasi);
        printf("Created At  : %s\n\n", ruang.createdAt);
    }

    fclose(file);
}

void updateRuangan(const char *kodeTarget)
{
    FILE *src, *tmp;
    struct Ruangan ruang;
    int found = 0;

    src = fopen("ruangan.dat", "rb");
    tmp = fopen("temp.dat", "wb");

    if (!src || !tmp)
    {
        printf("Gagal membuka file!\n");
        return;
    }

    while (fread(&ruang, sizeof(ruang), 1, src))
    {
        if (strcmp(ruang.kode, kodeTarget) == 0)
        {
            found = 1;

            printf("\n=== DATA LAMA ===\n");
            printf("Kode        : %s\n", ruang.kode);
            printf("Nama        : %s\n", ruang.namaRuangan);
            printf("Kapasitas   : %d\n", ruang.kapasitas);
            printf("Lokasi      : %s\n", ruang.lokasi);

            printf("\n=== UPDATE DATA ===\n");

            printf("Masukkan Kode Baru               : ");
            fgets(ruang.kode, sizeof(ruang.kode), stdin);
            ruang.kode[strcspn(ruang.kode, "\n")] = 0;

            printf("Masukkan Nama ruangan Baru       : ");
            fgets(ruang.namaRuangan, sizeof(ruang.namaRuangan), stdin);
            ruang.namaRuangan[strcspn(ruang.namaRuangan, "\n")] = 0;

            printf("Masukkan Kapasitas Baru          : ");
            scanf("%d", &ruang.kapasitas);
            getchar();

            printf("Masukkan Lokasi Baru             : ");
            fgets(ruang.lokasi, sizeof(ruang.lokasi), stdin);
            ruang.lokasi[strcspn(ruang.lokasi, "\n")] = 0;

            printf("Masukkan CreatedAt Baru          : ");
            fgets(ruang.createdAt, sizeof(ruang.createdAt), stdin);
            ruang.createdAt[strcspn(ruang.createdAt, "\n")] = 0;
        }

        fwrite(&ruang, sizeof(ruang), 1, tmp);
    }

    fclose(src);
    fclose(tmp);

    if (found)
    {
        src = fopen("ruangan.dat", "wb");
        tmp = fopen("temp.dat", "rb");

        while (fread(&ruang, sizeof(ruang), 1, tmp))
        {
            fwrite(&ruang, sizeof(ruang), 1, src);
        }

        fclose(src);
        fclose(tmp);

        printf("Data ruangan berhasil diupdate!\n");
    }
    else
    {
        printf("Kode %s tidak ditemukan.\n", kodeTarget);
    }
}

void hapusRuangan(const char *kodeTarget)
{
    FILE *src, *tmp;
    struct Ruangan ruang;
    int found = 0;

    src = fopen("ruangan.dat", "rb");
    tmp = fopen("temp.dat", "wb");

    if (!src || !tmp)
    {
        printf("Gagal membuka file!\n");
        return;
    }

    char yakin;
    printf("Apakah yakin ingin menghapus? (Y/N): ");
    scanf(" %c", &yakin);
    getchar();

    if (yakin != 'Y' && yakin != 'y')
    {
        printf("Penghapusan dibatalkan.\n");
        fclose(src);
        fclose(tmp);
        return;
    }

    while (fread(&ruang, sizeof(ruang), 1, src))
    {
        if (strcmp(ruang.kode, kodeTarget) == 0)
        {
            found = 1;
        }
        else
        {
            fwrite(&ruang, sizeof(ruang), 1, tmp);
        }
    }

    fclose(src);
    fclose(tmp);

    if (found)
    {
        src = fopen("ruangan.dat", "wb");
        tmp = fopen("temp.dat", "rb");

        while (fread(&ruang, sizeof(ruang), 1, tmp))
        {
            fwrite(&ruang, sizeof(ruang), 1, src);
        }

        fclose(src);
        fclose(tmp);

        printf("Data berhasil dihapus!\n");
    }
    else
    {
        printf("Kode %s tidak ditemukan.\n", kodeTarget);
    }
}

void Ruangan()
{
    int pilih;
    char kode[20];

    do
    {
        printf("\n=== MENU CRUD RUANG ===\n");
        printf("1. Tambah Ruangan\n");
        printf("2. Tampilkan Ruangan\n");
        printf("3. Update Ruangan\n");
        printf("4. Hapus Ruangan\n");
        printf("0. Keluar\n");
        printf("Pilih menu : ");
        scanf("%d", &pilih);
        getchar();

        switch (pilih)
        {
        case 1:
            tambahRuangan();
            break;

        case 2:
            tampilRuangan();
            break;

        case 3:
            printf("Masukkan kode ruangan yang ingin diupdate: ");
            fgets(kode, 20, stdin);
            kode[strcspn(kode, "\n")] = 0;
            updateRuangan(kode);
            break;

        case 4:
            printf("Masukkan kode ruangan yang ingin dihapus: ");
            fgets(kode, 20, stdin);
            kode[strcspn(kode, "\n")] = 0;
            hapusRuangan(kode);
            break;

        case 0:
            printf("Keluar program.\n");
            break;

        default:
            printf("Pilihan tidak valid!\n");
        }

    } while (pilih != 0);

    return;
}

// --- FUNGSI MENU UTAMA ---

void absen()
{
    system("cls");
    printf("=========================================\n");
    printf("\tABSENSI KELAS (Dapatkan Poin!)\n");
    printf("=========================================\n");
    printf("Halo, %s! Pilih kelas untuk absen:\n", dataUser[currentUserIndex].namaLengkap);

    for (int i = 0; i < 5; i++)
    {
        printf("%d. %s (%s)\n", daftarMatkul[i].id, daftarMatkul[i].namaMatkul, daftarMatkul[i].hari);
    }

    int pilih;
    printf("Pilih ID Matkul: ");
    scanf("%d", &pilih);

    if (pilih >= 1 && pilih <= 5)
    {
        printf("\n[SUKSES] Anda berhasil absen di %s.\n", daftarMatkul[pilih - 1].namaMatkul);
        printf("[BONUS] +10 Poin ditambahkan ke akun Anda!\n");

        // Logika Transaksi Poin
        dataUser[currentUserIndex].totalPoin += 10;
    }
    else
    {
        printf("[ERROR] ID Matkul tidak valid.\n");
    }
}

void leaderboard()
{
    // Logic: Sorting Bubble Sort sederhana berdasarkan Poin (Descending)
    // Kita copy dulu array user agar urutan asli tidak rusak
    User tempUser[100];
    for (int i = 0; i < jumlahUser; i++)
    {
        tempUser[i] = dataUser[i];
    }

    // Sorting
    for (int i = 0; i < jumlahUser - 1; i++)
    {
        for (int j = 0; j < jumlahUser - i - 1; j++)
        {
            if (tempUser[j].totalPoin < tempUser[j + 1].totalPoin)
            {
                User temp = tempUser[j];
                tempUser[j] = tempUser[j + 1];
                tempUser[j + 1] = temp;
            }
        }
    }

    printf("=========================================\n");
    printf("\tLEADERBOARD SISWA (Top Rank)\n");
    printf("=========================================\n");
    printf("%-5s | %-20s | %-10s\n", "Rank", "Username", "Poin");
    printf("-----------------------------------------\n");

    for (int i = 0; i < jumlahUser; i++)
    {
        if (tempUser[i].role == 1)
        { // Hanya tampilkan Siswa (Role 1)
            printf("%-5d | %-20s | %-10d\n", i + 1, tempUser[i].username, tempUser[i].totalPoin);
        }
    }
}

void pelatih()
{
    int idPilihan, found = 0;
    int indexPengajar = -1;
    char konfirmasi;

    do
    {
        system("cls");
        printf("==================================================================\n");
        printf("\tBOOKING JADWAL KONSULTASI (Saldo Poin: %d)\n", dataUser[currentUserIndex].totalPoin);
        printf("==================================================================\n");
        printf("%-4s | %-20s | %-20s | %-6s | %-15s\n", "ID", "Nama Pengajar", "Spesialisasi", "Rating", "Status");
        printf("------------------------------------------------------------------\n");

        for (int i = 0; i < 5; i++)
        {
            // Visualisasi Bintang
            char bintang[10] = "";
            if (daftarPengajar[i].rating >= 4.8)
                strcpy(bintang, "*");

            // Warna/Status Logic (Visual Text)
            printf("%-4d | %-20s | %-20s | %.1f %s | %-15s\n",
                   daftarPengajar[i].id,
                   daftarPengajar[i].nama,
                   daftarPengajar[i].spesialisasi,
                   daftarPengajar[i].rating,
                   bintang,
                   daftarPengajar[i].status);
        }
        printf("------------------------------------------------------------------\n");
        printf("Ketik 0 untuk kembali ke Dashboard.\n");
        printf("Masukkan ID Pengajar untuk Booking: ");

        if (scanf("%d", &idPilihan) != 1)
        {
            while (getchar() != '\n')
                ;
            idPilihan = -1;
        }

        if (idPilihan == 0)
            return; // Kembali

        // LOGIKA PENCARIAN ID
        found = 0;
        for (int i = 0; i < 5; i++)
        {
            if (daftarPengajar[i].id == idPilihan)
            {
                indexPengajar = i;
                found = 1;
                break;
            }
        }

        if (found)
        {
            // Cek Status: Apakah Available?
            if (strcmp(daftarPengajar[indexPengajar].status, "Available") == 0)
            {

                printf("\nIngin booking sesi dengan %s? (y/n): ", daftarPengajar[indexPengajar].nama);
                scanf(" %c", &konfirmasi); // Spasi sebelum %c untuk membersihkan buffer

                if (konfirmasi == 'y' || konfirmasi == 'Y')
                {

                    // 1. Kurangi Poin
                    dataUser[currentUserIndex].totalPoin -= 20;

                    // 2. Ubah Status Pengajar
                    // Kita tempelkan nama user agar terlihat siapa yang booking
                    sprintf(daftarPengajar[indexPengajar].status, "Booked by %s", dataUser[currentUserIndex].username);

                    printf("\n[SUKSES] Jadwal berhasil dibuat! Poin Anda tersisa: %d\n", dataUser[currentUserIndex].totalPoin);
                }
                else
                {
                    printf("\nBooking dibatalkan.\n");
                }
            }
            else
            {
                printf("\n[MAAF] Pengajar ini sedang tidak tersedia (%s).\n", daftarPengajar[indexPengajar].status);
            }
        }
        else
        {
            printf("\n[ERROR] ID Pengajar tidak ditemukan.\n");
        }

        printf("\nTekan tombol apa saja untuk lanjut...");
        getch();

    } while (idPilihan != 0);
}

void reservasi()
{
    int foundBooking = 0;

    system("cls");
    printf("==================================================================\n");
    printf("\tMY STUDY ROUTE (Jadwal Saya)\n");
    printf("==================================================================\n");
    printf("Nama Siswa : %s\n", dataUser[currentUserIndex].namaLengkap);
    printf("Total Poin : %d\n", dataUser[currentUserIndex].totalPoin);
    printf("==================================================================\n\n");

    // --- BAGIAN 1: JADWAL JANJI TEMU (BOOKING) ---
    printf("[1] JADWAL KONSULTASI (Mentor/Dosen)\n");
    printf("------------------------------------------------------------------\n");
    printf("%-20s | %-20s | %-15s\n", "Nama Pengajar", "Spesialisasi", "Status");
    printf("------------------------------------------------------------------\n");

    // Loop data pengajar global
    for (int i = 0; i < 5; i++)
    {
        // Cek apakah string status mengandung username user saat ini
        // strstr akan mengembalikan pointer jika teks ditemukan, NULL jika tidak
        if (strstr(daftarPengajar[i].status, dataUser[currentUserIndex].username) != NULL)
        {
            printf("%-20s | %-20s | %-15s\n",
                   daftarPengajar[i].nama,
                   daftarPengajar[i].spesialisasi,
                   "Terjadwal (Booked)");
            foundBooking = 1;
        }
    }

    if (!foundBooking)
    {
        printf("   >> Belum ada jadwal konsultasi yang dibooking.\n");
        printf("   >> Silakan ke menu 'Pelatih' untuk membuat janji.\n");
    }
    printf("------------------------------------------------------------------\n\n");

    // --- BAGIAN 2: JADWAL KULIAH REGULER ---
    printf("[2] JADWAL KULIAH REGULER (Route Minggu Ini)\n");
    printf("------------------------------------------------------------------\n");
    printf("%-5s | %-25s | %-10s\n", "ID", "Mata Kuliah", "Hari");
    printf("------------------------------------------------------------------\n");

    // Loop data matkul (Jadwal Statis)
    for (int i = 0; i < 5; i++)
    {
        printf("%-5d | %-25s | %-10s\n",
               daftarMatkul[i].id,
               daftarMatkul[i].namaMatkul,
               daftarMatkul[i].hari);
    }
    printf("------------------------------------------------------------------\n");

    printf("\nTips: Jangan lupa lakukan 'Absen' sesuai hari kuliah untuk dapat poin!\n");
    printf("\nTekan tombol apa saja untuk kembali ke Dashboard...");
    getch();
}

void pointShop() {
    FILE *fp;
    ItemShop item;
    ItemShop daftarBelanja[50]; // Buffer RAM sementara
    int totalBarang = 0;
    int pilihan, konfirmasi;
    int found = 0;

    do {
        // 1. LOAD DATA DARI FILE KE RAM
        // Kenapa diload dulu? Agar mudah menampilkan menu & memilih index
        totalBarang = 0;
        fp = fopen("database/shop.dat", "rb");
        if (fp == NULL) {
            msgBox("MAAF", "Toko sedang tutup (Database missing).", RED);
            return;
        }
        while (fread(&item, sizeof(ItemShop), 1, fp)) {
            daftarBelanja[totalBarang] = item;
            totalBarang++;
        }
        fclose(fp);

        // 2. TAMPILKAN MENU
        system("cls");
        printf("=========================================\n");
        printf("\tPOINT SHOP (Saldo Anda: %d Poin)\n", dataUser[currentUserIndex].totalPoin);
        printf("=========================================\n");
        printf("%-3s | %-25s | %-8s | %-5s\n", "ID", "Nama Item", "Harga", "Stok");
        printf("--------------------------------------------------\n");

        for (int i = 0; i < totalBarang; i++) {
             printf("%-3d | %-25s | %-8d | %-5d\n", 
                   daftarBelanja[i].id, 
                   daftarBelanja[i].namaItem, 
                   daftarBelanja[i].hargaPoin, 
                   daftarBelanja[i].stok);
        }
        printf("--------------------------------------------------\n");
        printf("0. Kembali ke Dashboard\n");
        printf("Pilih ID Barang yang ingin dibeli: ");
        scanf("%d", &pilihan);
        
        if (pilihan == 0) return;

        // 3. CARI BARANG YANG DIPILIH
        int indexDipilih = -1;
        for(int i=0; i<totalBarang; i++){
            if(daftarBelanja[i].id == pilihan) {
                indexDipilih = i;
                break;
            }
        }

        // 4. PROSES TRANSAKSI
        if (indexDipilih != -1) {
            ItemShop barang = daftarBelanja[indexDipilih];

            if (barang.stok <= 0) {
                 msgBox("GAGAL", "Stok barang habis!", RED);
            } 
            else if (dataUser[currentUserIndex].totalPoin < barang.hargaPoin) {
                 msgBox("MISKIN", "Poin tidak cukup!", RED);
            }
            else {
                // Konfirmasi
                printf("\nBeli '%s' seharga %d Poin? (1=Ya, 0=Batal): ", barang.namaItem, barang.hargaPoin);
                scanf("%d", &konfirmasi);

                if (konfirmasi == 1) {
                    // A. Update RAM (User)
                    dataUser[currentUserIndex].totalPoin -= barang.hargaPoin;
                    
                    // B. Update Database User (PENTING)
                    tambahPoinUser(dataUser[currentUserIndex].username, -barang.hargaPoin); // Minus untuk mengurangi

                    // C. Update Database Shop (Kurangi Stok)
                    // Teknik: Update di Array RAM -> Tulis Ulang Semua ke File
                    daftarBelanja[indexDipilih].stok -= 1; // Kurangi stok di RAM
                    
                    fp = fopen("database/shop.dat", "wb"); // Mode wb (Write Binary) menimpa file lama
                    for(int i=0; i<totalBarang; i++){
                        fwrite(&daftarBelanja[i], sizeof(ItemShop), 1, fp);
                    }
                    fclose(fp);

                    msgBox("SUKSES", "Pembelian berhasil! Barang masuk inventori.", GREEN);
                }
            }
        } else {
            msgBox("ERROR", "ID Barang tidak ditemukan.", RED);
        }

    } while (pilihan != 0);
}

void tambahPoinUser(char usernameTarget[], int poinTambahan)
{
    FILE *fp = fopen("database/user.dat", "rb");
    FILE *fpTemp = fopen("database/temp_user.dat", "wb");
    User u;
    int found = 0;

    if (fp == NULL)
        return;

    while (fread(&u, sizeof(User), 1, fp))
    {
        if (strcmp(u.username, usernameTarget) == 0)
        {
            u.totalPoin += poinTambahan; // Tambahkan poin!
            found = 1;
        }
        fwrite(&u, sizeof(User), 1, fpTemp);
    }

    fclose(fp);
    fclose(fpTemp);

    if (found)
    {
        remove("database/user.dat");
        rename("database/temp_user.dat", "database/user.dat");
    }
    else
    {
        remove("database/temp_user.dat");
    }
}

void jalankanQuiz()
{
    FILE *fp;
    DataQuiz q;
    DataQuiz daftarSoal[100]; // Wadah untuk menampung semua soal dari file
    int totalSoal = 0;

    // --- 1. BACA DATABASE QUIZ ---
    fp = fopen("database/quiz.dat", "rb");
    if (fp == NULL)
    {
        msgBox("MAAF", "Belum ada kuis yang tersedia saat ini.", BLUE);
        return;
    }

    // Masukkan semua soal dari file ke dalam Array RAM
    while (fread(&q, sizeof(DataQuiz), 1, fp))
    {
        daftarSoal[totalSoal] = q;
        totalSoal++;
    }
    fclose(fp);

    if (totalSoal == 0)
    {
        msgBox("KOSONG", "Bank soal masih kosong.", RED);
        return;
    }

    // --- 2. PILIH SOAL ACAK (RANDOM) ---
    // Gunakan fungsi rand()
    int randomIndex = rand() % totalSoal;
    DataQuiz soalTerpilih = daftarSoal[randomIndex];

    // --- 3. TAMPILKAN UI ---
    system("cls");

    // Header
    gotoxy(20, 3);
    printf("=== KUIS AKADEMIK ===");
    gotoxy(20, 4);
    printf("Poin Hadiah: %d", soalTerpilih.poin);
    gotoxy(20, 5);
    printf("Tipe: %s", (soalTerpilih.tipe == 1) ? "Pilihan Ganda" : "Essay");

    // Tampilkan Soal
    gotoxy(20, 7);
    printf("SOAL: %s", soalTerpilih.soal);

    // --- 4. LOGIKA BERDASARKAN TIPE ---

    if (soalTerpilih.tipe == 1)
    {
        // === LOGIKA PILIHAN GANDA ===
        char options[4][60];

        sprintf(options[0], "A. %-20s", soalTerpilih.opsiA);
        sprintf(options[1], "B. %-20s", soalTerpilih.opsiB);
        sprintf(options[2], "C. %-20s", soalTerpilih.opsiC);
        sprintf(options[3], "D. %-20s", soalTerpilih.opsiD);

        int pilihanIndex = drawMenu(20, 9, options, 4);
        char jawabanUser = 'A' + pilihanIndex;

        // Cek Jawaban (Huruf pertama saja yang dicek)
        if (jawabanUser == soalTerpilih.jawabanBenar[0])
        {
            msgBox("BENAR!", "Selamat, jawaban kamu tepat!", GREEN);

            // Update RAM & Database
            dataUser[currentUserIndex].totalPoin += soalTerpilih.poin;
            tambahPoinUser(dataUser[currentUserIndex].username, soalTerpilih.poin);
        }
        else
        {
            char pesanSalah[100];
            sprintf(pesanSalah, "Salah. Jawaban benar: %s", soalTerpilih.jawabanBenar);
            msgBox("SALAH", pesanSalah, RED);
        }
    }
    else
    {
        // === LOGIKA ESSAY ===
        char jawabanEssay[100];

        gotoxy(20, 9);
        printf("Jawaban Kamu (Satu Kata/Singkat):");
        gotoxy(20, 10);
        printf(">> ");

        // Membaca input kalimat (Scanf trik untuk spasi)
        scanf(" %[^\n]", jawabanEssay);

        // Cek Jawaban (Case Insensitive Manual)
        // Kita gunakan strcmpi (Windows) atau strcasecmp (Linux/GCC)
        // Jika error, pakai strcmp biasa tapi user harus teliti huruf besar/kecil
        if (strcasecmp(jawabanEssay, soalTerpilih.jawabanBenar) == 0)
        {
            msgBox("HEBAT!", "Jawaban Essay kamu Tepat!", GREEN);

            dataUser[currentUserIndex].totalPoin += soalTerpilih.poin;
            tambahPoinUser(dataUser[currentUserIndex].username, soalTerpilih.poin);
        }
        else
        {
            char pesanSalah[100];
            sprintf(pesanSalah, "Kurang tepat. Kunci: %s", soalTerpilih.jawabanBenar);
            msgBox("SALAH", pesanSalah, RED);
        }
    }
}

//||==========================================||
//||                Ini Login                 ||
//||==========================================||

// Fungsi Login mengembalikan 1 jika sukses, 0 jika gagal
int loginProcess()
{
    char username[50], password[50];
    User userCheck; // Variabel untuk membaca file
    int found = 0;

    // --- 1. SETUP TAMPILAN ---
    system("cls");
    int startX = ((getConsoleWidth() - 40) / 2);
    int startY = ((getConsoleHeight() - 12) / 2);

    // Gambar Kotak
    drawBoxWithShadow(startX, startY, 40, 12, "LOGIN USER");

    // --- 2. ANIMASI INTRO ---
    // Teks muncul di dalam kotak
    animateTyping(startX + 3, startY + 3, "Connecting to database...", 30);
    // Spinner muncul di sebelah kanan teks (posisi 29)
    animateSpinner(startX + 29, startY + 3, 3, 100);

    // Bersihkan area animasi agar bersih saat input form muncul
    clearArea(startX + 1, startY + 1, 38, 10);

    // Gambar ulang border (opsional, untuk memastikan rapi)
    borderDoubleLine(startX, startY, 40, 12);
    gotoxy(startX + 15, startY);
    printf(" LOGIN USER "); // Judul ulang

    // --- 3. INPUT FORM ---
    gotoxy(startX + 3, startY + 3);
    printf("Username : ");
    scanInputString(startX + 14, startY + 3, username, 20);

    gotoxy(startX + 3, startY + 5);
    printf("Password : ");
    scanInputPassword(startX + 14, startY + 5, password, 20);

    // --- 4. LOGIKA CEK KE FILE (DATABASE) ---
    FILE *fuser = fopen("database/user.dat", "rb");

    if (fuser == NULL)
    {
        msgBox("ERROR", "Database tidak ditemukan!", RED);
        return 0;
    }

    while (fread(&userCheck, sizeof(User), 1, fuser))
    {
        // Bandingkan Input dengan Data di File
        if (strcmp(userCheck.username, username) == 0 && strcmp(userCheck.password, password) == 0)
        {
            // Jika COCOK, simpan data ke memori sementara (biar bisa dipakai di Home)
            dataUser[0] = userCheck; // Kita pakai index 0 sebagai 'Active User'
            currentUserIndex = 0;
            found = 1;
            break;
        }
    }
    fclose(fuser);

    // --- 5. HASIL LOGIN ---
    if (found)
    {
        // Tampilkan MsgBox Sukses (Sesuai request)
        msgBox("SUCCESS", "Login Berhasil! Welcome.", GREEN);
        return 1;
    }
    else
    {
        msgBox("FAILED", "Username/Password Salah!", RED);
        return 0;
    }
}

void Register()
{
    User tempUser; // Variabel sementara

    system("cls");
    printf("=========================================\n");
    printf(" \tREGISTER NEW USER\n");
    printf("=========================================\n");

    // Input data
    printf("Email: ");
    scanf("%s", tempUser.email);
    printf("Nama Panggilan: ");
    scanf("%s", tempUser.namaLengkap);
    printf("Username: ");
    scanf("%s", tempUser.username);
    printf("Password: ");
    scanf("%s", tempUser.password);

    // Set data default
    tempUser.totalPoin = 0;
    tempUser.role = 1; // Default jadi Siswa (1)

    // --- SIMPAN KE FILE ---
    saveUserToDB(tempUser);
    // ----------------------

    printf("\nRegistrasi Berhasil! Data tersimpan di database.\n");
}

void exitProgram()
{
    FILE *fptr;
    char buffer[255]; // Buffer untuk menyimpan baris teks sementara

    system("cls"); // Bersihkan layar agar art terlihat jelas

    // Membuka file dalam mode "r" (read)
    fptr = fopen("thanks.txt", "r");

    // Cek apakah file berhasil dibuka
    if (fptr == NULL)
    {
        // Jika file tidak ditemukan (error handling)
        printf("=========================================\n");
        printf(" \tTerima Kasih!\n");
        printf("=========================================\n");
        printf("(File thanks.txt tidak ditemukan)\n");
    }
    else
    {
        // Jika file ditemukan, baca baris per baris
        while (fgets(buffer, sizeof(buffer), fptr))
        {
            printf("%s", buffer); // Cetak isi file ke layar
        }
        // Jangan lupa tutup file setelah selesai
        fclose(fptr);
    }

    // Memberi jeda sedikit sebelum program benar-benar menutup
    printf("\nTekan tombol apa saja untuk keluar...");
    getch();
    exit(0);
}

void About()
{
    system("cls");
    printf("=========================================\n");
    printf(" \tAbout Study Route App\n");
    printf("=========================================\n");
    printf("Aplikasi manajemen studi mahasiswa dengan fitur Gamifikasi.\n");
    printf("Versi: 1.2 (Dev Build)\n");
    printf("Dev: Informatics Management Student\n");
}

int main()
{

    fullscreen();
    srand(time(0));
    hideCursor();
    remove_scrollbar();
    checkShopDatabase();
    system("title Study Route - Academic Management App");

    int pilihan;
    do
    {
        system("cls"); // Bersihkan layar setiap kembali ke menu utama
        // setColor(BLACK, YELLOW);
        system("color E0");

        PrintTxt(1, 1, "Sub_Header.txt");

        printf("\n1. Login\n");
        printf("2. Register\n");
        printf("3. About\n");
        printf("4. Exit\n");
        printf("Pilihan [1-4]: ");

        if (scanf("%d", &pilihan) != 1)
        {
            while (getchar() != '\n')
                ;
            pilihan = -1;
        }

        switch (pilihan)
        {
        case 1:
            if (loginProcess() == 1)
            {
                // CEK ROLE SETELAH LOGIN SUKSES
                int role = dataUser[currentUserIndex].role;

                if (role == 1)
                {
                    dashboardSiswa();
                }
                else if (role == 2)
                {
                    dashboardPengajar();
                }
                else if (role == 3)
                {
                    dashboardPengawas();
                }
                else
                {
                    msgBox("ERROR", "Role User Tidak Dikenali!", RED);
                }
            }
            break;
        case 2:
            Register();
            getch();
            break;
        case 3:
            About();
            getch();
            break;
        case 4:
            exitProgram();
            break;
        default:
            printf("Pilihan tidak valid.\n");
            getch();
            break;
        }

    } while (pilihan != 4);

    return 0;
}
