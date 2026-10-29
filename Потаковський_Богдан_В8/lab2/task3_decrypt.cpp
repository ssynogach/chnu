#include <iostream>
#include <fstream>

using namespace std;

struct Fields {
    unsigned short high : 4;
    unsigned short pos : 7;
    unsigned short low : 4;
    unsigned short p : 1;
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
    Word data[128];
    in.read((char*)data, 128 * sizeof(Word));
    if (in.gcount() != 128 * (int)sizeof(Word)) {
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
        Word w = data[k];
        if (parity(w.value & 0x7FFF) != w.f.p) {
            cout << "Помилка: слово " << k << " - неправильний біт парності" << endl;
            errors++;
            continue;
        }
        text[w.f.pos] = (w.f.high << 4) | w.f.low;
    }

    if (errors == 0) {
        cout << "Біт парності правильний" << endl;
    } else {
        cout << "Кількість помилок: " << errors << endl;
    }

    ofstream out("task3_decrypted.txt");
    cout << "Розшифрований текст:" << endl;
    cout << "|" << text << "|" << endl;
    out << text << endl;
    out.close();
    cout << "Записано в task3_decrypted.txt" << endl;
    return 0;
}
