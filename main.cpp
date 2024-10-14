//  Program 5: The Scrambler
//  Course: CS 141, Fall 2023
//  System: Replit
//  Author: Hiba Mirza

#include <iostream>
#include <fstream>    
#include <vector>     
#include <cassert>    
#include <cctype>     
#include <ctime>
#include <string> 
#include <algorithm>  
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

//the Board class is used to generate and manipulate the board
class Board{
    public:
        Board(int boardSizeSet, int theScrambledTime, vector<string> dictionary);       
        void displayBoard();    
        void choice(char &userChoice);      
        void rotate();
        void origin();
        void reset();
        void generate();
        void solve();
        void checkWin(char &userchoice);

    private:
        int boardSize;
        int scrambledTime;
        string boardString;
        vector<string> dictionaryClass;
        vector<string> wordList;
        string completedBoard;
        string scrambledBoard;
};

//reads in the dictionary from a file and stores it in a vector
void readDictionary(vector<string>& dictionary) {
    ifstream inStream("dictionary.txt");
    string word;
    while (getline(inStream, word)) {
        dictionary.push_back(word);
    }
}

//rotates a row of the board by the given number of positions
void rotateRow(string& boardString, int boardSize, int rowNumber, int rotation) {
    int firstIndex = rowNumber * boardSize;
    int lastIndex = (rowNumber + 1) * boardSize - 1;
    string rowString = boardString.substr(firstIndex, boardSize);

    if (rotation > 0) {
        rotate(rowString.rbegin(), rowString.rbegin() + rotation, rowString.rend());
    }
    else {
        rotation = -rotation;
        rotate(rowString.begin(), rowString.begin() + rotation, rowString.end());
    }

    boardString.replace(firstIndex, boardSize, rowString);
}

//rotates a column of the board by the given number of positions
void rotateColumn(string& boardString, int boardSize, int columnNumber, int rotation) {
    const string tempString = boardString;
    for (int row = 0; row < boardSize; row++) {
        int oldIndex = row * boardSize + columnNumber;
        int newIndex = row * boardSize + (columnNumber + rotation + boardSize) % boardSize;
        boardString[newIndex] = tempString[oldIndex];
    }
}

Board::Board(int boardSizeSet, int theScrambledTime, vector<string> dictionary) :
    boardSize(boardSizeSet),
    scrambledTime(theScrambledTime),
    dictionaryClass(dictionary),
    boardString(boardSizeSet * boardSizeSet, ' '), 
    wordList() 
{
    int spaceRemain = boardSizeSet * boardSizeSet;
    //fill the board with valid words from dictionary
    while (spaceRemain > 2) { 
        string word = dictionaryClass[rand() % dictionaryClass.size()];
        if (word.length() + 1 <= spaceRemain) { 
            word += ' '; 
            boardString.replace(boardString.find(' ', rand() % boardString.length()), word.length(), word); 
            wordList.push_back(word);
            spaceRemain -= word.length();
        }
    }
    //save the original board state
    completedBoard = boardString;
    //randomly scramble the board
    for (int i = 0; i < theScrambledTime; i++) {
        rotateRow(boardString, boardSizeSet, rand() % boardSizeSet, 1);
        rotateColumn(boardString, boardSizeSet, rand() % boardSizeSet, 1);
    }
    //save the scrambled board state
    scrambledBoard = boardString;
}

//function to check if the user has won
void Board::checkWin(char& userChoice) {
    if (boardString == completedBoard) {      
        displayBoard();
        cout << "Current board words: " << boardString << endl;
        cout << "Congratulations, you won!\n";      
        userChoice = 'Q';
    }
    else {    
        displayBoard();
    }
}

//function to display the current state of the board
void Board::displayBoard() {
    cout << "Words to find:" << endl;
    for (int i = 0; i < wordList.size(); i++) {
        if (i != wordList.size() - 1) {
            cout << wordList[i] << endl;
        }
        else {
            cout << wordList[i];
        }
    }

    cout << endl << "Current board: " << endl;
    //display the column numbers
    cout << "    ";
    for (int i = 1; i <= boardSize; i++) {
        cout << i << "   "; 
    }
    //display the horizontal separator
    cout << endl << "   " << setfill('-') << setw(boardSize * 4) << " " << endl;
    //display the rows
    for (int i = 1; i <= boardSize; i++) {
    cout << i << " |";
    for (int j = 0; j < boardSize; j++) {
        cout << " " << boardString[(i-1)*boardSize + j] << " |";       }
    //display the horizontal separator
    cout << endl << "   " << setfill('-') << setw(boardSize * 4) << " " << endl;
}
    //display the current board state
    cout << "Current board words: " << boardString << endl;
}
//function to rotate a row or column of the board
void Board::rotate() {
    char userChoice = ' ';
    int rowColumnNum = -1, rotatePos = 0;

    while (true) {
        cout << endl;
        cout << "Enter the row and column to rotate and the number of positions to rotate in the format <R/C> <row/column number> <number of positions>, valid numbers between 0 and " << boardSize-1 << endl;
        cout << "Your choice: " << endl;

        cin >> userChoice >> rowColumnNum >> rotatePos;
        userChoice = toupper(userChoice);

        if ((userChoice != 'R' && userChoice != 'C') ||
            (rowColumnNum < 0 || rowColumnNum >= boardSize)) {
            cout << "Invalid input. Try again." << endl;
            continue;
        }

        break;
    }

    if (userChoice == 'R') {
        rotateRow(boardString, boardSize, rowColumnNum, rotatePos);
    } else {
        rotateColumn(boardString, boardSize, rowColumnNum, rotatePos);
    }
}
//displays the completed board
void Board::origin() {
    cout << "The completed board should look like:" << endl; 
    for (int i = 0; i < boardSize; i++) {
        cout << i << "   ";
    }
    cout << endl << "      " << setfill('-') << setw(boardSize * 4 ) << " " << endl;
    for (int i = 0; i < boardSize; i++) {
        cout << "   " << i << " |";
        for (int j = 0; j < boardSize; j++) {
            cout << " " << completedBoard[i*boardSize + j] << " |";
        }
        cout << endl << "      " << setfill('-') << setw(boardSize * 4 ) << " " << endl;
    }

    displayBoard();
}
//resets the board to the scrambled state
void Board::reset() {
    boardString = scrambledBoard;
    displayBoard();
}
//generates a new board
void Board::generate() {
    boardString.clear();
    wordList.clear();
    int spaceRemain = boardSize * boardSize;

    while (spaceRemain > 0) {
        string word = dictionaryClass[rand() % dictionaryClass.size()];
        if (word.length() > spaceRemain) {
            continue;
        }
        word.append(" ");
        spaceRemain -= word.length();
        boardString.append(word);
        wordList.push_back(word);
    }

    completedBoard = boardString;

    for (int i = 0; i < scrambledTime; i++) {
        rotateRow(boardString, boardSize, rand() % boardSize, 1);
        rotateColumn(boardString, boardSize, rand() % boardSize, 1);
    }

    scrambledBoard = boardString;

    displayBoard();
}

void Board::solve() {
    cout << "hello" << endl;
}
//handles user's choice of action
void Board::choice(char &userChoice) {
    while (userChoice != 'Q') {
        cout << "Enter one of the following: " << endl;
        cout << "R (rotate), C (completed board), B (reset), G (generate new board), Q (quit)\n"
                "Your choice: ";
        cin >> userChoice;
        userChoice = toupper(userChoice);

        switch (userChoice) {
            case 'R':
                rotate();
                checkWin(userChoice);
                break;
            case 'C':
                origin();
                break;
            case 'B':
                reset();
                break;
            case 'G':
                generate();
                break;
            case 'S':
                solve();
                break;
            case 'Q':
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}
//the main function that controls the game
int main() {
    char choice = ' ';
    vector<string> dictionary;
    srand(1);

    readDictionary(dictionary);

    while (toupper(choice) != 'Q') {
        cout << "The Scrambler" << endl;
        cout << endl;
        int boardSize, scrambledTime;

        do {
        cout << "Enter board size (anything from 3-9): ";
        cin >> boardSize;
        } while (boardSize < 3 || boardSize > 9);

        Board theBoard(boardSize, scrambledTime, dictionary);
        theBoard.displayBoard();
        theBoard.choice(choice);
    }

    cout << "Thank you for playing!\n"
            "Exiting program";
    return 0;
}