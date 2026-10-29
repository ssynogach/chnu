#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

unsigned short parity(unsigned short x) {
    unsigned short b = 0;
    while (x) {
        b ^= x & 1;
        x >>= 1;
    }
    return b;
}

unsigned short encryptSymbol(unsigned char c, unsigned short pos) {
    unsigned short r = 0;
    r |= c >> 4;
    r |= pos << 4;
    r |= (c & 0x0F) << 11;
    r |= parity(r & 0x7FFF) << 15;
    return r;
}

int main() {
    string text;
    int choice;
    cout << "1 - ввести текст з клавіатури" << endl;
    cout << "2 - прочитати з файлу task2_input.txt" << endl;
    cout << "Ваш вибір: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    if (choice == 1) {
        cout << "Введіть текст (до 128 символів): ";
        getline(cin, text);
        ofstream in("task2_input.txt");
        in << text << endl;
        in.close();
    } else {
        ifstream in("task2_input.txt");
        if (!in) {
            cout << "Файл task2_input.txt не знайдено!" << endl;
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

    unsigned short result[128];
    for (int i = 0; i < 128; i++) {
        result[i] = encryptSymbol(text[i], i);
    }

    ofstream out("task2_encrypted.bin", ios::binary);
    out.write((char*)result, 128 * sizeof(unsigned short));
    out.close();

    cout << "Зашифровані слова (hex):" << endl;
    for (int i = 0; i < 128; i++) {
        cout << hex << setw(4) << setfill('0') << result[i] << " ";
        if (i % 16 == 15) {
            cout << endl;
        }
    }
    cout << dec << setfill(' ') << "Записано в task2_encrypted.bin (256 байтів)" << endl;
    return 0;
}
