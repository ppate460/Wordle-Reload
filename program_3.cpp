/* program_3.cpp
    Wordle Reload is a game that allows you to guess either a 3 letter word or 5 letter word.
    You have a set amount of time to guess a word.  At the conlusion of game play
    you will be provided stats on your overall game play.

    Author: Pratik Patel
    System: Windows Visual Studio

*/

#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // Allows using the tolower() function
#include <vector>     // For vectors
#include <ctime>      // Timer functions
#include <iomanip>    // For setfill and setw
using namespace std;

//--------------------------------------------------------------------------------
// Display welcome message, introducing the user to the program and
// describing the instructions for the game
void gameDisplayInstructions()
{
    cout << "Program 3: Wordle Reload \n"
        << "CS 141, Spring 2022, UIC \n"
        << " \n"
        << "The objective of this game is to guess the randomly selected \n"
        << "word within a given number of attempts. You can select either \n"
        << "a three or five word board. \n"
        << "At the conclusion of the game, stats will be displayed.  \n"
        << "Indicators will be given if characters of the user entered \n"
        << "word are reflected in the guessed word. \n"
        << "  - If the character is in the correct position, the character \n"
        << "    will display as an uppercase value.\n"
        << "  - If the character is within the random word, the character \n"
        << "    will display as a lowercase value.\n"
        << "  - If you enter a character that is not in the word, an asterisk '*' \n"
        << "    will display."
        << " \n"
        << endl;
}//end displayWelcomeMessage()

//--------------------------------------------------------------------------------
// Use binary search to look up a search word in the dictionary vector, returning
// the index if found, -1 otherwise.
long binarySearch(string searchWord,   // Word we are searching for
    vector< string> dictionary)        // Dictionary of words as a vector
{
    long low, mid, high;    // array indices for binary search
    int searchResult = -1;  // Stores index of word if search succeeded, else -1

    
    //set random word to lowercase  
    for (int x = 0; x < searchWord.length(); x++) {
        searchWord[x] = tolower(searchWord[x]);
    }

    // Binary search for word
    low = 0;
    high = dictionary.size() - 1;
    while (low <= high) {
        mid = (low + high) / 2;

        // searchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = searchWord.compare(dictionary[mid]);

        if (searchResult == 0) {
            // Word IS in dictionary, so return the index where the word was found
            return mid;
        }
        else if (searchResult < 0) {
            high = mid - 1; // word should be located prior to mid location
        }
        else {
            low = mid + 1; // word should be located after mid location
        }
    } //end while( low...

    // Word was not found
    return -1;
}//end binarySearch()

//---------------------------------------------------------------------------
void readWordsFromFile(
    vector <string>& threeLetterList,   // Where three letter words will be stored
    vector <string>& fiveLetterList,   // Where five letter words will be stored
    const char fileName[])                // File name where words will be read from
{
    ifstream inStream;                      // Declare an input stream for reading
    inStream.open(fileName);               // Open wordlewords file
    assert(inStream.fail() == false);     // Ensure file open worked

    // Clear vectors in case they already had some words in it
    threeLetterList.clear();
    fiveLetterList.clear();

    // Keep repeating while input from the file yields a word
    string newWord;                         // Store a single input word
    while (inStream >> newWord) {           // While there is another word to be read

        if (newWord.length() == 3) {
            threeLetterList.push_back(newWord); // Add this new word to the end of the three letter vector, growing it in the process
        }
        if (newWord.length() == 5) {
            fiveLetterList.push_back(newWord);  // Add this new word to the end of the five letter vector, growing it in the process
        }
    }

    inStream.close(); // Close the file

} //end readWordsFromFile()

//---------------------------------------------------------------------------
//Function to generate a random word from 3 word vector or 5 word vector depending on user selection
string generateRandomWord(vector<string> wordList) {

    int wordIndex = rand() % wordList.size();     // Generate an index for some random word
    return wordList.at(wordIndex);
} //end generateRandomWords()

//---------------------------------------------------------------------------
// Utility function to set word to uppercase
void setWordToUpperCase(string & aWord) {

    for (int x = 0; x < aWord.length(); x++) {
        aWord[x] = toupper(aWord[x]);
    }
} //end setWordToUpperCase()

//---------------------------------------------------------------------------
//Populate playing board with random word
void populatePlayingBoard(int wordLength, string randomWord, vector<char> & playingBoard) {

    for (int position = 0; position <= wordLength; position++) {

        //Loop to iterate through random word and populate board
        for (int x = 0; x < wordLength; x++) {
            playingBoard.push_back(randomWord.at(x));
        }
    }
} //end populatePlayingBoard()

//---------------------------------------------------------------------------
//Function to display the board each time a word is guessed
void displayGameBoard (int wordLength, vector<char> displayBoard) {

    for (int x = 0; x < displayBoard.size(); x++) { //Looping structure to output board
        if ((x % wordLength == 0)) {
            cout << "\n";
        }
        cout << setw(wordLength - 2) << " [ ";
        cout << displayBoard.at(x);
        cout << setw(wordLength - 2) << " ] ";
    }
    cout << endl;
} //end displayGameBoard()

//---------------------------------------------------------------------------
//Function to compare the vectors go through each character and assign appropriate value to display board
//check playing board vector with user populated vector and determine the correct character to place within the display board
void generateDisplayBoard(int wordLength, vector<char> userEntryBoard, vector<char> playingBoard, vector<char> &displayBoard) {
    for (int i = userEntryBoard.size() - wordLength; i < userEntryBoard.size(); i++) { //Check each character to determine uppercase, asterisk or lowercase value

        if (playingBoard.at(i) == userEntryBoard.at(i)) {
            displayBoard.push_back(userEntryBoard.at(i)); //assign uppercase value to display board
        }
        if (playingBoard.at(i) != userEntryBoard.at(i)) {
            char aValue = '*';
            for (int y = userEntryBoard.size() - wordLength; y < userEntryBoard.size(); y++) {

                //Check to see if a character in the user word entry is equal to any character in playing board 
                //If character is found in the playing board, then lowercase the value
                if (userEntryBoard.at(i) == playingBoard.at(y)) {
                    aValue = tolower((userEntryBoard.at(i)));
                    break;
                }
            }

            displayBoard.push_back(aValue);  //Assigned the appropriate symbol if character is not found in evaluated position
        }
    }//end of compare vector looping structure
} //end generateDisplayBoard()

//---------------------------------------------------------------------------
//Wordle Game Playboard with user entry - Function 
void wordleGamePlay(string randomWord, vector<string> dictionary, int wordLength, int & MAX_TIME_LIMIT, int& totalWordCount, int& totalTime, int& currentStreak) {

    int attempts = 0;             // Variable to keep track of guesses
    string userWordEntry;         // Variable to hold word entered by user
    vector <char> playingBoard;   // Vector to store random word from either 3 or 5 letter vector
    vector<char> userEntryBoard;  // Vector to store words entered by user
    vector<char> displayBoard;    // Vector to store board after comparison of playing board and user word entry
    time_t startTime;             // Variable to initialize start time
    int elapsedSeconds;           // Variable to hold elapsed time
    int streakCount =0;           // sets streak count to 1 if word is guessed and 0 if not guessed.  Boolean data type would also be suitable.

    playingBoard.clear();    //Make sure playing board is empty
    userEntryBoard.clear();  //Make sure user entry board is empty

    setWordToUpperCase(randomWord); //utility function to set either random word or user entered word to uppercase 
    populatePlayingBoard(wordLength, randomWord, playingBoard); //Populate playing board with random word.

    //Looping structure to keep track of how many attempts user can guess word
    do{
        cout << "\nPlease enter word -->  ";
        cin >> userWordEntry;  //Accept user input

        //Check to make sure word user entered is 3 or 5 character word
        if (userWordEntry.length() != wordLength) {
            cout << "Invalid word entry - please enter a word that is " << wordLength << " characters long." << endl;
            continue;
        }
        cout << endl;

        setWordToUpperCase(userWordEntry);  //Covert user word entry to uppercase

        //Using binary search to check if word is in 3 or 5 word list.  If word is not
        //found, send a message that word is not a playable word.
        if (binarySearch(userWordEntry, dictionary) == -1) {
            cout << "Not a playable word, please select another word. " << endl;
            continue;
        }

        //Timer starts with the entry of first word
        if (attempts == 0) {
            startTime = time(NULL); // Declare a variable to hold a time, and set it to the current time
            elapsedSeconds = 0;
        }

        //Looping structure to add word to user entry board vector at each attempt
        int i = 0;
        while (i < wordLength) {
            userEntryBoard.push_back(userWordEntry.at(i));
            i++;
        }

        generateDisplayBoard(wordLength, userEntryBoard, playingBoard, displayBoard); //Function to determine correct character to place within the display board

        displayGameBoard(wordLength, displayBoard);    //Display playing board 

        // Stop Timer - Calculate the elapsed time as the difference between the current time and the start time
        elapsedSeconds = difftime(time(NULL), startTime);

        //Message to display if you random word is guessed within given amount of attempts
        if ((attempts <= wordLength) && (userWordEntry == randomWord)) {

            totalWordCount++;  //increment total word count if word is correctly guessed
            totalTime += elapsedSeconds;  // increase total time so calculation can occur from average total time
            streakCount =1;  //set streakCount to 1

            //Output stats for each game play if word is successfully completed
            cout << "\nNice Work!  You guessed the correct word" << endl;
            cout << "  - You completed the board in: " << elapsedSeconds << " seconds. " << endl;
            cout << "  - It took you " << (attempts + 1) << "/" << (wordLength + 1) << " attempts." << endl;
            break;
        }

        //Message to display if you timer is expired
        if (elapsedSeconds > MAX_TIME_LIMIT) {

            cout << "\nYour time has expired.  Try again." << endl;
            cout << "  - You are " << (elapsedSeconds - MAX_TIME_LIMIT) << " seconds over the " << MAX_TIME_LIMIT << " second time limit." << endl;
            
            streakCount = 0;  //set current current streak to 0
            break;
        }

        attempts++; //Increment guess attempts

        //Message to display if you timer is expired
        if (attempts > wordLength) {

            cout << "\nMaximum amount of attempts have been reached. Try again." << endl;

            streakCount = 0;   //Set current streak to 0
            break;
        }
    }while ((attempts <= wordLength) && (userWordEntry != randomWord));

    currentStreak = streakCount; //currentStreak is set to 1 or 0;
}// end of wordleGamePlay()

//--------------------------------------------------------------------------
//  Determine longest win streak
void assignLongestWinStreak(int &currentStreak, int &addToStreakCount, int &longestStreakHolder) {
    
    if (currentStreak == 0) { // if user did not successfully complete 3 or 5 letter word currentStreak flag variable equals 0
        addToStreakCount = 0; // reset streak count for next game play attempt
    }

    if (currentStreak == 1) {  // if user successfully completed 3 or 5 letter word currentStreak flag variable equals 1
        addToStreakCount++;    //increment current streak
        if (addToStreakCount > longestStreakHolder) {  //check to see if current streak count should replace longest streak
            longestStreakHolder = addToStreakCount;
        } 
    }
} //end assignLongestWinStreak()

//---------------------------------------------------------------------------
//Output Game play prompt for 3 letter word and 5 letter word instructions
void gamePlayPrompt(int WORD_LENGTH, int TIME_DURATION) {
    cout << "To get started, enter your first " << WORD_LENGTH << " letter word." << endl
        << "You have " << (WORD_LENGTH +1) << " attempts to guess the random word." << endl
        << "The timer will start after your first word entry." << endl
        << "Try to guess the word within " << TIME_DURATION << " seconds." << endl;
} //end gamePlayPrompt()

//---------------------------------------------------------------------------
// Calculate and display average completion time 
// count of words correctly guessed and longest win streak.
int displayStats(
    int & longestStreak,           // Value that holds the longest # of 
    int & totalWordCount,          // Total word count
    int & totalTime)               // Total Time
{
    cout << "Overall Stats:" << endl;

    //Total Word Count
    cout << "  - You guessed: " << totalWordCount << endl;

    //Longest Winning streak
    cout << "  - Your longest streak is: " << longestStreak << endl;
    
    //Total average time
    if (totalWordCount == 0) {
        cout << "  - Average word completion time: N/A" << endl;
    }
    else {
        cout << "  - Average word completion time: " << (totalTime / totalWordCount) << endl;
    }

    return 0;
} //end displayStats()

int main()
{
    int menuChoice;                     // Store user input for menu option chosen
    vector <string> threeLetterWords;   // Vector to store three letter words from file
    vector <string> fiveLetterWords;    // Vector to store five letter words from file
    string randomWordleWord;            // Holds the randomly generated word
    string wordEntry;                   // User entered word
    int totalTime = 0;                  // Holds total amount of time for words spelled correctly
    int totalCount = 0;                 // Total number of words spelled correctly
    int longestStreakHolder = 0;        // Holds the longest winning streak
    int currentStreak = 0;              // Holds value of running streak until game is lost
    int addToStreakCount = 0;           // Variable that keeps track of the current game streak count

    // Read in words from the wordlewords.txt
    readWordsFromFile(threeLetterWords, fiveLetterWords, "wordlewords.txt");

    //Display game instructions
    gameDisplayInstructions();

    // For random number generation seed the random number generator to 1, so that results
    // are predictable.  To make results different every time instead you would
    // use srand( time( NULL));   Don't do this if you want your code to match test cases!
    srand(1);

    // Keep looping until user selects the option to exit
    while (true) {

        cout << endl
            << "Select a menu option:" << endl
            << "   1. To play Wordle Reload 3 letter play" << endl
            << "   2. To play Wordle Reload 5 letter play" << endl
            << "   3. Exit the program" << endl
            << "Your choice --> ";
        cin >> menuChoice;
        cout << endl;

        // Exit if option 3 to exit was chosen
        if (menuChoice == 3) {
            break;      // Break out of enclosing infinite game-play loop
        }

        // Menu option to play 3 letter Wordle Reload game
        if (menuChoice == 1) {

            //Variable used for word length and total time to complete game
            int WORD_LENGTH = 3;
            int TIME_DURATION = 20;

            //Generate random word from vector that contains three letters
            randomWordleWord = generateRandomWord(threeLetterWords);

            //Output game play prompt
            gamePlayPrompt(WORD_LENGTH, TIME_DURATION);

            //Function to begin game play
            wordleGamePlay(randomWordleWord, threeLetterWords, WORD_LENGTH, TIME_DURATION, totalCount, totalTime, currentStreak);
            
            // Assign current streak
            assignLongestWinStreak(currentStreak, addToStreakCount, longestStreakHolder);

            continue;
        }
        else if (menuChoice == 2) { //Menu option to play 5 letter Wordle Reload game

            //Variable used for word length and total time to complete game
            int WORD_LENGTH = 5;
            int TIME_DURATION = 40;

            //generate random word from three letter vector
            randomWordleWord = generateRandomWord(fiveLetterWords);

            //Output game play prompt
            gamePlayPrompt(WORD_LENGTH, TIME_DURATION);

            //Function to begin game play
            wordleGamePlay(randomWordleWord, fiveLetterWords, WORD_LENGTH, TIME_DURATION, totalCount, totalTime, currentStreak);

            // Assign current streak
            assignLongestWinStreak(currentStreak, addToStreakCount, longestStreakHolder); 

            continue;

        } //end if( menuChoice == 2)    
    } //end while( true)

    //DISPLAY OVERALL STATS
    //  -- Longest winning streak  -- Average time for words correctly guessed -- Total number of words completed
    displayStats(longestStreakHolder, totalCount, totalTime);

    cout << "Exiting program" << endl;

    return 0;
}// End of Main

