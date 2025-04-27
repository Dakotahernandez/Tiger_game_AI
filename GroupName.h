#ifndef GroupName_h
#define GroupName_h

#include <vector>
#include "constants.h"

// ------------- Helper Functions -------------

// Check if a point is inside the 2×2 lair (rows 3–4, cols 3–4)
static bool inLair(const Point_t& p) {
    return p.row >= 3 && p.row <= 4
        && p.col >= 3 && p.col <= 4;
}

// Return true if the square is empty
static bool isEmpty(const Point_t& p,
                    const std::vector<Token_t>& state) {
    if (p.row < 0 || p.row > 7 || p.col < 0 || p.col > 7)
        return false;
    for (const auto& t : state)
        if (t.location.row == p.row && t.location.col == p.col)
            return false;
    return true;
}

// Return true if there's an opponent piece at p
static bool isOpponent(const Point_t& p,
                       Color_t turn,
                       const std::vector<Token_t>& state) {
    for (const auto& t : state)
        if (t.location.row == p.row && t.location.col == p.col
         && t.color != turn)
            return true;
    return false;
}

// ----------------- Move Generator -----------------

// Returns a vector of all legal Move_t (slides and captures) for 'turn'
static std::vector<Move_t> getValidMoves(const std::vector<Token_t>& state,
                                         Color_t turn) {
    std::vector<Move_t> valid;
    
    // Collect tokens belonging to `turn`
    std::vector<Token_t> mine;
    for (const auto& t : state)
        if (t.color == turn)
            mine.push_back(t);

    // Direction offsets
    const int drO[4] = {-1, 1, 0, 0};
    const int dcO[4] = { 0, 0,-1, 1};
    const int drD[4] = {-1,-1, 1, 1}; // diagonals
    const int dcD[4] = {-1, 1,-1, 1};

    for (const auto& tok : mine) {
        Point_t p = tok.location;
        for (int i = 0; i < 4; ++i) {
            Point_t adj{p.row + drO[i], p.col + dcO[i]};
            // slide
            if (isEmpty(adj, state)) {
                valid.push_back({tok, adj});
            }
            // jump-capture (tiger assumed RED)
            else if (turn == RED && isOpponent(adj, turn, state)) {
                Point_t land{p.row + 2*drO[i], p.col + 2*dcO[i]};
                if (isEmpty(land, state))
                    valid.push_back({tok, land});
            }
        }
        if (inLair(p)) {
            for (int i = 0; i < 4; ++i) {
                Point_t adj{p.row + drD[i], p.col + dcD[i]};
                if (isEmpty(adj, state)) {
                    valid.push_back({tok, adj});
                }
                else if (turn == RED && isOpponent(adj, turn, state)) {
                    Point_t land{p.row + 2*drD[i], p.col + 2*dcD[i]};
                    if (isEmpty(land, state))
                        valid.push_back({tok, land});
                }
            }
        }
    }

    return valid;
}

// ----------------- AI Entry Point -----------------

// This is the function called by the game engine
static Move_t Move_GroupName(const std::vector<Token_t>& state,
                             Color_t turn) {
    auto validMoves = getValidMoves(state, turn);
    // Use validMoves in your AI logic; here we just return the first as a placeholder
    if (!validMoves.empty())
        return validMoves.front();
    return Move_t{};
}

#endif /* GroupName_h */

