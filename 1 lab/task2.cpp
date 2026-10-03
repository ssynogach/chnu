#include "task2.h"
#include "utils.h"
#include "arrays_io.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

static const std::string T2_TEXT   = "data/task2_input.txt";
static const std::string T2_BIN    = "data/task2_input.bin";
static const std::string T2_RESULT = "data/task2_result.txt";

// Коди результату розв'язання
enum class T2Status {
    Ok,             // знайдено
    NoNegative,     // у масиві немає жодного від'ємного елемента
    NoCandidates    // від'ємний є, але праворуч немає парних додатних
};

// ---------------------------------------------------------------------
// (a) Введення масиву з консолі та запис у текстовий файл
// ---------------------------------------------------------------------
static void t2_inputFromConsole() {
    int n = readIntInRange("  Введіть кількість елементів масиву A (1..1000): N = ", 1, 1000);

    int* a = new int[n];
    for (int i = 0; i < n; ++i)
        a[i] = readInt("  A[" + std::to_string(i) + "] = ");

    printArray(a, n, "Введений масив A");
    if (writeArrayText(T2_TEXT, a, n))
        std::cout << "  Масив записано у текстовий файл: " << T2_TEXT << "\n";

    delete[] a;
}

// ---------------------------------------------------------------------
// (b) Розмір з консолі + датчик випадкових чисел + бінарний файл
// ---------------------------------------------------------------------
static void t2_randomToBinary() {
    int n = readIntInRange("  Введіть кількість елементів масиву A (1..1000): N = ", 1, 1000);

    int* a = new int[n];
    fillRandomInt(a, n, -20, 40);   // діапазон дібрано так, щоб траплялись
                                    // і від'ємні, і парні додатні числа

    printArray(a, n, "Згенерований масив A");
    if (writeArrayBinary(T2_BIN, a, n))
        std::cout << "  Масив записано у бінарний файл: " << T2_BIN << "\n";

    writeArrayText(T2_TEXT, a, n);  // дубль у текстовому вигляді для перегляду

    delete[] a;
}

// ---------------------------------------------------------------------
// Ядро задачі. Уся обробка — через змінну-вказівник pCur, яка вказує
// на поточний елемент масиву (як у прикладі 5 методичних вказівок).
// ---------------------------------------------------------------------
static T2Status solveTask2(const int* a, int n, int& index, int& value) {
    const int* pCur = a;             // вказівник на поточний елемент
    const int* pEnd = a + n;         // за останнім елементом

    // 1) шукаємо перший від'ємний елемент
    while (pCur < pEnd && *pCur >= 0)
        ++pCur;

    if (pCur == pEnd)
        return T2Status::NoNegative; // від'ємних елементів немає

    ++pCur;                          // переходимо ПРАВІШЕ першого від'ємного

    // 2) серед парних додатних шукаємо останній мінімальний.
    //    Умова "<=" гарантує, що при однакових значеннях запам'ятається
    //    саме ОСТАННІЙ з них.
    const int* pMin = nullptr;
    for (; pCur < pEnd; ++pCur) {
        if (*pCur > 0 && *pCur % 2 == 0) {
            if (pMin == nullptr || *pCur <= *pMin)
                pMin = pCur;
        }
    }

    if (pMin == nullptr)
        return T2Status::NoCandidates;

    index = static_cast<int>(pMin - a);   // адресна арифметика дає індекс
    value = *pMin;
    return T2Status::Ok;
}

// ---------------------------------------------------------------------
// (c) Розв'язання: дані з файлу, результат у новий файл + консоль
// ---------------------------------------------------------------------
static void t2_solve(bool fromBinary) {
    int* a = nullptr;
    int n = fromBinary ? readArrayBinary(T2_BIN, a)
                       : readArrayText(T2_TEXT, a);
    if (n < 0) return;
    if (n == 0) {
        std::cout << "  Масив порожній.\n";
        delete[] a;
        return;
    }

    std::cout << "\n  Джерело даних: " << (fromBinary ? T2_BIN : T2_TEXT) << "\n";
    printArray(a, n, "Вхідний масив A");

    int index = -1, value = 0;
    T2Status status = solveTask2(a, n, index, value);

    std::ofstream out(T2_RESULT);
    out << "Завдання 2, варіант 11\n";
    out << "Номер останнього мінімального елемента серед парних додатних,\n";
    out << "розташованих правіше першого від'ємного елемента.\n\n";
    out << "Вхідний масив (n = " << n << "):\n";
    for (int i = 0; i < n; ++i) out << a[i] << (i + 1 < n ? ' ' : '\n');
    out << "\n";

    std::cout << "\n  Результат:\n";
    switch (status) {
        case T2Status::Ok:
            std::cout << "    значення шуканого елемента : " << value << "\n"
                      << "    індекс (від 0)             : " << index << "\n"
                      << "    номер  (від 1)             : " << index + 1 << "\n";
            out << "Значення: " << value << "\n"
                << "Індекс (від 0): " << index << "\n"
                << "Номер (від 1): " << index + 1 << "\n";
            break;
        case T2Status::NoNegative:
            std::cout << "    у масиві немає жодного від'ємного елемента —\n"
                      << "    задача розв'язку не має.\n";
            out << "У масиві немає від'ємних елементів - розв'язку не існує.\n";
            break;
        case T2Status::NoCandidates:
            std::cout << "    правіше першого від'ємного елемента немає\n"
                      << "    парних додатних чисел — задача розв'язку не має.\n";
            out << "Правіше першого від'ємного немає парних додатних чисел.\n";
            break;
    }
    out.close();
    std::cout << "  Результат записано у файл: " << T2_RESULT << "\n";

    delete[] a;
}

// ---------------------------------------------------------------------
// (d) Читання у контейнер vector та вивід у консоль
// ---------------------------------------------------------------------
static void t2_readToVector(bool fromBinary) {
    int* a = nullptr;
    int n = fromBinary ? readArrayBinary(T2_BIN, a)
                       : readArrayText(T2_TEXT, a);
    if (n < 0) return;

    std::vector<int> v(a, a + n);
    delete[] a;

    std::cout << "\n  Дані завантажено у std::vector<int>.\n";
    printVector(v, "Вміст контейнера");

    // Демонстрація роботи з ітераторами
    if (!v.empty()) {
        std::cout << "  Перебір через ітератор: ";
        for (auto it = v.cbegin(); it != v.cend(); ++it)
            std::cout << *it << ' ';
        std::cout << "\n";
    }
}

// ---------------------------------------------------------------------
void task2Menu() {
    while (true) {
        std::cout << "\n---------------------------------------------------------\n"
                  << " ЗАВДАННЯ 2 (варіант 11)\n"
                  << " Знайти номер останнього мінімального елемента серед\n"
                  << " парних додатних елементів, що лежать правіше першого\n"
                  << " від'ємного елемента.\n"
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
            case 1: t2_inputFromConsole(); break;
            case 2: t2_randomToBinary();   break;
            case 3: t2_solve(false);       break;
            case 4: t2_solve(true);        break;
            case 5: {
                int src = readIntInRange("  Звідки читати? 1 - текстовий, 2 - бінарний: ", 1, 2);
                t2_readToVector(src == 2);
                break;
            }
            case 0: return;
        }
        pause();
    }
}
