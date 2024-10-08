#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <algorithm>

using namespace std;

string vigenereEncrypt(const string& text, const string& key) {
    string result = "";
    string repeatedKey = key;
    
    while (repeatedKey.length() < text.length()) {
        repeatedKey += key;
    }
    repeatedKey = repeatedKey.substr(0, text.length());

    for (size_t i = 0; i < text.length(); ++i) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int textChar = text[i] - base;
            int keyChar = toupper(repeatedKey[i]) - 'A';
            int encryptedChar = (textChar + keyChar) % 26;
            
            cout << "Encrypting '" << text[i] << "' with key '" << repeatedKey[i] 
                 << "': (" << textChar << " + " << keyChar << ") % 26 = " << encryptedChar << "\n";
            
            result += char(base + encryptedChar);
        } else {
            result += text[i];
        }
    }
    return result;
}

string vigenereDecrypt(const string& text, const string& key) {
    string result = "";
    string repeatedKey = key;
    
    while (repeatedKey.length() < text.length()) {
        repeatedKey += key;
    }
    repeatedKey = repeatedKey.substr(0, text.length());

    for (size_t i = 0; i < text.length(); ++i) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int textChar = text[i] - base;
            int keyChar = toupper(repeatedKey[i]) - 'A';
            int decryptedChar = (textChar - keyChar + 26) % 26;
            
            cout << "Decrypting '" << text[i] << "' with key '" << repeatedKey[i] 
                 << "': (" << textChar << " - " << keyChar << " + 26) % 26 = " << decryptedChar << "\n";
            
            result += char(base + decryptedChar);
        } else {
            result += text[i];
        }
    }
    return result;
}

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

string getValidKey() {
    string key;
    while (true) {
        cout << "Masukkan kunci (hanya huruf): ";
        getline(cin, key);
        
        // Menghapus karakter non-alfabet dan mengubah ke huruf besar
        key.erase(remove_if(key.begin(), key.end(), [](char c) { return !isalpha(c); }), key.end());
        transform(key.begin(), key.end(), key.begin(), ::toupper);
        
        if (!key.empty()) {
            return key;
        } else {
            cout << "Kunci tidak valid. Masukkan setidaknya satu huruf.\n";
        }
    }
}
void menu() {
    while (true) {
        cout << "----------------------------\n";
        cout << "|     Vigenère Cipher      |\n";
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

            string text = readText(inputMode);
            if (text.empty()) continue;

            string key = getValidKey();

            string resultText;
            if (choice == 1) {
                resultText = vigenereEncrypt(text, key);
                cout << "Teks terenkripsi: " << resultText << "\n";
            } else {
                resultText = vigenereDecrypt(text, key);
                cout << "Teks terdekripsi: " << resultText << "\n";
            }

            saveToFile(resultText);

        } else if (choice == 3) {
            cout << "Terima kasih telah menggunakan Vigenère Cipher!\n";
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