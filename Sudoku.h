/*************************************************************************************
 * Name: Vishnu Pydah
 * Date: 9/28/2017
 *************************************************************************************/

#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>
#include <vector>

/**
 * class that reads in Sudoku board from an appropriately formatted textfile and then
 * solves the puzzle (board must be a square board with a square number side length)
 */
class Sudoku {

public:
    /**
     * default constructor that assumes a 9 x 9 board filled with zeroes
     */
    Sudoku();

    /**
    *Stores a Sudoku board configuration from the given textfile name / path
    *
    *@param filename (textfile path/name containing Sudokuboard start in
    *appropriate
    * format)
    */
    void loadFromFile(std::string filename);

    /**
    *Prints out the Sudoku board
    */
    void print() const;

    /**
    * Solves Sudoku board by getting first open square, then calling place on this
    * open
    * square. Activitely modifies the existing board.
    *
    * @return true if solution exists, false if not solution exists
    */
    bool solve();

    /**
    * Checks whether every square on the other Sudoku object's board is equal to
    * the
    * corresponding square on this board
    *
    * @param other (Sudoku board object to be tested for equality)
    * @return true if all values match, false if not
    */
    bool equals(const Sudoku &other) const;

private:

    std::vector<std::vector<int>> SudoBoard; // stores Sudoku Board


    int side_length; // stores number of rows and cols in the board
    int box_size;    //side length of each Sudoku inner box (square root of side_length)
    int fill_counter; //number of non-empty spaces on the board

    /**
    * Recursive function that attempts to evaluate the Sudoku board. Recursively
    * backtracks and attempts all possibilities. A 'true' value propagates up the
    * recursive stack when there are no open squares left, a false propagates up
    * the stack
    * when there all possibilities have been exhausted and there are no solutions.
    *
    * @param thisSquare (open square on the board)
    * @return true if board is filled successfully, or if place(nextSquare) is
    * true. False
    * if no possibilities are found.
    */
    bool place(std::pair<int, int> thisSquare);

    /**
     * Useful, more intelligent place method that checks the board for squares with minimum
    * numbers of possibilities before proceeding to place and iterate through possibilites
    * through recursive backtracking.
    *
    * @return true if a solution is found on the board, false if not
    */
    bool smartPlace();

    /**
   * Given a square and value on the board, checks column, row, and appropriate Sudoku block
   * for numbers already in use. Returns false if the value is already in use, true if not.
   *
   * @param square (pair representing the appropriate 'square' on the board), val (value
     * to check)
   * @return false if the value val is not a potential value on the square, true if not
   */
    bool isValuePossible(std::pair<int, int> square, int val) const;

    /**
    * Helper method for advancedPlace
    * Given a square and a vector, it fills the vector with potential values of the square
    *
    * @param square (representing square to retrieve all possible values for)
    * @param tmp (tmp is filled with all possible values)
    */
    void getAllPotentialValues(std::pair<int, int> square, std::vector<int> &);

    /**
    * Finds the next open square on the board given the current 'square' on the
    * board.
    *
    * @param currentSquare (current std::pair<int,int> representing coordinates of
    * board
    * square
    * @return std:pair<int,int> representing the next open square on the board
    */
    std::pair<int, int> openSquare(std::pair<int, int> currentSquare) const;

    /**
    * Helper method for advancedPlace. Searches board for a square with the minimal number
    * of possibilities and returns the square. Also fills vector myvec reference with the
    * possibilites on that optimal square.
    *
    * @param myvec (vector to be populated with possibilities for optimal square)
    * @return (optimal square)
    */
    std::pair<int, int> leastAmbiguousSquare(std::vector<int> &);

    /**
    * More complex version of openSquare: searches for the square with the minimum
    * number of possibilities and returns it. Designed to be called once in solve.
    *
    * Also modifies private variable fill_counter every time it discovers a non-empty square
    * on the board
    *
    * @return pair (indicating best initial square to begin solving with)
    */
    std::pair<int, int> firstPass();

    /**
    * resets the board to 9 x 9 board filled with zeroes
    */
    void reset();
};

#endif // ends SUDOKU_H