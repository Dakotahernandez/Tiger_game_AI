/*
 * Authors: Dakota Hernandez,
 *----------Sofia Amador
 *----------Chase Caldwell
 *----------Larry OConnor
 *----------Xiangyu_Wu
 * FileName: Team2.h
 * File Description: Implements Move_GroupName with separate logic for RED
 *-------------------(tiger) and a placeholder for BLUE (men)
 * Due Date: 05/04/2025
 * Date Created: 04/22/2025
 * Date Last Modified: 04/29/2025
 */
#ifndef Team2_h
#define Team2_h

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
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&#G555G#&&#G555G#&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@&GJ!~!7?JJJ?7~~7?JJJ?7!~!JG&@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@&J7?YPGPY7~~7Y55J7~:::!JPPJ!:::~7J55Y7~~!YPGPY?7J&@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@& 75Y?777J5P5?^.   .::.      .::.   .^?5P5J777?Y57 &@@@@@@@@@@@@@@
@@@@@@@@@@@@@@# 5GGPJ7~^:.    .^!7?YPG5775GPY?7!^.    .:^~7JPGG5 #@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@7.PGGGGY!:  .~~^:.    :7PP7:    .:^~~.  :!YGGGGP.7@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@?.?PGGP7. !PJ!~~^^.          .^^~~!JP! .7PGGP?.?@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@P !GG7.  :         ^J5??5J^         :  .7GG! P@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@7.7GP:    ~.  ??7^    .??.    ^7??  .~    :PG7.7@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@~ JG55?.  !B! .:!Y5~        ~5Y!:. !B!  .?5PGJ ~@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@#:.J5GG^     ^J:  :.^5.      .5^.:  :J^     ^GG5J.:#@@@@@@@@@@@@@@
@@@@@@@@@@@@@@&B7 !G~..   ~!.      .        .      .!~   ..~G! 7B&@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@.~GG57   . YG7.   7:        :7   .7GY .   75GG~.@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@G PGGY .?7  5GY:   !Y7J?~~?J7Y!   :YG5  7?. YGGP G@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@# YGG!:GY   ~B57~:.   :?GG?:   .:~75B~   YG:!GGY #@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@? GG5PG:    ^PGJ^.     55     .^JGP^    :GP5GP ?@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@# YGGGG: ^P7  :!5J :??PGGP??: J5!:  7P^ :GGGGY #@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@!.PGGGJ~BGG^!J^:G~Y7 .!!. 7J~G:^J!^GGB~JGGGP.!@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@J.7PGGGYPGGBGG.!BPP.    .PPB!.GGBGGPYGGGP7.J@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@&?^~J~ JBP?PGY P^~Y~^^~Y~^P YGP?PBJ ~J~^?&@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@#Y~7.J: ?GG^.7~~~~~~~~7.^GG? :J.7~Y#@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@&~^7 PBP. ..::::.. .PBP 7^~&@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@7.7PG5?!~^^~!?5GP7.7@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&Y~~!7?JJJJ?7!~~Y&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&BP5555PB&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

 */
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
 * Authors: Dakota Hernandez, Sofia Amador
 * description: encapsulates all RED-turn logic into one function
 * return: Move_t
 * precondition: state contains valid RED and BLUE tokens
 * postcondition: returns the chosen tiger move
 */
/**
 FULL EXPLANATION OF TIGER MOVE LOGIC
 STEP1)
 *--------Uses a static variable to keep track of how many moves the tiger has made so far.
 *--------Manually moves the tiger for the first 3 moves to get out of the lair
 *--------lair rules complicate early movement, so we drive it out before applying full logic.
 STEP 2)
 *--------Calls getTigerValidMoves(state) to list every legal step and capture jump,
 *--------then runs leadsToTrap(m, state) on each to simulate dropping any jumped BLUE and
 *-------- ensure the tiger still has follow-up moves—any move that would strand it is discarded.
 STEP 3)
 *--------For each remaining move, computes bestJumpCount from the landing square to find the
 *--------maximum chain-capture length, and uses findNearestMan + manhattan distance to score how
 *-------- close the tiger stays to its prey; these become the move’s jumpCount and dist metrics.
 STEP 4)
 *--------Sorts the surviving candidates by highest jumpCount (maximize captures) and then by lowest dist
 *--------(stay near enemies), then selects the front of that sorted list as the chosen move.
 STEP 5)
 *--------If no non-trapping candidates survive, falls back to the raw list from getTigerValidMoves and
 *--------returns its first element (or a default Move_t if empty),
 *-------- guaranteeing moveTiger always retyrns  a valid Move_t.
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
    //STEP 1)
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
        //STEP 2
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
            //STEP 3
            if(!trap){
                int jumps = bestJumpCount(m.destination, nextState);
                Point_t nearest = findNearestMan(state, m.destination);
                int d = manhattan(m.destination, nearest);
                cands.push_back({m, jumps, d});
            }
        }
        //STEP 4
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
    //STEP 5
    return result;
}
// ------------ Men (BLUE) -------------
/*
7:
~@Y
J@B.                .::.
J@#:             :YB&&&#G?.
Y@@~            :B@@@@@@@@G:
J@&~            !@@@@@@@@@@!
?@#^            Y@@@@@@@@@@P.
7@#^            7B@@@@@@@@#J.
?@@!             .5@@@@@@#^
7@@!              ~@@@@@@Y
:#B:            :!G@@@@@@#Y~.
.BG.        :!JG&@@@@@@@@@@@#GPJ!.
.BB.      7G&@@@@@@@@@@@@@@@@@@@@B~
.BB.     ?@@@@@@@@@@@@@@@@@@@@@@@@#:
.G#.    :#@@@@@@@@@@@@@@@@@@@@@@@@@7
G#~    ?@@@@@@@@@@@@@@@@@@@@@@@@@@Y
:Y&@@J  !&@@@@@@@@@@@@@@@@@@@@@@@@@@#:
Y@@@@B.~&@@@@&B@@@@@@@@@@@@@@@@&@@@@@Y
!#@@@&P#@@@@&!7@@@@@@@@@@@@@@@@PJ@@@@&!
:B@&@@@@@@&7 !@@@@@@@@@@@@@@@@Y P@@@@#^
G&!JG##B5~  7@@@@@@@@@@@@@@@@P ^&@@@@Y
.G@~  ...    Y@@@@@@@@@@@@@@@@&^ 5@@@@B.
P@~         P@@@@@@@@@@@@@@@@@B!:B@@@&^
5@!        ~&@@@@@@@@@@@@@@@@@@? ^#@@@!
5@!        Y@@@@@@@@@@@@@@@@@@@7  ^#@@Y
5@7       :#@@@@@@@@@@@@@@@@@@@G.  ~#@G.
5@7       :&@@@@@@@@@@@@@@@@@@@@!  .G@@~
Y@?       7@@@@@@@@@@@@@@@@@@@@@Y  !@@@5
Y@?      .P@@@@@@@@@@@@@@@@@@@@@#^ ~&G@&^
Y@?      7@@@@@@@@@@@@@@@@@@@@@@@J ?&B&?
Y@?      :G@@@@@@@@@@@@@@@@@@@@@&! .^~^
J@?       7@@@@@@@@@BYP@@@@@@@@@?
J@?       !@@@@@@@@@! :#@@@@@@@@~
J@J       ~@@@@@@@@#:  5@@@@@@@@~
?@J       :#@@@@@@@P   7@@@@@@@&^
?@Y        P@@@@@@@?   ^&@@@@@@B.
?@Y        Y@@@@@@B:    J@@@@@@G.
?@5        5@@@@@&~      5@@@@@&^
?@5       :#@@@@@&~      7@@@@@@J
7@P       ?@@@@@@&~      !@@@@@@P
7@P       J@@@@@@G.      ~&@@@@@G.
 */
/*
 * Authors:Dakota Hernandez
 * description: generate all valid moves for the men (BLUE)
 * return: vector<Move_t>
 * precondition: state contains valid tokens with correct colors
 * postcondition: returns a list of single-step moves into empty,
 *---------------- non-lair adjacent squares
 */
// ------------ Men (BLUE) Helpers -------------
static vector<Move_t> getMenValidMoves(const vector<Token_t>& state) {
    vector<Move_t> valid;
    const int dr[8] = { -1, -1, -1,  0, 1, 1, 1,  0 };
    const int dc[8] = { -1,  0,  1,  1, 1, 0, -1, -1 };
    for (auto& tok : state) {
        if (tok.color == BLUE) {
            for (int i = 0; i < 8; ++i) {
                Point_t adj{ tok.location.row + dr[i],
                             tok.location.col + dc[i] };
                if (isEmpty(adj, state) && !inLair(adj)) {
                    valid.push_back({ tok, adj });
                }
            }
        }
    }
    return valid;
}

// ------------ Men (BLUE) Move Extraction -------------
// Only forward moves (one row up, same column), choosing from the
// bottom‐most row that has any such move. No breaks/continues, single return.
/*
 * Authors: Dakota Hernandez
 * description: encapsulates all BLUE‐turn (men) logic into one function
 * return: Move_t
 * precondition: state contains valid RED and BLUE tokens
 * postcondition: returns the chosen men move
 */
/**
 FULL EXPLANATION OF MEN MOVE LOGIC
 STEP1)
 *--------Calls getMenValidMoves(state) and filters to only single‐step forward moves (row–1, same column).
 STEP2)
 *--------Groups those forward moves by each man’s original row, so we know which pawns are on which ranks.
 STEP3)
 *--------Finds the bottom‐most row (highest row index) that has any forward move available.
 STEP4)
 *--------Within that row, selects pawns in a round‐robin fashion (using a static counter) to ensure each moves in turn.
 STEP5)
 *--------Ensures men advance together, never allowing any man to move more than one row ahead of the slowest pawn.
 */
static Move_t moveMen(const vector<Token_t>& state) {
    Move_t result{};
    static int manTurn = 0;

    // 1) Gather only single‐step forward moves
    vector<Move_t> forwardMoves;
    for (auto& m : getMenValidMoves(state)) {
        if (m.destination.row == m.token.location.row - 1 &&
            m.destination.col == m.token.location.col) {
            forwardMoves.push_back(m);
        }
    }

    // 2) Bucket these by their original row
    vector<vector<Move_t>> byRow(13);
    for (auto& m : forwardMoves) {
        byRow[m.token.location.row].push_back(m);
    }

    // 3) Determine bottom‐most BLUE row
    int maxRow = -1;
    for (auto& t : state) {
        if (t.color == BLUE && t.location.row > maxRow) {
            maxRow = t.location.row;
        }
    }

    // 4) From bottom up, pick the next pawn (round‐robin via manTurn)
    int rowsChecked = 0;
    int rowCursor = maxRow;
    while (rowsChecked < 13) {
        if (!byRow[rowCursor].empty()) {
            // pick the manTurn-th pawn in this row (mod row size)
            int idx = manTurn % byRow[rowCursor].size();
            result = byRow[rowCursor][idx];
            manTurn = (manTurn + 1) % byRow[rowCursor].size();
            break;
        }
        rowCursor = (rowCursor > 0 ? rowCursor - 1 : 12);
        rowsChecked++;
    }

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
static Move_t Move_Team2(const vector<Token_t>& state, Color_t turn){
    Move_t result{};
    if(turn == RED){
        result = moveTiger(state);
    }else{
        result = moveMen(state);
    }
    return result;
}

#endif /* Team2_h */
