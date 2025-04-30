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
        Point_t start{-1,-1}; Token_t tiger{};
        for (auto& t : state) {
            if (t.color == RED) {
                start = t.location;
                tiger = t;
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
            return {tiger, dest};
        }

        // choose move minimizing distance to nearest man
        auto vm = getTigerValidMoves(state);
        if (vm.empty()) return {};
        int bestDist = numeric_limits<int>::max();
        Move_t bestMove = vm.front();
        for (auto& m : vm) {
            Point_t target = findNearestMan(state, m.destination);
            int d = manhattan(m.destination, target);
            if (d < bestDist) {
                bestDist = d;
                bestMove = m;
            }
        }
        return bestMove;
    }
    else if (turn == BLUE) {
        // TODO: add BLUE/men move selection here
    }

    return {};
}

#endif // GroupName_h

