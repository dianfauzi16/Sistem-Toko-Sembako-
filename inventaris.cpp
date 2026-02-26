#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include "produk.h" 
#include "inventaris.h" 
#include "sqlite3.h" 

using namespace std;

sqlite3* db; 

bool inisialisasiDB() {
    int result = sqlite3_open("toko.db", &db);
    if (result != SQLITE_OK) {
        cerr << "Gagal membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    char* errMsg = 0;

    // 1. Buat Tabel BARANG
    string sqlBarang = "CREATE TABLE IF NOT EXISTS BARANG("
                       "ID INT PRIMARY KEY NOT NULL, "
                       "NAMA TEXT NOT NULL, "
                       "STOK INT NOT NULL, "
                       "HARGA REAL NOT NULL);";

    if (sqlite3_exec(db, sqlBarang.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cerr << "Gagal membuat tabel BARANG: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    // 2. Buat Tabel LAPORAN (INI YANG TADI TERLEWAT)
    string sqlLaporan = "CREATE TABLE IF NOT EXISTS LAPORAN("
                        "ID_TRANS INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "NAMA_BARANG TEXT, "
                        "JUMLAH INT, "
                        "TOTAL_HARGA REAL, "
                        "WAKTU DATETIME DEFAULT CURRENT_TIMESTAMP);";

    if (sqlite3_exec(db, sqlLaporan.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cerr << "Gagal membuat tabel LAPORAN: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

void tutupDB() {
sqlite3_close(db);
}


// 2. INPUT VALIDASI ANGKA
int inputAngka(string pesan) {
    int angka;
    while (true) {
        cout << pesan;
        if (!(cin >> angka)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\033[31mInput tidak valid! Harap masukkan angka.\033[0m\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return angka;
        }
    }
}

// 3. TAMBAH BARANG
void tambahBarang() {
    int id = inputAngka("Masukkan ID Barang (Angka): ");
    string nama;
    cout << "Masukkan Nama Barang: ";
    getline(cin >> ws, nama);
    int stok = inputAngka("Masukkan Jumlah Stok: ");
    
    double harga;
    cout << "Masukkan Harga: ";
    while(!(cin >> harga)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[31mHarga harus angka! Masukkan ulang: \033[0m";
    }

    string sql = "INSERT INTO BARANG (ID, NAMA, STOK, HARGA) VALUES (" +
                 to_string(id) + ", '" + nama + "', " + 
                 to_string(stok) + ", " + to_string(harga) + ");";

    char* errMsg = 0;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cout << "\033[31mSQL Error: " << errMsg << "\033[0m" << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "\033[32mBarang berhasil disimpan ke database!\033[0m" << endl;
    }
}

// 4. TAMPILKAN BARANG
void tampilkanBarang() {
    const char* sql = "SELECT * FROM BARANG;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        cout << "Gagal mengambil data: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "\n--- DAFTAR INVENTARIS (DATABASE) ---" << endl;
    cout << left << setw(5) << "ID" << setw(20) << "Nama" << setw(10) << "Stok" << "Harga" << endl;
    cout << "----------------------------------------------------" << endl;
    
    bool dataAda = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        dataAda = true;
        cout << left << setw(5) << sqlite3_column_int(stmt, 0)
             << setw(20) << sqlite3_column_text(stmt, 1)
             << setw(10) << sqlite3_column_int(stmt, 2)
             << "Rp" << fixed << setprecision(0) << sqlite3_column_double(stmt, 3) << endl;
    }
    
    if (!dataAda) cout << "Belum ada data barang." << endl;
    sqlite3_finalize(stmt);
    cout << "----------------------------------------------------" << endl;
}

// 5. EDIT BARANG
void editBarang() {
    int id = inputAngka("Masukkan ID Barang yang ingin diubah: ");

    // 1. VALIDASI: Cek apakah ID ada di database dan tampilkan datanya
    string queryCek = "SELECT * FROM BARANG WHERE ID = " + to_string(id) + ";";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, queryCek.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cout << "Gagal akses database!" << endl;
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Jika ID ditemukan, tampilkan data lama sebagai referensi
        cout << "\n\033[33m--- Data Barang Ditemukan ---\033[0m" << endl;
        cout << "ID    : " << sqlite3_column_int(stmt, 0) << endl;
        cout << "Nama  : " << sqlite3_column_text(stmt, 1) << endl;
        cout << "Stok  : " << sqlite3_column_int(stmt, 2) << endl;
        cout << "Harga : Rp" << fixed << setprecision(0) << sqlite3_column_double(stmt, 3) << endl;
        sqlite3_finalize(stmt); // Selesai cek, tutup statement pertama
    } else {
        // Jika ID tidak ada
        cout << "\033[31mError: Barang dengan ID " << id << " tidak ditemukan!\033[0m" << endl;
        sqlite3_finalize(stmt);
        return; // Keluar dari fungsi
    }

    // 2. PROSES INPUT DATA BARU
    cout << "\n--- Masukkan Data Baru ---" << endl;
    string namaBaru;
    cout << "Nama Baru: ";
    getline(cin >> ws, namaBaru);
    int stokBaru = inputAngka("Stok Baru: ");
    double hargaBaru = (double)inputAngka("Harga Baru: ");

    // 3. EKSEKUSI UPDATE KE DATABASE
    string sqlUpdate = "UPDATE BARANG SET NAMA='" + namaBaru + "', STOK=" + to_string(stokBaru) + 
                       ", HARGA=" + to_string(hargaBaru) + " WHERE ID=" + to_string(id) + ";";

    char* errMsg = 0;
    if (sqlite3_exec(db, sqlUpdate.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cout << "\033[31mGagal Update: " << errMsg << "\033[0m" << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "\033[32mBerhasil! Data barang telah diperbarui di database.\033[0m" << endl;
    }
}

// 6. HAPUS BARANG
void hapusBarang() {
    int id = inputAngka("Masukkan ID Barang yang akan dihapus: ");
    
    // Konfirmasi sederhana
    char yakin;
    cout << "Yakin hapus ID " << id << "? (y/n): ";
    cin >> yakin;
    if (yakin != 'y' && yakin != 'Y') return;

    string sql = "DELETE FROM BARANG WHERE ID=" + to_string(id) + ";";

    if (sqlite3_exec(db, sql.c_str(), 0, 0, 0) == SQLITE_OK) {
        if (sqlite3_changes(db) > 0) cout << "\033[32mBarang berhasil dihapus!\033[0m" << endl;
        else cout << "\033[31mID tidak ditemukan.\033[0m" << endl;
    }
}

// 7. CARI BARANG
void cariBarang() {
    string kunci;
    cout << "Masukkan Nama/Kata Kunci Barang: ";
    getline(cin >> ws, kunci);

    string sql = "SELECT NAMA, HARGA, STOK FROM BARANG WHERE NAMA LIKE '%" + kunci + "%';";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        cout << "\n--- INFORMASI HARGA & STOK ---" << endl;
        cout << left << setw(20) << "Nama Barang" << setw(15) << "Harga" << "Stok" << endl;
        cout << "------------------------------------------" << endl;
        
        bool ditemukan = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            ditemukan = true;
            cout << left << setw(20) << sqlite3_column_text(stmt, 0)
                 << "Rp" << setw(13) << fixed << setprecision(0) << sqlite3_column_double(stmt, 1)
                 << sqlite3_column_int(stmt, 2) << " unit" << endl;
        }
        if (!ditemukan) cout << "Barang '" << kunci << "' tidak ditemukan." << endl;
        sqlite3_finalize(stmt);
    }
}

void jualBarang() {
    int id = inputAngka("Masukkan ID Barang yang dibeli: ");
    
    // 1. Ambil data barang dari database
    string query = "SELECT NAMA, STOK, HARGA FROM BARANG WHERE ID = " + to_string(id) + ";";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cout << "Gagal akses database!" << endl;
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        string nama = (const char*)sqlite3_column_text(stmt, 0);
        int stokSekarang = sqlite3_column_int(stmt, 1);
        double harga = sqlite3_column_double(stmt, 2);

        cout << "\033[32mBarang: " << nama << " | Harga: Rp" << fixed << setprecision(0) << harga << "\033[0m" << endl;
        cout << "Stok tersedia: " << stokSekarang << endl;

        int jumlahBeli = inputAngka("Jumlah yang dibeli: ");

        // 2. Validasi Stok
        if (jumlahBeli > stokSekarang) {
            cout << "\033[31mMaaf, stok tidak mencukupi!\033[0m" << endl;
        } else if (jumlahBeli <= 0) {
            cout << "\033[31mJumlah tidak valid!\033[0m" << endl;
        } else {
            double total = harga * jumlahBeli;
            int stokBaru = stokSekarang - jumlahBeli;

            // --- PROSES DATABASE DIMULAI ---
            char* errMsg = 0;

            // A. UPDATE STOK BARANG
            string sqlUpdate = "UPDATE BARANG SET STOK = " + to_string(stokBaru) + " WHERE ID = " + to_string(id) + ";";
            int rc1 = sqlite3_exec(db, sqlUpdate.c_str(), 0, 0, &errMsg);

            // B. SIMPAN KE LAPORAN
            string sqlRecord = "INSERT INTO LAPORAN (NAMA_BARANG, JUMLAH, TOTAL_HARGA) VALUES ('" + 
                               nama + "', " + to_string(jumlahBeli) + ", " + to_string(total) + ");";
            int rc2 = sqlite3_exec(db, sqlRecord.c_str(), 0, 0, &errMsg);

            // C. CEK APAKAH KEDUANYA BERHASIL
            if (rc1 == SQLITE_OK && rc2 == SQLITE_OK) {
                cout << "\n\033[32m[ BERHASIL ] Transaksi tercatat dan stok diperbarui!\033[0m" << endl;
                
                // Tampilkan Struk
                cout << "\n===== NOTA TOKO SEMBAKO =====" << endl;
                cout << "Barang : " << nama << endl;
                cout << "Qty    : " << jumlahBeli << endl;
                cout << "Total  : Rp" << fixed << setprecision(0) << total << endl;
                cout << "=============================" << endl;
            } else {
                cout << "\033[31mGagal memproses transaksi: " << (errMsg ? errMsg : "Error Tak Dikenal") << "\033[0m" << endl;
                if(errMsg) sqlite3_free(errMsg);
            }
        }
    } else {
        cout << "\033[31mBarang tidak ditemukan!\033[0m" << endl;
    }
    sqlite3_finalize(stmt);
}

void lihatLaporan() {
    // Ubah SELECT  menjadi seperti ini:
    const char* sql = "SELECT ID_TRANS, NAMA_BARANG, JUMLAH, TOTAL_HARGA, datetime(WAKTU, 'localtime') FROM LAPORAN;";
    sqlite3_stmt* stmt;
    double grandTotal = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return;

    cout << "\n--- LAPORAN PENJUALAN HARIAN ---" << endl;
    cout << left << setw(5) << "ID" << setw(15) << "Barang" << setw(8) << "Qty" << setw(15) << "Subtotal" << "Waktu" << endl;
    cout << "------------------------------------------------------------" << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        double subtotal = sqlite3_column_double(stmt, 3);
        grandTotal += subtotal;

        cout << left << setw(5) << sqlite3_column_int(stmt, 0)
             << setw(15) << sqlite3_column_text(stmt, 1)
             << setw(8) << sqlite3_column_int(stmt, 2)
             << "Rp" << setw(13) << fixed << setprecision(0) << subtotal
             << sqlite3_column_text(stmt, 4) << endl;
    }
    
    cout << "------------------------------------------------------------" << endl;
    cout << "TOTAL PENDAPATAN: Rp" << fixed << setprecision(0) << grandTotal << endl;
    sqlite3_finalize(stmt);
}

void tampilkanHeader(string judul) {
    cout << "\033[36m==========================================\033[0m" << endl;
    cout << "\033[1;33m          " << judul << "          \033[0m" << endl;
    cout << "\033[36m==========================================\033[0m" << endl;
}

void bersihkanLayar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void menuKasir() {
    int pil;
    do {
        cout << "\n--- [ HALLO KASIR ] ---" << endl;
        cout << "1. Transaksi Jual\n2. Cari Detail Barang\n3. Daftar Harga & Stok\n4. Kembali" << endl;
        pil = inputAngka("Pilih: ");

        switch(pil) {
            case 1: jualBarang(); break;
            case 2: cariBarang(); break;
            case 3: tampilkanBarang(); break; // Kasir butuh lihat stok
        }
    } while (pil != 4);
}

void menuAdmin() {
    int pil;
    while (true) { // Gunakan while true agar lebih terkontrol
        cout << "\n--- [ HALLO ADMIN ] ---" << endl;
        cout << "1. Tambah Barang Baru" << endl;
        cout << "2. Update Stok/Harga" << endl;
        cout << "3. Hapus Barang" << endl;
        cout << "4. Lihat Laporan Stok" << endl;
        cout << "5. Riwayat Transaksi" << endl;
        cout << "6. Kembali" << endl;
        
        pil = inputAngka("Pilih: ");

        if (pil == 6) break; // Keluar dari loop jika pilih 6

        switch(pil) {
            case 1: tambahBarang(); break;
            case 2: editBarang(); break;
            case 3: hapusBarang(); break;
            case 4: tampilkanBarang(); break;
            case 5: lihatLaporan(); break;
            default: 
                cout << "Pilihan tidak tersedia!" << endl;
                break;
        }
    }
}