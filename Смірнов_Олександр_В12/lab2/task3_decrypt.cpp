#include <iostream>
#include <fstream>

using namespace std;

struct Fields {
    unsigned short row : 2;
    unsigned short high : 4;
    unsigned short p1 : 1;
    unsigned short low : 4;
    unsigned short pos : 4;
    unsigned short p2 : 1;
};

union Word {
    Fields f;
    unsigned short value;
};

unsigned short parity(unsigned short x) {
    unsigned short b = 0;
    while (x) {
        b ^= x & 1;
        x >>= 1;
    }
    return b;
}

int main() {
    ifstream in("task3_encrypted.bin", ios::binary);
    if (!in) {
        cout << "Файл task3_encrypted.bin не знайдено!" << endl;
        return 1;
    }
    Word data[64];
    in.read((char*)data, 64 * sizeof(Word));
    if (in.gcount() != 64 * (int)sizeof(Word)) {
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
        Word w = data[k];
        if (parity(w.value & 0x3F) != w.f.p1) {
            cout << "Помилка: слово " << k << " - неправильний біт парності 6" << endl;
            errors++;
            continue;
        }
        if (parity((w.value >> 7) & 0xFF) != w.f.p2) {
            cout << "Помилка: слово " << k << " - неправильний біт парності 15" << endl;
            errors++;
            continue;
        }
        text[w.f.row][w.f.pos] = (w.f.high << 4) | w.f.low;
    }

    if (errors == 0) {
        cout << "Біти парності правильні" << endl;
    } else {
        cout << "Кількість помилок: " << errors << endl;
    }

    ofstream out("task3_decrypted.txt");
    cout << "Розшифрований текст:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "|" << text[i] << "|" << endl;
        out << text[i] << endl;
    }
    out.close();
    cout << "Записано в task3_decrypted.txt" << endl;
    return 0;
}
