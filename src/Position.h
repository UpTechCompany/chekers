// Created by Никита Степанов on 27.05.24.

#ifndef CHEK_POSITION_H
#define CHEK_POSITION_H

#include <string>

// Класс для представления позиции на доске
class Position {
public:
    char x; // Координата по горизонтали (буква от 'A' до 'H')
    int y;  // Координата по вертикали (число от 1 до 8)

    // Конструктор инициализирует позицию с заданными координатами
    Position(char x, int y);

    // Метод для получения строки, представляющей позицию (например, "A1")
    std::string toString() const;

    // Оператор сравнения для проверки равенства двух позиций
    bool operator==(const Position &other) const;
};

// Реализация конструктора, инициализирует поля x и y
Position::Position(char x, int y) : x(x), y(y) {}

// Метод возвращает строковое представление позиции (например, "D4")
std::string Position::toString() const {
    return std::string(1, x) + std::to_string(y);
}

// Реализация оператора сравнения для позиций
bool Position::operator==(const Position &other) const {
    return x == other.x && y == other.y;
}

#endif //CHEK_POSITION_H
