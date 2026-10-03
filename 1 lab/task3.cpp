#include "task3.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>
#include <random>
#include <chrono>

static const std::string T3_TEXT   = "data/task3_input.txt";
static const std::string T3_BIN    = "data/task3_input.bin";
static const std::string T3_RESULT = "data/task3_result.txt";

static const int  T3_MAX_POINTS = 200;
static const double EPS = 1e-9;

// =====================================================================
//  Формат ТЕКСТОВОГО файлу:
//      n
//      x1 y1
//      ...
//      xn yn
//      xa ya xb yb xc yc      <- вершини трикутника
//
//  Формат БІНАРНОГО файлу:
//      int n, далі n структур Point, далі 3 структури Point (трикутник)
// =====================================================================

// ---------------------------------------------------------------------
// Векторний добуток (b-a) x (p-a). Знак показує, з якого боку від
// прямої AB лежить точка P.
// ---------------------------------------------------------------------
static double cross(const Point& a, const Point& b, const Point& p) {
    return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
}

// Точка належить трикутнику (разом із межею), якщо всі три векторні
// добутки мають однаковий знак (нуль означає "на стороні").
static bool pointInTriangle(const Point& p, const Point& A,
                            const Point& B, const Point& C) {
    double d1 = cross(A, B, p);
    double d2 = cross(B, C, p);
    double d3 = cross(C, A, p);

    bool hasNeg = (d1 < -EPS) || (d2 < -EPS) || (d3 < -EPS);
    bool hasPos = (d1 >  EPS) || (d2 >  EPS) || (d3 >  EPS);

    return !(hasNeg && hasPos);
}

static double triangleArea(const Point& A, const Point& B, const Point& C) {
    return std::fabs(cross(A, B, C)) / 2.0;
}

// ---------------------------------------------------------------------
// Запис / читання файлів
// ---------------------------------------------------------------------
static bool writeText(const std::string& file, const Point* pts, int n, const Point* tri) {
    std::ofstream out(file);
    if (!out) {
        std::cout << "  Не вдалося відкрити файл для запису: " << file << "\n";
        return false;
    }
    out << n << "\n";
    out << std::fixed << std::setprecision(4);
    for (int i = 0; i < n; ++i)
        out << pts[i].x << ' ' << pts[i].y << "\n";
    for (int i = 0; i < 3; ++i)
        out << tri[i].x << ' ' << tri[i].y << (i < 2 ? ' ' : '\n');
    out.close();
    return true;
}

static bool writeBinary(const std::string& file, const Point* pts, int n, const Point* tri) {
    std::ofstream out(file, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cout << "  Не вдалося відкрити файл для запису: " << file << "\n";
        return false;
    }
    out.write(reinterpret_cast<const char*>(&n), sizeof(n));
    if (n > 0)
        out.write(reinterpret_cast<const char*>(pts), sizeof(Point) * n);
    out.write(reinterpret_cast<const char*>(tri), sizeof(Point) * 3);
    out.close();
    return true;
}

// Повертає кількість точок або -1. Масив pts виділяється динамічно.
static int readText(const std::string& file, Point*& pts, Point* tri) {
    pts = nullptr;
    std::ifstream in(file);
    if (!in) {
        std::cout << "  Файл не знайдено: " << file
                  << "\n  Спочатку створіть вхідні дані (пункти 1 або 2 меню).\n";
        return -1;
    }
    int n = 0;
    if (!(in >> n) || n < 0 || n > T3_MAX_POINTS) {
        std::cout << "  Пошкоджений формат файлу: " << file << "\n";
        return -1;
    }
    pts = new Point[n > 0 ? n : 1];
    for (int i = 0; i < n; ++i) {
        if (!(in >> pts[i].x >> pts[i].y)) {
            std::cout << "  Недостатньо координат точок у файлі.\n";
            delete[] pts; pts = nullptr; return -1;
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (!(in >> tri[i].x >> tri[i].y)) {
            std::cout << "  У файлі відсутні координати вершин трикутника.\n";
            delete[] pts; pts = nullptr; return -1;
        }
    }
    return n;
}

static int readBinary(const std::string& file, Point*& pts, Point* tri) {
    pts = nullptr;
    std::ifstream in(file, std::ios::in | std::ios::binary);
    if (!in) {
        std::cout << "  Файл не знайдено: " << file
                  << "\n  Спочатку створіть вхідні дані (пункти 1 або 2 меню).\n";
        return -1;
    }
    int n = 0;
    in.read(reinterpret_cast<char*>(&n), sizeof(n));
    if (!in || n < 0 || n > T3_MAX_POINTS) {
        std::cout << "  Пошкоджений бінарний файл: " << file << "\n";
        return -1;
    }
    pts = new Point[n > 0 ? n : 1];
    if (n > 0) in.read(reinterpret_cast<char*>(pts), sizeof(Point) * n);
    in.read(reinterpret_cast<char*>(tri), sizeof(Point) * 3);
    if (!in) {
        std::cout << "  Бінарний файл неповний.\n";
        delete[] pts; pts = nullptr; return -1;
    }
    return n;
}

static void printPoints(const Point* pts, int n, const Point* tri) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Трикутник: A(" << tri[0].x << "; " << tri[0].y << ")  "
              << "B(" << tri[1].x << "; " << tri[1].y << ")  "
              << "C(" << tri[2].x << "; " << tri[2].y << ")\n";
    std::cout << "  Точки (n = " << n << "):\n";
    for (int i = 0; i < n; ++i)
        std::cout << "    P" << i << " (" << std::setw(8) << pts[i].x
                  << "; " << std::setw(8) << pts[i].y << ")\n";
}

// ---------------------------------------------------------------------
// (a) Введення з консолі та запис у текстовий файл
// ---------------------------------------------------------------------
static void t3_inputFromConsole() {
    int n = readIntInRange("  Введіть кількість точок (1..200): n = ", 1, T3_MAX_POINTS);

    Point* pts = new Point[n];
    for (int i = 0; i < n; ++i) {
        std::cout << "  Точка P" << i << ":\n";
        pts[i].x = readDouble("    x = ");
        pts[i].y = readDouble("    y = ");
    }

    Point tri[3];
    const char* names[3] = {"A", "B", "C"};
    for (int i = 0; i < 3; ++i) {
        std::cout << "  Вершина " << names[i] << ":\n";
        tri[i].x = readDouble("    x = ");
        tri[i].y = readDouble("    y = ");
    }

    printPoints(pts, n, tri);
    if (writeText(T3_TEXT, pts, n, tri))
        std::cout << "  Дані записано у текстовий файл: " << T3_TEXT << "\n";

    delete[] pts;
}

// ---------------------------------------------------------------------
// (b) Розмір з консолі + датчик випадкових чисел + бінарний файл
// ---------------------------------------------------------------------
static void t3_randomToBinary() {
    int n = readIntInRange("  Введіть кількість точок (1..200): n = ", 1, T3_MAX_POINTS);

    static std::mt19937 gen(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    Point* pts = new Point[n];
    for (int i = 0; i < n; ++i) {
        pts[i].x = dist(gen);
        pts[i].y = dist(gen);
    }

    // Трикутник беремо фіксований і достатньо великий, щоб частина
    // випадкових точок гарантовано потрапила всередину.
    Point tri[3] = { {-6.0, -5.0}, {7.0, -4.0}, {0.0, 8.0} };

    printPoints(pts, n, tri);
    if (writeBinary(T3_BIN, pts, n, tri))
        std::cout << "  Дані записано у бінарний файл: " << T3_BIN << "\n";

    writeText(T3_TEXT, pts, n, tri);   // дубль для перегляду

    delete[] pts;
}

// ---------------------------------------------------------------------
// (c) Розв'язання задачі на динамічних масивах
// ---------------------------------------------------------------------
static void t3_solve(bool fromBinary) {
    Point* pts = nullptr;
    Point tri[3];
    int n = fromBinary ? readBinary(T3_BIN, pts, tri)
                       : readText(T3_TEXT, pts, tri);
    if (n < 0) return;

    std::cout << "\n  Джерело даних: " << (fromBinary ? T3_BIN : T3_TEXT) << "\n";
    printPoints(pts, n, tri);

    double area = triangleArea(tri[0], tri[1], tri[2]);
    if (area < EPS) {
        std::cout << "\n  Вершини лежать на одній прямій — трикутник вироджений.\n";
        delete[] pts;
        return;
    }

    std::ofstream out(T3_RESULT);
    out << "Завдання 3, варіант 11\n";
    out << "Кількість точок, що належать трикутнику ABC\n\n";
    out << std::fixed << std::setprecision(4);
    out << "Трикутник: A(" << tri[0].x << "; " << tri[0].y << ") "
        << "B(" << tri[1].x << "; " << tri[1].y << ") "
        << "C(" << tri[2].x << "; " << tri[2].y << ")\n";
    out << "Площа трикутника: " << area << "\n\n";
    out << "Точки, що належать трикутнику:\n";

    int count = 0;
    Point* pCur = pts;               // вказівник на поточну точку
    Point* pEnd = pts + n;
    std::cout << "\n  Належать трикутнику:\n";
    for (int i = 0; pCur < pEnd; ++pCur, ++i) {
        if (pointInTriangle(*pCur, tri[0], tri[1], tri[2])) {
            ++count;
            std::cout << "    P" << i << " (" << pCur->x << "; " << pCur->y << ")\n";
            out << "P" << i << " (" << pCur->x << "; " << pCur->y << ")\n";
        }
    }
    if (count == 0) {
        std::cout << "    жодної точки\n";
        out << "(жодної)\n";
    }

    std::cout << "\n  Усього точок: " << n
              << ",  належать трикутнику: " << count << "\n";
    out << "\nУсього точок: " << n << "\nНалежать трикутнику: " << count << "\n";
    out.close();
    std::cout << "  Результат записано у файл: " << T3_RESULT << "\n";

    delete[] pts;
}

// ---------------------------------------------------------------------
// (d) Читання у контейнер vector та вивід у консоль
// ---------------------------------------------------------------------
static void t3_readToVector(bool fromBinary) {
    Point* pts = nullptr;
    Point tri[3];
    int n = fromBinary ? readBinary(T3_BIN, pts, tri)
                       : readText(T3_TEXT, pts, tri);
    if (n < 0) return;

    std::vector<Point> v(pts, pts + n);
    delete[] pts;

    std::cout << "\n  Дані завантажено у std::vector<Point>, size = "
              << v.size() << "\n";
    std::cout << std::fixed << std::setprecision(2);
    int i = 0;
    for (const Point& p : v)
        std::cout << "    P" << i++ << " (" << std::setw(8) << p.x
                  << "; " << std::setw(8) << p.y << ")\n";
    std::cout << "  Вершини трикутника: "
              << "A(" << tri[0].x << "; " << tri[0].y << ") "
              << "B(" << tri[1].x << "; " << tri[1].y << ") "
              << "C(" << tri[2].x << "; " << tri[2].y << ")\n";
}

// ---------------------------------------------------------------------
void task3Menu() {
    while (true) {
        std::cout << "\n---------------------------------------------------------\n"
                  << " ЗАВДАННЯ 3, додаткове (варіант 11)\n"
                  << " Обчислити кількість точок заданої множини,\n"
                  << " що належать трикутнику ABC.\n"
                  << "---------------------------------------------------------\n"
                  << "  1 - Ввести точки і трикутник з консолі -> текстовий файл\n"
                  << "  2 - Згенерувати точки випадково -> бінарний файл\n"
                  << "  3 - Розв'язати задачу (дані з ТЕКСТОВОГО файлу)\n"
                  << "  4 - Розв'язати задачу (дані з БІНАРНОГО файлу)\n"
                  << "  5 - Прочитати дані у контейнер vector та вивести\n"
                  << "  0 - Повернутися у головне меню\n";

        int choice = readIntInRange("  Ваш вибір: ", 0, 5);
        std::cout << "\n";
        switch (choice) {
            case 1: t3_inputFromConsole(); break;
            case 2: t3_randomToBinary();   break;
            case 3: t3_solve(false);       break;
            case 4: t3_solve(true);        break;
            case 5: {
                int src = readIntInRange("  Звідки читати? 1 - текстовий, 2 - бінарний: ", 1, 2);
                t3_readToVector(src == 2);
                break;
            }
            case 0: return;
        }
        pause();
    }
}
