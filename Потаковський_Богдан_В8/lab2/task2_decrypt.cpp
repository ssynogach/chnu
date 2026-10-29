#include <iostream>
#include <fstream>

using namespace std;

unsigned short parity(unsigned short x) {
    unsigned short b = 0;
    while (x) {
        b ^= x & 1;
        x >>= 1;
    }
    return b;
}

int main() {
    ifstream in("task2_encrypted.bin", ios::binary);
    if (!in) {
        cout << "Файл task2_encrypted.bin не знайдено!" << endl;
        return 1;
    }
    unsigned short data[128];
    in.read((char*)data, 128 * sizeof(unsigned short));
    if (in.gcount() != 128 * (int)sizeof(unsigned short)) {
        cout << "Файл має неправильний розмір!" << endl;
        return 1;
    }
    in.close();

    char text[129];
    for (int i = 0; i < 128; i++) {
        text[i] = '?';
    }
    text[128] = '\0';

    int errors = 0;
    for (int k = 0; k < 128; k++) {
        unsigned short r = data[k];
        unsigned short high = r & 0x0F;
        unsigned short pos = (r >> 4) & 0x7F;
        unsigned short low = (r >> 11) & 0x0F;
        unsigned short p = (r >> 15) & 1;

        if (parity(r & 0x7FFF) != p) {
            cout << "Помилка: слово " << k << " - неправильний біт парності" << endl;
            errors++;
            continue;
        }
        text[pos] = (high << 4) | low;
    }

    if (errors == 0) {
        cout << "Біт парності правильний" << endl;
    } else {
        cout << "Кількість помилок: " << errors << endl;
    }

    ofstream out("task2_decrypted.txt");
    cout << "Розшифрований текст:" << endl;
    cout << "|" << text << "|" << endl;
    out << text << endl;
    out.close();
    cout << "Записано в task2_decrypted.txt" << endl;
    return 0;
}
