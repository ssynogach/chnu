#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

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

unsigned short encryptSymbol(unsigned char c, unsigned short row, unsigned short pos) {
    Word w;
    w.value = 0;
    w.f.row = row;
    w.f.high = c >> 4;
    w.f.p1 = parity(w.value & 0x3F);
    w.f.low = c & 0x0F;
    w.f.pos = pos;
    w.f.p2 = parity((w.value >> 7) & 0xFF);
    return w.value;
}

int main() {
    string lines[4];
    int choice;
    cout << "1 - ввести 4 рядки з клавіатури" << endl;
    cout << "2 - прочитати з файлу task3_input.txt" << endl;
    cout << "Ваш вибір: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    if (choice == 1) {
        ofstream in("task3_input.txt");
        for (int i = 0; i < 4; i++) {
            cout << "Рядок " << i + 1 << " (до 16 символів): ";
            getline(cin, lines[i]);
            in << lines[i] << endl;
        }
        in.close();
    } else {
        ifstream in("task3_input.txt");
        if (!in) {
            cout << "Файл task3_input.txt не знайдено!" << endl;
            return 1;
        }
        for (int i = 0; i < 4; i++) {
            getline(in, lines[i]);
        }
        in.close();
    }

    Word result[64];
    int k = 0;
    cout << "Текст (доповнений пробілами):" << endl;
    for (int i = 0; i < 4; i++) {
        while (lines[i].size() < 16) {
            lines[i] += ' ';
        }
        lines[i] = lines[i].substr(0, 16);
        cout << "|" << lines[i] << "|" << endl;
        for (int j = 0; j < 16; j++) {
            result[k].value = encryptSymbol(lines[i][j], i, j);
            k++;
        }
    }

    ofstream out("task3_encrypted.bin", ios::binary);
    out.write((char*)result, 64 * sizeof(Word));
    out.close();

    cout << "Розмір слова: " << sizeof(Word) << " байти" << endl;
    cout << "Зашифровані слова (hex):" << endl;
    for (int i = 0; i < 64; i++) {
        cout << hex << setw(4) << setfill('0') << result[i].value << " ";
        if (i % 16 == 15) {
            cout << endl;
        }
    }
    cout << dec << setfill(' ') << "Записано в task3_encrypted.bin (128 байтів)" << endl;
    return 0;
}
