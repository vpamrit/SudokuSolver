//courtesy of Roth

#include <iostream>
#include <string>
#include <time.h>
#include "Sudoku.h"

int main(int argc, char * argv[]) {
   std::string ans, filename;
   Sudoku puzzle;
   Sudoku solution;

   const int num = 9; // number of test cases
   std::string infile[] = {
      "tests/sudoku-test1.txt",
      "tests/sudoku-test2.txt",
      "tests/sudoku-one.txt",
      "tests/sudoku-hard1.txt",
      "tests/sudoku-hard2.txt",
      "tests/diabolic1-start.txt",
      "tests/diabolic2-start.txt",
      "tests/sudoku-impossible.txt",
      "tests/curtis2.txt"
   };

   std::string outfile[] = {
      "tests/sudoku-test1-solution.txt",
      "tests/sudoku-test2-solution.txt",
      "tests/sudoku-one-solution.txt",
      "tests/sudoku-hard1-solution.txt",
      "tests/sudoku-hard2-solution.txt",
      "tests/diabolic1-done.txt",
      "tests/diabolic2-done.txt",
      "tests/diabolic2-done.txt",
      "tests/curtis2-solved.txt"
   };

   std::cout << "\nRunning Several Tests" << std::endl;
   std::cout << "------------------" << std::endl << std::endl;

   for (int i = 0; i < num; i++) {
      std::cout << "=========================================="
           << "\nsolving " << infile[i] << std::endl;
      puzzle.loadFromFile(infile[i]);
      solution.loadFromFile(outfile[i]);

      if (puzzle.equals(solution))  // should not be equal
         std::cout << "FAILURE OF EQUALS METHOD *************************" << std::endl;


      clock_t startTime = clock(); 
      clock_t endTime;

      if (puzzle.solve()) {
        endTime = clock(); 
        if (puzzle.equals(solution)) {
            std::cout << "Pass" << std::endl;
            puzzle.print();
         } else {
            std::cout << "\nFail ++++++++++++++++++++++\n" << std::endl;
            puzzle.print();
         }
      } else {
         endTime = clock(); 
         std::cout << std::endl << "No Solution" << std::endl; // indicate there
                                                               // is no solution

		 if (i != num-2) {  //only the second to last test does not have a solution
			 std::cout << "\nFail ++++++++++++++++++++++\nSolution was expected!!\n";
		 }
      }

      std::cout << "Time used: " << (endTime - startTime)/(double)CLOCKS_PER_SEC;
      std::cout << " seconds." << std::endl;
   } 

   //std::cout << "Press enter to continue" << std::endl;
   //cin.get();

   return 0;
}

