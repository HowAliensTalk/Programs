/*
	Dylan Greko

	COMPLETED: 3/26/20

	ANALYSIS:

	***This program original was to only implement the merge sort algorithm but I added elegance and a user-friendly interface***

	This program is a rework of an original homework assignment that is centered around merge sort. The original implementation uses void return
	types for the merge_sort(...) and merge(...) functions. I discovered that using return types of int * outputs the exact same results.
	Benefits? I'm not quite sure. Clearly with non-void return types there are extra bits of code that must be in place. I do not see any reason
	to use non-void return types over the void except insofar as to explore and discover that it produces the same results.

	With void typed recursion functions, the call stack builds so long as the length of the given array is greater than 1. But it's not hard to
	conceptualize that with non-void return recursive functions also build the call stack in a similar fashion so long as the given array length
	is greater than 1. The array continuously splits the left side z times, the right z times. Upon splitting the left, the splitting that half,
	there exists a left-left and left-right; splitting that half, there exists:

	[ left-left-left, left-left-right ] and [ left-right-left and left-right-right ]

	So on and so forth, building the stack. Upon reaching the base case, we stop adding calls to the stack and work downward calling the merge function.
	merge(...) now returns the given array but now sorted. This pattern continues until all calls in the stack are completed.

	Personally, I really like this with return types because I feel as though I can visualize the recursion in a better light. It makes sense
	that once the "left" side is in the process of sorting it's side, after each sort, the return statment is encountered but the call stack is still
	not empty, thus continuing the pattern.

	Quite new to this kind of analysis of recursion.

*/

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
using namespace std;

#define missionAccomplished 0
#define missionFailed -1
#define sentinel "q"


// Crux of Program ================== //
vector<int> populate_array(vector<int>, int, int);
int* merge_sort(int arr[], int, int);
int* merge(int arr[], int, int, int);
void print(vector<int>);
string print(vector<int> arr, string name);
int getInt(string);

// About ================== //
void about_program();

// To file
void arrayToFile(vector<int> unsorted, vector<int> sorted, int n, int lBound, int rBound, string dest, bool runtimeWriteArrays[]);

// Execute, exit, checks and conversions ================== //
int exe();
int exit_program(int);
bool check(string);
int stringToInt(string);

// File name extension ensure function ================== //
string ensure_fileExt(string);


int main() {

	about_program();
	if (exe() == -1) {
		cout << "Exiting program..." << endl;
		return missionFailed;
	}


	// Array variables
	int x_lowerBound{}, y_upperBound{};
	int numElems{ -1 };

	// String stream
	string sInput{};

	// Integer stream
	int iInput{};

	// File (output) variables
	string dest{};
	ofstream outFile{};
	bool runtimeWriteArrays[]{ false, false };


	// Prompt user to write to a file
	bool outerGate{ false };
	while (!check(sInput) && !outerGate) {

		cout << "Write to file? [y / n] ";
		cin >> sInput;

		// Sentinel check
		if (check(sInput)) {
			return exit_program(missionFailed);
		}
		else if (sInput == "y") {
			// Gate variable
			bool innerGate{ false };

			// Output file loop
			while (!innerGate) {
				cout << "Give a name to the output file: ";
				cin >> sInput;
				if (check(sInput)) {
					exit(exit_program(missionFailed));
				}
				dest = sInput;
				cout << "Are you sure you want to name the output file \"" << dest << "\"? [y / n] ";
				cin >> sInput;
				if (check(sInput)) {
					exit(exit_program(missionFailed));
				}
				innerGate = sInput == "y";
			}
			dest = ensure_fileExt(dest);

			// Ask user what they want written to file
			// Reset inner gate, clear input stream
			innerGate = false;
			cin.clear();
			while (!innerGate) {
				cout << "Options [ type the number for the preferred array to write to " << dest << " ] " << endl;;
				cout << "1 } Write ONLY sorted array" << endl;
				cout << "2 } write ONLY unsorted array" << endl;
				cout << "3 } Write BOTH sorted and unsorted arrays" << endl;
				cout << ": ";
				cin >> sInput;
				iInput = stringToInt(sInput);
				if (iInput > 0 && iInput < 4)
					innerGate = true;
			}

			// Switch statement signifies which arrays to write for later in program
			switch (iInput) {
			case 1:
				// ONLY sorted
				runtimeWriteArrays[0] = true;
				break;
			case 2:
				// ONLY unsorted
				runtimeWriteArrays[1] = true;
				break;
			default:
				// BOTH
				runtimeWriteArrays[0] = runtimeWriteArrays[1] = true;
			}
		}

		// Check if user wanted to write to file
		if (!dest.empty()) {
			outFile.open(dest);
			// No error message needed since we know it's an output file, not input
			cout << "Output file \"" << dest << "\" created." << endl;
		}

		outerGate = true;

	}

	outFile.close();


	// Prompt user for number of integer values
	while (numElems <= 0) {
		numElems = getInt("Enter the number of elements in array: ");
		if (numElems <= 0) {
			cerr << "Invalid number of elements. Must be a positive integer" << endl;
		}
	}

	// Prompt user for range of integer values
	x_lowerBound = getInt("Enter the lower bound for the values: ");
	y_upperBound = getInt("Enter the upper bound for the values: ");

	// Swap values
	if (x_lowerBound > y_upperBound) {
		int oldX{ x_lowerBound };
		x_lowerBound = y_upperBound;
		y_upperBound = oldX;
	}

	// Set array vector to length of numElems
	vector<int> unsorted_arr(numElems);

	// Populate the unsorted array
	unsorted_arr = populate_array(unsorted_arr, x_lowerBound, y_upperBound);
	cout << "Processing..." << endl;

	// Create an int array to be referenced
	int* refArray = new int[numElems] {};
	for (int i{ 0 }; i < numElems; i++) {
		refArray[i] = unsorted_arr[i];
	}

	// Merge sort array
	int* returnArray = merge_sort(refArray, 0, numElems - 1);


	// Pass back into a vector and print
	cout << "Ending..." << endl;
	vector<int> sorted_arr(numElems);
	for (int i{ 0 }; i < numElems; i++) {
		sorted_arr[i] = returnArray[i];
	}


	// Write to file
	if (runtimeWriteArrays[0] != false || runtimeWriteArrays[1] != false) {
		arrayToFile(unsorted_arr, sorted_arr, numElems, x_lowerBound, y_upperBound, dest, runtimeWriteArrays);
	}

	return exit_program(missionAccomplished);
}

// ================== Crux
int* merge_sort(int arr[], int left, int right) {
	if (left == right) {
		return NULL;
	}

	int mid{ (left + right) / 2 };

	merge_sort(arr, left, mid);
	merge_sort(arr, mid + 1, right);
	return merge(arr, left, mid, right);

}

int* merge(int arr[], int left, int mid, int right) {
	int i{ left }, j{ mid + 1 }, k{ 0 };
	vector<int> merged(right - left + 1);

	while (i <= mid && j <= right) {
		if (arr[i] <= arr[j]) {
			merged[k] = arr[i];
			i++;
		}
		else {
			merged[k] = arr[j];
			j++;
		}
		k++;
	}


	if (i == mid + 1) {
		for (int b{ j }; b <= right; b++) {
			merged[k] = arr[b];
			k++;
		}
	}
	else {
		for (int b{ i }; b < mid + 1; b++) {
			merged[k] = arr[b];
			k++;
		}
	}

	// Put merged elements back into original array
	for (int c{ 0 }; c < merged.size(); c++) {
		arr[left] = merged[c];
		left++;
	}

	return arr;

}

void print(vector<int> arr) {

	cout << "[";
	cout << setw(3) << internal << right;
	for (int e{ 0 }; e < arr.size(); e++) {
		if (e != arr.size() - 1) {
			cout << arr[e] << ", ";
		}
		else {
			cout << arr[e];
		}
	}

	cout << "]" << endl;

}

string print(vector<int> arr, string name) {
	stringstream s;
	s.str("");
	s << name << " : " << endl;
	s << setw(3) << internal << right;
	for (int e{ 0 }; e < arr.size(); e++) {
		if (e != arr.size() - 1) {
			s << arr[e] << "\n";
		}
		else {
			s << arr[e];
		}
	}
	s << endl;

	return s.str();

}

vector<int> populate_array(vector<int> arr, int x, int y) {

	for (int i{ 0 }; i < arr.size(); i++) {
		arr[i] = x + (rand() % (y - x + 1));
	}

	return arr;
}

void arrayToFile(vector<int> unsorted, vector<int> sorted, int n, int lBound, int rBound, string dest, bool runtimeWriteArrays[]) {
	ofstream outFile{};
	outFile.open(dest);

	outFile << "n = " << n << " | Range [ " << lBound << " : " << rBound << " ]" << endl;

	// this function assumes at least one of the runtimeWriteArrays bool elements is true

	if (runtimeWriteArrays[0] == runtimeWriteArrays[1]) {
		outFile << print(unsorted, "Unsorted");
		outFile << print(sorted, "Sorted");
	}
	else if (runtimeWriteArrays[0] == true) {
		outFile << print(sorted, "Sorted");
	}
	else {
		outFile << print(unsorted, "Unsorted");
	}
	outFile << endl;
	outFile.close();
}

// ================== About
void about_program() {
	// Welcome message
	string split_txt = string(5, '=');
	cout << split_txt + " W E L C O M E " + split_txt << endl;

	// Date and time
	cout << __DATE__ << " | " << __TIME__ << "\n\n";

	// Abt
	cout << split_txt + " " << " A B O U T " << " " + split_txt << endl;

	cout << "This program serves as a merge sorter insofar as creating an array with random values within a given range. " << endl;
	cout << "The original implementation of the merge sort algorithm would divde the array into many sub-arrays." << endl;
	cout << "To save memory, the algorithm used only uses one array to merge and sort the values in the array." << endl;
	cout << "You are given the options to write the un-sorted, sorted, both, or neither to a .txt file. " << endl;
	cout << "If you write to a file, the file name cannot be \"q.txt\" -> Type \"q\" to quit at any time. " << endl << endl;

}

// ================== Executions and checks
int exe() {
	string s{};
	cout << "Execute program? [y / n] ";
	cin >> s;
	s = tolower(s[0]);
	return  s == "y" ? 0 : -1;
}

int exit_program(int exitby) {
	cout << "Exiting program..." << endl;
	return exitby;
}

bool check(string s) {
	s = tolower(s[0]);
	return s == sentinel;
}

int getInt(string msg) {

	int sInt{};
	string cInput{};
	bool outerGate{ false };
	cout << msg;

	while (!outerGate) {
		try {
			cin.clear();
			cin >> cInput;
			sInt = std::stoi(cInput);
			outerGate = true;
		}
		catch (exception) {
			if (check(cInput)) {
				exit(exit_program(missionFailed));
			}
			cerr << "Invalid value. Try again." << endl;
			sInt = -1;
		}
	}

	return sInt;

}

int stringToInt(string s) {

	// Returns string to int; if parse error -> return -1
	// This assumes -1 cannot be an option for this function

	int parsedInteger{};

	try {
		parsedInteger = stoi(s);
		return parsedInteger;
	}
	catch (exception) {
		cerr << "Could not recognize input as integer" << endl;
	}
	return -1;
}

// ================== File name ext ensurer
string ensure_fileExt(string fileName) {

	// Assumes the user did not enter a file extension
	// And if they did ensure it's the correct extension (.txt)

	string txtExt = ".txt";

	for (int i{ 0 }; i < fileName.length(); i++) {
		if (fileName[i] == '.') {
			fileName = fileName.substr(0, i);
		}
	}
	return fileName + txtExt;
}