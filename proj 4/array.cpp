#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int appendToAll(string a[], int n, string value) {
	if (n < 0) {
		return -1;                              //return -1 if string length is negative
	}
	else {
		for (int i = 0; i < n; i++) {           //interate through every element
			a[i] += value;                      //append value to every element
		}
		return n;
	}
}

int lookup(const string a[], int n, string target) {
	if (n < 0) {
		return -1;                           //return -1 if string length is negative
	}
	else {
		for (int i = 0; i < n; i++) {           //interate through every element
			if (a[i] == target) {     
				return i;                      //return the current index when the current element is equal to target string
			}
		}
		return -1;                      //return -1 if the target string is not found in the array
	}
}

int positionOfMax(const string a[], int n) {
	if (n <= 0) {
		return -1;                          //return -1 if string length is negative or zero
	}
	else {
		int maxIndex = 0;
		string maxString = "";
		for (int i = 0; i < n; i++) {        //interate through every element in the array
			if (a[i] > maxString) {          //when an element is greater than maxString
				maxString = a[i];				//make maxString equal to that value
				maxIndex = i;                   //record the index of this element
			}
		}
		return maxIndex;                  //return the position of the element with the highest value
	}
}

int rotateLeft(string a[], int n, int pos) {
	if(n < 0){
		return -1;                              //return -1 if string length is negative
	}
	else {
		string posCopy = a[pos];                //save a copy of the element that is to be moved to the end
		for (int i = pos; i < n; i++) {
			if (i == n - 1) {
				a[n - 1] = posCopy;            //if we reached the end of the array, replace the last element with the copy of the element at pos
			}
			else {
				a[i] = a[i + 1];               //move all the elements after the element at pos one index down the array
			}
		}
		return pos;          //return the original index of the rotated element
	}
}

int countRuns(const string a[], int n) {
	if (n < 0) {
		return -1;                         //return -1 if string length is negative
	}
	else {
		string compare = a[0];               //to be used to compare with the elements of the string, initialized with the first element
		int countSets = 1;                 //by default there's one set of sequence
		for (int i = 0; i < n; i++) {      
			if (compare != a[i]) {           
				countSets++;               //add the amount of sets whenever a change of string is detected
				compare = a[i];            //also set compare to the newly detected string when new string is detected
			}
		}
		return countSets;                  //return the # of sets
	}
}

int flip(string a[], int n) {
	if (n < 0) {
		return -1;                          //return -1 if string length is negative
	}
	else {
		for (int i = 0; i < n/2; i++) {
			string copy = a[i];
			int mirrorIndex = n - 1 - i;           //switch the location of an element with its "mirror" element
			a[i] = a[mirrorIndex];  
			a[mirrorIndex] = copy;
		}
		return n;                  //return the length of the string
	}
}

int differ(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0) {
		return -1;                        //return -1 if string length is negative
	}
	else {
		int n;
		if (n1 < n2 || n1 == n2) {
			n = n1;                         //compare the size of n1 and n2 and use the lower one as the bound of the for loop below
		}
		else {
			n = n2;
		}
		for (int i = 0; i < n; i++) {
			if (a1[i] != a2[i]) {
				return i;                 //return the current index when the first corresponding element of the two arrays that are not equal
			}
		}
		return n;                   //return the length of the shorter array
	}
}

int subsequence(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0) {
		return -1;                        //length of these arrays cant be negative
	}
	else {
		if (n2 > n1) {
			return -1;                  //if a2 is contains more elements in a1, then obviously a1 cant have all the elements of a2 in a consecutive order
		}
		int startIndex;                       //store the start of a possible consecutive sequence (i.e. an element in a1 is equal to first element of a2)
		bool confirmStart = false;            //signals that a possible consecutive sequence is detected
		for (int i = 0; i < n1; i++) {        //loops through a1
			if (!confirmStart) {              //when possible consecutive sequence IS NOT detected...
				if (a1[i] == a2[0]) {			//checks if this particular element of a1 is equal to the first element of a2
					startIndex = i;             //if this element of a1 is equal to first element of a2, then store i as the start of a possible consecutive sequence
					confirmStart = true;        //signal that a possible consecutive sequence is detected
				}
			}
			if (confirmStart) {		                       //when a possible consecutive sequence IS detected...
				if (a2[i - startIndex] != a1[i]) {			//checks if the element in a2 (starting from the element at startIndex) is equal to the element in a1 (continuously/consecutively as long as possible sequence is still signaled as detected)
					confirmStart = false;					//if the element in a2 does not equal to the element in a1, then the the possible sequence is marked as undetected again
				} else if (i - startIndex == n2 - 1) {                 //if we have checked all the elements in a2 and the prevoius if statement has not been triggered...
					return startIndex;                      //return startIndex because a complete consecutive sequence does exist
				}
			}	
		}
		return -1;       //if the program went through the for loop above without returning anything, then the consecutive sequence must NOT exist in a1
	}
}

int lookupAny(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0) {
		return -1;                        //length of these arrays cant be negative
	}
	else {
		for (int i = 0; i < n1; i++) {
			for (int j = 0; j < n2; j++) {    //for every element in the first array, loop over every element in the second array
				if (a2[j] == a1[i]) {
					return i;            //when an element in the first array fits another element in the second array, return the location of the element in the first array
				}
			}
		}
		return -1;       //return zero when first arrray doesnt contain any element of the second array
	}
}

int separate(string a[], int n, string separator) {
	if (n < 0) {
		return -1;                            //return -1 if string length is negative
	}
	else {
		
		for (int i = n; i > 1; i--) {
			string max = "";                 // 
			int maxIndex;                    // 
			for (int j = 0; j < i; j++) {    // 
				if (a[j] > max) {            // 
					max = a[j];              //   <- sort every element in the array from the lowest value to the highest value
					maxIndex = j;            //
				}                            //
			}                                //
			string copy = a[i - 1];          //
			a[i - 1] = max;                  //
			a[maxIndex] = copy;              // 
		}
		for (int i = 0; i < n; i++) {
			if (a[i] >= separator) {          //interate through the sorted array to find the first element not smaller than separator
				return i;                     //return the location of that element
			}
		}
		return n;   //if no element in the array is greater than or equal to separator, return the size of the array.
	}
}

int main() {
	//append to all debug
	/*
	string cand1[5] = { "a", "A", "ab", "jill", "zjamu" };
	int length1 = 5;
	int result = appendToAll(cand1, length1, "lol");
	cout << result << endl;
	for (int i = 0; i < length1; i++) {
		cout << cand1[i] << " ";
	}
	*/

	//lookup debug
	string cand1[5] = {  };
	int length1 = 0;
	int result = lookup(cand1, length1, "ab");;
	cout << result << endl;

	//positionOfMax debug
	/*
	string cand1[5] = { "a", "A", "ab", "jill", "zjamu" };
	int length1 = 5;
	int result = positionOfMax(cand1, length1);
	cout << result;
	*/

	//rotateLeft debug
	/*
	string cand1[6] = { "a", "ab", "ac", "jill", "ajamu" };
	int length1 = 5;
	int result = rotateLeft(cand1, length1, 2);
	for (int i = 0; i < length1; i++) {
		cout << i << ":" << cand1[i] << ",";
	}
	cout << endl << result << endl;
	*/

	//countRuns debug
	/*
	string cand1[6] = { "a", "a", "a", "jill", "ajamu" };
	int length1 = 5;
	int result = countRuns(cand1, length1);
	cout << result << endl;
	*/

	//flip debug
	/*
	string cand1[6] = { "evan", "hillary", "mindy", "jill", "ajamu" };
	int length1 = 6;
	flip(cand1, length1);
	for (int i = 0; i < length1; i++) {
		cout << cand1[i] << " ";
	}
	cout << endl;
	*/

	//differ debug
	/*
	string cand1[6] = { "evan", "hillary", "mindy", "jill", "ajamu", "lmao" };
	int length1 = 6;
	string cand2[5] = { "evan", "hillarys", "mindy", "jill", "ajamu" };
	int length2 = 5;
	int result = differ(cand1, length1, cand2, length2);
	cout << result << endl;
	*/

	//subsequence debug
	/*
	string cand1[6] = { "evan", "hillary", "mindy", "jill", "ajamu", "lmao" };
	int length1 = 6;
	string cand2[2] = { "evans", "hillary" };
	int length2 = 2;
	//assert(subsequence(cand1, length1, cand2, length2));
	int result = subsequence(cand1, length1, cand2, length2);
	cout << result << endl;
	*/

	//lookUpAny debug
	/*
	string cand[4] = { "gary", "hillary", "jill", "donald" };
	int length1 = 4;
	string cand2[6] = { "evan", "hillary1", "mindy", "jill", "ajamu", "lmao" };
	int length2 = 6;
	int result = lookupAny(cand, length1, cand2, length2);
	cout << result << endl;
	cout << endl;
	*/

	//separate debug 
	/*
	string cand[6] = { "gary", "hillary", "jill", "donald" };
	int length = 4;
	int result = separate(cand, length, "hillary");
	cout << result << endl;
	for (int i = 0; i < length; i++) {
		cout << i << ": " << cand[i] << ", ";
	}
	cout << endl;
	*/
}