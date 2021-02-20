/***********************************************************************
* File: function.cpp
* Description: Source for the Function class. This class will parse a function
*  prototype into its seperate parts.
************************************************************************/

#include "function.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Constructors

/*********************************************
 * FUNCTION :: Constructor ~ Default
 * Initialize the function instance
 *********************************************/
Function::Function()
{
   // Nothing much to set up
}

// Private methods

/*********************************************
 * FUNCTION :: parseFunction
 * Take the input and parse it apart into its 
 * seperate parts
 *********************************************/
void Function::parseFunction()
{
   // Number to store where along the string we are
   int num = 0;

   // Remove the spaces if they exist
   if (this->input[0] == ' ' && this->input[1] == ' ' && this->input[2] == ' ')
      num = 3;

   // Remove the 'std::'
   if (this->input[num + 0] == 's' && this->input[num + 1] == 't' && this->input[num + 2] == 'd')
      num += 5;

   // Return type
   this->setReturnType(this->parseTillChar(' ', this->input, num));

   // Check to see if the function is actualy inline
   if (this->getReturnType() == "inline")
   {
      this->setIsInline(true);
      this->setReturnType(this->parseTillChar(' ', this->input, num));
   }

   // Next parse out the name
   this->setName(this->parseTillChar('(', this->input, num));

   // ...and then the parameters (defaults will be removed here)
   this->setParameters(this->parseTillChar(')', this->input, num));

   return;
}

// void Function::parseFunction()
// {
//    // Remove spaces if they exist
//    if (this->input[0] == ' ' && this->input[1] == ' ' && this->input[2] == ' ')
//       cutFunction(this->input, 3);

//    // Check for a few things first:
//    // Check for inline keyword
//    if (this->input.find("inline") != string::npos)
//       this->isInline = true;
//    if (this->input.find("const") != string::npos)
//       this->isConst = true;
//    if (this->input.find("virtual") != string::npos)
//       this->isVirtual = true;
// }

/*********************************************
 * FUNCTION :: parseTillChar
 * Parse till the give character is found
 *********************************************/
string Function::parseTillChar(const char input, const string line, int &num)
{
   // Simply loop through the line and copy it over until the character is found
   string temp;

   for (num; num < line.length(); num++)
   {
      if (line[num] == input)
         break;

      temp += line[num];
   }

   // Move the indicator off of the current character
   num++;

   return temp;
}

/*********************************************
 * FUNCTION :: stringContains
 * See if and where a string contains another string
 *********************************************/
int Function::stringContains(string find, string input)
{
   if (input.length() > find.length())
      return 0;

   if (find.find(input) != string::npos)
   {
      string temp;

      for (int i = 0; i < find.length(); i++)
         if (find[i] == input[0])
            for (int l = 0; l < input.length(); l++)
            {
               // If we did find the string, return the last character's position
               if (l == input.length())
                  return i + l;

               // If we didn't find the string
               if (find[i + l] != input[l])
                  break;
            }
   }

   return 0;
}

/*********************************************
 * FUNCTION :: cutFunction
 * Cut off the front characters of a function
 *********************************************/
void Function::cutFunction(string &input, int num)
{
   string temp;

   // Copy the string over, starting at the 'num' character in
   for (int i = num; i < input.length(); i++)
      temp[i - num] = input[i];

   // Transfer the string over
   input = temp;

   return;
}

/*********************************************
 * FUNCTION :: removeDefaultParameters
 * Remove the default parameters
 *********************************************/
void Function::removeDefaultParameters()
{
   // String to copy to
   string temp;

   // Loop through the entire input
   for (int i = 0; i < this->parameters.length(); i++)
   {
      // If an equals sign is detected in the next spot,
      if (this->parameters[i + 1] == '=')
         // ...keep incrementing 'i' until we pass the equals sign
         while (i++)
            if (this->parameters[i] == ',')
               break;

      // Check to make sure we haven't passed the end of the parameter string
      if (i >= this->parameters.length())
      {
         this->parameters = temp;
         return;
      }

      temp += this->parameters[i];
   }

   // Finally move temp back over to parameters
   this->parameters = temp;

   return;
}

// Public methods

/*********************************************
 * FUNCTION :: build
 * Run all the methods against the input
 *********************************************/
void Function::build()
{
   // Parse the input line
   this->parseFunction();

   return;
}

/*********************************************
 * FUNCTION :: build
 * Run all the methods against the input (with 
 * built-in set input)
 *********************************************/
void Function::build(string input)
{
   // First, set the input
   this->setInput(input);

   // Then call the pre-built 'build' function
   this->build();

   return;
}

/*********************************************
 * FUNCTION :: getInput
 * Return the input
 *********************************************/
string Function::getInput() const
{
   return this->input;
}

/*********************************************
 * FUNCTION :: setInput
 * Verify and set the input
 *********************************************/
void Function::setInput(string input)
{
   // Verification

   // Check for comments
   if (input.find("/") != string::npos)
      input = "";

   this->input = input;

   return;
}

/*********************************************
 * FUNCTION :: getIsInline
 * Return if the function is inline
 *********************************************/
bool Function::getIsInline() const
{
   return this->isInline;
}

/*********************************************
 * FUNCTION :: setIsInline
 * Set 'isInline'
 *********************************************/
void Function::setIsInline(bool isInline)
{
   this->isInline = isInline;

   return;
}

/*********************************************
 * FUNCTION :: getIsConst
 * Return whether the method is constant
 *********************************************/
bool Function::getIsConst() const
{
   return this->isConst;
}

/*********************************************
 * FUNCTION :: setIsConst
 * Set 'isConst'
 *********************************************/
void Function::setIsConst(bool isConst)
{
   this->isConst = isConst;

   return;
}

/*********************************************
 * FUNCTION :: getReturnType
 * Return the return type
 *********************************************/
string Function::getReturnType() const
{
   return this->returnType;
}

/*********************************************
 * FUNCTION :: setReturnType
 * Set the return type (should not be used if 
 * 'build' is used)
 *********************************************/
void Function::setReturnType(string returnType)
{
   // Verification

   // Check for comments
   if (returnType.find("/") != string::npos)
      returnType = "";

   this->returnType = returnType;

   return;
}

/*********************************************
 * FUNCTION :: getName
 * Return the name
 *********************************************/
string Function::getName() const
{
   return this->name;
}

/*********************************************
 * FUNCTION :: setName
 * Set the name (should not be used if 'build' is
 * used)
 *********************************************/
void Function::setName(string name)
{
   // Check for comments
   if (name.find("/") != string::npos)
      name = "";

   this->name = name;

   return;
}

/*********************************************
 * FUNCTION :: getParameters
 * Return the parameters
 *********************************************/
string Function::getParameters() const
{
   return this->parameters;
}

/*********************************************
 * FUNCTION :: setParameters
 * Set the parameters (should not be used if 'build'
 * is used)
 *********************************************/
void Function::setParameters(string parameters)
{
   // Check for comments
   if (parameters.find("/") != string::npos)
      parameters = "";

   this->parameters = parameters;

   // Remove the defaults in the parameters
   this->removeDefaultParameters();

   return;
}

/*********************************************
 * FUNCTION :: doesExist
 * Returns whether the function exists or not
 *********************************************/
bool Function::doesExist() const
{
   return this->exists;
}

/*********************************************
 * FUNCTION :: setExist
 * Set whether the function exists or not
 *********************************************/
void Function::setExist(bool exists)
{
   this->exists = exists;

   return;
}

// Deconstructor
/*********************************************
 * FUNCTION :: Deconstructor
 * Nothing really to clean up
 *********************************************/
Function::~Function()
{
}