#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_SIZE = 5;

// Fungsi untuk menghitung gcd
int gcd_func(int a, int b) {
    if (b == 0)
        return a;
    return gcd_func(b, a % b);
}

// Fungsi untuk menghitung modulo invers
int modInverse(int a, int m) {
    a = a % m;
    if (a < 0) a += m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

// Fungsi untuk melakukan perkalian matriks
void kaliMatriks(int key[][MAX_SIZE], int blok[], int hasil[], int n) {
    for (int i = 0; i < n; i++) {
        hasil[i] = 0;
        for (int j = 0; j < n; j++) {
            hasil[i] += key[i][j] * blok[j];
        }
        hasil[i] = hasil[i] % 26; 
    }
}

string hillCipher(const string& teks, int key[][MAX_SIZE], int n, const string& mode) {
    string hasil = "";
    int blok[MAX_SIZE], hasilBlok[MAX_SIZE];
    
    // Menghilangkan karakter non-alfabet dan mengubah ke uppercase
    string teksTerfilter = "";
    for (char c : teks) {
        if (isalpha(c)) {
            teksTerfilter += toupper(c);
        }
    }

    // Tambahkan padding jika teks tidak kelipatan n
    while (teksTerfilter.size() % n != 0) {
        teksTerfilter += 'X';
    }

    // Enkripsi/Dekripsi per blok sebesar n
    for (int i = 0; i < teksTerfilter.size(); i += n) {
        for (int j = 0; j < n; j++) {
            blok[j] = teksTerfilter[i + j] - 'A';
        }

        kaliMatriks(key, blok, hasilBlok, n);
        
        for (int j = 0; j < n; j++) {
            hasil += (hasilBlok[j] + 'A');
        }
    }

    return hasil;
}

string readText(const string& modeInput) {
    if (modeInput == "input") {
        cout << "Masukkan teks: ";
        string teks;
        getline(cin, teks);
        return teks;
    } else if (modeInput == "file") {
        string namaFile;
        cout << "Masukkan nama file (.txt): ";
        cin >> namaFile;

        try {
            ifstream file(namaFile);
            if (!file.is_open()) {
                throw runtime_error("File tidak ditemukan atau tidak bisa dibuka.");
            }
            string teks((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();
            return teks;
        } catch (const runtime_error& e) {
            cerr << e.what() << "\n";
            return "";
        }
    }
    return "";
}

void saveToFile(const string& teks) {
    char simpan;
    cout << "Apakah Anda ingin menyimpan hasil ke file? (y/n): ";
    cin >> simpan;
    if (tolower(simpan) == 'y') {
        string namaFile;
        cout << "Masukkan nama file untuk menyimpan hasil (misal: hasil.txt): ";
        cin >> namaFile;

        try {
            ofstream file(namaFile);
            if (!file.is_open()) {
                throw runtime_error("File tidak bisa dibuka untuk menulis.");
            }
            file << teks;
            file.close();
            cout << "Hasil telah disimpan ke file '" << namaFile << "'.\n";
        } catch (const runtime_error& e) {
            cerr << e.what() << "\n";
        }
    }
}

void getKey(int key[][MAX_SIZE], int n) {
    cout << "Masukkan elemen matriks kunci (" << n * n << " elemen):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "Masukkan elemen [" << i << "][" << j << "]: ";
            cin >> key[i][j];
        }
    }
}

int hitungDeterminan(int matriks[][MAX_SIZE], int n) {
    if (n == 1) return matriks[0][0];
    if (n == 2) return matriks[0][0] * matriks[1][1] - matriks[0][1] * matriks[1][0];

    int det = 0;
    for (int j = 0; j < n; j++) {
        int subMatriks[MAX_SIZE][MAX_SIZE];
        for (int i = 1; i < n; i++) {
            int subCol = 0;
            for (int k = 0; k < n; k++) {
                if (k == j) continue;
                subMatriks[i-1][subCol] = matriks[i][k];
                subCol++;
            }
        }
        det += (j % 2 == 0 ? 1 : -1) * matriks[0][j] * hitungDeterminan(subMatriks, n-1);
    }
    return det;
}

// Fungsi untuk menemukan kunci dengan mencoba semua kombinasi pasangan blok
void findKey(const string& plainText, const string& cipherText, int n) {
    if (n != 2) {
        cout << "Implementasi ini hanya mendukung matriks 2x2 untuk pencarian kunci.\n";
        return;
    }

    vector<int> P, C;
    for (char c : plainText) P.push_back(c - 'A');
    for (char c : cipherText) C.push_back(c - 'A');

    // Memastikan panjang plaintext dan ciphertext adalah kelipatan 2
    while (P.size() % 2 != 0) P.push_back('X' - 'A');
    while (C.size() % 2 != 0) C.push_back('X' - 'A');

    // Membuat matriks P dan C
    vector<vector<int>> matP, matC;
    for (size_t i = 0; i < P.size(); i += 2) {
        matP.push_back({P[i], P[i+1]});
        matC.push_back({C[i], C[i+1]});
    }

    bool keyFound = false;

    // Mencoba semua kombinasi pasangan blok
    for (size_t i = 0; i < matP.size(); i++) {
        for (size_t j = i + 1; j < matP.size(); j++) {
            // Membentuk matP dari pasangan blok i dan j
            int tempP[2][2] = {
                {matP[i][0], matP[i][1]},
                {matP[j][0], matP[j][1]}
            };

            int detP = (tempP[0][0] * tempP[1][1] - tempP[0][1] * tempP[1][0]) % 26;
            if (detP < 0) detP += 26;

            // Cek apakah determinan invertibel
            if (gcd_func(detP, 26) != 1) {
                continue; // Tidak invertibel, coba pasangan berikutnya
            }

            int invDetP = modInverse(detP, 26);
            if (invDetP == -1) {
                continue; // Tidak ada invers, coba pasangan berikutnya
            }

            // Membentuk invers dari matP
            int invP[2][2] = {
                {(tempP[1][1] * invDetP) % 26, ((-tempP[0][1] + 26) * invDetP) % 26},
                {((-tempP[1][0] + 26) * invDetP) % 26, (tempP[0][0] * invDetP) % 26}
            };

            // Membentuk matriks C dari pasangan blok yang sama
            int tempC[2][2] = {
                {matC[i][0], matC[i][1]},
                {matC[j][0], matC[j][1]}
            };

            // Menghitung K = invP * C mod26
            int key[2][2] = { {0, 0}, {0, 0} };
            for (int x = 0; x < 2; x++) {
                for (int y = 0; y < 2; y++) {
                    for (int k = 0; k < 2; k++) {
                        key[x][y] += invP[x][k] * tempC[k][y];
                    }
                    key[x][y] %= 26;
                    if (key[x][y] < 0) key[x][y] += 26;
                }
            }

            // Verifikasi kunci dengan seluruh blok
            bool valid = true;
            for (size_t k = 0; k < matP.size(); k++) {
                // Enkripsi blok plaintext k dengan kunci yang ditemukan
                int hasilEnkripsi[2];
                hasilEnkripsi[0] = (key[0][0] * matP[k][0] + key[0][1] * matP[k][1]) % 26;
                hasilEnkripsi[1] = (key[1][0] * matP[k][0] + key[1][1] * matP[k][1]) % 26;

                if (hasilEnkripsi[0] != matC[k][0] || hasilEnkripsi[1] != matC[k][1]) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                // Kunci ditemukan
                cout << "Kunci yang ditemukan:\n";
                for (int x = 0; x < 2; x++) {
                    for (int y = 0; y < 2; y++) {
                        cout << key[x][y] << " ";
                    }
                    cout << "\n";
                }

                // Verifikasi kunci
                int verifyKey[MAX_SIZE][MAX_SIZE];
                for (int x = 0; x < 2; x++) {
                    for (int y = 0; y < 2; y++) {
                        verifyKey[x][y] = key[x][y];
                    }
                }
                string hasilEnkripsiStr = hillCipher(plainText, verifyKey, 2, "encrypt");
                string cipherTextFiltered = "";
                for (char c : cipherText) {
                    if (isalpha(c)) {
                        cipherTextFiltered += toupper(c);
                    }
                }

                if (hasilEnkripsiStr == cipherTextFiltered) {
                    cout << "Kunci berhasil diverifikasi.\n";
                } else {
                    cout << "Kunci tidak dapat diverifikasi. Mungkin ada lebih dari satu kunci yang valid.\n";
                }

                keyFound = true;
                return; // Kunci ditemukan, keluar dari fungsi
            }
        }
    }

    if (!keyFound) {
        cout << "Tidak dapat menemukan kunci dengan pasangan blok yang diberikan.\n";
    }
}

void menu() {
    while (true) {
        cout << "----------------------------\n";
        cout << "|       Hill Cipher        |\n";
        cout << "----------------------------\n";
        cout << "| 1. Enkripsi              |\n";
        cout << "| 2. Dekripsi              |\n";
        cout << "| 3. Cari Kunci            |\n";
        cout << "| 4. Keluar                |\n";
        cout << "----------------------------\n";
        cout << "Pilih opsi (1/2/3/4): ";
        int pilihan;
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1 || pilihan == 2) {
            string operasi = (pilihan == 1) ? "encrypt" : "decrypt";

            cout << "----------------------------\n";
            cout << "| Pilih Metode Input       |\n";
            cout << "| 1. Input Langsung        |\n";
            cout << "| 2. Baca dari File        |\n";
            cout << "| 3. Kembali               |\n";
            cout << "----------------------------\n";
            cout << "Pilih opsi (1/2/3): ";
            int pilihanInput;
            cin >> pilihanInput;
            cin.ignore();

            string modeInput;
            if (pilihanInput == 1) {
                modeInput = "input";
            } else if (pilihanInput == 2) {
                modeInput = "file";
            } else if (pilihanInput == 3) {
                continue;
            } else {
                cout << "Opsi tidak valid, silakan coba lagi.\n";
                continue;
            }

            string teks;
            while (teks.empty()) {
                teks = readText(modeInput);
            }

            cout << "Masukkan ukuran matriks kunci (2-5): ";
            int n;
            cin >> n;

            if (n < 2 || n > MAX_SIZE) {
                cout << "Ukuran matriks tidak valid. Harus antara 2 dan " << MAX_SIZE << ".\n";
                continue;
            }

            int key[MAX_SIZE][MAX_SIZE];
            getKey(key, n);

            // Cek apakah matriks kunci memiliki invers modulo 26 (hanya untuk enkripsi)
            if (pilihan == 1) {
                int detK = hitungDeterminan(key, n) % 26;
                if (detK < 0) detK += 26;
                if (gcd_func(detK, 26) != 1) {
                    cout << "Matriks kunci tidak memiliki invers modulo 26. Silakan masukkan kunci lain.\n";
                    continue;
                }
            }

            string hasilTeks = hillCipher(teks, key, n, operasi);

            if (pilihan == 1) {
                cout << "Teks terenkripsi: " << hasilTeks << "\n";
            } else {
                cout << "Teks terdekripsi: " << hasilTeks << "\n";
            }

            saveToFile(hasilTeks);

        } else if (pilihan == 3) {
            string plainText, cipherText;
            cout << "Masukkan teks asli: ";
            getline(cin, plainText);
            cout << "Masukkan teks sandi yang sesuai: ";
            getline(cin, cipherText);
            
            cout << "Masukkan ukuran matriks kunci (2-5): ";
            int n;
            cin >> n;
            cin.ignore(); // Menambahkan ini untuk membersihkan newline setelah input integer

            findKey(plainText, cipherText, n);
        } else if (pilihan == 4) {
            cout << "Terima kasih telah menggunakan Hill Cipher!\n";
            break;
        } else {
            cout << "Opsi tidak valid, silakan coba lagi.\n";
        }
    }
}

int main() {
    menu();
    return 0;
}
