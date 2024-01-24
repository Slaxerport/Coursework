#include <iostream>
#include <string>

using namespace std;

int Size = 5;

struct Player {
	string Nickname;
	int Level;
	double Money;
	string RegistrationDate;
}; 

Player *players = new Player[Size];

string Field = "P..*.\n####.\n.*.*.\n.####\n....F\n";
string temp_field = Field;



// main functions
int searchByNickname(Player *p, string nick);
void RegistrationDateSorting(Player* p);
void moneySorting(Player* p);
void alphabetSorting(Player* p, int st, int end);
void Game(string field);
void BonusGame(string field, int& choice);
void menu();

// support functions
void DateConverter(int& day, int& month, int& year, string date);
void findSimilarLevelIndex(Player* p, int& start, int& end);
void levelSorting(Player* p);
void checkOnLetter(string& a, string& b);
int distanceToSymbol(string s, int pos);
void winCheck(char s, bool& win);
void winCheck(char s, bool& win, int& sc);
int definePlayerPosition(string f);
void leftMovement(string& field, int& position, bool& isWin);
void leftMovement(string& field, int& position, bool& isWin, int& sc, int& ch);
void rightMovement(string& field, int& position, bool& isWin);
void rightMovement(string& field, int& position, bool& isWin, int& sc, int& ch);
void downMovement(string& field, int& position, bool& isWin);
void downMovement(string& field, int& position, bool& isWin, int& sc, int& ch);
void upMovement(string& field, int& position, bool& isWin);
void upMovement(string& field, int& position, bool& isWin, int& sc, int& ch);
bool isDeadEnd(string field, int index, int dist, int count);

int main() {
	//players[0] = { "ADC",1,34.56,"22.10.2022" };
	//players[1] = { "ABC",2,3657.56,"23.10.2022" };
	//players[2] = { "AAA",3,345.56,"19.11.2022" };
	//players[3] = { "AAA",3,24345.56,"19.11.2022" };
	//players[4] = { "AAA",3,23.9,"19.11.2022" };
	//moneySorting(players);
	//cout << players[0].Money << ' ' << players[1].Money << ' ' << players[2].Money << ' ' << players[3].Money << ' ' << players[4].Money << ' ';
	menu();
}

// main functions
int searchByNickname(Player *p, string nick) {
	int i = 0;
	while(true){
		if (nick == p[i].Nickname) {
			return i;
		}
		i++;
	}
}
void moneySorting(Player* p) {
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size - 1; j++) {
			if (p[j].Money > p[j + 1].Money) {
				swap(p[j], p[j + 1]);
			}
		}
	}
}
void RegistrationDateSorting(Player* p) {
	for (int i = 0; i < Size; i++) {
		int day1, month1, year1, day2, month2, year2;
		DateConverter(day1, month1, year1, p[i].RegistrationDate);
		DateConverter(day2, month2, year2, p[i + 1].RegistrationDate);
		if (year1 > year2) {
			swap(p[i], p[i + 1]);
		}
		else if (year1 == year2) {
			if (month1 > month2) {
				swap(p[i], p[i + 1]);
			}
			else if (month1 == month2) {
				if (day1 > day2) {
					swap(p[i], p[i + 1]);
				}
			}
		}
	}
}
void alphabetSorting(Player* p, int st, int end) {
	if (end == Size) {
		return;
	}
	findSimilarLevelIndex(p, st, end);
	if (end - st) {
		for (int i = st; i < end + 1; i++) {
			for (int j = 0; j < end; j++) {
				checkOnLetter(p[j].Nickname, p[j + 1].Nickname);
			}
		}
	}
	alphabetSorting(p, end + 1, end + 1);
}
void Game(string field) {
	int position = definePlayerPosition(field);
	bool isWin = false;
	while (!isWin){
		cout << field;
		char move;
		cin >> move;
		switch (move) {
		case 'a':
		case 'A':
			leftMovement(field, position, isWin);
			break;
		case 'd':
		case 'D':
			rightMovement(field, position, isWin);
			break;
		case 'w':
		case 'W':
			upMovement(field, position, isWin);
			break;
		case 's':
		case 'S':
			downMovement(field, position, isWin);
			break;
		}
	}
}
void BonusGame(string field, int& choice) {
	int position = definePlayerPosition(field), star_count = 0;
	bool isWin = false;
	while (!isWin) {
		cout << field;
		char move;
		cin >> move;
		switch (move) {
		case 'a':
		case 'A':
			leftMovement(field, position, isWin, star_count, choice);
			break;
		case 'd':
		case 'D':
			rightMovement(field, position, isWin, star_count, choice);
			break;
		case 'w':
		case 'W':
			upMovement(field, position, isWin, star_count, choice);
			break;
		case 's':
		case 'S':
			downMovement(field, position, isWin, star_count, choice);
			break;
		}
	}
}
void menu() {
	string field = Field;
	int ch = true;
	while (ch) {
		if (ch != 2) {
			cout << "1 - Start game\n2 - Start bonus game\n0 - Exit\n";
			cin >> ch;
		}
		switch (ch) {
		case 1:
			Game(field);
			break;
		case 2:
			temp_field = Field;
			BonusGame(temp_field, ch);
			break;
		case 0:
			break;
		}
	}
}

// support functions
void levelSorting(Player* p) {
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size - 1; j++) {
			if (p[j].Level > p[j + 1].Level) {
				swap(p[j], p[j + 1]);
			}
		}
	}
}
void DateConverter(int& day, int& month, int& year, string date) {
	int sp = date.find('.', 0);
	day = stoi(date.substr(0, sp));
	month = stoi(date.substr(sp+1, date.find('.', sp + 1) - sp - 1));
	sp = date.find('.', sp+1);
	year = stoi(date.substr(sp+1, date.length()-sp-1));
}
void findSimilarLevelIndex(Player* p, int& start, int& end) {
	int l = p[start].Level, i = start;
	while (p[i].Level == l) {
		i++;
	}
	end = i-1;
}
void checkOnLetter(string& a, string& b) {
	int m = min(a.length(), b.length());
	for (int i = 0; i < m; i++) {
		if (toupper(a[i]) > toupper(b[i])) {
			swap(a, b);
			return;
		}
		if (toupper(a[i]) < toupper(b[i])) {
			return;
		}
	}
	if (m == a.length()) {
		swap(a, b);
		return;
	}
	else {
		return;
	}
}
int distanceToSymbol(string s, int pos) {
	int strdist = 0, temp_pos = pos;
	while (temp_pos >= 0 && s[temp_pos] != '\n') {
		temp_pos--;
		strdist++;
	}
	while (s[pos] != '\n') {
		pos++;
		strdist++;
	}
	return strdist;
}
void winCheck(char s, bool& win) {
	if (s == 'F') {
		win = true;
		cout << "WIN!\n";
	}
	win = false;
	return;
}
void winCheck(char s, bool& win, int& sc) {
	if (s == 'F' && sc==3) {
		win = true;
		cout << "WIN!\n";
	}
	else {
		win = false;
	}
}
int definePlayerPosition(string f) {
	int i = 0;
	while (f[i] != 'P') {
		i++;
	}
	return i;
}
void leftMovement(string& field, int& position, bool& isWin) {
	if (position != 0) {
		winCheck(field[position - 1], isWin);
	}
	else {
		cout << "Wrong move!\n";
		return;
	}
	if (field[position - 1] != '#' && field[position - 1] != '\n' && field[position - 1] != ' ') {
		swap(field[position], field[position - 1]);
		position--;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void leftMovement(string& field, int& position, bool& isWin, int& sc, int& ch) {
	if (position != 0) {
		winCheck(field[position - 1], isWin);
	}
	else {
		cout << "Wrong move!\n";
		return;
	}

	if (isWin) {
		ch = 1;
		return;
	}
	if (field[position - 1] == 'F' && sc < 3) {
		cout << "There are not enough stars!\n";
	}
	if (isDeadEnd(field, position, distanceToSymbol(field, position), sc)) {
		cout << "You are at a dead-end! Do you want to start over?(Y/N)\n";
		char c;
		cin >> c;
		if (c == 'Y') {
			isWin = true;
			ch = 2;
		}
		else {
			isWin = true;
			ch = 1;
		}
		return;
	}
	if (field[position - 1] == '.' && field[position - 1] != '\n' && field[position - 1] != ' ') {
		swap(field[position], field[position - 1]);
		field[position] = '#';
		position--;
	}
	else if (field[position - 1] == '*' && field[position - 1] != '\n' && position != 0 && field[position - 1] != ' ') {
		swap(field[position], field[position - 1]);
		field[position] = '#';
		position--;
		sc++;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void rightMovement(string& field, int& position, bool& isWin) {
	winCheck(field[position + 1], isWin);
	if (field[position + 1] != '#' && field[position + 1] != '\n' && field[position + 1] != ' ') {
		swap(field[position], field[position + 1]);
		position++;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void rightMovement(string& field, int& position, bool& isWin, int& sc, int& ch)  {
	winCheck(field[position + 1], isWin, sc);
	if (isWin) {
		ch = 1;
		return;
	}
	if (field[position + 1] == 'F' && sc < 3) {
		cout << "There are not enough stars!\n";
	}
	if (isDeadEnd(field, position, distanceToSymbol(field, position), sc)) {
		cout << "You are at a dead-end! Do you want to start over?(Y/N)\n";
		char c;
		cin >> c;
		if (c == 'Y') {
			ch = 2;
		}
		else {
			ch = 1;
		}
		isWin = true;
		return;
	}
	if (field[position + 1] == '.' && field[position + 1] != '\n' && field[position + 1] != ' ') {
		swap(field[position], field[position + 1]);
		field[position] = '#';
		position++;
	}
	else if (field[position + 1] == '*' && field[position + 1] != '\n' && field[position + 1] != ' ') {
		swap(field[position], field[position + 1]);
		field[position] = '#';
		position++;
		sc++;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void downMovement(string& field, int& position, bool& isWin) {
	int distance = distanceToSymbol(field, position);
	if (position + distance >= field.length()) {
		cout << "Wrong move!\n";
		return;
	}
	winCheck(field[position + distance], isWin);
	if (field[position + distance] != '#' && field[position + distance] != '\n' && position + distance <= field.length()) {
		swap(field[position], field[position + distance]);
		position += distance;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void downMovement(string& field, int& position, bool& isWin, int& sc, int& ch) {
	int distance = distanceToSymbol(field, position);
	if (position + distance >= field.length()) {
		cout << "Wrong move!\n";
		return;
	}
	else if (field[position + distance] == 'F' && sc < 3) {
		cout << "There are not enough stars!\n";
	}
	if (isDeadEnd(field, position, distance, sc)) {
		cout << "You are at a dead-end! Do you want to start over?(Y/N)\n";
		char c;
		cin >> c;
		if (c == 'Y') {
			isWin = true;
			ch = 2;
		}
		else {
			isWin = true;
			ch = 1;
		}
		return;
	}
	winCheck(field[position + distance], isWin, sc);
	if (isWin) {
		ch = 1;
		return;
	}
	else if (field[position + distance] == 'F' && sc < 3) {
		cout << "There are not enough stars!\n";
		return;
	}
	if (field[position + distance] == '.' && field[position + distance] != '\n' && position + distance <= field.length()) {
		swap(field[position], field[position + distance]);
		field[position] = '#';
		position += distance;
	}
	else if (field[position + distance] == '*' && field[position + distance] != '\n' && position + distance <= field.length()) {
		swap(field[position], field[position + distance]);
		field[position] = '#';
		position += distance;
		sc++;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void upMovement(string& field, int& position, bool& isWin) {
	int distance = distanceToSymbol(field, position);
	if (position - distance < 0 || field[position - distance] == ' ') {
		cout << "Wrong move!\n";
		return;
	}
	winCheck(field[position - distance], isWin);
	if (field[position - distance] != '#' && field[position - distance] != '\n') {
		swap(field[position], field[position - distance]);
		position -= distance;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void upMovement(string& field, int& position, bool& isWin, int& sc, int& ch) {
	int distance = distanceToSymbol(field, position);
	if (position - distance < 0 || field[position - distance] == ' ') {
		cout << "Wrong move!\n";
		return;
	}
	else if (field[position - distance] == 'F' && sc < 3) {
		cout << "There are not enough stars!\n";
	}
	if (isDeadEnd(field, position, distance, sc)) {
		cout << "You are at a dead-end! Do you want to start over?(Y/N)\n";
		char c;
		cin >> c;
		if (c == 'Y') {
			isWin = true;
			ch = 2;
		}
		else {
			isWin = true;
			ch = 1;
		}
		return;
	}
	winCheck(field[position - distance], isWin, sc);
	if (isWin) {
		ch = 1;
		return;
	}
	else if (field[position - distance] == 'F' && sc < 3) {
		cout << "There are not enough stars!\n";
		return;
	}
	if (field[position - distance] == '.' && field[position - distance] != '\n') {
		swap(field[position], field[position - distance]);
		field[position] = '#';
		position -= distance;
	}
	else if (field[position - distance] == '*' && field[position - distance] != '\n') {
		swap(field[position], field[position - distance]);
		field[position] = '#';
		position -= distance;
		sc++;
	}
	else {
		cout << "Wrong move!\n";
	}
}
bool isDeadEnd(string field, int index, int dist, int count) {
	if (index != 0) {
		if (field[index - 1] == '.' || field[index - 1] == '*' || (field[index - 1] == 'F' && count >= 3)) {
			return false;
		}
	}
	if (field[index + 1] == '.' || field[index + 1] == '*' || (field[index + 1] == 'F' && count >= 3)) {
		return false;
	}
	if (index + dist < field.length()) {
		if (field[index + dist] == '.' || field[index + dist] == '*' || (field[index + dist] == 'F' && count >= 3)) {
			return false;
		}
	}
	if (index - dist > 0) {
		if (field[index - dist] == '.' || field[index - dist] == '*' || (field[index - dist] == 'F' && count >= 3)) {
			return false;
		}
	}
	return true;
}



