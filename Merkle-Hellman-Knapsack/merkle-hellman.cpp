/*
    Nama    : Angga Prasetyo        - 140810220086
    Nama    : Giast Ahmad           - 140810220018
    Nama    : Dzakwan Fadhlullah    - 140810220060
*/

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <sstream>
#include <limits>

using namespace std;

// Fungsi untuk menghitung modular inverse
long long modInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

// Fungsi untuk mengenkripsi pesan
vector<long long> encrypt(const string& message, const vector<long long>& publicKey) {
    vector<long long> ciphertext;
    cout << "3. Enkripsi\n";
    for (size_t i = 0; i < message.length(); i++) {
        bitset<8> bits(message[i]);
        long long sum = 0;
        cout << message[i] << ": " << bits << " → y" << i+1 << " : ";
        for (size_t j = 0; j < publicKey.size(); j++) {
            if (bits[j]) {
                sum += publicKey[j];
                cout << publicKey[j];
                if (j < publicKey.size() - 1 && bits[j+1]) cout << " + ";
            }
        }
        cout << " = " << sum << endl;
        ciphertext.push_back(sum);
    }
    return ciphertext;
}

// Fungsi untuk mendekripsi pesan
string decrypt(const vector<long long>& ciphertext, const vector<long long>& privateKey, long long a, long long p) {
    string plaintext;
    long long a_inv = modInverse(a, p);
    cout << "4. Dekripsi\n";
    cout << "Z = a^(-1)y mod p\n";
    cout << "a^(-1) = " << a_inv << endl;
    
    for (size_t i = 0; i < ciphertext.size(); i++) {
        long long Z = (a_inv * ciphertext[i]) % p;
        cout << "y" << i+1 << " → Z = (" << a_inv << " * " << ciphertext[i] << ") mod " << p << " = " << Z << endl;
        
        bitset<8> bits;
        for (int j = privateKey.size() - 1; j >= 0; j--) {
            if (Z >= privateKey[j]) {
                bits[j] = 1;
                Z -= privateKey[j];
            }
        }
        plaintext += char(bits.to_ulong());
    }
    return plaintext;
}

// Fungsi untuk membaca input kunci pribadi
void readPrivateKey(vector<long long>& privateKey, long long& a, long long& p) {
    string input;
    cout << "Masukkan private key (pisahkan dengan spasi): ";
    getline(cin, input);
    istringstream iss(input);
    long long num;
    privateKey.clear();
    while (iss >> num) {
        privateKey.push_back(num);
    }

    cout << "Masukkan nilai a: ";
    cin >> a;
    cout << "Masukkan nilai p: ";
    cin >> p;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Fungsi untuk menampilkan proses enkripsi
void processEncryption(const vector<long long>& privateKey, long long a, long long p) {
    string plaintext;
    cout << "Masukkan plaintext: ";
    getline(cin, plaintext);

    cout << "Private key: s = (";
    for (size_t i = 0; i < privateKey.size(); i++) {
        cout << privateKey[i];
        if (i < privateKey.size() - 1) cout << ", ";
    }
    cout << ")" << endl;
    cout << "a = " << a << endl;
    cout << "p = " << p << endl;
    cout << "Plaintext: " << plaintext << endl;

    // Konversi plaintext ke biner
    cout << "1. Konversi plaintext menjadi biner\n";
    for (char c : plaintext) {
        cout << c << " (" << int(c) << "): " << bitset<8>(c) << endl;
    }

    // Hitung public key
    cout << "2. Hitung public key\n";
    cout << "t[i] = (a * s[i]) mod p\n";
    vector<long long> publicKey;
    for (size_t i = 0; i < privateKey.size(); i++) {
        long long t = (a * privateKey[i]) % p;
        publicKey.push_back(t);
        cout << "t[" << i+1 << "] = (" << a << " * " << privateKey[i] << ") mod " << p << " = " << t << endl;
    }
    cout << "Public key: t = (";
    for (size_t i = 0; i < publicKey.size(); i++) {
        cout << publicKey[i];
        if (i < publicKey.size() - 1) cout << ", ";
    }
    cout << ")" << endl;

    // Enkripsi
    vector<long long> ciphertext = encrypt(plaintext, publicKey);
    
    cout << "ciphertext: ";
    for (long long c : ciphertext) {
        cout << c << " ";
    }
    cout << endl;
}

// Fungsi untuk menampilkan proses dekripsi
void processDecryption(const vector<long long>& privateKey, long long a, long long p) {
    vector<long long> ciphertext;
    string input;
    cout << "Masukkan ciphertext (pisahkan dengan spasi): ";
    getline(cin, input);
    istringstream iss(input);
    long long num;
    while (iss >> num) {
        ciphertext.push_back(num);
    }

    string decrypted = decrypt(ciphertext, privateKey, a, p);
    
    cout << "maka didapat : ";
    for (char c : decrypted) {
        cout << int(c) << " ";
    }
    cout << endl << "atau menjadi : " << decrypted << endl;
}

int main() {
    vector<long long> privateKey;
    long long a, p;
    int choice;

    while (true) {
        cout << "\n----------------------------\n";
        cout << "| Merkle-Hellman Knapsack   |\n";
        cout << "----------------------------\n";
        cout << "| 1. Input Kunci Pribadi    |\n";
        cout << "| 2. Enkripsi               |\n";
        cout << "| 3. Dekripsi               |\n";
        cout << "| 4. Keluar                 |\n";
        cout << "----------------------------\n";
        cout << "Pilih opsi (1/2/3/4): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                readPrivateKey(privateKey, a, p);
                break;
            case 2:
                if (privateKey.empty()) {
                    cout << "Mohon input kunci pribadi terlebih dahulu.\n";
                } else {
                    processEncryption(privateKey, a, p);
                }
                break;
            case 3:
                if (privateKey.empty()) {
                    cout << "Mohon input kunci pribadi terlebih dahulu.\n";
                } else {
                    processDecryption(privateKey, a, p);
                }
                break;
            case 4:
                cout << "Terima kasih telah menggunakan program ini.\n";
                return 0;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    }

    return 0;
}