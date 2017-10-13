/*************************************************************************************
 * Name: Vishnu Pydah
 * Date: 9/22/2017
 *************************************************************************************/

#include "Sudoku.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

/**
 * default constructor that assumes a 9 x 9 board filled with zeroes
 * Sudoku object can read in a board from a file, solve its current board, as well
 * as print out the board
 */
Sudoku::Sudoku() : side_length(9), box_size(3), fill_counter(0) {
    reset();
}

/**
 * Stores a Sudoku board configuration from the given textfile name / path
 * Calls reset if error is detected
 *
 * @param filename (textfile path/name containing Sudokuboard in
 * appropriate format)
 */
void Sudoku::loadFromFile(std::string filename) {
    std::ifstream file(filename.c_str());
    std::string str;

    SudoBoard.clear();              // clears array and resets board size value
    side_length = 0;
    fill_counter = 0;

    if (!file) { // exit if unable to find file
        std::cout << "Unable to open file!" << std::endl;
        getline(std::cin, str);
        std::cin.get();
        exit(1);
    }

    while (std::getline(file, str)) {
        std::vector<int> tmp;              // stores each row in tmp

        for (char x : str) {
            if (x != ' ') {
                tmp.push_back(x - '0');
            }
        }

        SudoBoard.push_back(tmp);                //pushes each row to 2D vector
        tmp.clear();
        side_length++;
    }

    box_size = (int) (sqrt(side_length));        //assigns Sudoku "box" sizes
}

/**
 * Prints out the Sudoku board
 */
void Sudoku::print() const {

    if (side_length == 9) {                        //print format for 9 x 9
        for (int x = 0; x < side_length; ++x) {
            for (int y = 0; y < side_length; ++y) {
                if (y % 3 == 0 && y != 0 && y != side_length) {
                    std::cout << "| ";
                }
                std::cout << SudoBoard[x][y] << " ";
            }

            if ((x + 1) % 3 == 0 && x != side_length - 1 && x != 0) {
                std::cout << std::endl;
                std::cout << "------+-------+------";
            }


            std::cout << std::endl;
        }
    } else {                                     //print format for all other board sizes
        for (int x = 0; x < side_length; ++x) {
            for (int y = 0; y < side_length; ++y) {
                std::cout << SudoBoard[x][y] << " ";
            }

            std::cout << std::endl;
        }
    }
}

/**
 * Solves Sudoku board by getting first open square, then calling place on this
 * open
 * square. Actively modifies the existing board.
 *
 * @return true if solution exists, false if not solution exists
 */
bool Sudoku::solve() {
    std::pair<int, int> firstOpenSquare = firstPass();

    return place(firstOpenSquare);
}

/**
 * function that returns a bool based off whether or not the two Sudoku board objects
 * have the same dimensions and corresponding values in each square
 *
 * @param other (Sudoku object to check equality with)
 * @return true if boards match, false if not
 */
bool Sudoku::equals(const Sudoku &other) const {
    if (this->side_length != other.side_length) {         //if sizes don't match
        return false;
    }

    for (int x = 0; x < side_length; ++x) {
        for (int y = 0; y < side_length; ++y) {
            if (SudoBoard[x][y] != other.SudoBoard[x][y]) { //if all squares match
                return false;
            }
        }
    }

    return true;
}

/**
* Given a square and value on the board, checks column, row, and appropriate Sudoku block
* for numbers already in use. Returns false if the value is already in use, true if not.
*
* @param square (pair representing the appropriate 'square' on the board), val
 * (integer value to check)
* @return false if the value val is not a potential value on the square, true if not
*/
bool Sudoku::place(std::pair<int, int> thisSquare) {
    // iterates over all possible numbers for a square, and calls place on the
    // next open square
    if (fill_counter < side_length) {   //if not enough squares are full
        ++fill_counter;
        for (int x = 1; x <= side_length; x++) {
            if (!isValuePossible(thisSquare, x)) {
                continue;
            }
            SudoBoard[thisSquare.first][thisSquare.second] = x;
            std::pair<int, int> nextSquare = openSquare(thisSquare);

            if (place(nextSquare)) {
                return true;
            }
        }
    } else {                        //if enough squares are full use smartplace strategy
        return smartPlace();
    }


    SudoBoard[thisSquare.first][thisSquare.second] = 0; // if no solutions are found, reset
    --fill_counter;
    return false;
}

/**
 * Useful, more intelligent place method that checks the board for squares with minimum
 * numbers of possibilities before proceeding to place and iterate through possibilites
 * through recursive backtracking.
 *
 * @return true if a solution is found on the board, false if not
 */
bool Sudoku::smartPlace() {
    std::vector<int> allpossibles;
    std::pair<int, int> thisSquare = leastAmbiguousSquare(allpossibles);

    if (fill_counter == side_length * side_length) {  //if board is full return true
        return true;
    }

    if (thisSquare.first == -1) {    //if there is an impossible square to satisfy
        return false;
    }

    // iterates over all possible numbers for a square, and calls place on the
    // next open square
    ++fill_counter;
    for (int x = 0; x < allpossibles.size(); x++) {
        SudoBoard[thisSquare.first][thisSquare.second] = allpossibles[x];
        if (smartPlace()) {
            return true;
        }
    }

    SudoBoard[thisSquare.first][thisSquare.second] = 0; // reset if no solutions are found
    --fill_counter;                                     // decrements board fill count
    return false;
}

/**
 * Given a square on the board, checks column, row, and appropriate Sudoku block
 * for numbers already in use. Returns a std::vector of potential numbers to be
 * tried on
 * the square.
 *
 * @param square (pair representing the appropriate 'square' on the board)
 * @return std::vector of potential numbers that could be on the 'square'
 */
bool Sudoku::isValuePossible(std::pair<int, int> square, int val) const {

    // checks potential value against local column square values and row square values
    // that are already in use
    for (int x = 0; x < side_length; ++x) {
        if (SudoBoard[square.first][x] == val || SudoBoard[x][square.second] == val)
            return false;
    }

    // decides if val is valid by checking local Sudoku 3 x 3 "square" for values
    // already in use
    int left_upper_row = box_size * (square.first / box_size);
    int left_upper_col = box_size * (square.second / box_size);

    for (int a = 0; a < box_size; ++a) {
        for (int b = 0; b < box_size; ++b) {
            if (SudoBoard[left_upper_row + a][left_upper_col + b] == val)
                return false;
        }
    }

    return true;
}


/**
 * Helper method for advancedPlace
 * Given a square and a vector, it fills the vector with potential values of the square
 *
 * @param square (representing square to retrieve all possible values for)
 * @param tmp (tmp is filled with all possible values)
 */
void Sudoku::getAllPotentialValues(std::pair<int, int> square, std::vector<int> &tmp) {
    tmp.clear();

    for (int x = 1; x <= side_length; ++x) {
        tmp.push_back(x);
    }
    // eliminates possible values from potentialValues by checking local column
    // and row squares for values already in use
    for (int x = 0; x < side_length; ++x) {
        tmp.erase(std::remove(tmp.begin(), tmp.end(), SudoBoard[square.first][x]),
                  tmp.end());
        tmp.erase(std::remove(tmp.begin(), tmp.end(), SudoBoard[x][square.second]),
                  tmp.end());
    }


    // eliminates possible values from potentialValues by checking local Sudoku
    // 3 x 3 "square" for values already in use
    int left_upper_row = box_size * (square.first / box_size);
    int left_upper_col = box_size * (square.second / box_size);

    for (int a = 0; a < box_size; ++a) {
        for (int b = 0; b < box_size; ++b) {
            tmp.erase(std::remove(tmp.begin(), tmp.end(),
                                  SudoBoard[left_upper_row + a][left_upper_col + b]),
                      tmp.end());
        }
    }
}

/**
 * Finds the next open square on the board given the current 'square' on the
 * board.
 *
 * @param currentSquare (current std::pair<int,int> representing coordinates of
 * board square)
 * @return std:pair<int,int> representing the next open square on the board
 */
std::pair<int, int> Sudoku::openSquare(std::pair<int, int> currentSquare) const {
    std::pair<int, int> bestSquare;

    // maps currentSquare to a value 0 - 80, and iterates from that value to 80
    for (int x = currentSquare.first * side_length + currentSquare.second + 1;
         x < side_length * side_length; ++x) {

        // calculates appropriate row and col based off mapped x value
        int row_number = x / side_length;
        int col_number = x - row_number * side_length;

        // if the current square is open, return the coords
        if (SudoBoard[row_number][col_number] == 0) {
            bestSquare.first = row_number;
            bestSquare.second = col_number;
            return bestSquare;
        }
    }

    if (bestSquare.first == -1) {
        return openSquare(std::pair<int, int>(0, 0));
    }

    return {-1, -1}; // returned if no open squares are left
};

/**
 * Helper method for advancedPlace. Searches board for a square with the minimal number
 * of possibilities and returns the square. Also fills vector myvec reference with the
 * possibilites on that optimal square.
 *
 * @param myvec (vector to be populated with possibilities for optimal square)
 * @return (optimal square)
 */
std::pair<int, int> Sudoku::leastAmbiguousSquare(std::vector<int> &myvec) {
    //sets min value to be impossibly high
    int min_value = side_length + 1;
    std::pair<int, int> bestSquare(-1, -1);
    std::vector<int> tmp;
    // maps currentSquare to a value 0 - 80, and iterates from that value to 80
    for (int x = 0; x < side_length * side_length; ++x) {

        // calculates appropriate row and col based off mapped x value
        int row_number = x / side_length;
        int col_number = x - row_number * side_length;

        if (SudoBoard[row_number][col_number] == 0) {
            // if square is empty, retrieve all possible values for the square
            getAllPotentialValues(std::pair<int, int>(row_number, col_number), tmp);
            //curr_value stores number of possibilities on the specified square
            int curr_value = tmp.size();

            if (curr_value == 0) {  //if an impossible square is found, return (-1,-1)
                return {-1, -1};
            }

            if (curr_value == 1) {  //if a square with only 1 possibility is found
                myvec = tmp;
                return {row_number, col_number};
            }

            if (curr_value < min_value) {   //if lower possible square is found save data
                min_value = curr_value;
                bestSquare.first = row_number;
                bestSquare.second = col_number;
                myvec = tmp;
            }
        }


    }
    return bestSquare; // returned optimal square to be placed on
};

/**
 * More complex version of openSquare: searches for the square with the minimum
 * number of possibilities and returns it. Designed to be called once in solve.
 *
 * Also modifies private variable fill_counter every time it discovers a non-empty square
 * on the board
 *
 * @return pair (indicating best initial square to begin solving with)
 */
std::pair<int, int> Sudoku::firstPass() {
    bool valueFound = false;
    std::pair<int, int> bestSquare = {-1, -1};

    // maps currentSquare to a value 0 - 80, and iterates from that value to 80
    for (int x = 0; x < side_length * side_length; ++x) {

        // calculates appropriate row and col based off mapped x value
        int row_number = x / side_length;
        int col_number = x - row_number * side_length;

        // if the current square has the minimum possibilties, save data on square
        if (SudoBoard[row_number][col_number] == 0) {
            if (!valueFound) {
                bestSquare.first = row_number;
                bestSquare.second = col_number;
                valueFound = true;
            }
        } else {
            ++fill_counter;
        }
    }

    return bestSquare; // returns optimal square
};


/**
 * resets board to 9 x 9 Sudoku board filled with zeroes
 */
void Sudoku::reset() {

    SudoBoard.clear();
    side_length = 9;
    box_size = 3;
    fill_counter = 0;

    for (int x = 0; x < 9; x++) {
        std::vector<int> tmp;           // stores each row in tmp

        for (int y = 0; y < 9; y++) {
            tmp.push_back(0);           //fills row with zeroes
        }

        SudoBoard.push_back(tmp);       //puts tmp in board
        tmp.clear();
    }
}


