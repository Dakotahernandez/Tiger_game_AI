/*
 * Authors: Dakota Hernandez,
 *----------Sofia Amador
 *----------Chase Caldwell
 *----------Larry OConnor
 *----------Xiangyu_Wu
 * FileName: GroupName.h
 * File Description: Implements Move_GroupName with separate logic for RED
 *-------------------(tiger) and a placeholder for BLUE (men)
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
 * Authors:Dakota Hernandez Sofia Amador
 * description: check whether two points are the same
 * return: bool
 * precondition: points a and b are valid board positions
 * postcondition: returns true if rows and columns match, false otherwise
 */
static bool samePoint(const Point_t& a, const Point_t& b) {
    return a.row == b.row && a.col == b.col;
}

/*
 * Authors:Dakota Hernandez Sofia Amador
 * description:Calculates the Manhattan (grid-based) distance between two points
 * return: int
 * precondition: points a and b are valid board positions
 * postcondition: returns |a.row - b.row| + |a.col - b.col|
 */
static int manhattan(const Point_t& a, const Point_t& b) {
    return abs(a.row - b.row) + abs(a.col - b.col);
}

/*
 * Authors:Dakota Hernandez Sofia Amador
 * description: check if a point lies inside the tiger’s lair
 * return: bool
 * precondition: point p is a valid board position
 * postcondition: returns true if p is within rows 3–4 and cols 3–4,else false
 */
static bool inLair(const Point_t& p) {
    return p.row >= 3 && p.row <= 4 && p.col >= 3 && p.col <= 4;
}

/*
 * Authors:Dakota Hernandez Sofia Amador
 * description: determine if a board position is empty
 * return: bool
 * precondition: p within board bounds (0<=row<=12, 0<=col<=8), state contains
 *---------------valid tokens
 * postcondition: returns true if no token occupies p, false otherwise
 */
static bool isEmpty(const Point_t& p, const vector<Token_t>& state){
    bool ok = true;
    if(p.row < 0 || p.row > 12 || p.col < 0 || p.col > 8){
        ok = false;
    }else{
        for(auto& t : state){
            if(samePoint(t.location, p)){
                ok = false;
            }
        }
    }
    return ok;
}

// ------------ Tiger (RED) -------------

/*
 * Authors:Dakota Hernandez Sofia Amador
 * description: generate all valid moves for the tiger
 * return: vector<Move_t>
 * precondition: state contains valid tokens with correct colors
 * postcondition: returns moves into adjacent empty non-lair squares
 *---------------or capture jumps over BLUE tokens
 */
static vector<Move_t> getTigerValidMoves(const vector<Token_t>& state){
    vector<Move_t> valid;
    size_t menLeft = count_if(
        state.begin(), state.end(),
        [](auto& t){ return t.color == BLUE;}
    );
    const int dr[8] = {-1,-1,-1,0,1,1,1,0};
    const int dc[8] = {-1,0,1,1,1,0,-1,-1};
    for(auto& tok : state) {
        bool isTiger = (tok.color == RED);
        if(isTiger) {
            for(int i = 0; i < 8; ++i){
                Point_t adj{
                    tok.location.row +
                    dr[i], tok.location.col + dc[i] };
                if(isEmpty(adj, state) && (!inLair(adj) || menLeft <= 3)){
                    valid.push_back({tok, adj});
                }else{
                    Point_t land{
                        tok.location.row +
                        2*dr[i], tok.location.col
                        + 2*dc[i] };
                    if(isEmpty(land, state) && (!inLair(land) || menLeft <= 3)){
                        Point_t mid{ adj };
                        for(auto& m : state){
                            if(m.color == BLUE && samePoint(m.location, mid)){
                                valid.push_back({tok, land});
                            }
                        }
                    }
                }
            }
        }
    }
    return valid;
}

/*
 * Authors:Dakota Hernandez Sofia Amador
 * description: find the nearest BLUE token from a given point
 * return: Point_t
 * precondition: state contains at least one BLUE token, from is valid
 * postcondition: returns location of the closest BLUE token (Manhattan distance)
 */
static Point_t findNearestMan(
                              const vector<Token_t>& state,
                              const Point_t& from
                              ){
    int best = numeric_limits<int>::max();
    Point_t pick = from;
    for(auto& t : state){
        if(t.color == BLUE){
            int d = manhattan(from, t.location);
            if(d < best){
                best = d;
                pick = t.location;
            }
        }
    }
    return pick;
}

/*
 * Authors:Dakota Hernandez Sofia Amador
 * description: simulate a tiger move and check if
 *--------------it results in no further legal moves
 * return: bool
 * precondition: mv.origin is a RED token in the provided state
 * postcondition: returns true if, after applying mv
 *----------------(including removing any jumped BLUE), the tiger has zero moves
 */
static bool leadsToTrap(const Move_t& mv, const vector<Token_t>& state){
    vector<Token_t> next = state;
    for(auto& t : next) {
        if(t.color == RED && samePoint(t.location, mv.token.location)){
            t.location = mv.destination;
        }
    }
    int dr = mv.destination.row - mv.token.location.row;
    int dc = mv.destination.col - mv.token.location.col;
    if(abs(dr) == 2 || abs(dc) == 2){
        Point_t mid{ mv.token.location.row + dr/2, mv.token.location.col + dc/2 };
        next.erase(
            remove_if(
                next.begin(), next.end(),
                [&](auto& x){ return x.color == BLUE && samePoint(x.location, mid); }
            ), next.end()
        );
    }
    return getTigerValidMoves(next).empty();
}

/*
 * Authors:Dakota Hernandez Sofia Amador
 * description: compute the maximum number of sequential capture jumps the
 *--------------tiger can make from a given position
 * return: int
 * precondition: pos is the current location of a RED token; state contains
 *--------------valid RED and BLUE tokens
 * postcondition: returns the highest count of BLUE tokens the tiger can
 *----------------capture in a single move sequence
 */
static int bestJumpCount(const Point_t& pos, vector<Token_t> state){
    for(auto& t : state){
        if(t.color == RED){
            t.location = pos;
        }
    }
    int maxJumps = 0;
    const int dr[8] = {-1,-1,-1, 0, 1, 1, 1, 0};
    const int dc[8] = {-1, 0,  1, 1, 1, 0,-1,-1};
    for(int i = 0; i < 8; ++i){
        Point_t mid{ pos.row + dr[i], pos.col + dc[i] };
        Point_t land{ pos.row + 2*dr[i], pos.col + 2*dc[i] };
        if(isEmpty(land, state) && !inLair(land)){
            bool hasMan = false;
            for(auto& u : state){
                if(u.color == BLUE && samePoint(u.location, mid)){
                    hasMan = true;
                }
            }
            if(hasMan){
                vector<Token_t> nextState;
                nextState.reserve(state.size());
                for(auto& u : state){
                    if(!(u.color == BLUE && samePoint(u.location, mid))){
                        nextState.push_back(u);
                    }
                }
                int jumps = 1 + bestJumpCount(land, nextState);
                if(jumps > maxJumps){
                    maxJumps = jumps;
                }
            }
        }
    }
    return maxJumps;
}
// ------------ Tiger Move Extraction -------------

/*
 * Authors:Dakota Hernandez Sofia Amador
 * description: encapsulates all RED-turn logic into one function
 * return: Move_t
 * precondition: state contains valid RED and BLUE tokens
 * postcondition: returns the chosen tiger move
 */
static Move_t moveTiger(const vector<Token_t>& state) {
    static int tigerMoveCount = 0;
    Move_t result{};
    Point_t start{-1, -1};
    Token_t tigerToken{};
    for(auto& t : state){
        bool isTiger = (t.color == RED);
        if(isTiger){
            start = t.location;
            tigerToken = t;
        }
    }
    if (tigerMoveCount < 3) {
        Point_t dest = start;
        if(tigerMoveCount == 0){
            dest = {start.row + 1, start.col + 1};
        }else if(tigerMoveCount == 1){
            dest = {start.row + 1, start.col - 1};
        }else{
            dest = {start.row + 1, start.col};
        }
        
        result = {tigerToken, dest};
        tigerMoveCount++;
    }else{
        struct Candidate{
            Move_t mv; int jumpCount; int dist;
        };
        vector<Candidate> cands;
        for(auto& m : getTigerValidMoves(state)){
            bool trap = leadsToTrap(m, state);
            int dr = m.destination.row - m.token.location.row;
            int dc = m.destination.col - m.token.location.col;
            vector<Token_t> nextState = state;
            if (abs(dr) == 2 || abs(dc) == 2) {
                Point_t mid{
                    m.token.location.row + dr/2, m.token.location.col + dc/2
                };
                nextState.erase(
                    remove_if(
                        nextState.begin(), nextState.end(),
                              [&](auto& x){
                                  return x.color == BLUE
                                  && samePoint(x.location, mid);
                              }
                    ), nextState.end()
                );
            }
            if(!trap){
                int jumps = bestJumpCount(m.destination, nextState);
                Point_t nearest = findNearestMan(state, m.destination);
                int d = manhattan(m.destination, nearest);
                cands.push_back({m, jumps, d});
            }
        }
        if(!cands.empty()){
            sort(
                 cands.begin(), cands.end(),[]
                 (const Candidate& a,
                  const Candidate& b
                  ){
                if(a.jumpCount != b.jumpCount) return a.jumpCount > b.jumpCount;
                return a.dist < b.dist;
            });
            result = cands.front().mv;
        }
    }
    return result;
}
// ------------ Men (BLUE) Helpers -------------

/*
 * Authors:Dakota Hernandez
 * description: generate all valid moves for the men (BLUE)
 * return: vector<Move_t>
 * precondition: state contains valid tokens with correct colors
 * postcondition: returns a list of single-step moves into empty,
 *---------------- non-lair adjacent squares
 */
static vector<Move_t> getMenValidMoves(const vector<Token_t>& state) {
    vector<Move_t> valid;
    const int dr[8] = {-1, -1, -1,  0, 1, 1, 1,  0};
    const int dc[8] = {-1,  0,  1,  1, 1, 0,-1, -1};
    for(auto& tok : state){
        bool isMan = (tok.color == BLUE);
        if(isMan){
            for(int i = 0; i < 8; ++i){
                Point_t adj{tok.location.row + dr[i], tok.location.col + dc[i]};
                if(isEmpty(adj, state) && !inLair(adj)){
                    valid.push_back({ tok, adj });
                }
            }
        }
    }
    return valid;
}



// ------------ Men Move Extraction -------------

static Move_t moveMen(const vector<Token_t>& state){
    Move_t result{};
    
    //add yalls work here
    
    return result;
}
// ------------ Entry Point -------------

/*
 * Authors:Dakota Hernandez
 * description: choose and return the next move for the current turn
 * return: Move_t
 * precondition: state contains valid RED and BLUE tokens, turn is RED or BLUE
 * postcondition:if RED, delegates to moveTiger; if BLUE, placeholder empty move
 */
static Move_t Move_GroupName(const vector<Token_t>& state, Color_t turn){
    Move_t result{};
    if(turn == RED){
        result = moveTiger(state);
    }else{
        result = moveMen(state);
    }
    return result;
}

#endif // GroupName_h

