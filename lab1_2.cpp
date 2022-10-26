#include<fstream>
#include<string>
#include<queue>
#include<iostream>
#include<stack>
#include<ctype.h>

using namespace std;
// D:\SE\SE_LAB\LAB1\Sample.cpp
const string KeyWord[32] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

struct FileInclude {
	int sum = 0;
	int sum_switch_case = 0;
	int sum_if_else = 0;
	int sum_if_elseif_else = 0;
	int sum_case[100] = {0};
	int case_number = 0;
};


//Solve the backslash problem
void BackslashSolve( string & file_name) {
	int s = file_name.length();
	stack <char> temp;
	while (s--) {
		temp.push(file_name[s]);
	}
	file_name = "";
	while (!temp.empty()) {
		file_name += temp.top();
		if (temp.top() == '\\')
			file_name += '\\';
		temp.pop();
	}
}

//Search for keywords and count the number of keywords
void KeywordqueueMatch(FileInclude &f, queue<string> & keyword_queue) {
	int i = 0, j = 0;
	stack <string> temp;
	while (!keyword_queue.empty()) {
		if (keyword_queue.front() == "if") {
			temp.push(keyword_queue.front());
			keyword_queue.pop();
		} else if (keyword_queue.front() == "elseif") {
			temp.push(keyword_queue.front());
			keyword_queue.pop();
		} else if (keyword_queue.front() == "else") {
			if (temp.top() == "if") {
				i++;
				f.sum_if_else++;
				temp.pop();
			} else {

				while (temp.top() == "elseif" && !temp.empty()) {
					temp.pop();
				}
				if (!temp.empty()) {
					j++;
					f.sum_if_elseif_else++;
					temp.pop();
				}
			}
			keyword_queue.pop();

		}

		else if (keyword_queue.front() == "switch") {
			keyword_queue.pop();

			while ((keyword_queue.front() == "case" || keyword_queue.front() == "break" || keyword_queue.front() == "default") && !keyword_queue.empty()) {
				if (keyword_queue.front() == "case")
					f.sum_case[f.case_number]++;

				keyword_queue.pop();
			}
			f.case_number++;
		} else {
			keyword_queue.pop();
		}
	}
}


//count the number of switch and case
void cout2(FileInclude &f) {
	int i = f.case_number;
	int j = 0;
	cout << "switch num: " << f.sum_switch_case << endl;
	cout << "case num: ";
	while (i--) {
		cout << f.sum_case[j] << ' ';
		j++;
	}
	cout << endl;
}

//count the number of if-else
void cout3(FileInclude &f) {
	cout << "if-else num: " << f.sum_if_else << endl;
}

//count the number of if-elseif-else
void cout4(FileInclude &f) {
	cout << "if-elseif-else num: " << f.sum_if_elseif_else << endl;
}


void Match(FileInclude &f, string file_word, queue<string> &keyword_queue, fstream &fin ) {

	while (fin >> file_word) {
		if (file_word[0] == '/' && file_word[1] == '/' ) {
			cout << "//	" << file_word << endl;
			getline(fin, file_word); 
			fin >> file_word;
		} else if (file_word[0] == '/' && file_word[1] == '*') {
			while (fin >> file_word) {
				if (file_word[file_word.size() - 2] == '*' && file_word[file_word.size() - 1] == '/') {
					break;
				}
			}
		}
		int index = 0;
		int size_file_word = file_word.size();
		string temp_string;
		for ( int i = 0; i < size_file_word - 1; i++ ) {
			if ( !isalpha(file_word[i]) && isalpha(file_word[i + 1]) )
				index = i + 1;

			if ( ( isalpha(file_word[i]) && !isalpha(file_word[i + 1]) ) || ( i + 1 == size_file_word - 1 && isalpha(file_word[i + 1]) )) {

				if ( ( isalpha(file_word[i]) && !isalpha(file_word[i + 1]) ) )
					temp_string = file_word.substr(index, i + 1 - index);

				else
					temp_string = file_word.substr(index, i + 2 - index);

				for ( int j = 0; j < 32; j++ ) {
					if ( temp_string == KeyWord[j] ) {
						if ( temp_string != "else") {
							keyword_queue.push( temp_string );
							f.sum++;
							if ( temp_string == "switch")
								f.sum_switch_case++;
						} else {
							f.sum++;
							if (file_word[4] == '{') {	
								keyword_queue.push("else");
							} else {
								fin >> file_word;
								i++;
								if ( ( file_word[0] == 'i' && file_word[1] == 'f' && file_word[2] == '(') || file_word == "if" ) {
									keyword_queue.push("elseif");
									f.sum++;
								}
							}
						}
						break;
					}
				}
				index = 0;
				break;
			}
		}
	}
}

void MyCout(FileInclude &f, int rank) {
	cout << "total num: " << f.sum << endl;
	if (rank == 2)
		cout2(f);
	else if (rank == 3) {
		cout2(f);
		cout3(f);
	} else {
		cout2(f);
		cout3(f);
		cout4(f);
	}
}

int main() {
	int rank;
	string file_name ;
	cout << "Please enter the file address：";
	cin >> file_name;
	cout << "Please enter the completion rank：";
	cin >> rank;

	BackslashSolve( file_name );
	string file_word = "";
	fstream fin;
	fin.open(file_name.c_str(), ios::in );
	bool flag = 0;
	FileInclude f;
	queue<string> keyword_queue;

	Match( f, file_word, keyword_queue, fin );
	KeywordqueueMatch(f, keyword_queue);
	MyCout(f, rank);
	fin.close();

}
