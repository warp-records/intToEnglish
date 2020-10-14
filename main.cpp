#include <iostream>
#include <fstream>
#include <map>


	//maps basic numbers to their corresponding words
	std::map<char, std::string> ones {{'0', ""}, {'1', "one"}, {'2', "two"}, {'3', "three"}, 
	{'4', "four"}, {'5', "five"}, {'6', "six"}, {'7', "seven"},
	{'8', "eight"}, {'9', "nine"}};

	std::map<char, std::string> teens {{'0', "ten"}, {'1', "eleven"},
	{'2', "twelve"}, {'3', "thirteen"}, {'4', "fourteen"},
	{'5', "fifteen"}, {'6', "sixteen"}, {'7', "seventeen"},
	{'8', "eighteen"}, {'9', "nineteen"}};

	std::map<char, std::string> tens {{'2', "twenty"}, {'3', "thirty"},
	{'4', "forty"}, {'5', "fifty"}, {'6', "sixty"}, 
	{'7', "seventy"}, {'8', "eighty"}, {'9', "ninety"}};

	//based on how many zeros
	std::map<int, std::string> places {{0, ""}, {3, "thousand"}, {6, "million"},
	 {9, "billion"}, {12, "trillion"}, {15, "quadrillion"}, {18, "quintillion"},
	 {21, "sextillion"}, {24, "septillion"}, {27, "octillion"}, {30, "nondecillion"}};

	enum Type { Ones, Tens,  Hundreds, Teens, Zero };


std::pair<std::string, bool> partToEnglish(std::string const& str, int idx, bool lastPart = false);


std::string intToEnglish(long long num) {
	/*str is the string we are going to iterate
	over when we do our converting*/
	std::string const str = std::to_string(num);
	//this will be the 
	std::string english;

	/*each "part" is a set of (usually) three digits,
	that go by a pronoun, like thousands, millions, etc.
	The bool in part is wether or not the current part
	had all blank zeroes, in which case we wouldnt give it
	a pronoun*/
	std::pair<std::string, bool> part;
	//wether or not the previous iteration of part was filled with zeroes
	bool lastEmpty = true;
	bool negative = (str[0] == '-');

	//lol
	if (str== "0") return "zero";

	if (negative)
		english += "negative ";

	//set i to 1 if there is a netgative, else 0
	int i = negative;
 	
 	/*Convert the first "part" to english if the string
 	 isn't a multilpe of three chars. This way we can
 	 iterate over the string three digits at a time using
 	 our partToEnglish() function.*/
 	if ((str.size()-i) % 3 != 0) {
 		part = partToEnglish(str, i);
		english += part.first;
		lastEmpty = part.second;

		//go to nearest next part
		i = (str.size()-negative) % 3 + negative;
	}

	for (; i < str.size(); i+=3) {
		/*place value of the current digit. Note "i"
		is NOT the place value, i is the index. 
		i (increases) from right to right while placeValue
		inreases from  left to right.*/
		int placeValue = str.size() - i;
		//get next part()
		part = partToEnglish(str, i, placeValue==3);

		english += (lastEmpty ? "" : places[placeValue] + ' ') + part.first;
		lastEmpty = part.second;
	}

	//god that took too long to write
	return english;
}


std::pair<std::string, bool> partToEnglish(std::string const& str, int idx, bool lastPart) {
	std::string part;
	//wether or not this part was all zeroes
	bool emptyFlag = true;

	//trigger break from inside switch statements
	bool breakFlag = false;

	bool negative = (str[0] == '-');

	//will break in place
	for (int i = idx; !breakFlag; i++) {

		char digit = str[i];
		/*note this is VERY different from placeValue in the 
		parent function, as this place value is index 0 
		and the  one in our parent function is index 1. 
		This will also subtract one from the place value
		if there is a negative*/
		int placeValue = str.size()-i-1;
		//ones, tens, hundreds, or zero if theres a zero digit
		Type type;

		if (digit != '0') {
			type = static_cast<Type>(placeValue%3);
		} else {
			type = Zero;
		}

		if (digit == '1' && type == Tens) {
			type = Teens;
		}
		/*this is some weird ass code, and I wasn't sure
		what the best way to format it was... basically,
		if the digit is zero, don't execute the normal statements,
		and if the digit is zero in the ones place, break.*/
		switch (type) {
			case (Hundreds) :
				part += ones[digit] + " hundred";
				break;

			case (Tens) :
				if (lastPart) {
					part += "and ";
					//don't repeat for the ones
					lastPart = false;
				}
				part += tens[digit];
				break;
			
			case (Teens) :
				if (lastPart) 
					part += "and ";

				part += teens[str[i+1]];
				//finish after teens
				breakFlag = true;
				break;
			
			case (Ones) :
				if (lastPart) 
					part += "and ";

				part += ones[digit];
				breakFlag = true;
				break;
			
		}

		if (type != Zero) {
			emptyFlag = false;
			part += ' ';
		//sketchy way of saying "if digit is 0 and placevalue is ones"
		} else if (placeValue % 3 == 0) {
			breakFlag = true;
		}
	}

	return {part, emptyFlag};
}


int main() {
	long long test;
	std::string english;

	std::cout << "Enter an integer to convert to english:\n";
	std::cin >> test;

	english = intToEnglish(test);
	std::cout << "Your integer in english: \n" << english << "\n";

	return 0;
}
