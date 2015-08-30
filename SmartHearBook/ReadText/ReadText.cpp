#include "stdafx.h"
#include "ReadText.h"
#include <commdlg.h>
#include <fstream>


#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"sapi.lib")

#define	ONCE_READ_WORDS		128


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

bool CReadText::OpenFile(TCHAR* pFileName)
{
	if (NULL == pFileName)
		return false;

	std::wifstream in(pFileName, std::ios::in | std::ios::binary);
	int iFileLen = in.tellg();
	in.seekg(0, std::ios::beg);
	in.imbue(std::locale("chs"));
	
	std::wstring wstLine;
	while (!in.eof())
	{
		getline(in, wstLine);
		ReadText((TCHAR*)wstLine.c_str());
		wstLine.clear();
	}

	in.close();
	return true;
}


bool CReadText::SelectFile(std::wstring& wstrSelectFile)
{
	wstrSelectFile.clear();

	OPENFILENAME opfn;
	TCHAR strFilename[MAX_PATH];
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);
	opfn.lpstrFilter = NULL;//L"pdf Files(*.pdf)\0 *.pdf\0";//…Ë÷√π˝¬À
	opfn.nFilterIndex = 1;
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (::GetOpenFileName(&opfn))
	{
		wstrSelectFile = strFilename;
	}
	return true;
}