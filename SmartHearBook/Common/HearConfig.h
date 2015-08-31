#pragma once

#define JUDGENULL(p,rt)			if(NULL == (p)){return rt;} 
#define	JUDGETRUE(p,rt)			if(true == (p)){return rt;}
#define DELETEPOINT(P)			if(NULL != (p)){delete p; p = NULL;}

typedef enum Whose_Voice
{
	VOICE_WOMAN = 0,
	VOICE_MAN	= 1,
}WHOSE_VOICE;