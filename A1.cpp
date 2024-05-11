// Muhammad Kumail   L1F20BSCS0375
// Mohid Hameed L1F20BSCS0372
// Farhan Khan L1F20BSCS0388

#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include<unordered_map>
#include<vector>

using namespace std;

string removeSpecialCharacters(const string& input) {
	regex pattern("[^'A-Za-z\\s]+");
	return regex_replace(input, pattern,"");
}
string removeSpace(const string& input) {
	regex pattern("[\\s]+");
	return regex_replace(input, pattern, "_");
}
void lowercase(string& inp) {
	for (int i = 0; i < inp.size(); i++) {
		inp[i] = tolower(inp[i]);
	}
}


void copy(vector<string> ex, unordered_map<string, int>& lang) {
	for (int i = 0; i < ex.size(); i++) {
		lang[ex[i]]++;
	}
}
vector<string> ngrams(string book, int n){
	vector<string> wst;
	if (n == 1) {
		for (int i = 0; i < book.size(); i++) {
			if (book[i] == '_')continue;
			string s= "";
			s += book[i];
			wst.push_back(s);
		}
	}
	else {
		for (int i = 0; i < book.size(); i++) {
			string s = "";
			for (int j = 0; j < n; j++)
			{
				if (i + j >= book.size()) s += "_";
				else s += book[i + j];
			}
			wst.push_back(s);
		}
	}
	return wst;


}
bool cmp(pair<string, int>& a,pair<string, int>& b)
{
	return a.second > b.second;
}
 unordered_map<string,int> sort(unordered_map<string, int>& M)
{
	vector<pair<string, int> > A;
	for (auto& it : M) {
		A.push_back(it);
	}
	sort(A.begin(), A.end(), cmp);
	vector<pair<string, int> > B;
	int itr = 1;

	for (auto& x : A) {
		x.second = itr;
		B.push_back(x); itr++;

		if (itr == 301)break;
	}

	unordered_map<string, int> fin;

	int i = 1;

	for (auto x : B) {
		fin[x.first] = i++;
	}
	return fin;
}

string readFile(ifstream& fin, string book_type) {
	fin.open(book_type);
	string Book;
	while (!fin.eof()) {
		string wast;
		getline(fin, wast);
		Book += wast;
	}
	fin.close();
	return Book;
}

string preProcess(string input) {
	lowercase(input);
	string neww = removeSpecialCharacters(input);
	neww = removeSpace(neww);
	return neww;

}
unordered_map<string, int> make_Dict(string input) {
	vector<string> unigram = ngrams(input, 1);
	vector<string> bigram = ngrams(input, 2);
	vector<string> trigram = ngrams(input, 3);
	vector<string> quadgram = ngrams(input, 4);
	unordered_map<string, int> dict;
	copy(unigram, dict);
	copy(bigram, dict);
	copy(trigram, dict);
	copy(quadgram, dict);
	return dict;
}

double calculate_Distance(int x, int y) {
	double dist = x - y;
	if (dist < 0) return dist * -1;
	else return dist;
}

string out_of_place_measure(unordered_map<string, int> input, unordered_map<string, int> English, unordered_map<string, int> German, unordered_map<string, int> French, unordered_map<string, int> Spanish) {
	
	double arr[4] = { 0,0,0,0 };

	for (auto x : input) {
		auto it = English.find(x.first);
		if (it != English.end()) {
			arr[0] += calculate_Distance(x.second, English[x.first]);
		}
		else arr[0] += 300;

		it = German.find(x.first);
		if (it != German.end()) {
			arr[1] += calculate_Distance(x.second, German[x.first]);
		}
		else arr[1] += 300;
		it = French.find(x.first);
		if (it != French.end()) {
			arr[2] += calculate_Distance(x.second, French[x.first]);
		}
		else arr[2] += 300;
		it = Spanish.find(x.first);
		if (it != Spanish.end()) {
			arr[3] += calculate_Distance(x.second, Spanish[x.first]);
		}
		else arr[3] += 300;
	}
	cout << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << endl;


	int min = arr[0];
	int lang = 0;

	for (int i = 0; i < 4; i++) {
		if (arr[i] < min) {
			min = arr[i];
			lang = i;
		}
	}

	if (lang == 0) return "English";
	else if (lang == 1)return "German";
	else if (lang == 2)return "French";
	else if (lang == 3)return "Spanish";
	else return "Unidentified";
}

int main() {
	ifstream fin;
	string engBook = readFile(fin, "English.txt");
	engBook = preProcess(engBook);
	string gerBook = readFile(fin, "German.txt");
	gerBook = preProcess(gerBook);
	string freBook = readFile(fin, "French.txt");
	freBook = preProcess(freBook);
	string spnBook = readFile(fin, "Spanish.txt");
	spnBook = preProcess(spnBook);

	unordered_map<string, int> English = make_Dict(engBook);
	unordered_map<string, int> engSorted = sort(English);
	unordered_map<string, int> German = make_Dict(gerBook);
	unordered_map<string, int> gerSorted = sort(German);
	unordered_map<string, int> French = make_Dict(freBook);
	unordered_map<string, int> freSorted = sort(French);
	unordered_map<string, int> Spanish = make_Dict(spnBook);
	unordered_map<string, int> spnSorted = sort(Spanish);


	string input;
	input = readFile(fin, "input.txt");
	input = preProcess(input);
	unordered_map<string, int> inp_Dict = make_Dict(input);
	unordered_map<string, int> sorted_Input = sort(inp_Dict);

	string prediction = out_of_place_measure(sorted_Input, engSorted, gerSorted, freSorted, spnSorted);

	cout << prediction << endl;

	return 0;
}