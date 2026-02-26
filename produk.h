#ifndef PRODUK_H
#define PRODUK_H

#include <string>
#include <vector>

struct Produk {
    int id;
    std::string nama;
    int stok;
    double harga;
};

extern "C" {
    #include "sqlite3.h"
}
#endif