#include <iostream>
#include <fstream>

using namespace std;

// Function prototypes
int findLength(char[]);
bool word_found(char[], char[]);
void saveScore(int, fstream&);
void sort_scores(int[]);
void saveUser(char[], int, fstream&);
void findLeftToRight(char** board, char word[], int start[], int end[], int length, int rows, bool& flag, bool& found);
void findUpToBottom(char** board, char word[], int start[], int end[], int length, int rows, bool& flag, bool& found);
void findRightToleft(char** board, char word[], int start[], int end[], int length, int rows, bool& flag, bool& found);


int main() {

	// ----------------------  variable Declaration ------------------------------
	char name[20];
	int start[2];
	int end[2];
	int rows = 10;
	char dict_letter[15];
	char word[15];
	int length;
	bool valid, flag, found;
	char choice;
	int lives = 2;
	int score = 0;
	char num[3];
	int level = 1;

	// -------------------- FILES ----------------------------------------------
	fstream file;

	//2d pointer for the actual board
	char** board;
	//-----------------------------------------------------------------------------


		cout << "Press n for new game" << endl;
		cout << "Press r for resume game" << endl;
		cout << "Press i for level selection" << endl;
		cout << "Press h for high score" << endl;
		cout << "Press e for exit" << endl;

		cout << "Your choice: ";
		cin >> choice;

		if (choice == 'i') {

			cout << "Press 1 for easy and 2 for medium";
			cin >> level;

			while (!(level == 1 || level == 2))
			{
				cout << "select a valid option (1 or 2)";
				cin >> level;
			}
		}

		// Taking user name 
		cout << "Enter your Name: ";
		cin >> name;

	do {

	//   creation of 2D Board as a 2D matrix
		board = new char* [rows];
		for (int i = 0; i < rows; i++)
		{
			board[i] = new char[rows];
		}

		//   dictionary.open("dictionary.txt", ios::out);
		file.open("board.txt", ios::in);

		if (file.is_open()) {

			//   reading board from the file
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < rows; j++) {

					file >> board[i][j];
				}
			}
		}
		else {
			cout << "error occured while opening the file" << endl;
		}
		//   closing the file
		file.close();

		//   Displaying the board on console
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < rows; j++) {

				cout << board[i][j];
			}
			cout << endl;
		}

		//   Taking input from the user
		cout << "Enter word here: ";
		cin >> word;

		if (word[0] == 'P' || word[0] == 'p') {

			file.open("Pausedgame.txt", ios::out);

			//copyin the board into the file 'Pausedgame.txt'
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < rows; j++) {

					file << board[i][j];
				}
				cout << endl;
			}

			file.close();

			cout << "press 'r' to resume or press 'e' to exit:  ";
			cin >> choice;

			if (choice == 'e' || choice == 'E')
				break;
			else {
				continue;
			}
		}

		// finding length of the word
		length = findLength(word);

		//   verifying the word
		valid = false;
		file.open("dictionary.txt", ios::in);
		if (file) {

			while (!file.eof()) {

				file >> dict_letter;

				if (word_found(dict_letter, word)) {
					valid = true;
					break;
				}
			}

			file.close();
		}
		else {
			cout << "error checking in the dictionary" << endl;
		}

		if(!valid) {
			cout << "Word not found in the dictionery" << endl;
		}
	

		// ------------------------ Logic ------------------------- //

		flag = false;
		found = false;

		if (valid) {
			
			//Row
			findLeftToRight(board, word, start, end, length, rows, flag, found);
			
			//column
			if (!found) {
				findUpToBottom(board, word, start, end, length, rows, flag, found);
				
			}

			// backward row
			if (!found && level == 2) {

				findRightToleft(board, word, start, end, length, rows, flag, found);
			}
		}

		if (found) {

			// score increment
			score += 10;

			cout << "start at row " << start[0]+1 << ", column " << start[1]+1 << endl;
			cout << "end at row " << end[0]+1 << ", column " << end[1]+1 << endl;
		}
		else {
			score += 0;
			cout << "word not found" << endl;
			lives--;
		}

		if (lives < 0) {

			cout << "You are out of lives...! " << endl;
			
			break;
		}

		cout << "press [Y/y] to continue playing or [E/e] to exit the game" << endl;
		cin >> choice;

		system("cls");

	} while ((choice != 'e' && choice != 'E'));

	// keeping highscore and the user record
	saveScore(score, file);
	saveUser(name, score, file);

	// displaying highscores
	file.open("Highrecord.txt", ios::in);

	if (file) {
		cout << "Highrecords are: " << endl;

		while (!file.eof()) {

			file >> num;
			cout << num << endl;
		}
	}
	else {
		cout << "error opening the highrecord file" << endl;
	}
	file.close();

	//   deallocating the dynamic memory allocation
	for (int i = 0; i < rows; i++)
	{
		delete[] board[i];
	}
	delete[] board;

	return 0;
}

int findLength(char arr[]) {

	int count = 0;

	while (arr[count] != '\0') {

		count++;
	}

	return count;
}

bool word_found(char dict_word[], char word[]) {

	int i = 0;
	while (word[i++] != '\0') {

		if (dict_word[i] != word[i])
			return false;
	}

	return true;
}

void saveScore(int score, fstream& file) {

	file.open("Highrecord.txt", ios::in);

	int arr[3];
	int count = 0;

	while(!file.eof())
	{

		file >> arr[count++];
	}

	file.close();

	if (count >= 3) {
		file.open("Highrecord.txt", ios::out);

		sort_scores(arr);

		if (score > arr[2]) {

			arr[2] = score;
			sort_scores(arr);
		}

		for (int i = 0; i < 3; i++)
		{
			file << arr[i] << endl;
		}

		file.close();
	}
	else {

		file.open("Highrecord.txt", ios::app);

		file << score << endl;
		file.close();
	}
}

void sort_scores(int arr[]) {

	for (int i = 0; i < 2; i++)
	{
		for (int j = 1; j < 3; j++) {

			if (arr[i] < arr[j]) {

				int temp = arr[j];
				arr[j] = arr[i];
				arr[i] = temp;
			}
		}
	}
}

void saveUser(char user[], int score, fstream& file) {

	file.open("players.txt", ios::app);

	file << user << "\t\t";
	file << score << endl;;

	file.close();
}

void findLeftToRight(char **board, char word[], int start[], int end[], int length, int rows, bool& flag, bool& found) {

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < rows; j++) {

			flag = false;

			if (board[i][j] == word[0])
			{
				start[0] = i;
				start[1] = j;

				for (int k = 0; k < length; k++)
				{
					if ((k + j) > rows) {

						flag = true;
						break;
					}
					if (board[i][k + j] != word[k]) {

						flag = true;
						break;
					}
				}

				if (!flag) {
					end[0] = i;
					end[1] = j + length - 1;

					found = true;
					break;
				}
			}
			if (found)
				break;
		}
		if (found)
			break;
	}
}

void findUpToBottom(char** board, char word[], int start[], int end[], int length, int rows, bool& flag, bool& found) {

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < rows; j++) {

			flag = false;

			if (board[j][i] == word[0])
			{
				start[0] = j;
				start[1] = i;

				for (int k = 0; k < length; k++)
				{
					if ((k + j) >= rows) {

						flag = true;
						break;
					}
					if (board[k + j][i] != word[k]) {

						flag = true;
						break;
					}
				}

				if (!flag) {
					end[0] = j + length;
					end[1] = start[1];

					found = true;
					break;
				}
			}
			if (found)
				break;
		}
		if (found)
			break;
	}
}

void findRightToleft(char** board, char word[], int start[], int end[], int length, int rows, bool& flag, bool& found) {

	for (int i = rows - 1; i >= 0; i--)
	{
		for (int j = rows - 1; j >= 0; j--) {

			flag = false;

			if (board[i][j] == word[0])
			{
				start[0] = i;
				start[1] = j;

				for (int k = 0; k > (length * -1); k--)
				{
					if ((k + j) < 0) {

						flag = true;
						break;
					}
					if (board[i][k + j] != word[k * -1]) {

						flag = true;
						break;
					}
				}

				if (!flag) {
					end[0] = i;
					end[1] = j - length + 1;

					found = true;
					break;
				}
			}
			if (found)
				break;
		}
		if (found)
			break;
	}
}