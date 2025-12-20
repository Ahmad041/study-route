#include "lib.h"
// --- DEFINISI STRUKTUR DATA (MASTER DATA) ---

typedef char string[50];

typedef struct
{
    char username[50];
    char password[50];
    char email[50];
    char namaLengkap[50];
    int totalPoin; // Untuk Leaderboard
    int role;      // 1 = Siswa, 2 = Pengajar
} User;

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
int jumlahUser = 0; // Counter user yang terdaftar
int currentUserIndex = -1;

// --- DATA DUMMY BARANG ---
ItemShop daftarItem[5] = {
    {1, "Voucher Kantin 10k", 50, 20},
    {2, "Notebook Study Route", 100, 10},
    {3, "Sticker Pack Programming", 20, 50},
    {4, "Kaos 'C Programmer'", 300, 5},
    {5, "Akses Modul Premium", 150, 99}};

MataKuliah daftarMatkul[5] = {
    {1, "Algoritma Pemrograman", "Senin"},
    {2, "Struktur Data", "Selasa"},
    {3, "Basis Data", "Rabu"},
    {4, "Pemrograman Web", "Kamis"},
    {5, "Jaringan Komputer", "Jumat"}};

Pengajar daftarPengajar[5] = {
    {101, "Prof. Budi Santoso", "Algoritma & C++", 4.9, "Available"},
    {102, "Dr. Siti Aminah", "Web Dev (Laravel)", 4.8, "Busy"},
    {103, "Pak Anton Wijaya", "IoT & Embedded", 4.7, "Available"},
    {104, "Kak Dinda Pertiwi", "Frontend (React)", 4.5, "Available"},
    {105, "Pak Rudi Hermawan", "Dasar Programming", 4.2, "In Class"}};

// Variable untuk sesi login saat ini

// --- PROTOTYPE FUNGSI ---
void Home();
void absen();
void pelatih(); // Placeholder untuk fitur Pengajar
void leaderboard();
void reservasi();
void pointShop();
void exitProgram();
int loginProcess();
void Register();
void About();
void pointShop();
void reservasi();
void viewUser();
void adminAddUser();
void deleteUser();
void updateUser();
void menuKelolaUser();
void saveUserToDB(User);

// --- DASHBOARD SESUAI ROLE ---

void dashboardSiswa(){
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
        char menuSiswa[][50] = {
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
            msgBox("INFO", "Fitur Quiz (Coming Soon)", BLUE);
            break;
        case 5:
            return; // Logout
        }
    } while (1);
}

void dashboardPengajar(){
    int pilihan;
    do
    {
        system("cls");
        printf("=========================================\n");
        printf(" DASHBOARD PENGAJAR: %s \n", dataUser[currentUserIndex].namaLengkap);
        printf(" Saldo Poin: %d \n", dataUser[currentUserIndex].totalPoin);
        printf("=========================================\n");

        char menuPengajar[][50] = {
            "Jadwal Mengajar",
            "Buat Quiz Baru",
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
            msgBox("INFO", "Fitur Buat Quiz (Coming Soon)", BLUE);
            break;
        case 2:
            msgBox("INFO", "Fitur Booking Ruangan (Coming Soon)", BLUE);
            break;
        case 3:
            pointShop();
            break; // Pengajar juga bisa belanja
        case 4:
            return;
        }
    } while (1);
}

void dashboardPengawas(){
    int pilihan;
    do
    {
        system("cls");
        drawBoxWithShadow(2, 2, 50, 3, "ADMIN DASHBOARD");
        gotoxy(4, 4);
        printf("User: %s (Super Admin)", dataUser[currentUserIndex].username);

        char menuAdmin[][50] = {
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
            msgBox("INFO", "Menu Kelola Ruangan (Next Step)", RED);
            break;
        case 3:
            msgBox("INFO", "Menu Kelola Modul (Next Step)", RED);
            break;
        case 4:
            msgBox("INFO", "Menu Kelola Shop (Next Step)", RED);
            break;
        case 5:
            return;
        }
    } while (1);
}



//||===================================||
//||            CRUD USER              ||
//||===================================||

void viewUsers(){
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

void deleteUser(){
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

void updateUser(){
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

void readUserDatabase(){
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

//||===================================||
//||            CRUD USER              ||
//||===================================||



// Fungsi untuk menyimpan user baru ke file database
void saveUserToDB(User newUser){
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

void menuKelolaUser(){
    int pilihan;
    char menuCRUD[][50] = {
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

void pointShop()
{
    int pilihan, konfirmasi;

    // Loop agar user bisa belanja berkali-kali sampai memilih keluar
    do
    {
        system("cls");
        printf("=========================================\n");
        printf("\tPOINT SHOP (Saldo Anda: %d Poin)\n", dataUser[currentUserIndex].totalPoin);
        printf("=========================================\n");
        printf("%-3s | %-25s | %-8s | %-5s\n", "ID", "Nama Item", "Harga", "Stok");
        printf("--------------------------------------------------\n");

        for (int i = 0; i < 5; i++)
        {
            printf("%-3d | %-25s | %-8d | %-5d\n",
                   daftarItem[i].id,
                   daftarItem[i].namaItem,
                   daftarItem[i].hargaPoin,
                   daftarItem[i].stok);
        }
        printf("--------------------------------------------------\n");
        printf("0. Kembali ke Dashboard\n");
        printf("Pilih ID Barang yang ingin dibeli: ");

        if (scanf("%d", &pilihan) != 1)
        {
            while (getchar() != '\n')
                ;
            pilihan = -1;
        }

        if (pilihan == 0)
            return; // Kembali ke menu utama

        // LOGIKA TRANSAKSI
        if (pilihan >= 1 && pilihan <= 5)
        {
            int indexBarang = pilihan - 1; // Array mulai dari 0

            // 1. Cek Apakah Stok Ada?
            if (daftarItem[indexBarang].stok <= 0)
            {
                printf("\n[GAGAL] Stok barang habis!\n");
            }
            // 2. Cek Apakah Poin Cukup?
            else if (dataUser[currentUserIndex].totalPoin < daftarItem[indexBarang].hargaPoin)
            {
                printf("\n[GAGAL] Poin Anda tidak cukup! (Butuh: %d, Punya: %d)\n",
                       daftarItem[indexBarang].hargaPoin, dataUser[currentUserIndex].totalPoin);
            }
            // 3. Jika Valid, Lakukan Transaksi
            else
            {
                printf("\nAnda akan membeli '%s' seharga %d Poin.\n",
                       daftarItem[indexBarang].namaItem, daftarItem[indexBarang].hargaPoin);
                printf("Tekan 1 untuk Konfirmasi, 0 untuk Batal: ");
                scanf("%d", &konfirmasi);

                if (konfirmasi == 1)
                {
                    // Kurangi Poin User
                    dataUser[currentUserIndex].totalPoin -= daftarItem[indexBarang].hargaPoin;
                    // Kurangi Stok Barang
                    daftarItem[indexBarang].stok -= 1;

                    printf("\n[SUKSES] Pembelian berhasil! Sisa poin Anda: %d\n", dataUser[currentUserIndex].totalPoin);
                }
                else
                {
                    printf("\nPembelian dibatalkan.\n");
                }
            }
        }
        else
        {
            printf("\nItem tidak ditemukan.\n");
        }

        printf("\nTekan tombol apa saja untuk lanjut...");
        getch();

    } while (pilihan != 0);
}




void Home()
{
    int pilihan;
    do
    {
        system("cls");
        printf("=========================================\n");
        printf("\tDASHBOARD: %s (Poin: %d)\n", dataUser[currentUserIndex].namaLengkap, dataUser[currentUserIndex].totalPoin);
        printf("=========================================\n");
        printf("1. Absen Kelas (Tambah Poin)\n");
        printf("2. Data Pengajar (Pelatih)\n");
        printf("3. Leaderboard\n");
        printf("4. Study Route (Reservasi)\n");
        printf("5. Point Shop (Tukar Poin)\n"); // <--- MENU BARU
        printf("6. Logout\n");                  // Geser nomor logout
        printf("Pilihan [1-6]: ");

        if (scanf("%d", &pilihan) != 1)
        {
            while (getchar() != '\n')
                ;
            pilihan = -1;
        }

        switch (pilihan)
        {
        case 1:
            absen();
            break;
        case 2:
            pelatih();
            break;
        case 3:
            leaderboard();
            break;
        case 4:
            reservasi();
            break;
        case 5:
            pointShop();
            break; // <--- PANGGIL FUNGSI
        case 6:
            printf("Logging out...\n");
            return;
        default:
            printf("Pilihan salah.\n");
            break;
        }

        if (pilihan != 6)
        {
            printf("\nTekan tombol apa saja untuk lanjut...");
            getch();
        }
    } while (pilihan != 6);
}

// Fungsi Login mengembalikan 1 jika sukses, 0 jika gagal

//||==========================================||
//||                Ini Login                 ||
//||==========================================||

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
    hideCursor();
    remove_scrollbar();

    jumlahUser = 2;

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