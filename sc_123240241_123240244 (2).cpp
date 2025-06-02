#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

// Struct/Record
// Struct untuk menyimpan data transaksi
struct Transaksi {
    int jumlah;
    string jenis;
    string waktu;
    string kategori;
    string username;

    Transaksi() {}
    Transaksi(int jumlah, string jenis, string waktu, string kategori, string username)
        : jumlah(jumlah), jenis(jenis), waktu(waktu), kategori(kategori), username(username) {}
};

// Struct untuk menyimpan data pengguna (pengguna dan admin)
struct Pengguna {
    string username;
    string password;
    double saldo;

    Pengguna() {}
    Pengguna(string uname, string pass, double s = 0.0) {
        username = uname;
        password = pass;
        saldo = s;
    }

    void CekSaldo() {
        cout << "Saldo anda Rp " << fixed << setprecision(0) << saldo << endl;
    }
};

// ARRAY 1 DIMENSI & MULTI DIMENSI
// Array 1 dimensi untuk menyimpan data pengguna dan transaksi
Pengguna pengguna[100];
int jumlahPengguna = 0;
Transaksi daftarTransaksi[1000];
int jumlahTransaksi = 0;

// Array 2 dimensi untuk rekap transaksi per hari dan jenis transaksi
int rekapTransaksi[7][3] = {};

// ======================[POINTER]======================
// Pointer untuk menunjuk pengguna yang sedang login
Pengguna* loginPengguna = nullptr;

int hariSimulasi = 0;
int batasPengeluaranBulanan = 1000000; // Batas pengeluaran bulanan, contoh: 1 juta

// Deklarasi fungsi
void tampilkanHeader();
void tampilkanMenuLainnya();
void tarikSaldo();
void isiSaldo();
void menuLainnya();
void tampilkanMenuPengguna();
void tampilkanMenuAdmin();
void tampilkanRekap();
void tampilkanTransaksi();
void bubbleSortTransaksi(int mode);
void tambahKeRekap(string jenis);
string gunakanWaktuSekarang();
void cls();
void simpanKeFile();
void muatDariFile();
void hapusAkun();
void hapusTransaksi();
void tampilkanRekapKategori();
void cetakLaporanPengeluaran();
void tampilkanSaldoUser();

// Fungsi mengambil waktu sekarang sebagai string
string gunakanWaktuSekarang() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string waktu(dt);
    waktu.pop_back();
    return waktu;
}

// Fungsi menampilkan header "Monitoring Pengeluaran"
void tampilkanHeader() {
    cout << "|================================================================|\n";
    cout << "|                                                                |\n";
    cout << "|                     Monitoring Pengeluaran                     |\n";
    cout << "|                                                                |\n";
    cout << "|================================================================|\n";
}

// Tarik saldo
void tarikSaldo() {
    int nominal;
    cout << "Masukan nominal tarik saldo : ";
    while (!(cin >> nominal) || nominal <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Input tidak valid, harus angka positif. Masukkan lagi: ";
    }
    if (nominal > loginPengguna->saldo) {
        cout << "Saldo tidak cukup.\n";
        return;
    }
    loginPengguna->saldo -= nominal;
    cout << "Tarik saldo berhasil. Saldo sekarang : Rp " << fixed << setprecision(0) << loginPengguna->saldo << "\n";
    string kategori;
    cout << "Masukkan kategori pengeluaran (Makan/Transportasi/Hiburan/dll): ";
    cin >> kategori;
    while (kategori.empty()) {
        cout << "Kategori tidak boleh kosong. Masukkan lagi: ";
        cin >> kategori;
    }
    daftarTransaksi[jumlahTransaksi++] = {nominal, "Tarik", gunakanWaktuSekarang(), kategori, loginPengguna->username};
    tambahKeRekap("Tarik");
    tampilkanTransaksi(); 
}

// Isi saldo pengguna
void isiSaldo() {
    int nominal;
    cout << "Masukan nominal isi saldo : ";
    while (!(cin >> nominal) || nominal <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Input tidak valid, harus angka positif. Masukkan lagi: ";
    }
    loginPengguna->saldo += nominal;
    cout << "Isi saldo berhasil. Saldo anda sekarang : Rp " << fixed << setprecision(0) << loginPengguna->saldo << "\n";
    daftarTransaksi[jumlahTransaksi++] = {nominal, "Isi", gunakanWaktuSekarang(), "", loginPengguna->username};
    tambahKeRekap("Isi");
    tampilkanTransaksi(); 
}

// Menu lainnya/tambahan
void menuLainnya() {
    int pilihan;
    bool loop = true;

    while (loop) {
        tampilkanMenuLainnya();
        cout << "Pilih menu lainnya (1-7): ";
        while (!(cin >> pilihan) || pilihan < 1 || pilihan > 7) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Input tidak valid. Masukkan angka 1-7: ";
        }

        switch (pilihan) {
            case 1:
                tampilkanTransaksi();
                break;
            case 2: {
                int modeSort;
                cout << "Sortir Transaksi:\n";
                cout << "1. Berdasarkan Nominal Transaksi (Desc)\n";
                cout << "2. Berdasarkan Jenis Transaksi (Asc)\n";
                cout << "Pilih: ";
                cin >> modeSort;

                if (cin.fail() || modeSort <= 0) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Input tidak valid. Pilih 1 atau 2.\n";
                } else {
                    bubbleSortTransaksi(modeSort);
                    tampilkanTransaksi();
                }
                break;
            }
            case 3: {
                string pwLama, pwBaru;
                cin.ignore();
                cout << "Masukan password lama: ";
                getline(cin, pwLama);

                if (pwLama == loginPengguna->password) {
                    cout << "Masukan password baru: ";
                    getline(cin, pwBaru);
                    loginPengguna->password = pwBaru;
                    cout << "Password berhasil diubah.\n";
                    for (auto& u : pengguna) {
                        if (u.username == loginPengguna->username) {
                            u.password = pwBaru;
                            break;
                        }
                    }
                } else {
                    cout << "Password lama salah.\n";
                }
                break;
            }
            case 4:
                cout << "Masukkan batas pengeluaran bulanan (Rp): ";
                cin >> batasPengeluaranBulanan;
                cout << "Batas berhasil diatur!\n";
                cout << "\n";
                break;
            case 5:
                cetakLaporanPengeluaran();
                break;
            case 6:
                loop = false;
                break;
            case 7:
                cout << "Terima kasih telah menggunakan program ini.\n";
                exit(0);
            default:
                cout << "Pilihan tidak valid.\n";
        }
    }
}

// Tampilan menu pengguna
void tampilkanMenuPengguna() {
    int pilihan;
    do {
        cout << "\n====================================================================\n";
        cout << "|                        MENU PENGGUNA                            |\n";
        cout << "====================================================================\n";
        cout << "| Username : " << setw(15) << left << loginPengguna->username
             << " | Saldo : Rp " << setw(12) << right << fixed << setprecision(0) << loginPengguna->saldo << " |\n";
        cout << "--------------------------------------------------------------------\n";
        cout << "| 1 | Cek Saldo                                                    |\n";
        cout << "| 2 | Isi Saldo                                                    |\n";
        cout << "| 3 | Tarik Saldo                                                  |\n";
        cout << "| 4 | Menu Lainnya                                                 |\n";
        cout << "| 5 | Logout                                                       |\n";
        cout << "| 6 | Keluar                                                       |\n";
        cout << "====================================================================\n";
        cout << "Pilihan : ";
        while (!(cin >> pilihan) || pilihan < 1 || pilihan > 6) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Input tidak valid. Masukkan angka 1-6: ";
        }

        switch (pilihan) {
            case 1 :
                loginPengguna->CekSaldo();
                daftarTransaksi[jumlahTransaksi++] = {0, "Cek saldo", gunakanWaktuSekarang(), "", loginPengguna->username};
                tambahKeRekap("Cek saldo");
                break;
            case 2 :
                isiSaldo();
                break;
            case 3 :
                tarikSaldo();
                break;
            case 4 :
                menuLainnya();
                break;
            case 5 :
                loginPengguna = nullptr;
                cout << "Anda telah logout.\n";
                return;
            case 6 :
                cout << "Terima kasih telah menggunakan program ini.\n";
                exit(0);
            default :
                cout << "Pilihan tidak valid.\n";
        }

        // Update saldo pengguna
        for (auto& p : pengguna) {
            if (p.username == loginPengguna->username) {
                p = *loginPengguna;
                break;
            }
        }

        // Sinkronisasi saldo loginPengguna ke array pengguna
        for (int i = 0; i < jumlahPengguna; ++i) {
            if (pengguna[i].username == loginPengguna->username) {
                pengguna[i].saldo = loginPengguna->saldo;
                break;
            }
        }

        // Keterangan Pengeluaran Bulan Ini
        float totalPengeluaranBulanIni = 0;
        string bulanSekarang = gunakanWaktuSekarang().substr(4, 3);

        for (int i = 0; i < jumlahTransaksi; ++i) {
            if (daftarTransaksi[i].jenis == "Tarik" &&
                daftarTransaksi[i].waktu.substr(4, 3) == bulanSekarang &&
                daftarTransaksi[i].username == loginPengguna->username) {
                totalPengeluaranBulanIni += daftarTransaksi[i].jumlah;
            }
        }

        cout << "--------------------------------------------------------------------\n";
        cout << "Status Pengeluaran Bulan Ini: ";
        if (totalPengeluaranBulanIni > batasPengeluaranBulanan) {
            cout << "!! Boros, pengeluaran melebihi batas !!\n";
        } else {
            cout << "Aman, pengeluaran masih dalam batas.\n";
        }
        cout << "====================================================================\n";

    } while (pilihan != 6);
}

// Menu admin
void tampilkanMenuAdmin() {
    cout << "\n|================================================================|\n";
    cout << "|                           Mode Admin                           |\n";
    cout << "|================================================================|\n";
    cout << "| 1 | Lihat Transaksi (Rekursif)                                 |\n";
    cout << "| 2 | Lihat Saldo User                                           |\n";
    cout << "| 3 | Reset Password User                                        |\n";
    cout << "| 4 | Lihat Rekap Transaksi                                      |\n";
    cout << "| 5 | Hapus Akun                                                 |\n";
    cout << "| 6 | Hapus Transaksi                                            |\n";
    cout << "| 7 | Logout                                                     |\n";
    cout << "| 8 | Keluar                                                     |\n";
    cout << "|================================================================|\n\n";
}

// Fungsi untuk menambahkan transaksi ke rekap
void tambahKeRekap(string jenis) {
    if (jenis == "Cek saldo") rekapTransaksi[hariSimulasi][0]++;
    else if (jenis == "Tarik") rekapTransaksi[hariSimulasi][1]++;
    else if (jenis == "Isi") rekapTransaksi[hariSimulasi][2]++;
}

// Fungsi untuk menampilkan saldo seluruh user (mode admin)
void tampilkanSaldoUser() {
    const int wNo = 4, wUser = 16, wSaldo = 15;
    const int total = wNo + wUser + wSaldo + 4*3 + 1;
    cout << "\n" << string(total, '=') << endl;
    cout << "| " << setw(wNo-2) << left << "No"
         << " | " << setw(wUser) << left << "Username"
         << " | " << setw(wSaldo) << right << "Saldo (Rp)"
         << " |" << endl;
    cout << string(total, '=') << endl;
    for (int i = 0; i < jumlahPengguna; ++i) {
        cout << "| " << setw(wNo-2) << right << i+1
             << " | " << setw(wUser) << left << pengguna[i].username
             << " | " << setw(wSaldo) << right << fixed << setprecision(0) << pengguna[i].saldo
             << " |" << endl;
    }
    cout << string(total, '=') << endl;
}

// Fungsi untuk menampilkan rekap transaksi per hari (mode admin)
void tampilkanRekap() {
    const int wHari = 10, wCek = 8, wTarik = 8, wIsi = 8;
    const int total = wHari + wCek + wTarik + wIsi + 5*3 + 1;
    cout << "\n" << string(total, '=') << endl;
    cout << "| " << setw(wHari) << left << "Hari"
         << " | " << setw(wCek) << right << "Cek"
         << " | " << setw(wTarik) << right << "Tarik"
         << " | " << setw(wIsi) << right << "Isi"
         << " |" << endl;
    cout << string(total, '=') << endl;
    const string hari[] = {"Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu", "Minggu"};
    for (int i = 0; i < 7; i++) {
        cout << "| " << setw(wHari) << left << hari[i]
             << " | " << setw(wCek) << right << rekapTransaksi[i][0]
             << " | " << setw(wTarik) << right << rekapTransaksi[i][1]
             << " | " << setw(wIsi) << right << rekapTransaksi[i][2]
             << " |" << endl;
    }
    cout << string(total, '=') << endl;
}

// Fungsi untuk menampilkan transaksi (mode user)
void tampilkanTransaksi() {
    const int wNo = 4, wJenis = 12, wJumlah = 15, wWaktu = 22, wKategori = 15, wUser = 12;
    const int total = wNo + wJenis + wJumlah + wWaktu + wKategori + wUser + 7*3 + 1;
    cout << string(total, '=') << endl;
    cout << "| " << setw(wNo-2) << left << "No"
         << " | " << setw(wJenis) << left << "Jenis"
         << " | " << setw(wJumlah) << right << "Jumlah (Rp)"
         << " | " << setw(wWaktu) << left << "Waktu"
         << " | " << setw(wKategori) << left << "Kategori"
         << " | " << setw(wUser) << left << "Username"
         << " |" << endl;
    cout << string(total, '=') << endl;
    for (int i = 0; i < jumlahTransaksi; ++i) {
        cout << "| " << setw(wNo-2) << right << i+1
             << " | " << setw(wJenis) << left << daftarTransaksi[i].jenis
             << " | " << setw(wJumlah) << right << fixed << setprecision(0) << daftarTransaksi[i].jumlah
             << " | " << setw(wWaktu) << left << daftarTransaksi[i].waktu.substr(0, wWaktu)
             << " | " << setw(wKategori) << left
             << (daftarTransaksi[i].jenis == "Tarik" ? daftarTransaksi[i].kategori : "-")
             << " | " << setw(wUser) << left << daftarTransaksi[i].username
             << " |" << endl;
    }
    cout << string(total, '=') << endl;
}

// ======================[FUNGSI REKURSIF]======================
// Fungsi rekursif untuk menampilkan seluruh transaksi
void tampilkanTransaksiRekursif(int idx = 0) {
    if (idx >= jumlahTransaksi) {
        cout << "====================================================================================\n";
        return;
    }
    if (idx == 0) {
        cout << "====================================================================================\n";
        cout << "| No | Jenis       | Jumlah (Rp)    | Waktu                  | Username      |\n";
        cout << "====================================================================================\n";
    }
    cout << "| " << setw(2) << idx + 1 << " | "
         << setw(11) << left << daftarTransaksi[idx].jenis << " | "
         << setw(13) << right << fixed << setprecision(0) << daftarTransaksi[idx].jumlah << " | "
         << setw(22) << left << daftarTransaksi[idx].waktu << " | "
         << setw(13) << left << daftarTransaksi[idx].username << "|\n";
    tampilkanTransaksiRekursif(idx + 1);
}

// ======================[SORTING]======================
// Fungsi untuk mengurutkan transaksi (bubble sort)
void bubbleSortTransaksi(int mode) {
    for (int i = 0; i < jumlahTransaksi - 1; i++) {
        for (int j = 0; j < jumlahTransaksi - i - 1; j++) {
            bool tukar = false;
            if (mode == 1 && daftarTransaksi[j].jumlah < daftarTransaksi[j + 1].jumlah)
                tukar = true;
            else if (mode == 2 && daftarTransaksi[j].jenis > daftarTransaksi[j + 1].jenis)
                tukar = true;

            if (tukar) {
                // Manual swap tanpa algorithm
                Transaksi temp = daftarTransaksi[j];
                daftarTransaksi[j] = daftarTransaksi[j + 1];
                daftarTransaksi[j + 1] = temp;
            }
        }
    }
}

// ======================[OPERASI FILE]======================
// Fungsi untuk menyimpan data user/admin dan transaksi ke file txt
void simpanKeFile() {
    std::ofstream dataPengguna("dataPengguna.txt");
    dataPengguna << "admin 123 0\n";
    for (int i = 0; i < jumlahPengguna; ++i) {
        dataPengguna << pengguna[i].username << " " << pengguna[i].password << " " << pengguna[i].saldo << "\n";
    }
    dataPengguna.close();

    std::ofstream fileTransaksi("dataTransaksi.txt");
    for (int i = 0; i < jumlahTransaksi; ++i) {
        fileTransaksi << daftarTransaksi[i].jenis << " "
                      << daftarTransaksi[i].jumlah << " "
                      << daftarTransaksi[i].waktu << " "
                      << daftarTransaksi[i].kategori << " "
                      << daftarTransaksi[i].username << "\n";
    }
    fileTransaksi.close();
}

// Fungsi untuk membaca data user dan transaksi dari file txt
void muatDariFile() {
    ifstream fin("dataPengguna.txt");
    jumlahPengguna = 0;
    string username, password;
    double saldo;
    while (fin >> username >> password >> saldo) {
        pengguna[jumlahPengguna].username = username;
        pengguna[jumlahPengguna].password = password;
        pengguna[jumlahPengguna].saldo = saldo;
        jumlahPengguna++;
    }
    fin.close();

    ifstream finTrans("dataTransaksi.txt");
    jumlahTransaksi = 0;
    while (finTrans >> daftarTransaksi[jumlahTransaksi].jenis
           >> daftarTransaksi[jumlahTransaksi].jumlah
           >> std::ws) {
        getline(finTrans, daftarTransaksi[jumlahTransaksi].waktu, ' ');
        getline(finTrans, daftarTransaksi[jumlahTransaksi].kategori, ' ');
        getline(finTrans, daftarTransaksi[jumlahTransaksi].username);
        jumlahTransaksi++;
    }
    finTrans.close();
}

// Fungsi membersihkan layar
void cls() {
    system("cls");
}

// Fungsi menu lainnya (dummy, sudah ada implementasi aslinya di atas, jadi ini bisa dikosongkan jika tidak dipakai)
void tampilkanMenuLainnya() {
    cout << "|================================================================|\n";
    cout << "|                        Menu Lainnya                            |\n";
    cout << "|================================================================|\n";
    cout << "| 1 | Lihat Transaksi                                            |\n";
    cout << "| 2 | Sortir Transaksi                                           |\n";
    cout << "| 3 | Ubah Password                                              |\n";
    cout << "| 4 | Atur Batas Pengeluaran Bulanan                             |\n";
    cout << "| 5 | Cetak Laporan Pengeluaran                                  |\n";
    cout << "| 6 | Kembali                                                    |\n";
    cout << "| 7 | Keluar                                                     |\n";
    cout << "|================================================================|\n";
}

// Fungsi cetak laporan pengeluaran (dummy)
void cetakLaporanPengeluaran() {
    cout << "\n================================================================================================================\n";
    cout << "|                                    Laporan Pengeluaran Bulan Ini                                              |\n";
    cout << "================================================================================================================\n";
    cout << "| No |        Tanggal         |     Kategori     |     Jumlah (Rp)     |\n";
    cout << "================================================================================================================\n";
    float totalPengeluaran = 0;
    string bulanSekarang = gunakanWaktuSekarang().substr(4, 3);

    // Rekap per kategori
    const int MAX_KATEGORI = 50;
    string kategoriArr[MAX_KATEGORI];
    int totalArr[MAX_KATEGORI] = {0};
    int kategoriCount = 0;

    int no = 1;
    for (int i = 0; i < jumlahTransaksi; ++i) {
        if (
            daftarTransaksi[i].jenis == "Tarik" &&
            daftarTransaksi[i].waktu.substr(4, 3) == bulanSekarang &&
            loginPengguna && daftarTransaksi[i].kategori != "" &&
            daftarTransaksi[i].username == loginPengguna->username
        ) {
            cout << "| " << setw(2) << no++
                 << " | " << setw(20) << left << daftarTransaksi[i].waktu
                 << " | " << setw(15) << left << daftarTransaksi[i].kategori
                 << " | " << setw(18) << right << fixed << setprecision(0) << daftarTransaksi[i].jumlah << " |\n";
            totalPengeluaran += daftarTransaksi[i].jumlah;

            // Rekap kategori
            string kategori = daftarTransaksi[i].kategori;
            bool found = false;
            for (int j = 0; j < kategoriCount; ++j) {
                if (kategoriArr[j] == kategori) {
                    totalArr[j] += daftarTransaksi[i].jumlah;
                    found = true;
                    break;
                }
            }
            if (!found && kategoriCount < MAX_KATEGORI) {
                kategoriArr[kategoriCount] = kategori;
                totalArr[kategoriCount] = daftarTransaksi[i].jumlah;
                kategoriCount++;
            }
        }
    }
    cout << "================================================================================================================\n";
    cout << "| Total Pengeluaran Bulan Ini: Rp " << fixed << setprecision(0) << totalPengeluaran << " |\n";
    cout << "================================================================================================================\n";

    cout << "| Rekap Pengeluaran per Kategori: |\n";
    if (kategoriCount == 0) {
        cout << "|   (Belum ada pengeluaran bulan ini) |\n";
    } else {
        for (int i = 0; i < kategoriCount; ++i) {
            cout << "|   - " << setw(15) << left << kategoriArr[i]
                 << ": Rp " << fixed << setprecision(0) << totalArr[i] << " |\n";
        }
    }
    cout << "================================================================================================================\n";

    if (totalPengeluaran > batasPengeluaranBulanan) {
        cout << "| PERINGATAN: Pengeluaran anda bulan ini melebihi batas yang ditentukan! |\n";
    } else {
        cout << "| Pengeluaran anda masih dalam batas aman bulan ini. |\n";
    }
    cout << "================================================================================================================\n";
}

// Fungsi hapus akun
void hapusAkun() {
    if (!loginPengguna) {
        cout << "Anda harus login untuk menghapus akun.\n";
        return;
    }
    char konfirmasi;
    cout << "Apakah Anda yakin ingin menghapus akun ini beserta seluruh transaksi? (y/n): ";
    cin >> konfirmasi;
    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        // Hapus transaksi user
        int i = 0;
        while (i < jumlahTransaksi) {
            if (daftarTransaksi[i].username == loginPengguna->username) {
                for (int j = i; j < jumlahTransaksi - 1; ++j) {
                    daftarTransaksi[j] = daftarTransaksi[j + 1];
                }
                jumlahTransaksi--;
            } else {
                i++;
            }
        }
        // Hapus akun dari array pengguna
        int idx = -1;
        for (int i = 0; i < jumlahPengguna; ++i) {
            if (pengguna[i].username == loginPengguna->username) {
                idx = i;
                break;
            }
        }
        if (idx != -1) {
            for (int i = idx; i < jumlahPengguna - 1; ++i) {
                pengguna[i] = pengguna[i + 1];
            }
            jumlahPengguna--;
        }
        loginPengguna = nullptr;
        cout << "Akun dan seluruh transaksi berhasil dihapus.\n";
        simpanKeFile();
    } else {
        cout << "Penghapusan akun dibatalkan.\n";
    }
}

void hapusTransaksi() {
    if (!loginPengguna) {
        cout << "Anda harus login untuk menghapus transaksi.\n";
        return;
    }
    int idxList[1000], count = 0;
    cout << "Daftar transaksi Anda:\n";
    cout << "No | Jenis | Jumlah | Waktu | Kategori\n";
    for (int i = 0; i < jumlahTransaksi; ++i) {
        if (daftarTransaksi[i].username == loginPengguna->username) {
            cout << count + 1 << ". " << daftarTransaksi[i].jenis << " | "
                 << daftarTransaksi[i].jumlah << " | "
                 << daftarTransaksi[i].waktu << " | "
                 << daftarTransaksi[i].kategori << endl;
            idxList[count++] = i;
        }
    }
    if (count == 0) {
        cout << "Tidak ada transaksi yang bisa dihapus.\n";
        return;
    }
    int pilih;
    cout << "Masukkan nomor transaksi yang ingin dihapus (0 untuk batal): ";
    cin >> pilih;
    if (pilih <= 0 || pilih > count) {
        cout << "Penghapusan dibatalkan.\n";
        return;
    }
    char konfirmasi;
    cout << "Apakah Anda yakin ingin menghapus transaksi ini? (y/n): ";
    cin >> konfirmasi;
    if (konfirmasi != 'y' && konfirmasi != 'Y') {
        cout << "Penghapusan dibatalkan.\n";
        return;
    }
    int idxHapus = idxList[pilih - 1];

    // Update saldo jika transaksi "Isi" atau "Tarik"
    if (daftarTransaksi[idxHapus].jenis == "Isi") {
        loginPengguna->saldo -= daftarTransaksi[idxHapus].jumlah;
    } else if (daftarTransaksi[idxHapus].jenis == "Tarik") {
        loginPengguna->saldo += daftarTransaksi[idxHapus].jumlah;
    }

    for (int i = idxHapus; i < jumlahTransaksi - 1; ++i) {
        daftarTransaksi[i] = daftarTransaksi[i + 1];
    }
    jumlahTransaksi--;
    cout << "Transaksi berhasil dihapus.\n";
    simpanKeFile();
}

// Tambahkan fungsi khusus untuk admin
void hapusTransaksiAdmin() {
    string uname;
    cout << "Masukkan username user yang transaksinya ingin dihapus: ";
    cin >> uname;

    // Cari user
    bool ditemukan = false;
    for (int i = 0; i < jumlahPengguna; ++i) {
        if (pengguna[i].username == uname) {
            ditemukan = true;
            break;
        }
    }
    if (!ditemukan) {
        cout << "User tidak ditemukan.\n";
        return;
    }

    int idxList[1000], count = 0;
    cout << "Daftar transaksi user " << uname << ":\n";
    cout << "No | Jenis | Jumlah | Waktu | Kategori\n";
    for (int i = 0; i < jumlahTransaksi; ++i) {
        if (daftarTransaksi[i].username == uname) {
            cout << count + 1 << ". " << daftarTransaksi[i].jenis << " | "
                 << daftarTransaksi[i].jumlah << " | "
                 << daftarTransaksi[i].waktu << " | "
                 << daftarTransaksi[i].kategori << endl;
            idxList[count++] = i;
        }
    }
    if (count == 0) {
        cout << "Tidak ada transaksi yang bisa dihapus.\n";
        return;
    }
    int pilih;
    cout << "Masukkan nomor transaksi yang ingin dihapus (0 untuk batal): ";
    cin >> pilih;
    if (pilih <= 0 || pilih > count) {
        cout << "Penghapusan dibatalkan.\n";
        return;
    }
    char konfirmasi;
    cout << "Apakah Anda yakin ingin menghapus transaksi ini? (y/n): ";
    cin >> konfirmasi;
    if (konfirmasi != 'y' && konfirmasi != 'Y') {
        cout << "Penghapusan dibatalkan.\n";
        return;
    }
    int idxHapus = idxList[pilih - 1];

    // Update saldo user jika transaksi "Isi" atau "Tarik"
    for (int i = 0; i < jumlahPengguna; ++i) {
        if (pengguna[i].username == uname) {
            if (daftarTransaksi[idxHapus].jenis == "Isi") {
                pengguna[i].saldo -= daftarTransaksi[idxHapus].jumlah;
            } else if (daftarTransaksi[idxHapus].jenis == "Tarik") {
                pengguna[i].saldo += daftarTransaksi[idxHapus].jumlah;
            }
            break;
        }
    }

    for (int i = idxHapus; i < jumlahTransaksi - 1; ++i) {
        daftarTransaksi[i] = daftarTransaksi[i + 1];
    }
    jumlahTransaksi--;
    cout << "Transaksi berhasil dihapus.\n";
    simpanKeFile();
}

// Fungsi main
int main() {
    muatDariFile(); // Membaca dari file

    tampilkanHeader();

    char lanjut;
    cout << "Apakah anda ingin lanjut ke halaman berikutnya (y/n) : ";
    cin >> lanjut;
    cout << "\n";
    cin.ignore();
    cls();

    string password_awal = "123";
    while (true) {
        int opsiAwal;
        cout << "\n|================================================================|\n";
        cout << "|                       Bank Milik Ikhsan                        |\n";
        cout << "|================================================================|\n";
        cout << "| 1 | Login                                                      |\n";
        cout << "| 2 | Daftar (Register)                                          |\n";
        cout << "| 3 | Keluar                                                     |\n";
        cout << "|================================================================|\n";
        cout << "| Pilih opsi (1-3) : ";
        cin >> opsiAwal;

        if (opsiAwal == 1) {
            // Login 
            string username, password;
            cout << "\n|================================================================|\n";
            cout << "|                        Silahkan Login                          |\n";
            cout << "|================================================================|\n\n";
            cout << "| Masukan Username : ";
            cin >> username;
            cout << "| Masukan Password : ";
            cin >> password;

            // Loading yang lebih rapi
            cout << "\nMemuat data, mohon tunggu";
            cout.flush();
            cout << endl;
            cls(); // Bersihkan layar setelah loading

            // ======================[PERCABANGAN]======================
            // Login admin
            if (username == "admin" && password == "123") {
                // Mode admin
                bool menuAdmin = true;
                while (menuAdmin) {
                    tampilkanMenuAdmin();
                    int pilihan;
                    cout << "Pilih menu admin : ";
                    cin >> pilihan;

                    switch (pilihan) {
                        case 1 : 
                            cout << "Daftar transaksi seluruh user (rekursif): \n";
                            tampilkanTransaksiRekursif();
                            break;
                        case 2 : 
                            tampilkanSaldoUser();
                            break;
                        case 3 : 
                            password_awal = "123";
                            cout << "Password user berhasil direset ke default.\n";
                            break;
                        case 4 : 
                            tampilkanRekap();
                            break;
                        case 5 : 
                            hapusAkun();
                            return 0;
                        case 6:
                            hapusTransaksiAdmin();
                            break;
                        case 7:
                            menuAdmin = false; // Kembali ke menu utama
                            break;
                        case 8:
                            cout << "Keluar dari program. Terimakasih.\n";
                            exit(0); // Langsung keluar dari program
                        default :
                            cout << "Pilihan tidak valid.\n";
                    }
                }
            } else {
                // ======================[SEARCHING]======================
                // Mencari user pada array pengguna
                bool ditemukan = false;
                for (int i = 0; i < jumlahPengguna; ++i) {
                    if (pengguna[i].username == username && pengguna[i].password == password) {
                        ditemukan = true;
                        loginPengguna = &pengguna[i]; // [POINTER]
                        break;
                    }
                }

                if (!ditemukan) {
                    cout << "Username atau password salah.\n";
                    continue;
                }

                cout << "Login berhasil sebagai " << username << endl;
                cout << "Selamat Datang " << loginPengguna->username << "!\n";
                cls();

                // Input hari simulasi
                string hariInput;
                hariSimulasi = -1;
                while (hariSimulasi == -1) {
                    cout << "Masukan hari simulasi (Senin, Selasa, Rabu, Kamis, Jumat, Sabtu, Minggu): ";
                    cin >> hariInput;
                    for (auto& ch : hariInput) ch = tolower(ch);
                    if (hariInput == "senin") hariSimulasi = 0;
                    else if (hariInput == "selasa") hariSimulasi = 1;
                    else if (hariInput == "rabu") hariSimulasi = 2;
                    else if (hariInput == "kamis") hariSimulasi = 3;
                    else if (hariInput == "jumat") hariSimulasi = 4;
                    else if (hariInput == "sabtu") hariSimulasi = 5;
                    else if (hariInput == "minggu") hariSimulasi = 6;
                    else cout << "Input tidak dikenali. Silakan masukkan nama hari dengan benar.\n";
                }
                cls();
                simpanKeFile(); // Simpan data ke file setelah login
                cout << "Data berhasil disimpan ke file.\n";
                cout << "\n";

                char repeat_instructions = 'y';
                while (repeat_instructions == 'y' || repeat_instructions == 'Y') {
                    tampilkanMenuPengguna();
                    if (loginPengguna == nullptr) {
                        // Jika user logout, kembali ke menu utama (login/register)
                        break;
                    }
                    cout << "Apakah anda ingin mengulangi instruksi (y/n)? ";
                    cin >> repeat_instructions;
                }
            }
        } else if (opsiAwal == 2) {
            // Registrasi
            cout << "\n|================================================================|\n";
            cout << "|                        Silahkan Registrasi                     |\n";
            cout << "|================================================================|\n\n";                
            string uname, pw;
            cout << "Masukan username baru : ";
            cin >> uname;
            while (uname.empty() || uname.find(' ') != string::npos) {
                cout << "Username tidak boleh kosong atau mengandung spasi. Masukkan lagi: ";
                cin >> uname;
            }
            cout << "Masukan password : ";
            cin >> pw;
            while (pw.empty() || pw.find(' ') != string::npos) {
                cout << "Password tidak boleh kosong atau mengandung spasi. Masukkan lagi: ";
                cin >> pw;
            }
            pengguna[jumlahPengguna++] = Pengguna(uname, pw);

            // Loading saat registrasi
            cout << "\nMendaftarkan akun";
            cout.flush();
            cout << endl;

            // Simpan data ke file
            cout << "Menyimpan data ke file";
            cout.flush();
            cout << endl;
            cls(); // Bersihkan layar setelah loading

            cout << "Registrasi berhasil! Silahkan login.\n";
            cout << "Tekan ENTER untuk melanjutkan...";
            cin.ignore();
            cin.get();
            cls();
        } else if (opsiAwal == 3) {
            cout << "Keluar dari program. Terimakasih.\n";
            return 0;
        } else {
            cout << "Opsi tidak valid.\n";
        }
    }
    return 0;
}