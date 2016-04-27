#include <iostream>
#include "zai_test.h"
#include "zai.h"

using namespace ZLib::ZAI;
using namespace ZAITest;

int main() {
    vector<char> pos = {0, O, X,
                   X, X, 0,
                   O, 0, 0};

    TicTacState state(pos, O);    

    TicTacMove move = GameAlgorithms::NegaMax(&state, 9);
    std::cout << move.cell << " " << move.player << std::endl;
    return 0;
}
