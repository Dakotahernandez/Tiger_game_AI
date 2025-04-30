/*
 * Authors: Dakota Hernandez
 * FileName: GroupName.h
 * File Description: Implements Move_GroupName with separate logic for RED (tiger) and a placeholder for BLUE (men)
 * Due Date: 05/04/2025
 * Date Created: 04/22/2025
 * Date Last Modified: 04/29/2025
 */
#ifndef GroupName_h
#define GroupName_h

#include <vector>
#include <algorithm>
#include <limits>
#include <utility>
#include "constants.h"

using namespace std;

// ------------ Generic Helpers -------------

/*
 * description: check whether two points are the same
 * return: bool
 * precondition: points a and b are valid board positions
 * postcondition: returns true if rows and columns match, false otherwise
 */
static bool samePoint(const Point_t& a, const Point_t& b) {
    return a.row == b.row && a.col == b.col;
}

/*
 * description: compute Manhattan distance between two points
 * return: int
 * precondition: points a and b are valid board positions
 * postcondition: returns |a.row - b.row| + |a.col - b.col|
 */
static int manhattan(const Point_t& a, const Point_t& b) {
    return abs(a.row - b.row) + abs(a.col - b.col);
}

/*
 * description: check if a point lies inside the tiger’s lair
 * return: bool
 * precondition: point p is a valid board position
 * postcondition: returns true if p is within rows 3–4 and cols 3–4, false otherwise
 */
static bool inLair(const Point_t& p) {
    return p.row >= 3 && p.row <= 4 && p.col >= 3 && p.col <= 4;
}

/*
 * description: determine if a board position is empty
 * return: bool
 * precondition: p within board bounds (0<=row<=12, 0<=col<=8), state contains valid tokens
 * postcondition: returns true if no token occupies p, false otherwise
 */
static bool isEmpty(const Point_t& p, const vector<Token_t>& state) {
    if (p.row < 0 || p.row > 12 || p.col < 0 || p.col > 8) return false;
    for (auto& t : state) {
        if (samePoint(t.location, p)) return false;
    }
    return true;
}

// ------------ Tiger (RED) Helpers -------------

/*
 * description: generate all valid moves for the tiger
 * return: vector<Move_t>
 * precondition: state contains valid tokens with correct colors
 * postcondition: returns moves into adjacent empty non-lair squares or capture jumps over BLUE tokens
 */
static vector<Move_t> getTigerValidMoves(const vector<Token_t>& state) {
    vector<Move_t> valid;
    const int dr[8] = {-1,-1,-1,0,1,1,1,0};
    const int dc[8] = {-1,0,1,1,1,0,-1,-1};

    for (auto& tok : state) {
        if (tok.color != RED) continue;
        for (int i = 0; i < 8; ++i) {
            Point_t adj{tok.location.row + dr[i], tok.location.col + dc[i]};
            if (isEmpty(adj, state) && !inLair(adj)) {
                valid.push_back({tok, adj});
            } else {
                Point_t mid{adj};
                Point_t land{tok.location.row + 2*dr[i], tok.location.col + 2*dc[i]};
                if (isEmpty(land, state) && !inLair(land)) {
                    for (auto& m : state) {
                        if (samePoint(m.location, mid) && m.color == BLUE) {
                            valid.push_back({tok, land});
                            break;
                        }
                    }
                }
            }
        }
    }
    return valid;
}

/*
 * description: find the nearest BLUE token from a given point
 * return: Point_t
 * precondition: state contains at least one BLUE token, from is valid
 * postcondition: returns location of the closest BLUE token (Manhattan distance)
 */
static Point_t findNearestMan(const vector<Token_t>& state, const Point_t& from) {
    int best = numeric_limits<int>::max();
    Point_t pick = from;
    for (auto& t : state) {
        if (t.color == BLUE) {
            int d = manhattan(from, t.location);
            if (d < best) {
                best = d;
                pick = t.location;
            }
        }
    }
    return pick;
}
/*
 * description: simulate a tiger move and check if it results in no further legal moves
 * return: bool
 * precondition: mv.origin is a RED token in the provided state
 * postcondition: returns true if, after applying mv (including removing any jumped BLUE), the tiger has zero valid moves
 */
static bool leadsToTrap(const Move_t& mv, const vector<Token_t>& state) {
    // 1) build a copy of the board
    vector<Token_t> next = state;

    // 2) find & update the tiger’s token
    for (auto& t : next) {
        if (t.color == RED && samePoint(t.location, mv.token.location)) {
            t.location = mv.destination;
            break;
        }
    }

    // 3) if this was a capture, remove the jumped‐over man
    int dr = mv.destination.row - mv.token.location.row;
    int dc = mv.destination.col - mv.token.location.col;
    if (abs(dr) == 2 || abs(dc) == 2) {
        Point_t mid{
            mv.token.location.row + dr/2,
            mv.token.location.col + dc/2
        };
        next.erase(
            remove_if(
                next.begin(), next.end(),
                [&](auto& x){ return x.color == BLUE && samePoint(x.location, mid); }
            ),
            next.end()
        );
    }

    // 4) if the tiger now has no valid moves, it’s trapped
    return getTigerValidMoves(next).empty();
}

/*
 * description: compute the maximum number of sequential capture jumps the tiger can make from a given position
 * return: int
 * precondition: pos is the current location of a RED token; state contains valid RED and BLUE tokens
 * postcondition: returns the highest count of BLUE tokens the tiger can capture in a single move sequence
 */
static int bestJumpCount(const Point_t& pos, vector<Token_t> state) {
    // place the tiger at the new position
    for (auto& t : state) {
        if (t.color == RED) {
            t.location = pos;
            break;
        }
    }

    int maxJumps = 0;
    const int dr[8] = {-1,-1,-1, 0, 1, 1, 1, 0};
    const int dc[8] = {-1, 0,  1, 1, 1, 0,-1,-1};

    // try each direction for a possible jump
    for (int i = 0; i < 8; ++i) {
        Point_t mid{ pos.row + dr[i],        pos.col + dc[i] };
        Point_t land{ pos.row + 2*dr[i], pos.col + 2*dc[i] };

        // can only jump if landing square is empty, not in lair, and there's a BLUE at mid
        if (isEmpty(land, state) && !inLair(land)) {
            bool hasMan = false;
            for (auto& u : state) {
                if (u.color == BLUE && samePoint(u.location, mid)) {
                    hasMan = true;
                    break;
                }
            }
            if (!hasMan) continue;

            // simulate the jump: remove the captured BLUE
            vector<Token_t> nextState;
            nextState.reserve(state.size());
            for (auto& u : state) {
                if (!(u.color == BLUE && samePoint(u.location, mid))) {
                    nextState.push_back(u);
                }
            }

            // recurse from the landing spot
            int jumps = 1 + bestJumpCount(land, nextState);
            maxJumps = max(maxJumps, jumps);
        }
    }

    return maxJumps;
}

// ------------ Men (BLUE) Helpers -------------

/*
 * description: generate all valid moves for the men (BLUE)
 * return: vector<Move_t>
 * precondition: state contains valid tokens with correct colors
 * postcondition: returns a list of single-step moves into empty, non-lair adjacent squares
 */
static vector<Move_t> getMenValidMoves(const vector<Token_t>& state) {
    vector<Move_t> valid;
    const int dr[8] = {-1, -1, -1,  0, 1, 1, 1,  0};
    const int dc[8] = {-1,  0,  1,  1, 1, 0,-1, -1};

    for (auto& tok : state) {
        if (tok.color != BLUE) continue;
        for (int i = 0; i < 8; ++i) {
            Point_t adj{ tok.location.row + dr[i],
                         tok.location.col + dc[i] };
            if (isEmpty(adj, state) && !inLair(adj)) {
                valid.push_back({ tok, adj });
            }
        }
    }
    return valid;
}

// ------------ Entry Point -------------

/*
 * description: choose and return the next move for the current turn
 * return: Move_t
 * precondition: state contains valid RED and BLUE tokens, turn is RED or BLUE
 * postcondition: if RED, returns tiger’s move; if BLUE, placeholder empty move
 */
static Move_t Move_GroupName(const vector<Token_t>& state, Color_t turn) {
    if (turn == RED) {
        // locate tiger
        Point_t start{-1,-1};
        Token_t tigerToken{};
        for (auto& t : state) {
            if (t.color == RED) {
                start      = t.location;
                tigerToken = t;
                break;
            }
        }

        // forced first three moves: exit lair
        static int tigerMoveCount = 0;
        if (tigerMoveCount < 3) {
            Point_t dest = start;
            if (tigerMoveCount == 0)      dest = {start.row + 1, start.col + 1};
            else if (tigerMoveCount == 1) dest = {start.row + 1, start.col - 1};
            else                          dest = {start.row + 1, start.col};
            ++tigerMoveCount;
            return {tigerToken, dest};
        }

        // collect and score candidates (multi-jump + trap avoidance)
        struct Candidate { Move_t mv; int jumpCount; int dist; };
        vector<Candidate> cands;
        for (auto& m : getTigerValidMoves(state)) {
            int jumps = bestJumpCount(m.token.location, state);
            if (leadsToTrap(m, state))
                continue;
            Point_t nearest = findNearestMan(state, m.destination);
            int d = manhattan(m.destination, nearest);
            cands.push_back({m, jumps, d});
        }

        if (cands.empty())
            return {};

        // sort by highest jumpCount, then shortest distance to nearest man
        sort(cands.begin(), cands.end(), [](const Candidate& a, const Candidate& b){
            if (a.jumpCount != b.jumpCount)
                return a.jumpCount > b.jumpCount;
            return a.dist < b.dist;
        });

        return cands.front().mv;
    }
    else if (turn == BLUE) {
        return {};
    }

    // fallback
    return {};
}

#endif // GroupName_h

