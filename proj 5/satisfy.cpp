#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>

using namespace std;

const int MAX_WORD_LENGTH = 20;

void markDelete(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int distance[], int rulePos) { //mark strings as empties and ints as negatives
		strcpy(word1[rulePos], "");
		strcpy(word2[rulePos], "");
		distance[rulePos] = -1;
}

void markSame(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int distance[], int nRules, int targetIndex) { //mark same rules INCOMPLETE(needs to delete one of the two repeats)
	char target1[MAX_WORD_LENGTH + 1], target2[MAX_WORD_LENGTH + 1];
	strcpy(target1, word1[targetIndex]);
	strcpy(target2, word2[targetIndex]);
	for (int i = 0; i < nRules; i++) {
		if (strcmp(word1[i], target1) == 0 && strcmp(word2[i], target2) == 0 && i != targetIndex) {
			if (distance[targetIndex] >= distance[i]) {
				markDelete(word1, word2, distance, i);
			}
			else {
				markDelete(word1, word2, distance, targetIndex);
			}
		} //NEEDS TO ADD REVERSE ORDER SAME RULE
		if (strcmp(word2[i], target1) == 0 && strcmp(word1[i], target2) == 0 && i != targetIndex) {    //find reverse order same rule
			if (distance[targetIndex] >= distance[i]) {
				markDelete(word1, word2, distance, i);
			}
			else {
				markDelete(word1, word2, distance, targetIndex);
			}
		} //NEEDS TO ADD REVERSE ORDER SAME RULE
	}
}

void markBadWord(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int distance[], int targetIndex) { //mark empty words, negative distance, and non alphas
	if (strcmp(word1[targetIndex], "") == 0 || strcmp(word2[targetIndex], "") == 0 || distance[targetIndex] < 0) {
		markDelete(word1, word2, distance, targetIndex);
	}
	int length = strlen(word1[targetIndex]);
	for (int j = 0; j < length; j++) {
		if (!isalpha(word1[targetIndex][j])) {
			markDelete(word1, word2, distance, targetIndex);
		}
	}
}

void deleteShuffle(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int distance[], int& nRules) {   //delete empty words and negative distances, also nRules will be changed to the # of normalized rules
	for (int i = 0; i < nRules; i++) {
		while (strcmp(word1[i], "") == 0 || strcmp(word2[i], "") == 0 || distance[i] < 0) {
			if (i == nRules - 1) {
				nRules--;    //if we get to the last index then stop moving words forward
				break;       //topkek
			}
			else {
				for (int j = i; j < nRules - 1; j++) {   //move the next word forward to replace the current word
					strcpy(word1[j], word1[j + 1]);
					strcpy(word2[j], word2[j + 1]);
					distance[j] = distance[j + 1];
				}
				nRules--;
			}
		}
	}
}

void allToLower(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int nRules) {
	for (int i = 0; i < nRules; i++) {
		int length = strlen(word1[i]);
		for (int j = 0; j < length; j++) {
			word1[i][j] = tolower(word1[i][j]);
		}
		int length2 = strlen(word2[i]);
		for (int j = 0; j < length2; j++) {
			word2[i][j] = tolower(word2[i][j]);
		}
	}
}

int normalizeRules(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int distance[], int nRules) {    //must return # of normalized rules
	/*
	for loop that marks bad stuff : markSame, markBadword
	statement that deletes everything marked: deleteShuffle
	*/
	if (nRules < 0) {
		nRules = 0;
	}
	for (int i = 0; i < nRules; i++) {
		markSame(word1, word2, distance, nRules, i);
		markBadWord(word1, word2, distance, i);
	}
	deleteShuffle(word1, word2, distance, nRules);     //nRules is passed by reference, will be changed to # of normalized rules
	//gotta turn shits into lowercase
	allToLower(word1, word2, nRules);
	return nRules;
}

int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH + 1], const char word2[][MAX_WORD_LENGTH + 1], const int distance[], int nRules, const char document[]) { //its archery time and im hungry
	const int MAX_DOC_SIZE = 200;   
	//TREAT NEGATIVE NRULE AS ZERO!!!!
	if (nRules < 0) {
		nRules = 0;
	}
	int satisfyScore = 0;
	//parsing words in document into an array of cstrings
	int wordSize = 0;        //size of the word detected
	int wordNum = 0;         //number of words detected
	char wordList[MAX_DOC_SIZE + 1][MAX_DOC_SIZE + 1];          //the list of words will be stored here
	bool parsingWord = false;                                   //signal whether the program is in the process of parsing a word from document
	for (int j = 0; document[j] != '\0'; j++) {                    //iterate through every char in document
		/*
		if (j == MAX_DOC_SIZE - 1 && parsingWord) {
			wordList[wordNum][wordSize] = '\0';
			wordNum++;
		}else 
		*/
		if (isalpha(document[j])) {                             
			wordList[wordNum][wordSize] = document[j];          //add the char into a slot in the array if it is alpah, no action if not alpha
			wordSize++;                                         //increase wordSize by 1, so function knows which slot to put the next char that is alpha
			parsingWord = true;                                 //confirm that the function in the process of parsing a word
		}
		else if (document[j] == ' ' && parsingWord) {		                                        
			wordList[wordNum][wordSize] = '\0';     //if the current char is a space and the function is parsing, then we end the parsing
			wordNum++;                              //move to the next slot in wordList
			wordSize = 0;
			parsingWord = false;                    //parsing signal = off
		}
	}
	if (parsingWord) {
		wordList[wordNum][wordSize] = '\0';
		wordNum++;
	}
	//wordList[wordNum][wordSize] = '\0';   
	//NEED TO SOMEHOW TURN WORDLIST TO ALL LOWERCASE
	for (int i = 0; i < wordNum; i++) {
		for (int j = 0; wordList[i][j] != '\0'; j++) {
			wordList[i][j] = tolower(wordList[i][j]);
		}
	}

	 //DEBUG: lists out every element of wordList
	/*
	for (int i = 0; i < wordNum; i++) {
		cout << "[" << wordList[i] << "]" << endl;
	}
	*/

	//calculating score
	for (int i = 0; i < nRules; i++) {           //loop through all the rules
		//cout << "using rule: " << i << endl;       //DEBUG
		int minDistance = MAX_DOC_SIZE;
		for (int j = 0; j < wordNum; j++) {                //loop through all the wordList
			//cout << "checking worList[" << j << "]" << endl;       //DEBUG
			if (strcmp(word1[i], wordList[j]) == 0) {      //check if the current word in wordList fits the current word in word1
				//cout << "found word1" << endl;       //DEBUG
				for (int k = 0; k < wordNum; k++) {               //if we found word1 then we loop through wordList from the beginning again in search for word2
					if (strcmp(word2[i], wordList[k]) == 0 && k != j) {     //check if any word in wordList is word2
						//cout << "found word2" << endl;       //DEBUG
						if (abs(k - j) < minDistance) {          //if the absolute value of distance between k and j is less than or equal to distance
							minDistance = abs(k - j);                      //add 1 to satisfaction score
							//cout << "satisfied rule: " << i << endl;       //DEBUG
							//cout << "distance is: " << abs(k - j) << endl;       //DEBUG
						}
					}
				}
			}
		}
		if (minDistance <= distance[i]) {
			satisfyScore++;
		}
	}
	return satisfyScore;
}

int main() {
	char word1[4][MAX_WORD_LENGTH + 1] = {
		"mad", "deranged", "nefarious", "have"
	};
	char word2[4][MAX_WORD_LENGTH + 1] = {
		"scientist", "robot", "plot", "mad"
	};
	int distance[4] = {
		2, 4, 1, 13
	};
	int nRules = 4;
	/*
	for (int i = 0; i < nRules; i++) {
		cout << "word1: " << word1[i] << ", word2: " << word2[i] << ", distance: " << distance[i] << endl;
	}
	*/
	//char doc[201] = "The mad UCLA scientist unleashed a deranged evil giant robot.";
	
	//assert(normalizeRules(word1, word2, distance, nRules) == 3);
	
	//int result = normalizeRules(word1, word2, distance, nRules);
	//cout << endl << "new # of rules: " << result << endl;
	//markSame(word1, word2, distance, nRules, 1);
	/*
	deleteShuffle(word1, word2, distance, nRules);

	cout << endl;
	cout << nRules;

	for (int i = 0; i < nRules; i++) {
		cout << "word1: " << word1[i] << ", word2: " << word2[i] << ", distance: " << distance[i] << endl; 
	}
	//NOTE: TREAT NEGATIVE # AS ZERO???? CHECK NORMALIZE RULES AGAIN
	*/
	int result = calculateSatisfaction(word1, word2, distance, nRules, "The mad UCLA scientist unleashed a deranged evil giant robot.");
	cout << "rules satisfied: " << result << endl;
	
}
