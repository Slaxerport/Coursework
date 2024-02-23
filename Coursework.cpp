#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;

int Size = 10;

struct Player {
	string Nickname = "";
	int Level = 0;
	double Money = 0;
	string RegistrationDate = "";
};

Player* players = new Player[Size];

string Field = "P*..\n.##.\n.#.*\n..##\n#..F\n";
string temp_field = Field;

ifstream fin("Output.txt");


// main functions
int searchByNickname(Player* p);
void RegistrationDateSorting(Player* p);
void moneySorting(Player* p);
void alphabetSorting(Player* p, int st, int end);
void printPlayers(Player* p);
void Game(string field);
void BonusGame(string field, char& choice);
void menu();
void addPlayer(Player* p);
void deletePlayer(Player* p);
void editPlayer(Player* p);
void fillPlayer(Player* p, int index);
void GameMenu(char& ch);
void consolePrint(Player* p);

// support functions
void DateConverter(int& day, int& month, int& year, string date);
void findSimilarLevelIndex(Player* p, int& start, int& end);
void levelSorting(Player* p);
int distanceToSymbol(string s, int pos);
void winCheck(char s, bool& win);
void winCheck(char s, bool& win, int& sc);
int definePlayerPosition(string f);
void leftMovement(string& field, int& position, bool& isWin);
void leftMovement(string& field, int& position, bool& isWin, int& sc, char& ch);
void rightMovement(string& field, int& position, bool& isWin);
void rightMovement(string& field, int& position, bool& isWin, int& sc, char& ch);
void downMovement(string& field, int& position, bool& isWin);
void downMovement(string& field, int& position, bool& isWin, int& sc, char& ch);
void upMovement(string& field, int& position, bool& isWin);
void upMovement(string& field, int& position, bool& isWin, int& sc, char& ch);
bool isDeadEnd(string field, int index, int dist, int count);
int detectNextFreeElement(Player* p, int size);
void writeFromFileToArray(Player* p, int size);
void getFile(string& field, string path);

int main() {
	menu();
}

// main functions
int searchByNickname(Player* p) {
	string nick;
	cout << "Enter a nickname: ";
	cin >> nick;
	for (int i = 0; i < Size; i++) {
		if (nick == p[i].Nickname) {
			return i;
		}
	}
	return -1;
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
	for (int i = 0; i < Size - 1; i++) {
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
			for (int j = i; j < end; j++) {
				if (p[j].Nickname > p[j + 1].Nickname); {
					swap(p[j], p[j + 1]);
				}
			}
		}
	}
	alphabetSorting(p, end + 1, end + 1);
}
void Game(string field) {
	int position = definePlayerPosition(field);
	bool isWin = false, iskbhit = true;
	while (!isWin) {
		if (iskbhit) {
			system("cls");
			cout << field;
		}
		if (_kbhit()) {
			iskbhit = true;
			switch (_getch()) {
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
			default:
				cout << "Enter a correct choice!\n";
				break;
			}
		}
		else {
			iskbhit = false;
		}
	}
}
void BonusGame(string field, char& choice) {
	int position = definePlayerPosition(field), star_count = 0;
	bool isWin = false, iskbhit = true;
	while (!isWin) {
		if (iskbhit) {
			system("cls");
			cout << field;
		}
		if (_kbhit()) {
			iskbhit = true;
			switch (_getch()) {
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
			default:
				cout << "Enter a correct choice!\n";
				break;
			}
		}
		else {
			iskbhit = false;
		}
	}
}
void menu() {
	Size = detectNextFreeElement(players, Size);
	if (Size == 0) {
		Size++;
	}
	while (!fin.eof()) {
		addPlayer(players);
		Size++;
	}
	Size--;
	int number;
	string field = Field;
	char ch = true;
	while (ch - '0') {
		system("cls");
		if (ch != '2') {
			cout << "1 - Start game\n2 - Start bonus game\n3 - Print all players\n4 - Add a player\n5 - Edit a player\n6 - Delete a player\n7 - Search a player by nickname\n8 - Sort by money count\n9 - Sort by registration date\ns - Sort by level and nickname\n0 - Exit\n";
			cin >> ch;
		}
		system("cls");
		switch (ch) {
		case '1':
			GameMenu(ch);
			break;
		case '2':
			temp_field = Field;
			BonusGame(temp_field, ch);
			break;
		case '3':
			consolePrint(players);
			system("pause");
			break;
		case '4':
			fillPlayer(players, detectNextFreeElement(players, Size));
			break;
		case '5':
			editPlayer(players);
			break;
		case '6':
			deletePlayer(players);
			break;
		case '7':
			number = searchByNickname(players);
			if (number == -1) {
				cout << "There are no players with that nickname!\n";
			}
			else {
				cout << number << endl;
			}
			system("pause");
			break;
		case '8':
			moneySorting(players);
			break;
		case '9':
			RegistrationDateSorting(players);
			break;
		case 's':
			levelSorting(players);
			alphabetSorting(players, 0, 0);
			break;
		case '0':
			printPlayers(players);
			break;
		default:
			cout << "Enter a correct choice!\n";
			break;
		}
	}
}
void addPlayer(Player* p) {
	int tpos;
	string f;
	getline(fin, f); //
	getline(fin, f); // skip first two lines
	getline(fin, f);
	if (f.find(':') == std::string::npos) {
		return;
	}
	tpos = f.find(':') + 2;
	p[Size - 1].Nickname = f.substr(tpos, f.length() - tpos);
	getline(fin, f);
	if (f.find(':') == std::string::npos) {
		return;
	}
	tpos = f.find(':') + 2;
	p[Size - 1].Level = stoi(f.substr(tpos, f.length() - tpos));
	getline(fin, f);
	if (f.find(':') == std::string::npos) {
		return;
	}
	tpos = f.find(':') + 2;
	p[Size - 1].Money = stod(f.substr(tpos, f.length() - tpos));
	getline(fin, f);
	if (f.find(':') == std::string::npos) {
		return;
	}
	tpos = f.find(':') + 2;
	p[Size - 1].RegistrationDate = f.substr(tpos, f.length() - tpos);
}
void printPlayers(Player* p) {
	ofstream fout("Output.txt");
	for (int i = 0; i < Size; i++) {
		fout << endl << "Player " << i + 1 << endl << "Nickname: " << p[i].Nickname << endl << "Level: " << p[i].Level << endl << "Money: " << p[i].Money << endl << "Registration date: " << p[i].RegistrationDate;
		if (i != Size - 1) {
			fout << endl;
		}
	}
	fout.close();
}
void deletePlayer(Player* p) {
	int delindex;
	cout << "Enter the index of the player: ";
	cin >> delindex;
	if (delindex >= Size) {
		cout << "There are no players with that index!" << endl;
		system("pause");
		return;
	}
	for (int i = delindex; i < Size - 1; i++) {
		swap(p[i], p[i + 1]);
	}
	Size--;
	Player* temp_arr = new Player[Size];
	for (int i = 0; i < Size; i++) {
		temp_arr[i] = p[i];
	}
	for (int i = 0; i < Size; i++) {
		p[i] = temp_arr[i];
	}
}
void editPlayer(Player* p) {
	int ind;
	cout << "Enter the index of the player: ";
	cin >> ind;
	fillPlayer(p, ind);
	Size--;
}
void fillPlayer(Player* p, int index) {
	cout << "Nickname: ";
	cin >> p[index].Nickname;
	cout << "Level: ";
	cin >> p[index].Level;
	cout << "Money: ";
	cin >> p[index].Money;
	cout << "Registration date: ";
	cin >> p[index].RegistrationDate;
	Size++;
}
void GameMenu(char& ch) {
	while (ch - '0') {
		string f;
		cout << "1 - Level 1\n2 - Level 2\n3 - Level 3\n4 - Level 4\n5 - Level 5\n0 - Exit\n";
		cin >> ch;
		switch (ch) {
		case '1':
			getFile(f, "Level1.txt");
			Game(f);
			break;

		case '2':
			getFile(f, "Level2.txt");
			Game(f);
			break;

		case '3':
			getFile(f, "Level3.txt");
			Game(f);
			break;

		case '4':
			getFile(f, "Level4.txt");
			Game(f);
			break;

		case '5':
			getFile(f, "Level5.txt");
			Game(f);
			break;
		case '0':
			break;
		default:
			cout << "Enter a correct choice!\n";
			break;
		}
	}
	ch++;
}
void consolePrint(Player* p) {
	for (int i = 1; i <= detectNextFreeElement(p, Size); i++) {
		cout << "Player " << i << endl;
		cout << "Nickname: " << p[i - 1].Nickname << endl;
		cout << "Level: " << p[i - 1].Level << endl;
		cout << "Money: " << p[i - 1].Money << endl;
		cout << "Registration date: " << p[i - 1].RegistrationDate << endl;
		cout << endl;
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
	month = stoi(date.substr(sp + 1, date.find('.', sp + 1) - sp - 1));
	sp = date.find('.', sp + 1);
	year = stoi(date.substr(sp + 1, date.length() - sp - 1));
}
void findSimilarLevelIndex(Player* p, int& start, int& end) {
	int l = p[start].Level, i = start;
	while (p[i].Level == l) {
		i++;
	}
	end = i - 1;
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
	else {
		win = false;
	}
	return;
}
void winCheck(char s, bool& win, int& sc) {
	if (s == 'F' && sc == 3) {
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
	if (isWin) {
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
void leftMovement(string& field, int& position, bool& isWin, int& sc, char& ch) {
	if (position != 0) {
		winCheck(field[position - 1], isWin);
	}
	else {
		cout << "Wrong move!\n";
		return;
	}

	if (isWin) {
		ch = '1';
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
			ch = '2';
		}
		else {
			isWin = true;
			ch = '1';
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
	if (isWin) {
		return;
	}
	if (field[position + 1] != '#' && field[position + 1] != '\n' && field[position + 1] != ' ') {
		swap(field[position], field[position + 1]);
		position++;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void rightMovement(string& field, int& position, bool& isWin, int& sc, char& ch) {
	winCheck(field[position + 1], isWin, sc);
	if (isWin) {
		ch = '1';
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
			ch = '2';
		}
		else {
			ch = '1';
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
	if (isWin) {
		return;
	}
	if (field[position + distance] != '#' && field[position + distance] != '\n' && position + distance <= field.length()) {
		swap(field[position], field[position + distance]);
		position += distance;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void downMovement(string& field, int& position, bool& isWin, int& sc, char& ch) {
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
			ch = '2';
		}
		else {
			isWin = true;
			ch = '1';
		}
		return;
	}
	winCheck(field[position + distance], isWin, sc);
	if (isWin) {
		ch = '1';
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
	if (isWin) {
		return;
	}
	if (field[position - distance] != '#' && field[position - distance] != '\n') {
		swap(field[position], field[position - distance]);
		position -= distance;
	}
	else {
		cout << "Wrong move!\n";
	}
}
void upMovement(string& field, int& position, bool& isWin, int& sc, char& ch) {
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
			ch = '2';
		}
		else {
			isWin = true;
			ch = '1';
		}
		return;
	}
	winCheck(field[position - distance], isWin, sc);
	if (isWin) {
		ch = '1';
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
int detectNextFreeElement(Player* p, int size) {
	size--;
	while (true) {
		if (size == -1) {
			return 0;
		}
		if (p[size].Nickname != "") {
			return size + 1;
		}
		size--;
	}

}
void writeFromFileToArray(Player* p, int size) {
	for (int i = 0; i < size; i++) {
		addPlayer(p);
	}
	fin.close();
}
void getFile(string& field, string path) {
	string temp;
	ifstream fin(path);
	while (!fin.eof()) {
		getline(fin, temp);
		field += temp;
		field += '\n';
	}
	fin.close();
}
