#ifndef INVENTARIS_H
#define INVENTARIS_H

#include "sqlite3.h"
#include <string>

// Objek database global
extern sqlite3* db;

// Fungsi dasar database
bool inisialisasiDB();
void tutupDB();

// Fungsi CRUD (Sekarang menggunakan database)
void tambahBarang();
void tampilkanBarang();
void editBarang();
void hapusBarang();
void cariBarang();
void menuKasir();
void menuAdmin();
void lihatLaporan();
void jualBarang();
int inputAngka(std::string pesan);
void bersihkanLayar();
void tampilkanHeader(std::string judul);



#endif