#include "stdafx.h"
#include "ReadText.h"
#include "..\Common\HearConfig.h"
#include <commdlg.h>
#include <fstream>
#include <process.h>
#include <sphelper.h>

#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"sapi.lib")

#define	ONCE_READ_WORDS		512


CReadText::CReadText() :m_pVoice(NULL), m_pSpToken(NULL), m_ReadIng(false)
{
	m_wstrCurrentSelectFile.clear();
}

CReadText::~CReadText()
{
	Unit();
}

bool CReadText::Init()
{
	if(FAILED(::CoInitialize(NULL)))
		return false;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
	if (FAILED(hr))
		return false;

	IEnumSpObjectTokens *cpEnum = NULL;
	if (FAILED(SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum)))
	{
		return false;
	}		

	cpEnum->GetCount(&m_ulVoiceTypeCount);
	cpEnum->Release();
	return true;
}

bool CReadText::Unit()
{
	JUDGENULL(m_pVoice, false)

	if (NULL != m_pSpToken)
	{
		m_pSpToken->Release();
		m_pSpToken = NULL;
	}
	
	m_pVoice->Release();
	m_pVoice = NULL;

	::CoUninitialize();
	
	return true;
}


bool CReadText::ReadText(TCHAR* pText)
{
	JUDGENULL(pText, false)
	JUDGENULL(m_pVoice, false)
	HRESULT hr = m_pVoice->Speak(pText, SPF_DEFAULT, NULL);
	
	return true;
}

bool CReadText::SetRate(long lRate)
{
	JUDGETRUE((lRate<-10 || lRate >10), false)
	JUDGENULL(m_pVoice, false)
	return  SUCCEEDED(m_pVoice->SetRate(lRate));
}
bool CReadText::GetRate(long* plRate)
{
	JUDGENULL(m_pVoice, false)
	return  SUCCEEDED(m_pVoice->GetRate(plRate));
}
bool CReadText::SetVolume(int iVol)
{
	JUDGENULL(m_pVoice, false)
	return  SUCCEEDED(m_pVoice->SetVolume(iVol));
}
bool CReadText::GetVolume(int& iVol)
{
	JUDGENULL(m_pVoice, false)

	unsigned short usVol;
	if (SUCCEEDED(m_pVoice->GetVolume(&usVol)))
	{
		return false;
	}
	
	iVol = int(usVol);
	return  true;
}
bool CReadText::Pause(void)
{
	JUDGENULL(m_pVoice, false)
		return  (SUCCEEDED(m_pVoice->Pause()));
}
bool CReadText::Resume(void)
{
	JUDGENULL(m_pVoice, false)
	return (SUCCEEDED(m_pVoice->Resume()));
}
bool CReadText::OpenFile(TCHAR* pFileName)
{
	JUDGENULL(pFileName, false)

	std::wifstream in(pFileName, std::ios::in | std::ios::binary);
	in.seekg(0, std::ios::beg);
	in.imbue(std::locale("chs"));

	std::wstring wstLine;
	std::wstring wstWord;
	while (!in.eof())
	{
		//getline(in, wstLine);
		//ReadText((TCHAR*)wstLine.c_str());
		//wstLine.clear();
		in >> wstWord;
		wstLine.append(wstWord);
		wstLine.append(L" ");
		if (wstLine.length() > ONCE_READ_WORDS)
		{
			ReadText((TCHAR*)wstLine.c_str());
			wstLine.clear();
		}
	}

	if (!wstLine.empty())
	{
		ReadText((TCHAR*)wstLine.c_str());
		wstLine.clear();
	}
	in.close();
	return true;
}


bool CReadText::IsReadIng()
{
	return m_ReadIng;
}

std::wstring CReadText::GetCurrentSelectFile()
{
	return m_wstrCurrentSelectFile;
}


bool CReadText::SelectFile()
{
	OPENFILENAME opfn;
	TCHAR strFilename[MAX_PATH];
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);
	opfn.lpstrFilter = L"txt Files(*.txt)\0 *.txt\0";//L"pdf Files(*.pdf)\0 *.pdf\0";//设置过滤
	opfn.nFilterIndex = 1;
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (::GetOpenFileName(&opfn))
	{
		m_wstrCurrentSelectFile = strFilename;
	}
	else
	{
		return false;
	}
	
	return true;
}

bool CReadText::SelectTheReader(unsigned long id)
{
	JUDGETRUE((id >= m_ulVoiceTypeCount),false)

	IEnumSpObjectTokens *cpEnum = NULL;
	// 列举所有的语音token，可以通过pSpEnumTokens指向的接口得到
	if (SUCCEEDED(SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum)))
	{
		if (NULL != m_pSpToken)
		{
			m_pSpToken->Release();
			m_pSpToken = NULL;
		}
		
		while (SUCCEEDED(cpEnum->Next(1, &m_pSpToken, NULL)) && m_pSpToken != NULL)
		{
			if (id-- == 0)
			{
				m_pVoice->SetVoice(m_pSpToken);
				break;
			}
			m_pSpToken->Release();//释放token
			m_pSpToken = NULL;
		}
		cpEnum->Release();// 释放pSpEnumTokens接口
	}

	return true;
}