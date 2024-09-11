#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

// Fungsi untuk enkripsi/dekripsi menggunakan Shift Cipher
string shiftCipher(const string& text, int shift, const string& mode) {
    string result = "";
    
    // Periksa jika mode dekripsi, ubah shift menjadi negatif
    if (mode == "decrypt") {
        shift = -shift;
    }

    // Lakukan iterasi setiap karakter dalam teks
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            result += char(int(base + (c - base + shift + 26) % 26));
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

// Fungsi untuk meminta input shift dengan pengecekan
int getShiftInput() {
    int shift;
    while (true) {
        cout << "Masukkan jumlah pergeseran (shift): ";
        cin >> shift;

        // Memeriksa apakah input merupakan integer yang valid
        // Jika input bukan integer, beri pesan kesalahan
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input tidak valid, masukkan angka bulat.\n";
        } else {
            cin.ignore(1000, '\n');
            return shift;
        }
    }
}

// Fungsi menu utama
void menu() {
    while (true) {
        cout << "----------------------------\n";
        cout << "|      Caesar Cipher       |\n";
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

            int shift = getShiftInput();

            string resultText = shiftCipher(text, shift, operation);

            if (choice == 1) {
                cout << "Teks terenkripsi: " << resultText << "\n";
            } else {
                cout << "Teks terdekripsi: " << resultText << "\n";
            }

            saveToFile(resultText);

        } else if (choice == 3) {
            cout << "Terima kasih telah menggunakan Caesar Cipher!\n";
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
