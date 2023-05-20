#pragma warning(disable : 4996)

#include <iostream>
#include <vector>
#include <string>
#include <windows.h> //Sleep(ms)
#include <conio.h>
#include <map>
#include <iomanip>
using namespace std;


class screen {



	
	

public :


	static void clear_screen()
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
	}


	static char screen_form(string Title,vector <string> elements) {

		
		string line = "===========================================\n";
		string Arrow = "        <--------";
		static char current_non_confirmed_chose = 0;
		bool confirmed = false;

	

		do {
			system("cls");

			cout << setw(20) << left << "" << line;
			cout << setw(20) << left << "" << "\t\t" << Title << "\n";
			cout << setw(20) << left << "" << line;

			for (size_t i = 0; i < elements.size(); i++)
			{

				cout << setw(20) << left << "" << "\t[" + to_string(i + 1) + "] " + elements[i];


				if (current_non_confirmed_chose == i) {
					cout << Arrow;
				}
				cout << endl;
			}

			cout << setw(20) << left << "" << line;

			cout << setw(20) << left << "" << "Press [Enter] to enter [" << elements[current_non_confirmed_chose] << "] screen\n\t\t or move the arrow to chose another screen";

			char ch = tolower(_getch());

			if (ch > 255 || ch < 0)
				ch = 0;
			else if ((ch == 'w') && current_non_confirmed_chose > 0)
				current_non_confirmed_chose--;
			else if ((ch == 's') && current_non_confirmed_chose < 4)
				current_non_confirmed_chose++;
			else if ((ch == '\r'))
				confirmed = true;
			else if (isdigit(ch) && ((ch - 48) <= 5) && ((ch - 48) >= 1))
				current_non_confirmed_chose = ch - 49;

		

		} while ((!confirmed)) ;


		system("cls");
		return current_non_confirmed_chose;
	}

	static enum enColor {
		enGray = 1,//8
		enWhight,  //F
		enGreen,   //2
		enBlue,    //1
		enYellow,  //6
		enPurple,  //5
		enLightRed,//C
		enRed,    //4
	};

	static void change_color(enColor color) {

		switch (color)
		{
		case enGray:system("color 08");
			break;
		case enWhight:system("color 0F");
			break;
		case enGreen:system("color 02");
			break;
		case enBlue:system("color 01");
			break;
		case enYellow:system("color 06");
			break;
		case enPurple:system("color 05");
			break;
		case enLightRed:system("color 0C");
			break;
		case enRed:system("color 04");
			break;

		}
	}
};

class Snake {

		vector<pair<char,char>> body;						// Positions of all snake nodes
		string board = "";									// String contain the frames of the game
		pair<char,char> tail,apple, old_apple;				// Tail : pos of last node on snake,apple & old_apple pos of current appel and old apple
		bool new_apple = true;								// Check if we have to delete preveus apple and put another one
		bool Ongoing = true;								// Check if game stile going
		long long int score = 0;
		int appel_reword = 0;								// Score added by eating 1 apple (change acouring to level ...)
		bool level_enable = 0;								// Enable or Desable level change in gameplay
		const char colms = 50, rows = 26;					// Real Space
		const char colm = 21, row = 23;						// Virtual Space


	void build_game() {

		// Called only one by the constructor
		// 1- put inital point for the snake
		// 2- put inital point for the apple
		// 3- build string (map)

		body.push_back({ 10, 10 });

		for (char i = 0; i < rows; i++)
		{
			for (char j = 0; j < colms; j++)
			{
				if (j == 0 || j == 1 || j == colms - 2 || j == colms - 3) {

					board += '#';
				}
				else if (j == colms - 1) {
					board += '\n';
				}
				else if (i == 0 || i == 1 || i == rows - 1 || i == rows - 2) {

					board += '#';
				}
			
				else {
					board += ' ';
				}
			}

			

		}

		old_apple = { 0,0 };
		apple = { rand() % colm,rand() % row };
		set(apple.first, apple.second, 'A');

		appel_reword = level;
	}

	void set(char x,char y,char pointer) {

		if (x > colm + 2 || x < 0 || y >= row || y < 0)
			return;

		board[(2 + y) * (colms)+2 + x * 2] = pointer;
	}

	void reset(char x, char y) {
		set(x, y, ' ');
	}

	char current_move = 'd', old_move = 'd';

	void test_if_snake_overlaped() {

		map<pair<char,char>,char> snake_pos_counter;

		for (size_t i = 0; i < body.size(); i++)
		{
			snake_pos_counter[body[i]]++;

			if (snake_pos_counter[body[i]] >1) {
				Ongoing = false;
				break;
			}
		}

	}

	void update_score() {
		score += appel_reword;
	}
	
	void update_reword() {
		appel_reword = level * level;
	}

	void update_apperance() {
		
		static char old_level = 1;

		if (old_level == level)
			return;

		old_level = level;

		screen::change_color(screen::enColor(level));
	}

	void move_snake_to_the_other_side_of_map() {

		if (body.front().first <= -1) {
			body.front().first = colm + 1;
		}
		if (body.front().first >= colm + 2) {
			body.front().first = 0;
		}

		if (body.front().second <= -1) {
			body.front().second = row - 2;
		}
		if (body.front().second >= row - 1) {
			body.front().second = 0;
		}
	}

	void move_snake_one_step() {

		tail = body.back();// save previeus tail pos

		for (char i = body.size() - 1; i > 0; i--)// Move the body one step
		{
			body[i] = body[i - 1];
		}

		// Move the head
		switch (current_move)
		{
		case'w': {
			body.front().second--;
			break;
		}

		case's': {
			body.front().second++;
			break;
		}

		case'a': {
			body.front().first--;
			break;
		}

		case'd': {
			body.front().first++;
			break;
		}
		}

	}

	void update_apple_status() {
		// Check If Apple ate and put new one
		if (body.front() == apple) {
			body.push_back(tail);
			old_apple = apple;
			apple = { rand() % (colm - 5),rand() % (row - 4) };

			new_apple = true;
			update_score();

		}
	}

	void update_game() {

		move_snake_one_step();
		update_apple_status();
		move_snake_to_the_other_side_of_map();
		test_if_snake_overlaped();
		update_reword();
		update_apperance();
	}

	void get_user_input() {

		char input = _getch();
		input = tolower(input);

		if (input == 'w' || input == 's') {

			if (current_move != 'w' && current_move != 's')
			{
				old_move = current_move;
				current_move = input;
			}
			else if ((current_move == input) && (level < 8) && level_enable) {
				level++;
			}
			else if ((level > 1) && (current_move != input) && level_enable) {
				level--;
			}
		
		}
		else if (input == 'd' || input == 'a') {

			if(current_move != 'd' && current_move != 'a')
			{
				old_move = current_move;
				current_move = input;
			}
			else if ((current_move == input) && (level < 8) && level_enable) {
				level++;
			}
			else if ((level > 1)&&(current_move != input) && level_enable) {
				level--;
			}
		}
		else if (input == 'l') {
			level_enable = !level_enable;
		}
		
	}

	void delay() {

		if(level < 8)
			Sleep(100/level);
		else
			Sleep(9);

	}


	void print() {

		reset(tail.first, tail.second);

		if (new_apple) {
			reset(old_apple.first, old_apple.second);
			set(apple.first, apple.second, 'A');
			new_apple = false;
		}

		set(body.front().first, body.front().second, 'S');
		set(body.back().first, body.back().second, 'S');
		screen::clear_screen();
		delay();

		cout << "Move Using 'W', 'S', 'D', 'A'\n";
		cout << "Level Lock : " << setw(3) << left << ((level_enable) ? "OFF" : "ON") << " (Press 'L' to turn lock OFF or ON)" << "\n" << "Level : " << level <<( (level == 8)? "(MAX)   " : "        ") ;
		cout << "Apples : " << setw(7) << body.size() - 1;
		cout << "Score : " << setw(10) << left << score << endl;
		cout << board << endl;


	}

public :

	Snake() {
		build_game();
	}

	int level = 2;

	void run() {

		bool block_input = true;

		do {

			while ((!_kbhit() || block_input) && Ongoing ) {
				update_game();
				print();
				block_input = false;
			}
			get_user_input();
			block_input = true;

		} while (Ongoing);
	}


};

enum enOptions {

	opPlay,
	opSettings,
	opBestScore,
	opREADME,
	opExit
};

void Play() {

	Snake s;
	s.run();
}

// Ongoing
void Settings() {

	screen::screen_form("\tSettings",{"Lock Level/op[ON:OFF]","Change Color with Level/op[ON:OFF]","Defult Level/op[1..8]"});

	system("pause>0");
}

// Empty
void BestScore() {

	cout << "Screen Will be here\n";
	system("pause>0");

}

// Empty
void README() {

	cout << "Screen Will be here\n";

	system("pause>0");

}

// Empty
void Exit() {

	cout << "Screen Will be here\n";

	system("pause>0");

}

void Perform_Option(enOptions choose) {

	switch (choose)
	{
	case opPlay: {
		Play();
		break;
	}
	case opSettings:
		Settings();
		break;
	case opBestScore:
		BestScore();
		break;
	case opREADME:
		README();
		break;
	case opExit:
		Exit();
		break;
	default:
		break;
	}
}

void Show_Menu() {

	char result = screen::screen_form("Welcome To Snake Game",{"Play","Settings","Best Score","README","Exit"});

	Perform_Option((enOptions)result);

}

void Play_Snake_Game() {

	while(1)
	Show_Menu();

	
}

int main() {

	srand(time(NULL));
	Play_Snake_Game();
	
	return 0;
}
