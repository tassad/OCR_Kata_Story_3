#pragma once

#include "defs.h"
// file operations
#include <io.h>
#include <fcntl.h>


class CFileManager
{
  	// file manager base class

public:

	/// construction

	// default constructor
	//
	CFileManager(void);


	// default distructor
	//
	~CFileManager(void);
	

protected:

	/// member variables

	// file size in bytes
	//
	int	m_iFileSize;

	// processed file handle
	//
	int	m_hFile;

};


class CSourceFile: public CFileManager
{
		// source file processing class

public:

	/// construction

	// default constructor
	//
	CSourceFile(void);


	// default distructor
	//
	~CSourceFile(void);

	
	/// operations

	// open source file for reading
	//
	int OpenSrc(char* FileName);	
	

	// simple inspection of source file format: identify line termination char count
	//
	int LineTermCount();


	// identify file size
	//
	int GetSize();


	// read data from file, load it into passed buffer parameter, returns actual size of loaded data, or  -1 on error
	//
	int LoadScanData(unsigned char* Buffer, int BytesToLoad);

};


class CResultsFile: public CFileManager
{
	// results file processing class

public:

	/// construction

	// default constructor
	//
	CResultsFile(void);


	// default distructor
	//
	~CResultsFile(void);
	

	
	/// operations

	// open and create output file for writing results
	//
	int OpenDst(char* FileName);

	
	// format result accounts records to be written to results file, one by one
	//
	void SaveToFile(Account *src, int NumberOfAcc);


private:

	// write single account record to file
	//
	int WriteToFile(unsigned char* Buffer, int BytesToWrite);

	// insure saving data to disk file
	//
	int SaveFile();
};

