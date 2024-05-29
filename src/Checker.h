// Created by Никита Степанов on 27.05.24.

#ifndef CHEK_CHECKER_H
#define CHEK_CHECKER_H

#include "Position.h"

// Класс для представления шашки
class Checker {
public:
    Position pos;  // Позиция шашки на доске
    bool isKing;   // Является ли шашка дамкой
    bool isWhite;  // Является ли шашка белой

    // Конструктор инициализирует позицию, цвет и статус дамки
    Checker(Position pos, bool isWhite, bool isKing = false);

    virtual ~Checker() = default; // Виртуальный деструктор

    // Метод возвращает символ, представляющий шашку
    virtual char getSymbol() const;
};

// Класс для представления дамки, наследуется от Checker
class KingChecker : public Checker {
public:
    KingChecker(Position pos, bool isWhite);

    // Переопределение метода для получения символа дамки
    char getSymbol() const override;
};

// Конструктор Checker, инициализирует поля
Checker::Checker(Position pos, bool isWhite, bool isKing)
        : pos(pos), isWhite(isWhite), isKing(isKing) {}

// Метод возвращает символ для обычной шашки и дамки
char Checker::getSymbol() const {
    return isWhite ? (isKing ? 'q' : 'w') : (isKing ? 'k' : 'b');
}

// Конструктор KingChecker, всегда задает isKing = true
KingChecker::KingChecker(Position pos, bool isWhite)
        : Checker(pos, isWhite, true) {}

// Переопределение метода для получения символа дамки
char KingChecker::getSymbol() const {
    return isWhite ? 'q' : 'k';
}

#endif //CHEK_CHECKER_H
