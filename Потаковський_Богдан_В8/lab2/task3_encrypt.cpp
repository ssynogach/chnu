#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

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

unsigned short encryptSymbol(unsigned char c, unsigned short pos) {
    Word w;
    w.value = 0;
    w.f.high = c >> 4;
    w.f.pos = pos;
    w.f.low = c & 0x0F;
    w.f.p = parity(w.value & 0x7FFF);
    return w.value;
}

int main() {
    string text;
    int choice;
    cout << "1 - ввести текст з клавіатури" << endl;
    cout << "2 - прочитати з файлу task3_input.txt" << endl;
    cout << "Ваш вибір: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    if (choice == 1) {
        cout << "Введіть текст (до 128 символів): ";
        getline(cin, text);
        ofstream in("task3_input.txt");
        in << text << endl;
        in.close();
    } else {
        ifstream in("task3_input.txt");
        if (!in) {
            cout << "Файл task3_input.txt не знайдено!" << endl;
            return 1;
        }
        getline(in, text);
        in.close();
    }

    while (text.size() < 128) {
        text += ' ';
    }
    text = text.substr(0, 128);
    cout << "Текст (доповнений пробілами до 128):" << endl;
    cout << "|" << text << "|" << endl;

    Word result[128];
    for (int i = 0; i < 128; i++) {
        result[i].value = encryptSymbol(text[i], i);
    }

    ofstream out("task3_encrypted.bin", ios::binary);
    out.write((char*)result, 128 * sizeof(Word));
    out.close();

    cout << "Розмір слова: " << sizeof(Word) << " байти" << endl;
    cout << "Зашифровані слова (hex):" << endl;
    for (int i = 0; i < 128; i++) {
        cout << hex << setw(4) << setfill('0') << result[i].value << " ";
        if (i % 16 == 15) {
            cout << endl;
        }
    }
    cout << dec << setfill(' ') << "Записано в task3_encrypted.bin (256 байтів)" << endl;
    return 0;
}
