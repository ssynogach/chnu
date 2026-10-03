#pragma once
#include <string>

// ---- Допоміжні функції вводу з консолі та роботи з файловою системою ----

// Зчитує ціле число, повторюючи запит доки введено некоректні дані
int readInt(const std::string& prompt);

// Зчитує ціле число з перевіркою діапазону [lo, hi]
int readIntInRange(const std::string& prompt, int lo, int hi);

// Зчитує дійсне число
double readDouble(const std::string& prompt);

// Створює каталог "data", якщо його ще немає (там зберігаються всі файли)
void ensureDataDir();

// Пауза "натисніть Enter"
void pause();
