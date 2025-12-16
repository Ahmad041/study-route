# Campus Study Route System

Aplikasi manajemen studi mahasiswa berbasis konsol dengan fitur gamifikasi untuk meningkatkan motivasi belajar. Dibangun menggunakan bahasa C dengan fokus pada UI interaktif dan pengalaman pengguna yang menarik.

## Deskripsi Proyek

Campus Study Route System adalah aplikasi sederhana yang dirancang untuk membantu mahasiswa mengelola jadwal kuliah, melakukan absensi, booking konsultasi dengan pengajar, dan menukar poin dengan item di shop. Aplikasi ini mengintegrasikan elemen gamifikasi seperti poin, leaderboard, dan reward untuk membuat proses belajar lebih menyenangkan.

Proyek ini dikembangkan sebagai bagian dari pembelajaran programming C dan konsep struktur data. Menggunakan library standar C dan Windows API untuk manipulasi konsol.

## Fitur Utama

### 🔐 Autentikasi
- **Login**: Masuk dengan username dan password
- **Register**: Daftar akun baru sebagai siswa
- **Session Management**: Sistem login session dengan penyimpanan user aktif

### 📚 Manajemen Studi
- **Absensi Kelas**: Absen mata kuliah dan dapatkan poin (+10 poin per absen)
- **Jadwal Kuliah**: Lihat jadwal reguler mata kuliah
- **Reservasi Konsultasi**: Booking jadwal konsultasi dengan pengajar (biaya 20 poin)

### 🎮 Gamifikasi
- **Sistem Poin**: Kumpulkan poin dari absensi dan aktivitas lainnya
- **Leaderboard**: Peringkat siswa berdasarkan total poin (descending)
- **Point Shop**: Tukar poin dengan item seperti voucher, notebook, kaos, dll.

### 👨‍🏫 Data Pengajar
- Lihat daftar pengajar dengan spesialisasi, rating, dan status ketersediaan
- Booking konsultasi real-time dengan update status

### 🎨 UI/UX Konsol
- Interface interaktif dengan border, warna, dan efek shadow
- Menu navigasi dengan arrow keys
- Loading animation dan message box
- Fullscreen mode untuk pengalaman optimal

## Persyaratan Sistem

- **OS**: Windows (karena menggunakan Windows API)
- **Compiler**: GCC/MinGW atau Visual Studio
- **Library**: Standar C (stdio.h, conio.h, windows.h, dll.)
- **RAM**: Minimal 512MB
- **Storage**: Minimal 10MB untuk executable dan file pendukung

## Instalasi dan Setup

1. **Clone Repository**:
   ```bash
   git clone https://github.com/Ahmad041/study-route.git
   cd study-route
   ```

2. **Compile Program**:
   - Menggunakan GCC:
     ```bash
     gcc main.c -o study-route.exe
     ```
   - Atau buka di Visual Studio Code dan compile dengan ekstensi C/C++.

3. **Jalankan Program**:
   ```bash
   ./study-route.exe
   ```

## Cara Penggunaan

### Menu Utama
1. **Login**: Pilih opsi 1, masukkan username dan password (default: admin/admin atau User/User)
2. **Register**: Pilih opsi 2 untuk membuat akun baru
3. **About**: Lihat informasi aplikasi
4. **Exit**: Keluar aplikasi

### Setelah Login (Dashboard)
1. **Absen Kelas**: Pilih mata kuliah, dapatkan poin
2. **Data Pengajar**: Lihat dan booking pengajar
3. **Leaderboard**: Lihat peringkat siswa
4. **Study Route**: Lihat jadwal konsultasi dan kuliah
5. **Point Shop**: Belanja item dengan poin
6. **Logout**: Kembali ke menu utama

### Navigasi
- Gunakan angka untuk memilih menu
- Tekan Enter untuk konfirmasi
- Gunakan arrow keys di beberapa menu interaktif
- Tekan tombol apa saja untuk lanjut di prompt

## Struktur Kode

- **main.c**: File utama dengan logika aplikasi, struct data, dan fungsi fitur
- **lib.h**: Header library dengan utility functions untuk UI konsol
- **thanks.txt**: File teks untuk pesan exit (opsional)

## Struktur Data

```c
// User: username, password, email, nama, poin, role
// Pengajar: id, nama, spesialisasi, rating, status
// MataKuliah: id, nama, hari
// ItemShop: id, nama, harga, stok
```

## Data Dummy

Aplikasi sudah dilengkapi data dummy untuk testing:
- 2 user default (admin, User)
- 5 mata kuliah
- 5 pengajar
- 5 item shop

## Hal Penting Lainnya

### Keamanan
- Password masking dengan asterisk (*)
- Input validation dasar
- Session-based access

### Keterbatasan
- Data tidak persistent (hilang saat restart)
- Maksimal 100 user, 5 pengajar/matkul/item
- Hanya mendukung Windows
- Tidak ada backup/restore data

### Pengembangan Selanjutnya
- [ ] Integrasi database (SQLite/MySQL)
- [ ] Multi-platform support (Linux/Mac)
- [ ] Export data ke file (CSV/JSON)
- [ ] Admin panel untuk manage data
- [ ] Notifikasi reminder jadwal
- [ ] Unit testing dengan framework C

### Troubleshooting
- **Program tidak fullscreen**: Pastikan console legacy mode atau jalankan sebagai administrator
- **Input tidak responsive**: Restart aplikasi atau clear console buffer
- **Compile error**: Pastikan semua header file tersedia dan compiler support Windows API

## Lisensi

Distributed under the MIT License. See `LICENSE` for more information.

## Kontak

- Developer: Informatics Management Student
- Email: ahamdferdyalamsyah@gmail.com
- Project Link: [https://github.com/Ahmad041/study-route](https://github.com/Ahmad041/study-route)

---

**Versi**: 1.0 (Dev Build)  
**Bahasa**: C (Console Application)  
**Tanggal Release**: Desember 2025
