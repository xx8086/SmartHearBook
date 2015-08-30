#pragma once
#include <string>
#include <sapi.h>

typedef enum File_Type
{
	File_Type_unKnow=	0,
	File_Type_TXT	=	1,
	File_Type_PDF	=	2,
	File_Type_DOC	=	3,
}FILE_TYPE;

class CReadText
{
public:
	CReadText();
	virtual ~CReadText();
public:
	bool Init();
	bool Unit();
	bool OpenFile(TCHAR* pFileName);
	bool ReadText(TCHAR* pText);
	bool SelectFile(std::wstring& wstrSelectFile);
private:
	
private:
	FILE*		m_pFile;
	ISpVoice*	m_pVoice;

};