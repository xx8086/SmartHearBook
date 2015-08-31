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
	bool SelectFile();
public:
	bool SetRate(long lRate); //�����ʶ��ٶȣ�ȡֵ��Χ�� - 10��10
	bool GetRate(long* plRate);
	bool SetVolume(int iVol);//��������0-100
	bool GetVolume(int& iVol);
	bool Pause(void);//��ͣ
	bool Resume(void);//�ָ� 
	bool SelectTheReader(unsigned long id);
public:
	bool IsReadIng();
	std::wstring GetCurrentSelectFile();
protected:

protected:
	std::wstring	m_wstrCurrentSelectFile;
	bool			m_ReadIng;
private:
	ISpVoice*		m_pVoice;
	ISpObjectToken*	m_pSpToken;
	unsigned long	 m_ulVoiceTypeCount;

};