/***********************************************************************
* File: path.cpp
* Description: Source for the Path class. When given arguments, this class
*  will build and absolute path
************************************************************************/

#include "path.h"

#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

// Used to create the path
#include <windows.h>
using namespace std;

// Constructors

/*********************************************
 * PATH :: Constructor ~ Default
 * Initialize the path instance
 *********************************************/
Path::Path()
{
   this->absolutePath = this->defaultPath;
}

// Private methods

/*********************************************
 * PATH :: verifyDirectory
 * Verify the given directory exists
 *********************************************/
bool Path::verifyDirectory(string input)
{
   // Code copied from Stack Overflow
   // https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi/8233867
   DWORD ftyp = GetFileAttributesA(input.c_str());
   if (ftyp == INVALID_FILE_ATTRIBUTES)
      return false; //something is wrong with your path!

   if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
      return true; // this is a directory!

   return false; // this is not a directory!
}

/*********************************************
 * PATH :: createDirectory
 * Create the given directory
 *********************************************/
bool Path::createDirectory(string input)
{
   // Ifndef stuff is to remove the red squigle. Doesn't affect the program at all
#ifndef __INTELLISENSE__
   // Code copied from Stack Overflow
   //https://stackoverflow.com/questions/9235679/create-a-directory-if-it-doesnt-exist
   if (CreateDirectory(input.c_str(), NULL) ||
       ERROR_ALREADY_EXISTS == GetLastError())
      return true;
   else
      return false;
#endif // __INTELLISENSE__
}

/*********************************************
 * PATH :: buildFromDirectPath
 * Given an absolute path, fill in the necessary 
 * members
 *********************************************/
bool Path::buildFromDirectPath()
{
   // Reset the absolute path
   this->absolutePath = "";

   // Parse out the file name and extension
   parseFileName(this->temp);

   // Create/validate the path (and use them as error codes)
   if (this->makeDirectory)
      return this->createDirectory(this->absolutePath);
   else
      return this->verifyDirectory(this->absolutePath);
}

/*********************************************
 * PATH :: parseFileName
 * Part of 'buildFromDirectPath', remove a file name
 * and extension from the given input
 *********************************************/
void Path::parseFileName(string input)
{
   // Find the position of the last slash

   // Loop incrementer
   int i = 0;

   // Position
   int r = 0;

   for (i; i < input.length(); i++)
      if (input[i] == '/' || input[i] == '\\')
         r = i;

   // Now 'r' has the position of the last slash

   // Copy over the rest of the 'input' to the file name
   for (int l = r + 1; l < input.length(); l++)
   {
      if (input[l] == '.')
         break;
      this->fileName += input[l];
   }

   // Copy over all of 'input' to 'absolutePath'
   for (i = 0; i <= r; i++)
      this->absolutePath += input[i];

   return;
}

/*********************************************
 * PATH :: buildFromPartialPath
 * Given a sub directory, build a partial path
 *********************************************/
bool Path::buildFromPartialPath()
{
   // Error checking
   if (this->defaultPath == "")
      return false;

   string temporary = this->defaultPath + this->temp;

   this->absolutePath = "";

   // Break the file name apart from the path
   parseFileName(temporary);

   cout << this->absolutePath << endl;

   // Create/validate the path
   if (this->makeDirectory)
      return this->createDirectory(this->absolutePath);
   else
      return this->verifyDirectory(this->absolutePath);
}

/*********************************************
 * PATH :: buildFromFileName
 * == This function is VERY peronalized to my setup ==
 * Given a file name, build an appropriate directory
 *********************************************/
bool Path::buildFromFileName()
{
   // Start the absolute path off with the default path
   this->absolutePath = this->defaultPath;

   // Variable to store whether there is a chapter folder
   bool chapters = false;

   // Decide which directory the file should go in
   switch (toupper(this->fileName[0]))
   {
   case 'C':
      this->absolutePath += "Assignments/Checkpoints/";
      chapters = true;
      break;
   case 'H':
      this->absolutePath += "Assignments/Homework/";
      chapters = true;
      break;
   case 'P':
      this->absolutePath += "Projects/";
      break;
   case 'T':
      this->absolutePath += "Tests/";
      break;
   default:
      break;
   }

   // If there are chapters
   if (chapters)
   {
      string chapterNumber;

      // Find the first digit
      for (int i = 0; i < this->fileName.length(); i++)
         if (isdigit(this->fileName[i]))
         {
            chapterNumber = this->fileName[i];
            break;
         }

      // Add the chapter and number to the path
      this->absolutePath += "Chapter " + chapterNumber + "/";
   }

   // As of 1/28/20, I've started putting each assignment in its own folder (of the same name as the file)
   // Add that folder to the path
   this->absolutePath += this->fileName + "/";

   // Create/validate the path
   if (this->makeDirectory)
      return this->createDirectory(this->absolutePath);
   else
      return this->verifyDirectory(this->absolutePath);
}

/*********************************************
 * PATH :: resetAll
 * Set all members to their defaults if there was 
 * an error creating a path
 *********************************************/
void Path::resetAll()
{
   // Set all memebers back to default
   this->defaultPath = "C:/Users/jesse/Documents/CS165/";
   this->absolutePath = "";
   this->fileName = "";
   this->className = "";
   this->temp = "";
   this->stdPath = "";
   this->headerPath = "";
   this->sourcePath = "";
   this->makeDirectory = false;

   return;
}

// Public methods

/*********************************************
 * PATH :: buildStdPath
 * Given an input, build an absolute path
 * This is what the client calls
 *********************************************/
bool Path::buildStdPath(string path)
{
   // First store the path into the 'temp' variable
   this->temp = path;

   bool success;

   // ...then check to see if the input is a path or file name
   if (toupper(path[0]) == 'C' && path[1] == ':' && (path[2] == '/' || path[2] == '\\'))
      success = this->buildFromDirectPath();
   else if (path.find("/") != string::npos || path.find("\\") != string::npos)
      success = this->buildFromPartialPath();
   else
   {
      this->fileName = path;
      success = this->buildFromFileName();
   }

   // Now build 'stdPath' (SOURCE is ".cpp")
   this->stdPath = this->absolutePath + this->fileName + ".cpp";

   // If there was an error building the path, reset all members to zilch (nada, nufing, zip, etc.)
   if (!success)
      this->resetAll();

   return success;
}


/*********************************************
 * PATH :: setDefaultPath
 * Set whether error messages will be displayed
 *********************************************/
void Path::setErrorReporting(bool input)
{
   this->errorReporting = input;

   return;
}

/*********************************************
 * PATH :: getErrorReporting
 * Return whether error messages will be displayed
 *********************************************/
bool Path::getErrorReporting() const
{
   return this->errorReporting;
}

/*********************************************
 * PATH :: setDefaultPath
 * Set the default working direcotry
 * Default is 'C:/users/jesse/Documents/CS165/'
 *********************************************/
bool Path::setDefaultPath(string input)
{
   if (this->verifyDirectory(input))
   {
      this->defaultPath = input;
      return true;
   }
   else
      return false;
}

/*********************************************
 * PATH :: getDefaultPath
 * Return the current default path
 *********************************************/
string Path::getDefaultPath() const
{
   return this->defaultPath;
}

/*********************************************
 * PATH :: createDirectory
 * Set createDirectory to either true or false
 *********************************************/
void Path::createDirectory(bool input)
{
   this->makeDirectory = input;

   return;
}

/*********************************************
 * PATH :: createDirectory
 * Return whether a directory will be created or not
 *********************************************/
bool Path::createDirectory() const
{
   return this->makeDirectory;
}

/*********************************************
 * PATH :: setFileName
 * Set the file name
 *********************************************/
void Path::setFileName(string fileName)
{
   this->fileName = fileName;

   return;
}

/*********************************************
 * PATH :: getFileName
 * Return the file name
 *********************************************/
string Path::getFileName() const
{
   return this->fileName;
}

/*********************************************
 * PATH :: getStd
 * Return the path to the standard file (with extension)
 *********************************************/
string Path::getStd() const
{
   // Make sure the path exists with a file name
   if (this->fileName != "")
      return this->stdPath;
   else
      return "";
}

/*********************************************
 * PATH :: getWorkingPath
 * Return the directory that is being worked with
 * Note: no files or extensions
 *********************************************/
string Path::getWorkingPath() const
{
   return this->absolutePath;
}

// Deconstructor

/*********************************************
 * PATH :: Destructor
 * Nothing really to clean up
 *********************************************/
Path::~Path()
{
}