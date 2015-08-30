#include "stdafx.h"
#include "ReadText.h"


#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"sapi.lib")

CReadText::CReadText() :m_pVoice(NULL), m_pFile(NULL)
{}

CReadText::~CReadText()
{
	Unit();
}

bool CReadText::Init()
{
	if(FAILED(::CoInitialize(NULL)))
		return false;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
	if (SUCCEEDED(hr))
		return true;

	return false;
}

bool CReadText::Unit()
{
	if (NULL == m_pVoice)
		return false;

	m_pVoice->Release();
	m_pVoice = NULL;
	::CoUninitialize();
	
	return true;
}

bool CReadText::ReadText(TCHAR* pText)
{
	if (NULL == pText || NULL == m_pVoice)
		return false;

	HRESULT hr = m_pVoice->Speak(pText, 0, NULL);;
	return true;
}