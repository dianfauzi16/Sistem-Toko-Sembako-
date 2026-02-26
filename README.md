# 🛒 Sistem Toko Sembako Jaya (C++ & SQLite)

Sistem manajemen inventaris dan kasir berbasis terminal (CLI) yang dirancang untuk membantu operasional toko kelontong dengan penyimpanan data permanen.

## 🚀 Fitur Utama
- **Akses Berbasis Peran**: Menu berbeda untuk Kasir (transaksi) dan Admin (manajemen stok).
- **Keamanan**: Menu Admin dilindungi oleh sistem login password.
- **Database Terintegrasi**: Menggunakan SQLite untuk menyimpan data barang dan riwayat transaksi secara permanen.
- **Laporan Real-time**: Pencatatan riwayat transaksi otomatis dengan konversi waktu lokal (WIB).
- **Validasi Input**: Menangani kesalahan input stok atau ID agar database tetap konsisten.

## 🛠️ Teknologi yang Digunakan
- **Bahasa**: C++
- **Database**: SQLite3
- **Compiler**: MinGW (G++ & GCC)

## 📦 Struktur Proyek
- `main.cpp`: Alur utama program dan logika keamanan login.
- `inventaris.cpp` & `.h`: Logika bisnis (CRUD, transaksi, laporan).
- `produk.h`: Definisi struktur data produk.
- `sqlite3.c` & `.h`: Library database SQLite.

## ⚙️ Cara Instalasi & Menjalankan
1. Clone repository ini:
   ```bash
   git clone [https://github.com/dianfauzi16/Sistem-Toko-Sembako-.git](https://github.com/dianfauzi16/Sistem-Toko-Sembako-.git)
2. Kompilasi library SQLite:
   ```bash
   gcc -c sqlite3.c -o sqlite3.o
3. Kompilasi kode utama:
   ```Bash
   g++ main.cpp inventaris.cpp sqlite3.o -o program.
4. Jalankan aplikasi:
   ```Bash
   ./program

👤 Pengembang
Nama: M Dian Fauzi
Instansi: Amikom University Yogyakarta
Jurusan: Informatics
