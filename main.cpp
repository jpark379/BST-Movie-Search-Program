//
// <<JUDY PARK>>
// U. of Illinois, Chicago
// CS 251: Fall 2019
//
// Project #02: inputs movies and reviews, allowing the user to search
// by movie ID, movie Name, or review ID.  Uses binary search trees for 
// fast search; no other data structures are allowed.
//

#include <iostream>
#include <fstream>
#include <string>
#include "bst.h"

using namespace std;

// struct that stores the movie data
struct MovieData {
	string Name;
	int ID;
	int PubYear;
	int Num5Stars;
	int Num4Stars;
	int Num3Stars;
	int Num2Stars;
	int Num1Stars;
};

// trim
//
// trims whitespace from beginning and end of string.
//
// Ref: https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string
// Modified: to handle empty strings, and strings of all blanks.
// 
string trim(const string& str)
{
	size_t first = str.find_first_not_of(" \t\n\r");
	size_t last = str.find_last_not_of(" \t\n\r");

	if (first == string::npos && last == string::npos)  // empty, or all blanks:
		return "";
	else if (first == string::npos)  // no blanks at the start:
		first = 0;                     // 0-based
	else if (last == string::npos)   // no blanks trailing at end:
		last = str.length() - 1;       // 0-based

	return str.substr(first, (last - first + 1));
}

//
// InputMovies
//
// Inputs the data from the "movies" file, which contains N>0 lines, where
// each line contains:
//     id pubYear name
// and updates movieInfo struct to hold the name and pubYear of the movie.
// It then inserts the information into two trees--one keyed with the movie id and the other with the movie name
// 
void InputMovies(string moviesFilename, binarysearchtree<int,MovieData> *idTree, 
				 binarysearchtree<string, MovieData> *nameTree, MovieData &movieInfo)
{
	// function variable declarations
	ifstream moviesFile(moviesFilename); // saves moviesFilename file into variable moviesFilename
	
	// declare variables to hold id, pubyear, and name
	int      id, pubYear;
	string   name;
	
	// check if the file is good
	if (!moviesFile.good())
	{
		cout << "**Error: unable to open movies file '" << moviesFilename << "', exiting" << endl;
		return;
	}

	moviesFile >> id;  // get first ID, or set EOF flag if file empty:

	while (!moviesFile.eof())
	{
		// we have more data, so input publication year and movie name:
		moviesFile >> pubYear;
			getline(moviesFile, name);  // movie name fills rest of input line:

		// trim potential whitespace from both ends of movie name:
		name = trim(name);  
		
		// initialize the variables in struct movieInfo
		movieInfo.Name = name;
		movieInfo.ID = id;
		movieInfo.PubYear = pubYear;
		movieInfo.Num5Stars = 0;
		movieInfo.Num4Stars = 0;
		movieInfo.Num3Stars = 0;
		movieInfo.Num2Stars = 0;
		movieInfo.Num1Stars = 0;
		
		// populate the two trees with updated movieInfo
		idTree->insert(id, movieInfo);
		nameTree->insert(name, movieInfo);
		
		moviesFile >> id;  // get next ID, or set EOF flag if no more data:
	} // while
}

//
// InputReviews
//
// Inputs the data from the "reviews" file, which contains N>0 lines, where
// each line contains:
//     reviewID movieID rating
//  Then update the MovieData value in both trees to hold the number of stars the movie got respectively
//  
void InputReviews(string reviewsFilename, binarysearchtree<int,MovieData> *idTree, 
				  binarysearchtree<string, MovieData> *nameTree, int &numReviews)
{
	// function variable declarations
	ifstream reviewsFile(reviewsFilename); // saves moviesFilename file into variable moviesFilename
	struct MovieData *intMovieValue; // a pointer that will point to the node in the idTree
	struct MovieData *nameMovieValue; // a pointer that will point to the node in the nameTree
	
	// declare variables to hold reviewID, movieID, and rating
	int      reviewID, movieID, rating;
	string name; // string that will hold the name of the movie if the input was the movie ID
	
	// check if the file is good
	if (!reviewsFile.good())
	{
		cout << "**Error: unable to open reviews file '" << reviewsFilename << "', exiting" << endl;
		return;
	}

	reviewsFile >> reviewID;  // get first reviewID, or set EOF flag if file empty:

	while (!reviewsFile.eof())
	{
		// we have more data, so input the movieID:
		reviewsFile >> movieID;
		
		// initialize the pointers to point to the MovieData associated with the movie name/id
		intMovieValue = idTree->search(movieID);
		name = intMovieValue->Name; // update name to hold the name of the movie after searched in the idTree
		nameMovieValue = nameTree->search(name);
		
		// input the rating
		reviewsFile >> rating;

		// update the numstars in MovieData for both trees based on the respective rating
		if ( rating == 5) {
			intMovieValue->Num5Stars++;
			nameMovieValue->Num5Stars++;
		}
		if ( rating == 4) {
			intMovieValue->Num4Stars++;
			nameMovieValue->Num4Stars++;
		}
		if ( rating == 3) {
			intMovieValue->Num3Stars++;
			nameMovieValue->Num3Stars++;
		}
		if ( rating == 2) {
			intMovieValue->Num2Stars++;
			nameMovieValue->Num2Stars++;
		}
		if ( rating == 1) {
			intMovieValue->Num1Stars++;
			nameMovieValue->Num1Stars++;
		}
		
		numReviews++; // update numReviews
		
		reviewsFile >> reviewID;  // get next ID, or set EOF flag if no more data:
	} // while
}

//
// checkChar
//
// determines whether the input uses the name of the movie or the id
// if there are letters in the input, return true.
// if the input consists of only numbers, return false.
// 
bool checkChar(string input)
{
	// declare and initialize variable to hold the length of input
	int length;
	length = input.length();
	
	// loop through input to check if there are any letters in it
	for ( int i = 0; i < length; i++) {
		if ( isalpha(input[i])) {
			return true;
		}
 	}
	
	// a letter was not found, so the input consists of only integers
	return false;
}

//
// computeAverage
//
// takes in a pointer to a Value of a Key in a bst and totals a sum of the reviews for the movie.
// it then totals up each star review and determines the average review 
// 
double computeAverage(struct MovieData *movieInfo)
{
	double average; // variable that holds the average of reviews
	double sum; // variable that holds the total number of reviews for the movie
	double total; // variable that holds the sum of the values of each review
	int num5StarsTotal; // variable that holds the total number of 5 Star reviews
	int num4StarsTotal; // variable that holds the total number of 4 Star reviews
	int num3StarsTotal; // variable that holds the total number of 3 Star reviews
	int num2StarsTotal; // variable that holds the total number of 2 Star reviews
	int num1StarsTotal; // variable that holds the total number of 1 Star reviews
	
	// compute the sum
	sum = ((movieInfo->Num5Stars) + (movieInfo->Num4Stars) + (movieInfo->Num3Stars) + (movieInfo->Num2Stars) + (movieInfo->Num1Stars));
	
	// compute the number of stars respectively
	num5StarsTotal = (5 * (movieInfo->Num5Stars));
	num4StarsTotal = (4 * (movieInfo->Num4Stars));
	num3StarsTotal = (3 * (movieInfo->Num3Stars));
	num2StarsTotal = (2 * (movieInfo->Num2Stars));
	num1StarsTotal = (1 * (movieInfo->Num1Stars));
	
	// add up all the reviews
	total = num5StarsTotal + num4StarsTotal + num3StarsTotal + num2StarsTotal + num1StarsTotal;
	
	// the movie has reviews
	if ( sum > 0) {
		average = total / sum; // compute the average
		return average;
	}

	// movie has no reviews so return 0
	return 0;
}

//
// main
//
int main()
{
	// variable declarations
	string moviesFilename; // = "movies1.txt";
	string reviewsFilename; // = "reviews1.txt";
	string input; // string variable for the input
	string pound = "#"; // string variable that holds '#'
	
	binarysearchtree<int, MovieData> bstMovieByID; // create a bst with the key as the movie ID
	binarysearchtree<string, MovieData> bstMovieByName; // create a bst with the key as the movie name
	
	struct MovieData movieInfo; // declare a MovieData struct to hold the movieInfo
	struct MovieData *searchedMovieInfo = nullptr; // declare a pointer that will point to the searched MovieData

	int numReviews = 0; // int that holds the number of reviews in total
	
	// prompt user for movie file
	cout << "movies file?> ";
	cin >> moviesFilename;
	
	// input the movie data into the trees and struct
	InputMovies(moviesFilename, &bstMovieByID, &bstMovieByName, movieInfo);
	
	// prompt user for the review file
	cout << "reviews file?> ";
	cin >> reviewsFilename;
	
	// update the MovieData Value for each node in both trees to hold the number of star reviews
	// also update numReviews to hold the total number of reviews
	InputReviews(reviewsFilename, &bstMovieByID, &bstMovieByName, numReviews);
	
	// print out file information
	cout << "Num movies: " << bstMovieByID.size() << endl; // print the number of movies in movieFilename file
	cout << "Num reviews: " << numReviews << endl << endl; // print the number of reviews in reviewsFilename file
	
	// print out tree information
	// print the height of the bstMovieByID tree
	cout << "Tree by movie id: size=" << bstMovieByID.size() << ", height=" << bstMovieByID.height() << endl; 
	// print the height of the bstMovieByName tree
	cout << "Tree by movie name: size=" << bstMovieByName.size() << ", height=" << bstMovieByName.height() << endl;
	
	string junk;
	getline(cin, junk);  // discard EOL following last input:
	
	// infinite loop
	while ( 1) {
		// prompt user for the movie they want to search via the ID or the name
		cout << endl << "Enter a movie id or name (or # to quit)> ";
		getline(cin, input); // read entire input line
		
		// created trimmedInput to be used to check if '#' was used
		string trimmedInput = trim(input);
		
		// quits the program if '#' is entered as the input
		if ( trimmedInput.compare(pound) == 0 ) {
			return 0;
		}

		// a movie name was searched for
		if ( checkChar(input) == true) {
			searchedMovieInfo = bstMovieByName.search(input); // search the bstMovieByName tree for input
		}

		// a movie name was searched for
		if ( checkChar(input) == false) {
			searchedMovieInfo = bstMovieByID.search(stoi(input)); // search the bstReviewsByName tree for input and have 
													              // searchedMovieInfo point to the searched MovieData Value
		}
		
		// movie searched for was not found
		if ( searchedMovieInfo == nullptr) {
			cout << "not found..." << endl;
		}

		// movie searched for was found
		else if ( searchedMovieInfo != nullptr) {
			// print the MovieData information that was searched
			cout << "Movie ID: " << searchedMovieInfo->ID << endl; // print the Movie ID
			cout << "Movie Name: " << searchedMovieInfo->Name << endl; // print the Movie Name
			cout << "Avg rating: " << computeAverage(searchedMovieInfo) << endl; // call the computeAverage function and print average

			// print the number of stars that the movie got
			cout << "5 stars: " << searchedMovieInfo->Num5Stars << endl;
			cout << "4 stars: " << searchedMovieInfo->Num4Stars << endl;
			cout << "3 stars: " << searchedMovieInfo->Num3Stars << endl;
			cout << "2 stars: " << searchedMovieInfo->Num2Stars << endl;
			cout << "1 star: " << searchedMovieInfo->Num1Stars << endl;
		}
		
	} // while
}
