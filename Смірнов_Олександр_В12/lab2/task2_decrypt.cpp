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
    unsigned short data[64];
    in.read((char*)data, 64 * sizeof(unsigned short));
    if (in.gcount() != 64 * (int)sizeof(unsigned short)) {
        cout << "Файл має неправильний розмір!" << endl;
        return 1;
    }
    in.close();

    char text[4][17];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            text[i][j] = '?';
        }
        text[i][16] = '\0';
    }

    int errors = 0;
    for (int k = 0; k < 64; k++) {
        unsigned short r = data[k];
        unsigned short row = r & 0x03;
        unsigned short high = (r >> 2) & 0x0F;
        unsigned short p1 = (r >> 6) & 1;
        unsigned short low = (r >> 7) & 0x0F;
        unsigned short pos = (r >> 11) & 0x0F;
        unsigned short p2 = (r >> 15) & 1;

        if (parity(r & 0x3F) != p1) {
            cout << "Помилка: слово " << k << " - неправильний біт парності 6" << endl;
            errors++;
            continue;
        }
        if (parity((r >> 7) & 0xFF) != p2) {
            cout << "Помилка: слово " << k << " - неправильний біт парності 15" << endl;
            errors++;
            continue;
        }
        text[row][pos] = (high << 4) | low;
    }

    if (errors == 0) {
        cout << "Біти парності правильні" << endl;
    } else {
        cout << "Кількість помилок: " << errors << endl;
    }

    ofstream out("task2_decrypted.txt");
    cout << "Розшифрований текст:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "|" << text[i] << "|" << endl;
        out << text[i] << endl;
    }
    out.close();
    cout << "Записано в task2_decrypted.txt" << endl;
    return 0;
}
