#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const int MAX_SIZE = 5;

// Fungsi untuk menghitung modulo invers
int modInverse(int a, int m) {
    a = a % m;
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
            hasil += (hasilBlok[j] % 26 + 'A');
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
    if (n == 2) return ((matriks[0][0] * matriks[1][1] - matriks[0][1] * matriks[1][0]) % 26 + 26) % 26;

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
        int subDet = hitungDeterminan(subMatriks, n-1);
        det = (det + (j % 2 == 0 ? 1 : -1) * matriks[0][j] * subDet) % 26;
    }
    return (det + 26) % 26;
}

void printMatrix(int matrix[][MAX_SIZE], int n, const string& name) {
    cout << name << ":\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(4) << matrix[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void findKey(const string& plainText, const string& cipherText, int n) {
    if (n < 2 || n > MAX_SIZE) {
        cout << "Ukuran matriks harus antara 2 dan " << MAX_SIZE << ".\n";
        return;
    }

    if (plainText.length() != cipherText.length()) {
        cout << "Panjang plainteks dan cipherteks harus sama.\n";
        return;
    }

    if (plainText.length() < n * n) {
        cout << "Panjang teks harus minimal " << n * n << " karakter untuk matriks " << n << "x" << n << ".\n";
        return;
    }

    vector<int> plaintextVec, ciphertextVec;
    for (char c : plainText) plaintextVec.push_back(toupper(c) - 'A');
    for (char c : cipherText) ciphertextVec.push_back(toupper(c) - 'A');

    int key[MAX_SIZE][MAX_SIZE] = {0};
    int matriksP[MAX_SIZE][MAX_SIZE] = {0};
    int matriksC[MAX_SIZE][MAX_SIZE] = {0};

    // Membangun matriks P dan C menggunakan n*n karakter pertama
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int index = i * n + j;
            matriksP[i][j] = plaintextVec[index];
            matriksC[i][j] = ciphertextVec[index];
        }
    }

    cout << "Proses pencarian kunci:\n\n";
    printMatrix(matriksP, n, "Matriks Plainteks (P)");
    printMatrix(matriksC, n, "Matriks Cipherteks (C)");

    // Menghitung determinan P
    int detP = hitungDeterminan(matriksP, n);
    cout << "Determinan matriks P: " << detP << "\n\n";

    if (detP == 0) {
        cout << "Determinan matriks P adalah 0. Tidak dapat menemukan kunci.\n";
        return;
    }

    int invDetP = modInverse(detP, 26);

    if (invDetP == -1) {
        cout << "Matriks plaintext tidak memiliki invers. Tidak dapat menemukan kunci.\n";
        return;
    }

    cout << "Invers determinan P mod 26: " << invDetP << "\n\n";

    // Menghitung adjoin matriks P
    int adjP[MAX_SIZE][MAX_SIZE] = {0};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int subMatriks[MAX_SIZE][MAX_SIZE];
            int subIndex = 0;
            for (int r = 0; r < n; r++) {
                for (int c = 0; c < n; c++) {
                    if (r != i && c != j) {
                        subMatriks[subIndex / (n-1)][subIndex % (n-1)] = matriksP[r][c];
                        subIndex++;
                    }
                }
            }
            int cofactor = hitungDeterminan(subMatriks, n-1);
            if ((i + j) % 2 == 1) cofactor = -cofactor;
            adjP[j][i] = ((cofactor % 26) + 26) % 26;
        }
    }

    printMatrix(adjP, n, "Matriks Adjoin P");

    // Menghitung invers matriks P
    int invP[MAX_SIZE][MAX_SIZE] = {0};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            invP[i][j] = (adjP[i][j] * invDetP) % 26;
        }
    }

    printMatrix(invP, n, "Matriks Invers P");

    // Menghitung matriks kunci
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum = (sum + matriksC[i][k] * invP[k][j]) % 26;
            }
            key[i][j] = (sum + 26) % 26;
        }
    }

    printMatrix(key, n, "Matriks Kunci yang ditemukan");

    // Verifikasi kunci
    string hasilEnkripsi = hillCipher(plainText.substr(0, n*n), key, n, "encrypt");
    if (hasilEnkripsi == cipherText.substr(0, n*n)) {
        cout << "Kunci berhasil diverifikasi.\n";
    } else {
        cout << "Kunci tidak dapat diverifikasi. Mungkin ada lebih dari satu kunci yang valid.\n";
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