#include "zfunctional.h"
#include "zai.h"

namespace ZAITest {

using namespace ZLib::ZFunctional;
using namespace ZLib::ZAI;

enum Player {
    O = '0',
    X = 'X'
};

class TicTacMove;

class TicTacState : public IGameState<TicTacMove> {
    vector<char> state;
    Player curPlayer;
public:
    TicTacState();
    TicTacState(const vector<char>& s, Player startPlayer);
    TicTacState(const TicTacState& );
    bool HasPlayerWon(Player p) const;
    char GetCell(int n) const { return state[n]; }
    void SetCell(int n, char c) { state[n] = c; }
    void TogglePlayer() { curPlayer = curPlayer == O ? X : O; }

    bool IsLeafState() const;
    int32_t Evaluate() const;
    vector<TicTacMove> GetMoves();
    void ApplyMove(const TicTacMove& );
    void UndoMove(const TicTacMove& );
};

struct TicTacMove {
    int cell;
    Player player;
public:
    TicTacMove(int cellNum = -1, Player p = O) : cell(cellNum), player(p) {}
};

TicTacState::TicTacState() {
    state = vector<char >(9, 0);
    curPlayer = O;
}

TicTacState::TicTacState(const vector<char>& s, Player startPlayer) {
    state = s;
    curPlayer = startPlayer;
}

TicTacState::TicTacState(const TicTacState& s) {
    state = s.state;
    curPlayer = s.curPlayer;
}

bool TicTacState::HasPlayerWon(Player p) const {
    static vector<vector<int> > winningPositions =
    {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}
    };

    return any_of(all(winningPositions), [this, p](const vector<int>& a){
        return all_of(all(a), [this, p](int i){
            return state[i] == p;
        });
    });
}

bool TicTacState::IsLeafState() const {
    return HasPlayerWon(O) || HasPlayerWon(X) || none_of(all(state), [](const char c){
        return c == 0;
    });
}

vector<TicTacMove> TicTacState::GetMoves() {
    vector<TicTacMove> moves;
    for(int i = 0; i < 9; i++) {
        if (GetCell(i) == 0) {
            moves.push_back(TicTacMove(i, curPlayer));
        }
    }
    return moves;
}

void TicTacState::ApplyMove(const TicTacMove &m) {
    SetCell(m.cell, m.player);
    TogglePlayer();
}

void TicTacState::UndoMove(const TicTacMove &m) {
    SetCell(m.cell, 0);
    TogglePlayer();
}

int32_t TicTacState::Evaluate() const {
    Player otherPlayer = curPlayer == O ? X : O;

    if (HasPlayerWon(curPlayer)) {
        return 100;
    }
    if (HasPlayerWon(otherPlayer)) {
        return -100;
    }

    return 0;
}

}
