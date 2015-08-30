#pragma once

#include <sapi.h>

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
private:
	FILE*		m_pFile;
	ISpVoice*	m_pVoice;

};