#ifndef INCLUDE_CRITICALSECTION
#define INCLUDE_CRITICALSECTION

#include <windows.h>

/************************************************************************/
/* CCriticalSection                                                     */
/************************************************************************/
class CCriticalSection
{
public:
	CCriticalSection() { InitializeCriticalSection(&m_CriticalSection); }
	~CCriticalSection() { DeleteCriticalSection(&m_CriticalSection); }

public:
	void Enter() { EnterCriticalSection(&m_CriticalSection); }	//进入临界区
	void Leave() { LeaveCriticalSection(&m_CriticalSection); }	//离开临界区

protected:
	CRITICAL_SECTION m_CriticalSection;
};

#endif
