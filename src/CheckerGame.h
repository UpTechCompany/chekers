// Created by Никита Степанов on 27.05.24.

#ifndef CHEK_CHECKERGAME_H
#define CHEK_CHECKERGAME_H

#include <vector>
#include <string>
#include <sstream>
#include "Board.h"
#include "Checker.h"
#include "Position.h"
#include <fstream>
#include <iostream>
#include <algorithm>

// Класс для управления игрой в шашки
class CheckerGame {
private:
    Board board; // Объект доски
    std::vector<Checker> whiteCheckers; // Вектор белых шашек
    std::vector<Checker> blackCheckers; // Вектор черных шашек

    // Метод для чтения ввода из файла
    void parseInput(const std::string &filename);

    // Метод для генерации возможных ходов
    std::vector<std::pair<Position, Position>> generateMoves(const Checker &checker) const;

    // Проверка на выигрышную позицию
    bool isWinningPosition(bool isWhite) const;

    // Метод для выполнения хода
    void performMove(Checker &checker, Position newPos, Position capturePos);

    // Метод для отмены хода
    void undoMove(Checker &checker, Position originalPos, Position capturedPos);

    // Метод для анализа возможных ходов
    void analyzeMoves(std::vector<Checker> &checkers, std::vector<Checker> &opponentCheckers, bool isWhite, int depth,
                      std::vector<std::vector<std::string>> &winningSequences,
                      std::vector<std::string> currentSequence = {});

    // Проверка, является ли позиция допустимой
    bool isPositionValid(const Position &pos) const;

    // Проверка, находится ли позиция на черной клетке
    static bool isOnBlackSquare(const Position &pos);

    // Проверка на уникальность позиции
    static bool isPositionUnique(const Position position, const std::vector<Checker> &checkers);

public:
    CheckerGame(const std::string &filename); // Конструктор инициализирует игру из файла

    void findWinningSequences(int depth = 6); // Метод для поиска выигрышных последовательностей
};

// Конструктор, вызывает метод для чтения ввода
CheckerGame::CheckerGame(const std::string &filename) {
    parseInput(filename);
}

int stringToInt(const std::string& str) {
    std::istringstream iss(str);
    int number;
    if (!(iss >> number)) {
        throw std::invalid_argument("Invalid input string");
    }
    return number;
}



// Проверка, является ли позиция допустимой
bool CheckerGame::isPositionValid(const Position &pos) const {
    return board.isInsideBoard(pos);
}

// Проверка, находится ли позиция на черной клетке
bool CheckerGame::isOnBlackSquare(const Position &pos) {
    return (pos.x - 'A' + pos.y) % 2 == 1;
}

// Проверка на уникальность позиции
bool CheckerGame::isPositionUnique(const Position position, const std::vector<Checker> &checkers) {
    for (const auto &ch: checkers) {
        if (ch.pos == position) {
            return false;
        }
    }
    return true;
}

// Метод для чтения ввода из файла и инициализации шашек
void CheckerGame::parseInput(const std::string &filename) {
    std::ifstream infile(filename);
    std::string line;
    bool isWhite = true;

    while (std::getline(infile, line)) {
        if (line.find("White:") != std::string::npos || line.find("Black:") != std::string::npos) {
            isWhite = (line.find("White:") != std::string::npos);
        } else if (!line.empty()) {
            try {
                bool isKing = line[0] == 'M';
                char x = isKing ? line[1] : line[0];
                int y = stringToInt(line.substr(isKing ? 2 : 1));
                Position pos(x, y);

                if (!isPositionValid(pos)) {
                    std::cerr << "Invalid position: " << pos.toString() << std::endl;
                    return;
                }

                if (!isOnBlackSquare(pos)) {
                    std::cerr << "Checker on white square: " << pos.toString() << std::endl;
                    return;
                }

                if (!isPositionUnique(pos, whiteCheckers) || !isPositionUnique(pos, blackCheckers)) {
                    std::cerr << "Duplicate checker position: " << pos.toString() << std::endl;
                    return;
                }

                Checker checker(pos, isWhite, isKing);


                (isWhite ? whiteCheckers : blackCheckers).push_back(checker);
            } catch (const std::invalid_argument &e) {
                std::cerr << "Invalid input format: " << line << std::endl;
                exit(0);
            }
        }
    }

    for (const auto &checker: whiteCheckers) {
        board.placeChecker(checker);
    }

    for (const auto &checker: blackCheckers) {
        board.placeChecker(checker);
    }
}

// Метод для генерации возможных ходов для шашки
std::vector<std::pair<Position, Position>> CheckerGame::generateMoves(const Checker &checker) const {
    std::vector<std::pair<Position, Position>> moves;
    int directions[4][2] = {{1,  1}, {1,  -1}, {-1, 1}, {-1, -1}};

    for (auto &dir: directions) {
        int dx = dir[0];
        int dy = dir[1];

        if (checker.isKing) {
            for (int dist = 1; dist < 8; ++dist) {
                Position newPos(checker.pos.x + dx * dist, checker.pos.y + dy * dist);
                if (!board.isInsideBoard(newPos)) break;
                if (board.isPositionEmpty(newPos)) {
                    moves.push_back(std::make_pair(newPos, Position(0, 0)));
                } else {
                    Position capturePos(checker.pos.x + dx * (dist + 1), checker.pos.y + dy * (dist + 1));
                    if (board.isInsideBoard(capturePos) && board.isPositionEmpty(capturePos)) {
                        std::string midChecker = board.getChecker(newPos);
                        if ((!checker.isWhite && (midChecker == "w" || midChecker == "kw")) ||
                            (checker.isWhite && (midChecker == "b" || midChecker == "kb"))) {
                            moves.push_back(std::make_pair(capturePos, newPos));
                        }
                    }
                    break;
                }
            }
        } else {
            Position newPos(checker.pos.x + dx, checker.pos.y + dy);
            if (board.isInsideBoard(newPos) && board.isPositionEmpty(newPos)) {
                if ((checker.isWhite && dy == 1) || (!checker.isWhite && dy == -1)) {
                    moves.push_back(std::make_pair(newPos, Position(0, 0)));
                }
            } else {
                Position capturePos(checker.pos.x + 2 * dx, checker.pos.y + 2 * dy);
                if (board.isInsideBoard(capturePos) && board.isPositionEmpty(capturePos)) {
                    std::string midChecker = board.getChecker(newPos);
                    if ((!checker.isWhite && (midChecker == "w" || midChecker == "kw")) ||
                        (checker.isWhite && (midChecker == "b" || midChecker == "kb"))) {
                        moves.push_back(std::make_pair(capturePos, newPos));
                    }
                }
            }
        }
    }

    return moves;
}

// Метод проверяет, является ли текущая позиция выигрышной
bool CheckerGame::isWinningPosition(bool isWhite) const {
    if (isWhite) {
        return blackCheckers.empty();
    } else {
        return whiteCheckers.empty();
    }
}

// Метод для выполнения хода
void CheckerGame::performMove(Checker &checker, Position newPos, Position capturePos) {
    if (capturePos.x != 0 && capturePos.y != 0) {
        auto &opponentCheckers = checker.isWhite ? blackCheckers : whiteCheckers;
        auto it = std::find_if(opponentCheckers.begin(), opponentCheckers.end(), [&](const Checker &ch) {
            return ch.pos == capturePos;
        });
        if (it != opponentCheckers.end()) {
            board.removeChecker(*it);
            opponentCheckers.erase(it);
        }
    }

    board.removeChecker(checker);
    checker.pos = newPos;
    board.placeChecker(checker);

    if ((checker.isWhite && newPos.y == 8) || (!checker.isWhite && newPos.y == 1)) {
        checker.isKing = true;
    }
}

// Метод для отмены хода
void CheckerGame::undoMove(Checker &checker, Position originalPos, Position capturedPos) {
    board.removeChecker(checker);
    checker.pos = originalPos;
    board.placeChecker(checker);

    if (capturedPos.x != 0 && capturedPos.y != 0) {
        Checker capturedChecker(capturedPos, !checker.isWhite);
        if (capturedPos.y == 1 || capturedPos.y == 8) {
            capturedChecker.isKing = true;
        }
        auto &opponentCheckers = checker.isWhite ? blackCheckers : whiteCheckers;
        opponentCheckers.push_back(capturedChecker);
        board.placeChecker(capturedChecker);
    }
}

// Метод для анализа возможных ходов
void CheckerGame::analyzeMoves(std::vector<Checker> &checkers, std::vector<Checker> &opponentCheckers, bool isWhite,
                               int depth, std::vector<std::vector<std::string>> &winningSequences,
                               std::vector<std::string> currentSequence) {
    if (depth == 0 || isWinningPosition(isWhite)) {
        if (isWinningPosition(isWhite)) {
            winningSequences.push_back(currentSequence);
        }
        return;
    }

    for (auto &checker: checkers) {
        std::vector<std::pair<Position, Position>> moves = generateMoves(checker);
        for (auto &move: moves) {
            std::string moveStr = checker.pos.toString() + " -> " + move.first.toString();
            currentSequence.push_back(moveStr);

            Checker original = checker;
            Position originalPos = checker.pos;
            Position capturedPos = move.second;

            performMove(checker, move.first, move.second);

            if (capturedPos.x != 0 && capturedPos.y != 0) {
                analyzeMoves(checkers, opponentCheckers, isWhite, depth - 1, winningSequences, currentSequence);
            } else {
                analyzeMoves(opponentCheckers, checkers, !isWhite, depth - 1, winningSequences, currentSequence);
            }

            undoMove(checker, originalPos, capturedPos);
            currentSequence.pop_back();
        }
    }
}

// Метод для поиска выигрышных последовательностей
void CheckerGame::findWinningSequences(int depth) {
    std::vector<std::vector<std::string>> winningSequences;
    analyzeMoves(whiteCheckers, blackCheckers, true, depth, winningSequences);

    if (winningSequences.empty()) {
        std::cout << "No winning sequences found." << std::endl;
    } else {
        std::cout << "Winning sequences for white:" << std::endl;
        for (const auto &seq: winningSequences) {
            for (const auto &move: seq) {
                std::cout << move << " ";
            }
            std::cout << std::endl;
        }
    }

    analyzeMoves(whiteCheckers, blackCheckers, false, depth, winningSequences);

    if (winningSequences.empty()) {
        std::cout << "No winning sequences found." << std::endl;
    } else {
        std::cout << "Winning sequences for black:" << std::endl;
        for (const auto &seq: winningSequences) {
            for (const auto &move: seq) {
                std::cout << move << " ";
            }
            std::cout << std::endl;
        }
    }
    board.display();
}

#endif //CHEK_CHECKERGAME_H
