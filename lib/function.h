/***********************************************************************
* File: function.h
* Description: Header for the function class. Function class will store 
*  and parse the function/method prototypes. This will store ONE function.

* Note: The term function will be used in this class, however, it will also 
*  apply to methods.
************************************************************************/

#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <string>

class Function
{
private:
   // Input string (shouldn't change once it is set)
   std::string input;

   // Function parts
   bool isInline = false;
   bool isConst = false;
   bool isVirtual = false;
   std::string returnType;
   std::string name;
   std::string parameters;

   // Place to store wheter the function exists in a file
   bool exists = false;

   // Parse the function into it's seperate parts
   void parseFunction();

   // Basic 'parse till a character is encoutered' method (used by 'parseFunction')
   std::string parseTillChar(const char input, const std::string line, int &num);

   // See if and where a string contains another string
   int stringContains(std::string find, std::string input);

   // Function to cut off the front 'num' characters from a string
   void cutFunction(std::string &input, int num);

   // Remove any default parameters
   void removeDefaultParameters();

public:
   // Constructor
   Function();

   // This function does the magic
   void build();

   // This one is an overload which sets 'input' as well
   void build(std::string input);

   // Getters and setters
   std::string getInput() const;
   void setInput(std::string input);

   bool getIsInline() const;
   void setIsInline(bool isInline);

   bool getIsConst() const;
   void setIsConst(bool isConst);

   std::string getReturnType() const;
   void setReturnType(std::string returnType);

   std::string getName() const;
   void setName(std::string name);

   std::string getParameters() const;
   void setParameters(std::string parameters);

   bool doesExist() const;
   void setExist(bool exists);

   // Deconstructor
   ~Function();
};

#endif // FUNCTION_H