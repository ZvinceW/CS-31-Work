#include <iostream>
#include <string>
#include <cctype>
using namespace std;


//*************************************
//  isValidUppercaseStateCode
//*************************************

// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.

bool isValidUppercaseStateCode(string stateCode)
{
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
		"KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
		"ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos  &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}


bool hasCorrectSyntax(string pollData) {
	if (pollData == "") {                     
		return true;                                     //empty pollData string is correct format (zero state forcast)
	}
	else if (pollData.size() < 4) {
		return false;                                    
	}
	else {
		int forcastLength;
		int dataLength = pollData.size();
		for (int i = 0; i < dataLength; i += forcastLength) {              //every interation the index jumps over the length of the forcast, which could be 4 chars or 5 chars long

			if (!isalpha(pollData[i + 0]) || !isalpha(pollData[i + 1])) {       //first two chars must be alphabets
				return false;
			}

			pollData[i + 0] = toupper(pollData[i + 0]);
			pollData[i + 1] = toupper(pollData[i + 1]);

			string stateCode = pollData.substr(i + 0, 2);

			if (!isValidUppercaseStateCode(stateCode)) {     //first 2 chars must form a state code
				return false;
			}

			if (!isdigit(pollData[i + 2])) {                 //3rd char must be digit
				return false;
			}

			if (!isalnum(pollData[i + 3])) {                 //4rd char must be alpha/digit
				return false;
			}
			else {
				if (isalpha(pollData[i + 3])) {              //if 4th char is alpha then the forcast must be 4 chars long
					forcastLength = 4;
				}
				else if (isdigit(pollData[i + 3])) {         //if 4th char is digit then the forcast must be 5 chars long
					forcastLength = 5;
				}
			}

			if (forcastLength == 5) {
				if (!isalpha(pollData[i + 4])) {		     //if 5th char exists, it must be alpha
					return false;
				}
			}	

			int remainingChars = pollData.size() - (i + forcastLength); //<-- # of chars left unprocessed in the string

			if (remainingChars < 4 && remainingChars > 0) {      //the rest of the string must have enough chars to form a single state forcast 
				return false;
			}
		}
		return true;
	}
}


int countVotes(string pollData, char party, int& voteCount) {
	if (!hasCorrectSyntax(pollData)) {                                   //function returns 1 when pollData doesnt have correct syntax
		return 1;
	}
	else if (!isalpha(party)) {                                          //function returns 3 when party is not an alpha
		return 3;
	}
	else {                                                               //when pollData has correct syntax and party is an alpha
		int forcastLength;
		int dataSize = pollData.size();
		int voteTotal = 0;
		for (int i = 0; i < dataSize; i += forcastLength) {       //with every interation the index jumps over the length of one forcast (could be 4 or 5)
			if (isalpha(pollData[i + 3])) {                              //if the 4th char in the forcast is alpha...
				forcastLength = 4;										 //...then the string must be 4 chars long
				int partyIndex = i + forcastLength - 1;					 //<-- index of the char that represents party
				if (toupper(pollData[partyIndex]) == toupper(party)) {		//if the char at partyIndex equals to party (turn both chars to upper before comparing in case they have different cases)...
					int stateVote = pollData[i + 2] - '0';				    //...then we find out the # of votes that party got in this state
					if (stateVote == 0) {                                //if that party happend to get 0 votes for this state...
						return 2;                                        //...then we return 2
					}
					voteTotal += stateVote;                              //adding the # of votes this party got from this state to the total # of votes this party got
				}
			}
			else if (isdigit(pollData[i + 3])) {                                         //if the 4th char in the forcast is digit...
				forcastLength = 5;                                                       //...then the frocast must be 5 chars long
				int partyIndex = i + forcastLength - 1;                                  //<-- index of the char that represents party
				if (toupper(pollData[partyIndex]) == toupper(party)) {                            //if the char at partyIndex equal to party (turn both chars to upper before comparing in case they have different cases)...
					int stateVote = (pollData[i + 2] - '0') * 10 + (pollData[i + 3] - '0');      //...then we find the # of votes that party got in this state
					if (stateVote == 0) {                                                //if this party got 0 votes from this state...
						return 2;                                                        //...then we return 2;
					}
					//cout << stateVote << endl; //DELETE
					voteTotal += stateVote;                                              //add the # of votes this party received from this state to the total # of votes this party got
				}
			}

		}
		//cout << voteTotal << endl;  //REMEMBER TO DELETE
		voteCount = voteTotal;            //setting voteCount to the total # of votes this party received
		return 0;
	}
}


int main() {
	int voteCount;
	cout << "hit me with the data fam: " << endl;
	string pollData;
	getline(cin, pollData);
	cout << "which partay tho: " << endl;
	int party;
	cin >> party;
	bool a =  hasCorrectSyntax(pollData);
	if (!a) {
		cout << "code aint right" << endl;
	}
	else {
		cout << "codes gud" << endl;
	}
	countVotes(pollData, 'd', voteCount);
	cout << voteCount << endl;
}