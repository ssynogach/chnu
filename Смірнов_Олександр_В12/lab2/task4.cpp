#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void encode(unsigned char* a, int n) {
    unsigned char mask;
    if (n % 2 == 0) {
        mask = 0x55;
    } else {
        mask = 0xAA;
    }
    for (int i = 0; i < n; i++) {
        a[i] = a[i] ^ mask;
    }
}

void printHex(const char* title, unsigned char* a, int n) {
    cout << title;
    for (int i = 0; i < n; i++) {
        if (a[i] < 16) {
            cout << "0";
        }
        cout << hex << (int)a[i] << " ";
    }
    cout << dec << endl;
}

int main() {
    string line;
    cout << "Введіть послідовність символів: ";
    getline(cin, line);
    int n = line.size();
    if (n == 0) {
        cout << "Порожня послідовність!" << endl;
        return 1;
    }

    unsigned char* a = new unsigned char[n];
    for (int i = 0; i < n; i++) {
        a[i] = line[i];
    }
    printHex("Вхідні байти:      ", a, n);
    cout << "Розмір " << n << (n % 2 == 0 ? " (парний) - маска 01010101" : " (непарний) - маска 10101010") << endl;

    encode(a, n);
    printHex("Закодовані байти:  ", a, n);

    ofstream out("task4_encoded.bin", ios::binary);
    out.write((char*)a, n);
    out.close();
    cout << "Записано в task4_encoded.bin" << endl;

    ifstream in("task4_encoded.bin", ios::binary);
    unsigned char* b = new unsigned char[n];
    in.read((char*)b, n);
    in.close();

    encode(b, n);
    printHex("Після декодування: ", b, n);
    cout << "Текст: ";
    for (int i = 0; i < n; i++) {
        cout << (char)b[i];
    }
    cout << endl;

    delete[] a;
    delete[] b;
    return 0;
}
