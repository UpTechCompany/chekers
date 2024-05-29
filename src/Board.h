// Created by Никита Степанов on 27.05.24.

#ifndef CHEK_BOARD_H
#define CHEK_BOARD_H

#include <vector>
#include <string>
#include <iostream>
#include "Checker.h"
#include "Position.h"

// Класс для представления доски
class Board {
public:
    std::vector<std::vector<std::string>> grid; // Сетка доски

    // Конструктор инициализирует пустую доску
    Board();

    // Методы для размещения и удаления шашек
    void placeChecker(const Checker &checker);
    void removeChecker(const Checker &checker);

    // Метод для отображения доски
    void display() const;

    // Проверка, находится ли позиция внутри доски
    bool isInsideBoard(Position pos) const;

    // Проверка, пуста ли позиция
    bool isPositionEmpty(Position pos) const;

    // Получение символа шашки на данной позиции
    std::string getChecker(Position pos) const;
};

// Конструктор инициализирует сетку доски
Board::Board() {
    grid = std::vector<std::vector<std::string>>(8, std::vector<std::string>(8, "#"));
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2) {
                grid[i][j] = ".";
            }
        }
    }
}

// Метод размещает шашку на доске
void Board::placeChecker(const Checker &checker) {
    char symbol = checker.getSymbol();
    grid[8 - checker.pos.y][checker.pos.x - 'A'] = symbol;
}

// Метод удаляет шашку с доски
void Board::removeChecker(const Checker &checker) {
    grid[8 - checker.pos.y][checker.pos.x - 'A'] = ".";
}

// Метод отображает доску в консоли
void Board::display() const {
    std::cout << "    A   B   C   D   E   F   G   H" << std::endl;
    std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;

    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " |";
        for (int j = 0; j < 8; ++j) {
            std::cout << " " << grid[i][j] << " |";
        }
        std::cout << " " << 8 - i << std::endl;
        std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    }

    std::cout << "    A   B   C   D   E   F   G   H" << std::endl;
    std::cout << "\n";
    std::cout << "w - white checker\n";
    std::cout << "q - white king checker\n";
    std::cout << "b - black checker\n";
    std::cout << "k - black king checker\n";
    std::cout << ". - black square\n";
    std::cout << "# - white square\n";
}

// Метод проверяет, находится ли позиция внутри доски
bool Board::isInsideBoard(Position pos) const {
    return pos.x >= 'A' && pos.x <= 'H' && pos.y >= 1 && pos.y <= 8;
}

// Метод проверяет, пуста ли позиция на доске
bool Board::isPositionEmpty(Position pos) const {
    return grid[8 - pos.y][pos.x - 'A'] == ".";
}

// Метод возвращает символ шашки на данной позиции
std::string Board::getChecker(Position pos) const {
    return grid[8 - pos.y][pos.x - 'A'];
}

#endif //CHEK_BOARD_H
