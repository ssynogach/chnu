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

unsigned short encryptSymbol(unsigned char c, unsigned short row, unsigned short pos) {
    unsigned short r = 0;
    r |= row;
    r |= (c >> 4) << 2;
    r |= parity(r & 0x3F) << 6;
    r |= (c & 0x0F) << 7;
    r |= pos << 11;
    r |= parity((r >> 7) & 0xFF) << 15;
    return r;
}

int main() {
    string lines[4];
    int choice;
    cout << "1 - ввести 4 рядки з клавіатури" << endl;
    cout << "2 - прочитати з файлу task2_input.txt" << endl;
    cout << "Ваш вибір: ";
    cin >> choice;
    cin.ignore(1000, '\n');

    if (choice == 1) {
        ofstream in("task2_input.txt");
        for (int i = 0; i < 4; i++) {
            cout << "Рядок " << i + 1 << " (до 16 символів): ";
            getline(cin, lines[i]);
            in << lines[i] << endl;
        }
        in.close();
    } else {
        ifstream in("task2_input.txt");
        if (!in) {
            cout << "Файл task2_input.txt не знайдено!" << endl;
            return 1;
        }
        for (int i = 0; i < 4; i++) {
            getline(in, lines[i]);
        }
        in.close();
    }

    unsigned short result[64];
    int k = 0;
    cout << "Текст (доповнений пробілами):" << endl;
    for (int i = 0; i < 4; i++) {
        while (lines[i].size() < 16) {
            lines[i] += ' ';
        }
        lines[i] = lines[i].substr(0, 16);
        cout << "|" << lines[i] << "|" << endl;
        for (int j = 0; j < 16; j++) {
            result[k] = encryptSymbol(lines[i][j], i, j);
            k++;
        }
    }

    ofstream out("task2_encrypted.bin", ios::binary);
    out.write((char*)result, 64 * sizeof(unsigned short));
    out.close();

    cout << "Зашифровані слова (hex):" << endl;
    for (int i = 0; i < 64; i++) {
        cout << hex << setw(4) << setfill('0') << result[i] << " ";
        if (i % 16 == 15) {
            cout << endl;
        }
    }
    cout << dec << setfill(' ') << "Записано в task2_encrypted.bin (128 байтів)" << endl;
    return 0;
}
