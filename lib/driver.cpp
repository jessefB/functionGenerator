/***********************************************************************
* Program:
*    Project , (fill in name here)
*    Brother Manley, CS165
* Author:
*    Jesse Fryar
* Summary: 
*    Enter a brief description of your program here!  Please note that if
*    you do not take the time to fill out this block, YOU WILL LOSE POINTS.
*    Before you begin working, estimate the time you think it will
*    take you to do the assignment and include it in this header block.
*    Before you submit the assignment include the actual time it took.
*
*    Estimated:  0.0 hrs   
*    Actual:     0.0 hrs
*      Please describe briefly what was the most difficult part.
************************************************************************/

#include "path.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Function prototypes

/**********************************************************************
 * Function: main
 * Description: Main will drive the program.
 ***********************************************************************/
int main()
{
     Path myPath;

     // Output the working directory
     cout << "Working directory:\n\t";
     cout << myPath.getWorkingPath() << endl
          << endl;

     // Try to build a path
     myPath.createDirectory(true);
     myPath.buildStdPath("cp09b");

     // Print it out
     cout << "cp09b buildStdPath():\n\t";
     cout << myPath.getStd() << endl
          << endl;

     // Output the working directory
     cout << "Working directory:\n\t";
     cout << myPath.getWorkingPath() << endl
          << endl;

     Path myPathTwo;

     // Build a path again
     myPathTwo.buildStdPath("c:/users/jesse/documents/test.cpp");

     // Print it out
     cout << "Absolute path - buildStdPath():\n\t";
     cout << myPathTwo.getStd() << endl
          << endl;

     // Output the working directory
     cout << "Working directory:\n\t";
     cout << myPathTwo.getWorkingPath() << endl
          << endl;

     Path myPathThree;

     // Try a class
     myPathThree.createDirectory(true);
     myPathThree.buildClassPath("c:/users/jesse/documents/classTest/", "classNameTest");

     // Print them out

     cout << myPathThree.getStd() << endl;
     cout << myPathThree.getHeader() << endl;
     cout << myPathThree.getSource() << endl;

     return 0;
}
