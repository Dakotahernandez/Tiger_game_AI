#include <iostream>
using namespace std;

enum Color_t {RED, BLUE};

struct Point_t{
	int row, col;
};

struct Token_t{
	Color_t color;
	Point_t location;
};

struct Move_t{
	Token_t token;
	Point_t destination;
};

Move_t  Move(vector<Token_t>, Color_t turn);

/*
 * Each group will implement the Move function and
 * submit their code using the Upload Site.
 * You should name your move function: Move_GroupName.
 * This will create a function prototype something like:
 *  Move_t Move_GroupName(vector<Token_t>, Color_t turn);
 */

int main(){
  cout << "Tiger Game" << endl;
  return 0;
}
