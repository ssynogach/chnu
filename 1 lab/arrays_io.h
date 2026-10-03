#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

// =====================================================================
//  Узагальнені (шаблонні) функції введення-виведення масивів.
//  Формат ТЕКСТОВОГО файлу:   n
//                             a0 a1 a2 ... a(n-1)
//  Формат БІНАРНОГО файлу:    int n, далі n значень типу T підряд.
// =====================================================================

// ---------- запис масиву в текстовий файл ----------
template <typename T>
bool writeArrayText(const std::string& fileName, const T* a, int n) {
    std::ofstream out(fileName);
    if (!out) {
        std::cout << "  Не вдалося відкрити файл для запису: " << fileName << "\n";
        return false;
    }
    out << n << "\n";
    for (int i = 0; i < n; ++i)
        out << a[i] << (i + 1 < n ? ' ' : '\n');
    out.close();
    return true;
}

// ---------- читання масиву з текстового файлу ----------
// Пам'ять виділяється ДИНАМІЧНО всередині функції (оператор new[]).
// Повертає кількість елементів або -1 у разі помилки.
template <typename T>
int readArrayText(const std::string& fileName, T*& a) {
    a = nullptr;
    std::ifstream in(fileName);
    if (!in) {
        std::cout << "  Файл не знайдено: " << fileName
                  << "\n  Спочатку створіть вхідні дані (пункти 1 або 2 меню).\n";
        return -1;
    }
    int n = 0;
    if (!(in >> n) || n < 0) {
        std::cout << "  Пошкоджений формат файлу: " << fileName << "\n";
        return -1;
    }
    a = new T[n > 0 ? n : 1];
    for (int i = 0; i < n; ++i) {
        if (!(in >> a[i])) {
            std::cout << "  У файлі менше даних, ніж вказано (" << n << ").\n";
            delete[] a;
            a = nullptr;
            return -1;
        }
    }
    return n;
}

// ---------- запис масиву в бінарний файл ----------
template <typename T>
bool writeArrayBinary(const std::string& fileName, const T* a, int n) {
    std::ofstream out(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cout << "  Не вдалося відкрити файл для запису: " << fileName << "\n";
        return false;
    }
    out.write(reinterpret_cast<const char*>(&n), sizeof(n));
    if (n > 0)
        out.write(reinterpret_cast<const char*>(a), sizeof(T) * n);
    out.close();
    return true;
}

// ---------- читання масиву з бінарного файлу ----------
template <typename T>
int readArrayBinary(const std::string& fileName, T*& a) {
    a = nullptr;
    std::ifstream in(fileName, std::ios::in | std::ios::binary);
    if (!in) {
        std::cout << "  Файл не знайдено: " << fileName
                  << "\n  Спочатку створіть вхідні дані (пункти 1 або 2 меню).\n";
        return -1;
    }
    int n = 0;
    in.read(reinterpret_cast<char*>(&n), sizeof(n));
    if (!in || n < 0) {
        std::cout << "  Пошкоджений бінарний файл: " << fileName << "\n";
        return -1;
    }
    a = new T[n > 0 ? n : 1];
    if (n > 0) {
        in.read(reinterpret_cast<char*>(a), sizeof(T) * n);
        if (!in) {
            std::cout << "  У бінарному файлі менше даних, ніж вказано.\n";
            delete[] a;
            a = nullptr;
            return -1;
        }
    }
    return n;
}

// ---------- вивід масиву в консоль ----------
template <typename T>
void printArray(const T* a, int n, const std::string& title) {
    std::cout << "  " << title << " (n = " << n << "):\n  ";
    if (n == 0) {
        std::cout << "[порожній]\n";
        return;
    }
    for (int i = 0; i < n; ++i) {
        std::cout << std::setw(10) << a[i];
        if ((i + 1) % 8 == 0 && i + 1 < n) std::cout << "\n  ";
    }
    std::cout << "\n";
}

// ---------- вивід вмісту контейнера vector ----------
template <typename T>
void printVector(const std::vector<T>& v, const std::string& title) {
    std::cout << "  " << title << " (size = " << v.size() << "):\n  ";
    if (v.empty()) {
        std::cout << "[порожній]\n";
        return;
    }
    std::size_t i = 0;
    for (const T& item : v) {          // range-based for по контейнеру
        std::cout << std::setw(10) << item;
        if ((++i) % 8 == 0 && i < v.size()) std::cout << "\n  ";
    }
    std::cout << "\n";
}

// ---------- генератор випадкових чисел ----------
void fillRandomInt(int* a, int n, int lo, int hi);
void fillRandomDouble(double* a, int n, double lo, double hi);
