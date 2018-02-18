#include <string>
#include <queue>
#include <map>
#include <set>
#include <algorithm>

#include "inverter.h"
#include "fstream"

using namespace std;


string build_inverted_index(string filename){
	ifstream inFile;
	ifstream inFile2;
	inFile.open(filename);
	string file;
	string s;

	int indexFile = 0;
	int count = 0;
	string stringList[10000];
	map< string, set<int> > invertedIndex;

	while (!inFile.eof()) {
		getline(inFile, file);
		inFile2.open(file);
		string currentString = "";
		while (!inFile2.eof()) {
			getline(inFile2, s);
			int tempSize = s.size();
			for (int i = 0; i < tempSize; i++) {
				if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')) {
					currentString = currentString + s[i];
				}
				else {
					if (currentString != "") {
						if (invertedIndex.find(currentString) == invertedIndex.end()) {
							set<int> temp;
							temp.insert(indexFile);
							invertedIndex[currentString] = temp;
							stringList[count] = currentString;
							count+= 1;
						}
						else {
							invertedIndex[currentString].insert(indexFile);
						}
						currentString = "";
					}
				}
			}
			if (currentString != "") {
				if (invertedIndex.find(currentString) == invertedIndex.end()) {
					set<int> temp;
					temp.insert(indexFile);
					invertedIndex[currentString] = temp;
					stringList[count] = currentString;
					count+= 1;
				}
				else {
					invertedIndex[currentString].insert(indexFile);
				}
				currentString = "";
			}
		}
		if (currentString != "") {
			if (invertedIndex.find(currentString) == invertedIndex.end()) {
				set<int> temp;
				temp.insert(indexFile);
				invertedIndex[currentString] = temp;
				stringList[count] = currentString;
				count+= 1;
			}
			else {
				invertedIndex[currentString].insert(indexFile);
			}
			currentString = "";
		}
		inFile2.close();
		indexFile += 1;
	}
	sort(stringList, stringList + count);
	set<int> c;
	string res = "";
	for (int i = 0; i < count; i++) {
		res = res + stringList[i] + ":";
		c = invertedIndex[stringList[i]];
		for (set<int>::iterator it=c.begin(); it!=c.end();++it) {
			res = res + " " + to_string(*it);
		}
		res = res + "\n";
	}
	// cout << invertedIndex["this"];
	inFile.close();
	return res;
}
