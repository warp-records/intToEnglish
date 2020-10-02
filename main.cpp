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

	enum Type { Ones, Tens, Hundreds, Zero };


std::pair<std::string, bool> partToEnglish(std::string const& str, int idx);


std::string intToEnglish(long long num) {
	std::string str = std::to_string(num);
	std::string number;

	/*temp variable for pairs of 3 digits,
	like thousands, millions, etc, as well
	as wether or not that part was all zeroes*/
	std::pair<std::string, bool> part;
	bool tmp;
 	
 	//convert the first part to english
 	//if (str.size()%3 != 0)
 	part = partToEnglish(str, 0);
	number += part.first;
	tmp = part.second;

	for (int i = str.size()%3 == 0 ? 3 : str.size()%3; i < str.size(); i+=3) {
		//put the place value of the current digit at the next hundreds place
		int placeValue = str.size() - i;
		part = partToEnglish(str, i);

		number += (tmp ? "" : places[placeValue]  + ' ') + part.first;
		tmp = part.second;
	}

	//god that took too long to write
	return number;
}


std::pair<std::string, bool> partToEnglish(std::string const& str, int idx) {
	std::string part;
	bool emptyFlag = true;

	//will break in place
	for (int i = idx;; i++) {

		char const digit = str[i];
		/*note this is different from placeValue in the parent
		function, as this place value is index 0 and the 
		one in our parent function is index 1*/
		int placeValue = str.size()-i-1;
		//ones, tens, or hundreds
		Type const type = static_cast<Type>(placeValue%3);

		/*this is some weird ass code, and I wasn't sure
		what the best way to format it was... basically,
		if the digit is zero, don't execute the normal statements,
		and if the digit is zero in the ones place, break.*/
		
		if (digit != '0') {
			/*at least one of the digits wasn't zero,
			so it wasn't empty*/
			emptyFlag = false;

			if (type == Hundreds) {
				part += ones[digit] + " hundred";

			} else if (type == Tens) {
				if (digit!='1') {
					part += tens[digit];
				} else {
					//must be teens
					//need to peek at the next digit to determine what "teen" number it is
					part += teens[str[i+1]];
					//get outta here!
					break;
				} 
			} else {
				//must be ones
				part += ones[digit];
				break;
			}

			part += ' ';
		} else if (type == Ones) {
			break;
		}
	}

	part += ' ';
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