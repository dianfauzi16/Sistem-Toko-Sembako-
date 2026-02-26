#include <iostream>
#include "produk.h"
#include "inventaris.h"

using namespace std;

int main() {
    if (!inisialisasiDB()) {
        cerr << "Gagal menginisialisasi database. Program akan keluar." << endl;
        return 1; // Keluar dengan kode error
    }
    int peran;
    do {
        bersihkanLayar();
        cout << "\033[1;32m   _____ ______  __  ______   ___    __  __ _____   \033[0m" << endl;
        cout << "\033[1;32m  / ___// ____/ /  |/  / __ )/   |  / / / / __  /   \033[0m" << endl;
        cout << "\033[1;32m  \\__ \\/ __/   / /|_/ / __  // | / /_/ / / / /    \033[0m" << endl;
        cout << "\033[1;32m ___/ / /___  / /  / / /_/ / ___ |/ __  / /_/ /     \033[0m" << endl;
        cout << "\033[1;32m/____/_____/ /_/  /_/_____/_/  |_/_/ /_/_____/      \033[0m" << endl;
        
        tampilkanHeader("SISTEM TOKO SEMBAKO");
        cout << " [1] MASUK SEBAGAI KASIR" << endl;
        cout << " [2] MASUK SEBAGAI ADMIN" << endl;
        cout << " [3] KELUAR" << endl;
        peran = inputAngka("\nPilih Akses: ");

        if (peran == 1) {
            menuKasir(); // Antarmuka Kasir
        } else if (peran == 2) {
            string password;
            cout << "\n[ KEAMANAN ADMIN ]" << endl;
            cout << "Masukkan Password: ";
            
            // Sembunyikan input atau biarkan terlihat sederhana dulu
            cin >> password;

            if (password == "admin123") { // Kamu bisa ganti password-nya di sini
                cout << "\033[32mLogin Berhasil! Selamat Datang, Admin.\033[0m" << endl;
                menuAdmin();
            } else {
                cout << "\033[31mAkses Ditolak! Password Salah.\033[0m" << endl;
            }
                }
            } while (peran != 3);
    tutupDB();
    return 0;
}