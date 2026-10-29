#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

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
    if (!f || n < 0) {
        cout << "Файл " << name << " пошкоджений!" << endl;
        n = 0;
        return NULL;
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
    cout << endl << "--- Завдання " << task << ".8 ---" << endl;
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
            int* a = new int[2 * n];
            for (int i = 0; i < n; i++) {
                cout << "A[" << i << "] = ";
                cin >> a[i];
            }
            for (int i = 0; i < n; i++) {
                cout << "B[" << i << "] = ";
                cin >> a[n + i];
            }
            writeI("task1_input.txt", a, 2 * n, false);
            delete[] a;
            cout << "Записано в task1_input.txt" << endl;
        }
        else if (choice == 2) {
            int n;
            cout << "Введіть n: ";
            cin >> n;
            int* a = new int[2 * n];
            for (int i = 0; i < 2 * n; i++) {
                a[i] = rand() % 41 - 20;
            }
            writeI("task1_input.bin", a, 2 * n, true);
            cout << "Згенерований масив A:" << endl;
            for (int i = 0; i < n; i++) {
                cout << a[i] << " ";
            }
            cout << endl << "Згенерований масив B:" << endl;
            for (int i = n; i < 2 * n; i++) {
                cout << a[i] << " ";
            }
            cout << endl;
            delete[] a;
            cout << "Записано в task1_input.bin" << endl;
        }
        else if (choice == 3 || choice == 4) {
            int total;
            int* a;
            if (choice == 3) {
                a = readI("task1_input.txt", total, false);
            } else {
                a = readI("task1_input.bin", total, true);
            }
            if (a != NULL) {
                if (total % 2 != 0) {
                    cout << "Масиви A і B мають різний розмір!" << endl;
                } else {
                    int n = total / 2;
                    int* pa = a;
                    int* pb = a + n;
                    int* c = new int[n];
                    int* pc = c;

                    cout << "Масив A:" << endl;
                    for (int i = 0; i < n; i++) {
                        cout << pa[i] << " ";
                    }
                    cout << endl << "Масив B:" << endl;
                    for (int i = 0; i < n; i++) {
                        cout << pb[i] << " ";
                    }
                    cout << endl;

                    while (pa < a + n) {
                        *pc = *pa * *pb;
                        pa++;
                        pb++;
                        pc++;
                    }

                    cout << "Масив C = A * B:" << endl;
                    for (int i = 0; i < n; i++) {
                        cout << c[i] << " ";
                    }
                    cout << endl;
                    writeI("task1_result.txt", c, n, false);
                    cout << "Результат записано в task1_result.txt" << endl;
                    delete[] c;
                }
                delete[] a;
            }
        }
        else if (choice == 5) {
            showI("task1_input.txt", false);
        }
        else if (choice == 6) {
            showI("task1_input.bin", true);
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

                int* p = a;
                int* end = a + n;
                int* best = NULL;
                while (p < end && *p % 2 == 0) {
                    if (best == NULL || *p > *best) {
                        best = p;
                    }
                    p++;
                }

                ofstream f("task2_result.txt");
                if (best == NULL) {
                    cout << "Парних елементів до першого непарного немає" << endl;
                    f << "Парних елементів до першого непарного немає" << endl;
                } else {
                    int index = best - a;
                    cout << "Максимальний парний елемент: " << *best << endl;
                    cout << "Номер елемента (з нуля): " << index << endl;
                    f << "Значення: " << *best << ", номер (з нуля): " << index << endl;
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

bool rootsPositive(int a) {
    long long d = 9LL * a * a - 20;
    if (d < 0) {
        return false;
    }
    return a < 0;
}

void task3() {
    int choice;
    do {
        printMenu(3);
        cin >> choice;

        if (choice == 1) {
            int n;
            do {
                cout << "Введіть n (до 200): ";
                cin >> n;
            } while (n < 1 || n > 200);
            int* a = new int[n];
            for (int i = 0; i < n; i++) {
                cout << "A[" << i << "] = ";
                cin >> a[i];
            }
            writeI("task3_input.txt", a, n, false);
            delete[] a;
            cout << "Записано в task3_input.txt" << endl;
        }
        else if (choice == 2) {
            int n;
            do {
                cout << "Введіть n (до 200): ";
                cin >> n;
            } while (n < 1 || n > 200);
            int* a = new int[n];
            for (int i = 0; i < n; i++) {
                a[i] = rand() % 41 - 20;
            }
            writeI("task3_input.bin", a, n, true);
            cout << "Згенерований масив:" << endl;
            for (int i = 0; i < n; i++) {
                cout << a[i] << " ";
            }
            cout << endl;
            delete[] a;
            cout << "Записано в task3_input.bin" << endl;
        }
        else if (choice == 3 || choice == 4) {
            int n;
            int* a;
            if (choice == 3) {
                a = readI("task3_input.txt", n, false);
            } else {
                a = readI("task3_input.bin", n, true);
            }
            if (a != NULL) {
                cout << "Масив A:" << endl;
                for (int i = 0; i < n; i++) {
                    cout << a[i] << " ";
                }
                cout << endl;

                int* b = new int[n];
                int m = 0;
                for (int i = 0; i < n; i++) {
                    if (rootsPositive(a[i])) {
                        b[m] = a[i];
                        m++;
                    }
                }

                ofstream f("task3_result.txt");
                if (m == 0) {
                    cout << "Таких чисел немає" << endl;
                    f << "Таких чисел немає" << endl;
                } else {
                    cout << "Масив B (по п'ять чисел у рядку):" << endl;
                    for (int i = 0; i < m; i++) {
                        cout << b[i] << " ";
                        f << b[i] << " ";
                        if (i % 5 == 4) {
                            cout << endl;
                            f << endl;
                        }
                    }
                    if (m % 5 != 0) {
                        cout << endl;
                        f << endl;
                    }
                    cout << "Розмір масиву B: " << m << endl;
                }
                f.close();
                cout << "Результат записано в task3_result.txt" << endl;
                delete[] b;
                delete[] a;
            }
        }
        else if (choice == 5) {
            showI("task3_input.txt", false);
        }
        else if (choice == 6) {
            showI("task3_input.bin", true);
        }
    } while (choice != 0);
}

int main() {
    srand(time(0));

    int choice;
    do {
        cout << endl << "===== Лабораторна робота 1, варіант 8 =====" << endl;
        cout << "1 - Завдання 1.8" << endl;
        cout << "2 - Завдання 2.8" << endl;
        cout << "3 - Завдання 3.8" << endl;
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
