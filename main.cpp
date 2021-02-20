#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "./lib/path.h"
#include "./lib/function.h"
using namespace std;

#define DBG

#ifdef DBG
#define DEBUG(x) x
#define NORM(x)
#else
#define DEBUG(x)
#define NORM(x) x
#endif

// Function prototypes
int handleInput(const int num, vector<string> args, Path &path);
void displayInstructions();

// Functions
bool functions(const Path &path);
int countFunctions(const Path &path);
int readFunctions(const Path &path, vector<Function> &funcs);
void functionsExist(const Path &path, vector<Function> funcs, int num);
int writeFunctions(const Path &path, vector<Function> funcs, int num);

// Universal
bool isAFunction(string input);
Function *allocateMemory(const Path &path, int (*counter)(const Path &path), int &num);
void deleteMemory(Function *p);

/**********************************************************************
 * Function: main
 * Description: Main will drive the program.
 ***********************************************************************/
int main(int numArgs, char **args)
{
   // Create a file path
   Path filePath;

   // No, do not create a directory
   filePath.createDirectory(false);

   // Convert **args to vector of strings
   vector<string> arguments;
   for (int i = 0; i < numArgs; i++)
   {
      arguments.push_back(args[i]);
   }
   

   // Handle the input and create the specific file paths
   int key = handleInput(numArgs, arguments, filePath);

   functions(filePath);


   return 0;
}

/**********************************************************************
 * Function Name: handleInput
 * Description: Handle all input. Attempt to build a path.
***********************************************************************/
int handleInput(const int num, vector<string> args, Path &path)
{
   int key = 0;

   switch (num)
   {
   case 1:
      displayInstructions();
      exit(0);
   case 2:
      // Check for help
      if (args[1] == "-help" || args[1] == "--help" || args[1] == "-h" || args[1] == "--h")
      {
         displayInstructions();
         exit(0);
      }

      if (!path.buildStdPath(args[1]))
         cout << "Error building your path.\n";
      break;
   default:
      cout << "Not sure what you tried to pass. Use \"function -help\" for help.\n";
      exit(0);
   }

   return key;
}

/**********************************************************************
 * Function Name: displayInstructions
 * Description: Description: Display some o' dose instructions.
***********************************************************************/
void displayInstructions()
{
   // Display some o' dose info stuffies :-)
   cout << "func <file name/path>\n";
   cout << "I have spoken.\n";

   exit(0);
   return;
}

/**********************************************************************
 * Function Name: functions
 * Description: A sub-main. This will handle everything for functions
***********************************************************************/
bool functions(const Path &path)
{
   // Count the number of functions and allocate the memory
   int num = countFunctions(path); // Will be updated when functions are counted
   // Function *funcs = allocateMemory(path, &countFunctions, num);

   vector<Function> funcs;

   cout << "Here: " << path.getStd() << endl;
   cout << num << endl;

   // Some basic error checking
   if (num == 0)
      return false;

   // Try to read the functions
   if (readFunctions(path, funcs))
      return false;

   // Check to see if the functions exist already
   functionsExist(path, funcs, num);

   // Try to write the functions
   if (writeFunctions(path, funcs, num))
      return false;

   // Delete the allocated memory
   // deleteMemory(funcs);

   // Success message
   if (num != 1)
      cout << "Successfully wrote " << num << " functions at\n\t";
   else
      cout << "Successfully wrote " << num << " functions at\n\t";

   cout << path.getWorkingPath() << endl;

   return true;
}

/**********************************************************************
 * Function Name: countFunctions
 * Description: Count the number of functions in the given file
***********************************************************************/
int countFunctions(const Path &path)
{
   // Open the file
   ifstream fin(path.getStd().c_str());

   // Error checking
   if (fin.fail())
      return -1;

   string input;

   // Read the file down till "// Function prototypes" is found
   while (getline(fin, input))
      if (input == "// Function prototypes")
         break;

   // Now start counting the functions
   int number = 0;
   while (getline(fin, input))
   {
      // If there is a blank line or comment, skip it
      while (input.length() == 0 || (input[0] == '/' && input[1] == '/'))
         getline(fin, input);

      // Continue reading lines until a comment is detected
      if (input[0] == '/' && input[1] == '*')
         break;

      // Add up the number of functions
      number++;
   }

   fin.close();

   // Error checking
   if (fin.fail())
      return -1;

   return number;
}

/**********************************************************************
 * Function Name: readFunctions
 * Description: Read the actual functions into the array
***********************************************************************/
int readFunctions(const Path &path, vector<Function> &funcs)
{
   // Open the file
   ifstream fin(path.getStd().c_str());

   // Error checking
   if (fin.fail())
      return -1;

   string input;

   // Read the file down till "// Function prototypes" is found
   while (getline(fin, input))
      if (input == "// Function prototypes")
         break;

   // Now start reading in the functions
   int number = 0;
   while (getline(fin, input))
   {
      // If there is a blank line or comment, skip it
      while (input.length() == 0 || (input[0] == '/' && input[1] == '/'))
         getline(fin, input);

      // Continue reading lines until a comment is detected
      if (input[0] == '/' && input[1] == '*')
         break;

      // Simple as pass the input line and let the class do the work
      Function temp;
      temp.build(input);
      cout << input << endl;
      funcs.push_back(temp);

      number++;
   }

   fin.close();

   // Error checking
   if (fin.fail())
      return -1;

   return 0;
}

/**********************************************************************
 * Function Name: functionsExist
 * Description: Check to see if the function declaration already exists
***********************************************************************/
void functionsExist(const Path &path, vector<Function> funcs, int num)
{
   // Open the file
   ifstream fin(path.getStd().c_str());

   // Error checking
   if (fin.fail())
      return;

   string input;

   // Read the file down till "// Function prototypes" is found
   while (getline(fin, input))
      if (input == "// Function prototypes")
         break;

   // Now keep reading each line, checking to see if it is a function
   while (getline(fin, input))
   {
      // If the end of a comment header is detected
      if (input[0] != '/' && input[5] == '*' && input[12] == '*')
      {
         // ...we've got some stuff to do.

         // Read the next line, which should be a function
         getline(fin, input);

         // Loop through each function already stored
         for (int i = 0; i < num; i++)
         {
            // Make sure we're not reading the same functions every time
            if (!funcs[i].doesExist())
            {
               // Temporary function holder
               Function temp;

               temp.build(input);

               // Statements
               bool returnType = (temp.getReturnType() == funcs[i].getReturnType());
               bool names = (temp.getName() == funcs[i].getName());
               bool parameters = (temp.getParameters() == funcs[i].getParameters());

               // Check to see if the function has the same name
               if (returnType && names && parameters)
                  funcs[i].setExist(true);
            }
         }
      }
   }

   fin.close();

   // Error checking
   if (fin.fail())
      return;

   return;
}

/**********************************************************************
 * Function Name: writeFunctions
 * Description: Pretty simple "writeFunctions". These will append to the end.
 * It isn't possible to insert into the middle of the file.
***********************************************************************/
int writeFunctions(const Path &path, vector<Function> funcs, int num)
{
   // Open the file
   ofstream fout(path.getStd().c_str(), ios::app);

   // Error checking
   if (fout.fail())
      return -1;

   // Number to store the number of funtions written
   int numWritten = 0;

   cout << "Here trying to write!" << funcs.size() << endl;

   // Loop through each function
   for (int i = 0; i < num; i++)
   {
      cout << funcs[i].getName() << endl;
      // Move the next function if the current one exists
      while (funcs[i].doesExist())
         i++;

      // Write the comment block header
      fout << "\n/**********************************************************************\n";
      fout << " * Function Name: " << funcs[i].getName() << endl;
      fout << " * Description: fill in here\n";
      fout << "***********************************************************************/\n";

      // Write the actual function
      if (funcs[i].getIsInline() == true)
         fout << "inline ";

      fout << funcs[i].getReturnType() << " ";
      fout << funcs[i].getName() << "(";
      fout << funcs[i].getParameters() << ")";
      fout << "\n{\n";

      // Put in the correct return type
      if (funcs[i].getReturnType() == "int")
         fout << "   return 0;\n";
      else if (funcs[i].getReturnType() == "bool")
         fout << "   return true;\n";
      else if (funcs[i].getReturnType() == "char")
         fout << "   return 'a';\n";
      else if (funcs[i].getReturnType() == "string")
         fout << "   return \"string goes here\";\n";
      else if (funcs[i].getReturnType() == "float" || funcs[i].getReturnType() == "double")
         fout << "   return 0.0;\n";
      else
         fout << "   return;\n";

      // Closing bracket
      fout << "}\n";

      // Wrote a function, increment numWritten
      numWritten++;
   }

   // Write a comment to indicate this program wrote some code
   if (numWritten > 1)
      fout << "\n// Note: " << numWritten << " functions were created from their prototypes by a program.\n";
   else
      fout << "\n// Note: " << numWritten << " function was created from its prototype by a program.\n";

   // Close the file
   fout.close();

   // Error checking
   if (fout.fail())
      return -1;

   return 0;
}

/**********************************************************************
 * Function Name: isAFunction
 * Description: Return whether the input is a function
***********************************************************************/
bool isAFunction(string input)
{
   // If the input contains an opening parenthesis
   for (int i = 0; i < input.length(); i++)
      if (input[i] == '(')
         return true;

   return false;
}

/**********************************************************************
 * Function Name: allocateMemory
 * Description: Allocate memory by calling the passed pointer to get the 
 *    number of items to allocate.
***********************************************************************/
Function *allocateMemory(const Path &path, int (*counter)(const Path &path), int &num)
{
   // This is some fancy callback stuff - used to count the number of functions/methods
   num = (*counter)(path);

   // Allocate the memory
   try
   {
      Function *p = new Function[num];
      return p;
   }
   catch (bad_alloc)
   {
      cout << "There was an error allocating memory.\n";
      return NULL;
   }
}

/**********************************************************************
 * Function Name: deleteMemory
 * Description: Delete the memory allocated previously
***********************************************************************/
void deleteMemory(Function *p)
{
   // Pretty standard stuff. Just delete
   delete[] p;

   // Set 'p' to null
   p = NULL;

   return;
}

// Note: 14 functions were created from their prototypes by a program.
