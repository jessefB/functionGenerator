/***********************************************************************
* File: path.h
* Description: Header for the Path class. When given arguments, this class
*  will build and absolute path
************************************************************************/

#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <string>

class Path
{
private:
   // This is the default path. It can be changed with 'setDefaultPath(string path)'
   std::string defaultPath = "A:/git/";

   // Path data
   std::string absolutePath;
   std::string fileName;
   std::string className;

   // Temporary string to store paths as they move from method to methdo
   std::string temp;

   // These contain the full paths with extensions and everything
   std::string stdPath;
   std::string headerPath;
   std::string sourcePath;

   // Verify or create a directory (Default: false)
   bool makeDirectory = false;
   bool errorReporting = true;

   // Verify/create the directory
   bool verifyDirectory(std::string input);
   bool createDirectory(std::string input);

   bool buildFromDirectPath();
   void parseFileName(std::string input);

   bool buildFromPartialPath();

   // Build a file path from a file name (very custom to me)
   bool buildFromFileName();

   // Reset function to set all members back to zip
   void resetAll();

public:
   // Constructor
   Path();

   // This is where the magic happens
   // Build the paths
   bool buildStdPath(std::string path);

   // Getters and setters
   void setErrorReporting(bool input);
   bool getErrorReporting() const;

   bool setDefaultPath(std::string input);
   std::string getDefaultPath() const;

   void createDirectory(bool input);
   bool createDirectory() const;

   void setFileName(std::string fileName);
   std::string getFileName() const;

   // This is basically where we want to end up
   std::string getStd() const;
   std::string getWorkingPath() const;

   // Deconstructor
   ~Path();
};

#endif // PATH_H