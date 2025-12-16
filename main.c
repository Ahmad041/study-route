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
void exitProgram();
int loginProcess();
void Register();
void About();

// --- FUNGSI FITUR UTAMA ---

void absen()
{
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

// --- FUNGSI MENU UTAMA ---

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
int loginProcess()
{
    char username[50];
    char password[50];
    int found = 0;

    system("cls");
    int startX = ((getConsoleWidth() - 40) / 2);
    int startY = ((getConsoleHeight() - 12) / 2);
    drawBoxWithShadow(startX, startY, 40, 12, "LOGIN USER");
    
    gotoxy(startX + 2, startY + 3);
    printf("Username : ");
    scanInputString(startX + 13, startY + 3, username, 20);
    gotoxy(startX + 2, startY + 4);
    printf("Password : ");
    scanInputPassword(startX + 13, startY + 4, password, 20) ;


    drawBoxWithShadow(startX, startY, 40, 12, "WELCOME");
    animateTyping(startX + 5, startY + 3, "Halo! Sistem sedang memuat data...", 50);
    
    
    animateSpinner(startX + 17, startY + 6, 5, 100); 
    

    animateBlinkText(startX + 13, startY + 4, "PRESS ENTER TO START", 5);
    // Cek ke "Database" Array
    for (int i = 0; i < jumlahUser; i++)
    {
        if (strcmp(dataUser[i].username, username) == 0 && strcmp(dataUser[i].password, password) == 0)
        {
            currentUserIndex = i; // Simpan siapa yang login
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("\nLogin Berhasil! Selamat datang, %s.\n", dataUser[currentUserIndex].namaLengkap);
        getch();
        return 1;
    }
    else
    {
        printf("\nLogin Gagal! Username atau Password salah.\n");
        getch();
        return 0;
    }
}

void Register()
{
    char newUsername[50];
    char newPassword[50];
    char email[50];
    char nama[50];

    system("cls");
    printf("=========================================\n");
    printf(" \tREGISTER NEW USER\n");
    printf("=========================================\n");
    printf("Email: ");
    scanf("%s", email);
    printf("Nama Panggilan: ");
    scanf("%s", nama);
    printf("Username: ");
    scanf("%s", newUsername);
    printf("Password: ");
    scanf("%s", newPassword);

    // Simpan ke Array
    strcpy(dataUser[jumlahUser].email, email);
    strcpy(dataUser[jumlahUser].namaLengkap, nama);
    strcpy(dataUser[jumlahUser].username, newUsername);
    strcpy(dataUser[jumlahUser].password, newPassword);
    dataUser[jumlahUser].totalPoin = 0; // Poin awal 0
    dataUser[jumlahUser].role = 1;      // Default role Siswa

    jumlahUser++; // Increment jumlah user

    printf("\nRegistrasi Berhasil! Silakan Login.\n");
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

int main()
{
    fullscreen();
    hideCursor();
    remove_scrollbar();
    // Seed data dummy (agar tidak perlu register terus saat testing)
    strcpy(dataUser[0].username, "admin");
    strcpy(dataUser[0].password, "admin");
    strcpy(dataUser[0].namaLengkap, "Administrator");
    dataUser[0].totalPoin = 100;
    dataUser[0].role = 1;
    strcpy(dataUser[1].username, "User");
    strcpy(dataUser[1].password, "User");
    strcpy(dataUser[1].namaLengkap, "User Biasa");
    dataUser[1].totalPoin = 300;
    dataUser[1].role = 1;
    jumlahUser = 2;

    int pilihan;
    do
    {
        system("cls"); // Bersihkan layar setiap kembali ke menu utama
        tengah(7,"=========================================\n");
        tengah(8,"CAMPUS STUDY ROUTE SYSTEM\n");
        tengah(9,"=========================================\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. About\n");
        printf("4. Exit\n");
        printf("Pilihan [1-4]: ");

        if (scanf("%d", &pilihan) != 1)
        {
            while (getchar() != '\n');
            pilihan = -1;
        }

        switch (pilihan)
        {
        case 1:
            if (loginProcess() == 1)
            {
                Home(); // Masuk ke dashboard jika login sukses
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