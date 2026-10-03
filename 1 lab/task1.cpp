#include "task1.h"
#include "utils.h"
#include "arrays_io.h"

#include <iostream>
#include <vector>
#include <string>

// Імена файлів, з якими працює завдання 1
static const std::string T1_TEXT   = "data/task1_input.txt";
static const std::string T1_BIN    = "data/task1_input.bin";
static const std::string T1_RESULT = "data/task1_result.txt";

// ---------------------------------------------------------------------
// (a) Введення масиву з консолі та запис у ТЕКСТОВИЙ файл
// ---------------------------------------------------------------------
static void t1_inputFromConsole() {
    int n = readIntInRange("  Введіть кількість елементів масиву A (1..1000): N = ", 1, 1000);

    // динамічний масив
    double* a = new double[n];
    for (int i = 0; i < n; ++i) {
        a[i] = readDouble("  A[" + std::to_string(i) + "] = ");
    }

    printArray(a, n, "Введений масив A");
    if (writeArrayText(T1_TEXT, a, n))
        std::cout << "  Масив записано у текстовий файл: " << T1_TEXT << "\n";

    delete[] a;              // обов'язкове звільнення динамічної пам'яті
}

// ---------------------------------------------------------------------
// (b) Введення розміру з консолі, заповнення датчиком випадкових чисел
//     та запис у БІНАРНИЙ файл
// ---------------------------------------------------------------------
static void t1_randomToBinary() {
    int n = readIntInRange("  Введіть кількість елементів масиву A (1..1000): N = ", 1, 1000);

    double* a = new double[n];
    fillRandomDouble(a, n, -50.0, 50.0);

    printArray(a, n, "Згенерований масив A");
    if (writeArrayBinary(T1_BIN, a, n))
        std::cout << "  Масив записано у бінарний файл: " << T1_BIN << "\n";

    // Для зручності той самий масив дублюємо і в текстовий файл,
    // щоб можна було подивитися дані звичайним редактором.
    writeArrayText(T1_TEXT, a, n);

    delete[] a;
}

// ---------------------------------------------------------------------
// Ядро задачі: першу половину * 2, другу * 3.
// Якщо N непарне, "перша половина" — це N/2 елементів (цілочисельне
// ділення), середній елемент потрапляє у другу половину.
// ---------------------------------------------------------------------
static void multiplyHalves(double* a, int n) {
    int half = n / 2;

    double* pCur = a;                    // вказівник на поточний елемент
    double* pMid = a + half;             // межа між половинами
    double* pEnd = a + n;

    for (; pCur < pMid; ++pCur) *pCur *= 2.0;
    for (; pCur < pEnd; ++pCur) *pCur *= 3.0;
}

// ---------------------------------------------------------------------
// (c) Розв'язання задачі з використанням динамічних масивів.
//     Дані читаються з файлу, результат пишеться у НОВИЙ файл
//     і виводиться в консоль.
// ---------------------------------------------------------------------
static void t1_solve(bool fromBinary) {
    double* a = nullptr;
    int n = fromBinary ? readArrayBinary(T1_BIN, a)
                       : readArrayText(T1_TEXT, a);
    if (n < 0) return;
    if (n == 0) {
        std::cout << "  Масив порожній, нема чого обробляти.\n";
        delete[] a;
        return;
    }

    std::cout << "\n  Джерело даних: " << (fromBinary ? T1_BIN : T1_TEXT) << "\n";
    printArray(a, n, "Вхідний масив A");

    multiplyHalves(a, n);

    std::cout << "\n  Перші " << n / 2 << " елементів помножено на 2, "
              << "решту (" << n - n / 2 << ") — на 3.\n";
    printArray(a, n, "Результат");

    if (writeArrayText(T1_RESULT, a, n))
        std::cout << "  Результат записано у файл: " << T1_RESULT << "\n";

    delete[] a;
}

// ---------------------------------------------------------------------
// (d) Читання даних з файлу в контейнер vector та вивід у консоль
// ---------------------------------------------------------------------
static void t1_readToVector(bool fromBinary) {
    double* a = nullptr;
    int n = fromBinary ? readArrayBinary(T1_BIN, a)
                       : readArrayText(T1_TEXT, a);
    if (n < 0) return;

    // Переносимо дані у контейнер стандартної бібліотеки
    std::vector<double> v(a, a + n);
    delete[] a;                          // динамічний масив більше не потрібен

    std::cout << "\n  Дані завантажено у std::vector<double>.\n";
    printVector(v, "Вміст контейнера");

    std::cout << "  v.size() = " << v.size()
              << ", v.empty() = " << (v.empty() ? "true" : "false") << "\n";
    if (!v.empty())
        std::cout << "  v.front() = " << v.front()
                  << ", v.back() = " << v.back() << "\n";
}

// ---------------------------------------------------------------------
void task1Menu() {
    while (true) {
        std::cout << "\n---------------------------------------------------------\n"
                  << " ЗАВДАННЯ 1 (варіант 11)\n"
                  << " Першу половину елементів масиву A помножити на 2,\n"
                  << " а другу - на 3.\n"
                  << "---------------------------------------------------------\n"
                  << "  1 - Ввести масив з консолі та записати у текстовий файл\n"
                  << "  2 - Задати розмір, згенерувати випадково, записати у бінарний файл\n"
                  << "  3 - Розв'язати задачу (дані з ТЕКСТОВОГО файлу)\n"
                  << "  4 - Розв'язати задачу (дані з БІНАРНОГО файлу)\n"
                  << "  5 - Прочитати дані у контейнер vector та вивести\n"
                  << "  0 - Повернутися у головне меню\n";

        int choice = readIntInRange("  Ваш вибір: ", 0, 5);
        std::cout << "\n";
        switch (choice) {
            case 1: t1_inputFromConsole();  break;
            case 2: t1_randomToBinary();    break;
            case 3: t1_solve(false);        break;
            case 4: t1_solve(true);         break;
            case 5: {
                int src = readIntInRange("  Звідки читати? 1 - текстовий, 2 - бінарний: ", 1, 2);
                t1_readToVector(src == 2);
                break;
            }
            case 0: return;
        }
        pause();
    }
}
