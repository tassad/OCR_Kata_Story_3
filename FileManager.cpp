

#include "FileManager.h"

/****************************************************
  	CFileManager base class definition

*****************************************************/

CFileManager::CFileManager(void)
{	
	m_hFile = -1;
}


CFileManager::~CFileManager(void)
{
	if(m_hFile != -1)
	{
		_close(m_hFile);
	}
}


/****************************************************
		CSourceFile class definition

*****************************************************/

CSourceFile::CSourceFile(void)
{
}


CSourceFile::~CSourceFile(void)
{
}


int CSourceFile::OpenSrc(char* FileName)
{

	int ret = 0;

	m_hFile = _open(FileName, _O_RDONLY | _O_BINARY);	// 

	if(m_hFile == -1)
	{
		ret = -1;
	}

	return ret;
}


int CSourceFile::LineTermCount()
{
	int count = 0;

	_lseek( m_hFile, LINE_CHARS, SEEK_SET);	// make sure we are at the beginning of the file

	char term[3];

	_read(m_hFile, term, 2);

	if(term[0] == 10 || term[0] == 13)
	{
		count++;
	}

	if(term[1] == 10 || term[1] == 13)
	{
		count++;
	}

	_lseek( m_hFile, 0, SEEK_SET);	// restore beginning of file position

	return count;

}


int CSourceFile::GetSize()
{

	m_iFileSize =_lseek( m_hFile, 0, SEEK_END );

	return m_iFileSize;
}


int CSourceFile::LoadScanData(unsigned char* Buffer, int BytesToLoad)
{
	_lseek( m_hFile, 0, SEEK_SET);	// make sure we are at the beginning of the file

	int count = -1;
	
	count = _read(m_hFile, Buffer, m_iFileSize);
	
	return count;	
}


/****************************************************
		CResultsFile class definition

*****************************************************/

CResultsFile::CResultsFile(void)
{
}


CResultsFile::~CResultsFile()
{
}


int CResultsFile::OpenDst(char* FileName)
{
	
	int ret = 0;
	
	m_hFile = _open(FileName, _O_WRONLY | _O_CREAT | _O_BINARY /*, _S_IREAD |  _S_IWRITE */); 
	
	if(m_hFile == -1)
	{
		ret = -1;
	}

	return ret;
}


void CResultsFile::SaveToFile(Account *src, int NumberOfAcc)
{

	int ret = 0;
		
	// preparation buffer, format account row
	unsigned char out[ACC_DIGITS +1 +2 + 4];

	// null terminated string containing literal account status code, second column
	char stat[5];
	
	// extra chars to write : ' ' + "status code" + 10 + 13
	// or: 10 + 13 (line feed + carriage return)
	int count = 2;


	// format results
	//
	for(int res = 0; res < NumberOfAcc; res++)
	{
		// select appropriate code.. if any
		//
		switch(src[res].status)	
		{
		case 1:
			{
				stat[0] = ' ';
				stat[1] = 'E';
				stat[2] = 'R';
				stat[3] = 'R';				
				stat[4] = NULL;

				count = 6;

				break;
			}

		case -1:
			{
				stat[0] = ' ';
				stat[1] = 'I';
				stat[2] = 'L';
				stat[3] = 'L';
				stat[4] = NULL;

				count = 6;

				break;
			}

		default:
			{
				count = 2;

				break;
			}

		}// switch
		
				
		if(count == 2)
		{
			// format single column account record
			//
			sprintf((char*)out, "%c%c%c%c%c%c%c%c%c%c%c", src[res].dSrc[0] +48, src[res].dSrc[1]+48, src[res].dSrc[2]+48, src[res].dSrc[3]+48, src[res].dSrc[4]+48, src[res].dSrc[5]+48, src[res].dSrc[6]+48, src[res].dSrc[7]+48, src[res].dSrc[8]+48, 13, 10);				
		}
		else
		{
			// format dual column account record
			//
			sprintf((char*)out, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", src[res].dSrc[0] +48, src[res].dSrc[1]+48, src[res].dSrc[2]+48, src[res].dSrc[3]+48, src[res].dSrc[4]+48, src[res].dSrc[5]+48, src[res].dSrc[6]+48, src[res].dSrc[7]+48, src[res].dSrc[8]+48, stat[0], stat[1], stat[2], stat[3], 13, 10);				
		}

		// write formatted account record to file
		//
		WriteToFile(out, ACC_DIGITS + count);


		// test preview results					
		//printf("%d: %s\n", res+1, out);
	
	}// for

	// insure saving data to disk file
	//
	SaveFile();
}


int CResultsFile::WriteToFile(unsigned char* Buffer, int BytesToWrite)
{


	int count = -1;

	count = _write(m_hFile, Buffer, BytesToWrite);

	return count;	
}


int CResultsFile::SaveFile()
{

	int ret = -1;

	ret = _commit(m_hFile);

	return ret;	
}
