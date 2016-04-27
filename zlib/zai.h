#ifndef ZAI_H
#define ZAI_H

#include <functional>
#include <vector>
#include <cinttypes>
#include <limits>

namespace ZLib {
namespace ZAI {

using namespace  std;

template<class Move>
class IGameState {
public:

    // return true if no more moves left or any player has won
    virtual bool IsLeafState() const = 0;

    // return Score from the perspective of current player
    // current player:= player who has to move
    virtual int32_t Evaluate() const = 0;

    virtual vector<Move> GetMoves() = 0;
    virtual void ApplyMove(const Move& ) = 0;
    virtual void UndoMove(const Move& ) = 0;
};

class GameAlgorithms {
public:
    template<class Move>
    static Move NegaMax(IGameState<Move> * state, int depth);
private:
    template<class Move>
    static int32_t NegaMax(IGameState<Move> * state, int depth, int32_t alpha, int32_t beta);
};

template<class Move>
Move GameAlgorithms::NegaMax(IGameState<Move> * state, int depth) {
    Move bestMove;

    int32_t bestValue = numeric_limits<int32_t>::min();

    for(auto move : state->GetMoves()) {
        state->ApplyMove(move);
        // +adding +1 to min to avoid overflow when negating alpha beta values
        int32_t v = -NegaMax(state, depth, numeric_limits<int32_t>::min()+1, numeric_limits<int32_t>::max()-1);
        state->UndoMove(move);

        if (v > bestValue) {
            bestValue = v;
            bestMove = move;
        }
    }

    return bestMove;
}

template<class Move>
int32_t GameAlgorithms::NegaMax(IGameState<Move> * state, int depth, int32_t alpha, int32_t beta) {
    if (depth == 0 || state->IsLeafState()) {
        return state->Evaluate();
    }
    int32_t bestValue = numeric_limits<int32_t>::min();
    for(auto move : state->GetMoves()) {

        state->ApplyMove(move);
        int32_t v = -NegaMax(state, depth - 1, -beta, -alpha);
        state->UndoMove(move);
        bestValue = max(bestValue, v);
        alpha = max(alpha, v);
        if (alpha >= beta) {
            break;
        }
    }
    return alpha;
}

}
}
#endif
