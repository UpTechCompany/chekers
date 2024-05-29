#include "src/CheckerGame.h"

int main() {
    CheckerGame game("../files/position.txt");
    game.findWinningSequences();

    CheckerGame game_with_error_position("../files/error_position.txt");
    game_with_error_position.findWinningSequences();

    CheckerGame game_with_white_position("../files/white_position.txt");
    game_with_white_position.findWinningSequences();

    CheckerGame game_with_duplicate_position("../files/duplicate_position.txt");
    game_with_duplicate_position.findWinningSequences();
    return 0;
}
