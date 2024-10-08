#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <numeric> // untuk std::gcd

using namespace std;

// Fungsi untuk mencari invers modular dari a dengan mod m
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    throw runtime_error("Invers modular tidak ditemukan (a dan m harus relatif prima).");
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Fungsi untuk enkripsi menggunakan Affine Cipher
string affineEncrypt(const string& text, int a, int b) {
    string result = "";
    int m = 26; // jumlah huruf dalam alfabet

    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            int x = c - base; // posisi dalam alfabet (0 untuk A/a, 25 untuk Z/z)
            int encryptedChar = (a * x + b) % m; // proses enkripsi
            cout << "Encrypting '" << c << "' (posisi " << x << "): (" << a << " * " << x << " + " << b << ") % " << m << " = " << encryptedChar << "\n";
            result += char(base + encryptedChar);
        } else {
            result += c;
        }
    }

    return result;
}

// Fungsi untuk dekripsi menggunakan Affine Cipher
string affineDecrypt(const string& text, int a, int b) {
    string result = "";
    int m = 26; // jumlah huruf dalam alfabet
    int a_inv = modInverse(a, m); // invers modular dari a

    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            int y = c - base; // posisi dalam alfabet (0 untuk A/a, 25 untuk Z/z)
            int decryptedChar = (a_inv * (y - b + m)) % m; // proses dekripsi
            cout << "Decrypting '" << c << "' (posisi " << y << "): (" << a_inv << " * (" << y << " - " << b << " + " << m << ")) % " << m << " = " << decryptedChar << "\n";
            result += char(base + decryptedChar);
        } else {
            result += c;
        }
    }

    return result;
}

// Fungsi untuk membaca teks dari input langsung atau file
string readText(const string& inputMode) {
    if (inputMode == "input") {
        cout << "Masukkan teks: ";
        string text;
        getline(cin, text);
        return text;
    } else if (inputMode == "file") {
        string filePath;
        cout << "Masukkan nama file (.txt): ";
        cin >> filePath;

        try {
            ifstream file(filePath);
            if (!file.is_open()) {
                throw runtime_error("File tidak ditemukan atau tidak bisa dibuka.");
            }
            string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();
            return text;
        } catch (const runtime_error& e) {
            cerr << e.what() << "\n";
            return "";
        }
    }
    return "";
}

// Fungsi untuk menyimpan hasil ke file
void saveToFile(const string& text) {
    char save;
    cout << "Apakah Anda ingin menyimpan hasil ke file? (y/n): ";
    cin >> save;
    if (tolower(save) == 'y') {
        string filePath;
        cout << "Masukkan nama file untuk menyimpan hasil (misal: hasil.txt): ";
        cin >> filePath;

        try {
            ofstream file(filePath);
            if (!file.is_open()) {
                throw runtime_error("File tidak bisa dibuka untuk menulis.");
            }
            file << text;
            file.close();
            cout << "Hasil telah disimpan ke file '" << filePath << "'.\n";
        } catch (const runtime_error& e) {
            cerr << e.what() << "\n";
        }
    }
}

// Fungsi untuk meminta input integer
int getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input tidak valid, masukkan angka bulat.\n";
        } else {
            cin.ignore(1000, '\n');
            return value;
        }
    }
}

// Fungsi untuk memeriksa apakah a dan 26 relatif prima
bool areCoprime(int a, int m) {
    return gcd(a, m) == 1;
}

// Fungsi menu utama
void menu() {
    while (true) {
        cout << "----------------------------\n";
        cout << "|      Affine Cipher        |\n";
        cout << "----------------------------\n";
        cout << "| 1. Enkripsi              |\n";
        cout << "| 2. Dekripsi              |\n";
        cout << "| 3. Keluar                |\n";
        cout << "----------------------------\n";
        cout << "Pilih opsi (1/2/3): ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1 || choice == 2) {
            string operation = (choice == 1) ? "encrypt" : "decrypt";

            cout << "----------------------------\n";
            cout << "| Pilih Metode Input       |\n";
            cout << "| 1. Input Langsung        |\n";
            cout << "| 2. Baca dari File        |\n";
            cout << "| 3. Kembali               |\n";
            cout << "----------------------------\n";
            cout << "Pilih opsi (1/2/3): ";
            int inputChoice;
            cin >> inputChoice;
            cin.ignore();

            string inputMode;
            if (inputChoice == 1) {
                inputMode = "input";
            } else if (inputChoice == 2) {
                inputMode = "file";
            } else if (inputChoice == 3) {
                continue;
            } else {
                cout << "Opsi tidak valid, silakan coba lagi.\n";
                continue;
            }

            string text;
            while (text.empty()) {
                text = readText(inputMode);
            }

            int a, b;
            while (true) {
                a = getIntInput("Masukkan kunci a: ");
                if (!areCoprime(a, 26)) {
                    cout << "Kunci a harus relatif prima terhadap 26, coba lagi.\n";
                } else {
                    break;
                }
            }

            b = getIntInput("Masukkan kunci b: ");

            string resultText = (choice == 1) ? affineEncrypt(text, a, b) : affineDecrypt(text, a, b);

            if (choice == 1) {
                cout << "Teks terenkripsi: " << resultText << "\n";
            } else {
                cout << "Teks terdekripsi: " << resultText << "\n";
            }

            saveToFile(resultText);

        } else if (choice == 3) {
            cout << "Terima kasih telah menggunakan Affine Cipher!\n";
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
