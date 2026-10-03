#include "utils.h"

#include <iostream>
#include <limits>
#include <filesystem>

int readInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            // прибираємо залишок рядка (символ переводу рядка тощо)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "  Помилка: введіть ціле число.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int readIntInRange(const std::string& prompt, int lo, int hi) {
    while (true) {
        int value = readInt(prompt);
        if (value >= lo && value <= hi) return value;
        std::cout << "  Помилка: значення має бути в межах ["
                  << lo << ", " << hi << "].\n";
    }
}

double readDouble(const std::string& prompt) {
    double value = 0.0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "  Помилка: введіть дійсне число.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void ensureDataDir() {
    std::error_code ec;
    std::filesystem::create_directory("data", ec);
}

void pause() {
    std::cout << "\n  Натисніть Enter, щоб продовжити...";
    std::cin.get();
}
