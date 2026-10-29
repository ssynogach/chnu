#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

void writeD(const char* name, double* a, int n, bool binary) {
    if (binary) {
        ofstream f(name, ios::binary);
        f.write((char*)&n, sizeof(n));
        f.write((char*)a, n * sizeof(double));
        f.close();
    } else {
        ofstream f(name);
        f << n << endl;
        for (int i = 0; i < n; i++) {
            f << a[i] << " ";
        }
        f << endl;
        f.close();
    }
}

double* readD(const char* name, int& n, bool binary) {
    ifstream f;
    if (binary) {
        f.open(name, ios::binary);
    } else {
        f.open(name);
    }
    if (!f) {
        cout << "Файл " << name << " не знайдено!" << endl;
        n = 0;
        return NULL;
    }
    if (binary) {
        f.read((char*)&n, sizeof(n));
    } else {
        f >> n;
    }
    double* a = new double[n];
    if (binary) {
        f.read((char*)a, n * sizeof(double));
    } else {
        for (int i = 0; i < n; i++) {
            f >> a[i];
        }
    }
    f.close();
    return a;
}

void showD(const char* name, bool binary) {
    int n;
    double* a = readD(name, n, binary);
    if (a == NULL) {
        return;
    }
    vector<double> v;
    for (int i = 0; i < n; i++) {
        v.push_back(a[i]);
    }
    delete[] a;
    cout << "Вектор з файлу " << name << ":" << endl;
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

void writeI(const char* name, int* a, int n, bool binary) {
    if (binary) {
        ofstream f(name, ios::binary);
        f.write((char*)&n, sizeof(n));
        f.write((char*)a, n * sizeof(int));
        f.close();
    } else {
        ofstream f(name);
        f << n << endl;
        for (int i = 0; i < n; i++) {
            f << a[i] << " ";
        }
        f << endl;
        f.close();
    }
}

int* readI(const char* name, int& n, bool binary) {
    ifstream f;
    if (binary) {
        f.open(name, ios::binary);
    } else {
        f.open(name);
    }
    if (!f) {
        cout << "Файл " << name << " не знайдено!" << endl;
        n = 0;
        return NULL;
    }
    if (binary) {
        f.read((char*)&n, sizeof(n));
    } else {
        f >> n;
    }
    int* a = new int[n];
    if (binary) {
        f.read((char*)a, n * sizeof(int));
    } else {
        for (int i = 0; i < n; i++) {
            f >> a[i];
        }
    }
    f.close();
    return a;
}

void showI(const char* name, bool binary) {
    int n;
    int* a = readI(name, n, binary);
    if (a == NULL) {
        return;
    }
    vector<int> v;
    for (int i = 0; i < n; i++) {
        v.push_back(a[i]);
    }
    delete[] a;
    cout << "Вектор з файлу " << name << ":" << endl;
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

void printMenu(int task) {
    cout << endl << "--- Завдання " << task << ".12 ---" << endl;
    cout << "1 - ввести дані з клавіатури (текстовий файл)" << endl;
    cout << "2 - згенерувати дані (бінарний файл)" << endl;
    cout << "3 - розв'язати (з текстового файлу)" << endl;
    cout << "4 - розв'язати (з бінарного файлу)" << endl;
    cout << "5 - показати текстовий файл через vector" << endl;
    cout << "6 - показати бінарний файл через vector" << endl;
    cout << "0 - назад" << endl;
    cout << "Ваш вибір: ";
}

void task1() {
    int choice;
    do {
        printMenu(1);
        cin >> choice;

        if (choice == 1) {
            int n;
            cout << "Введіть n: ";
            cin >> n;
            double* a = new double[n];
            for (int i = 0; i < n; i++) {
                cout << "a[" << i << "] = ";
                cin >> a[i];
            }
            writeD("task1_input.txt", a, n, false);
            delete[] a;
            cout << "Записано в task1_input.txt" << endl;
        }
        else if (choice == 2) {
            int n;
            cout << "Введіть n: ";
            cin >> n;
            double* a = new double[n];
            for (int i = 0; i < n; i++) {
                a[i] = rand() % 201 - 100;
            }
            writeD("task1_input.bin", a, n, true);
            cout << "Згенерований масив:" << endl;
            for (int i = 0; i < n; i++) {
                cout << a[i] << " ";
            }
            cout << endl;
            delete[] a;
            cout << "Записано в task1_input.bin" << endl;
        }
        else if (choice == 3 || choice == 4) {
            int n;
            double* a;
            if (choice == 3) {
                a = readD("task1_input.txt", n, false);
            } else {
                a = readD("task1_input.bin", n, true);
            }
            if (a != NULL) {
                cout << "Початковий масив:" << endl;
                for (int i = 0; i < n; i++) {
                    cout << a[i] << " ";
                }
                cout << endl;

                if (n == 0) {
                    cout << "Масив порожній!" << endl;
                } else {
                    double sum = 0;
                    double* p = a;
                    double* end = a + n;
                    while (p < end) {
                        sum += *p;
                        p++;
                    }
                    double avg = sum / n;

                    cout << "Середнє арифметичне: " << avg << endl;
                    ofstream f("task1_result.txt");
                    f << "Середнє арифметичне: " << avg << endl;
                    f.close();
                    cout << "Результат записано в task1_result.txt" << endl;
                }
                delete[] a;
            }
        }
        else if (choice == 5) {
            showD("task1_input.txt", false);
        }
        else if (choice == 6) {
            showD("task1_input.bin", true);
        }
    } while (choice != 0);
}

void task2() {
    int choice;
    do {
        printMenu(2);
        cin >> choice;

        if (choice == 1) {
            int n;
            cout << "Введіть n: ";
            cin >> n;
            int* a = new int[n];
            for (int i = 0; i < n; i++) {
                cout << "a[" << i << "] = ";
                cin >> a[i];
            }
            writeI("task2_input.txt", a, n, false);
            delete[] a;
            cout << "Записано в task2_input.txt" << endl;
        }
        else if (choice == 2) {
            int n;
            cout << "Введіть n: ";
            cin >> n;
            int* a = new int[n];
            for (int i = 0; i < n; i++) {
                a[i] = rand() % 41 - 20;
            }
            writeI("task2_input.bin", a, n, true);
            cout << "Згенерований масив:" << endl;
            for (int i = 0; i < n; i++) {
                cout << a[i] << " ";
            }
            cout << endl;
            delete[] a;
            cout << "Записано в task2_input.bin" << endl;
        }
        else if (choice == 3 || choice == 4) {
            int n;
            int* a;
            if (choice == 3) {
                a = readI("task2_input.txt", n, false);
            } else {
                a = readI("task2_input.bin", n, true);
            }
            if (a != NULL) {
                cout << "Масив:" << endl;
                for (int i = 0; i < n; i++) {
                    cout << a[i] << " ";
                }
                cout << endl;

                int t1, t2;
                cout << "Введіть T1: ";
                cin >> t1;
                cout << "Введіть T2: ";
                cin >> t2;

                int* p = a;
                int* end = a + n;
                while (p < end && *p != t2) {
                    p++;
                }

                int* best = NULL;
                if (p < end) {
                    p++;
                    while (p < end) {
                        if (*p < t1) {
                            if (best == NULL || *p <= *best) {
                                best = p;
                            }
                        }
                        p++;
                    }
                }

                ofstream f("task2_result.txt");
                if (best == NULL) {
                    cout << "Такого елемента немає" << endl;
                    f << "Такого елемента немає" << endl;
                } else {
                    int index = best - a;
                    cout << "Номер елемента (з нуля): " << index << endl;
                    cout << "Номер елемента (з одиниці): " << index + 1 << endl;
                    cout << "Значення: " << *best << endl;
                    f << "Номер (з нуля): " << index << ", значення: " << *best << endl;
                }
                f.close();
                cout << "Результат записано в task2_result.txt" << endl;
                delete[] a;
            }
        }
        else if (choice == 5) {
            showI("task2_input.txt", false);
        }
        else if (choice == 6) {
            showI("task2_input.bin", true);
        }
    } while (choice != 0);
}

void task3() {
    int choice;
    do {
        printMenu(3);
        cin >> choice;

        if (choice == 1 || choice == 2) {
            int n, m;
            do {
                cout << "Кількість рядків n (до 100): ";
                cin >> n;
            } while (n < 1 || n > 100);
            do {
                cout << "Кількість стовпців m (до 15): ";
                cin >> m;
            } while (m < 1 || m > 15);

            int total = 1 + n * m;
            double* a = new double[total];
            a[0] = m;
            if (choice == 1) {
                for (int i = 0; i < n * m; i++) {
                    cout << "A(" << i + 1 << ") = ";
                    cin >> a[1 + i];
                }
                writeD("task3_input.txt", a, total, false);
                cout << "Записано в task3_input.txt" << endl;
            } else {
                for (int i = 0; i < n * m; i++) {
                    a[1 + i] = rand() % 201 - 100;
                }
                writeD("task3_input.bin", a, total, true);
                cout << "Згенеровано (перше число - m, далі n*m елементів):" << endl;
                for (int i = 0; i < total; i++) {
                    cout << a[i] << " ";
                }
                cout << endl;
                cout << "Записано в task3_input.bin" << endl;
            }
            delete[] a;
        }
        else if (choice == 3 || choice == 4) {
            int total;
            double* a;
            if (choice == 3) {
                a = readD("task3_input.txt", total, false);
            } else {
                a = readD("task3_input.bin", total, true);
            }
            if (a != NULL) {
                int m = (int)a[0];
                if (total < 2 || m < 1 || (total - 1) % m != 0) {
                    cout << "Неправильні дані у файлі!" << endl;
                } else {
                    int n = (total - 1) / m;
                    cout << "Масив " << n << " x " << m << ":" << endl;
                    for (int i = 0; i < n; i++) {
                        for (int j = 0; j < m; j++) {
                            cout << a[1 + i * m + j] << " ";
                        }
                        cout << endl;
                    }

                    double v = 0;
                    for (int i = 0; i < n; i++) {
                        double* p = a + 1 + i * m;
                        double mx = *p;
                        for (int j = 1; j < m; j++) {
                            if (p[j] > mx) {
                                mx = p[j];
                            }
                        }
                        cout << "max рядка " << i + 1 << " = " << mx << endl;
                        if (i == 0 || mx < v) {
                            v = mx;
                        }
                    }
                    cout << "V = " << v << endl;

                    ofstream f("task3_result.txt");
                    f << "V = " << v << endl;
                    f.close();
                    cout << "Результат записано в task3_result.txt" << endl;
                }
                delete[] a;
            }
        }
        else if (choice == 5) {
            showD("task3_input.txt", false);
        }
        else if (choice == 6) {
            showD("task3_input.bin", true);
        }
    } while (choice != 0);
}

int main() {
    srand(time(0));

    int choice;
    do {
        cout << endl << "===== Лабораторна робота 1, варіант 12 =====" << endl;
        cout << "1 - Завдання 1.12" << endl;
        cout << "2 - Завдання 2.12" << endl;
        cout << "3 - Завдання 3.12" << endl;
        cout << "0 - Вихід" << endl;
        cout << "Ваш вибір: ";
        cin >> choice;

        if (choice == 1) {
            task1();
        } else if (choice == 2) {
            task2();
        } else if (choice == 3) {
            task3();
        }
    } while (choice != 0);

    return 0;
}
