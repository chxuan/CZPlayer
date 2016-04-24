#ifndef INCLUDE_THREAD
#define INCLUDE_THREAD

/************************************************************************/
/* CThread                                                              */
/************************************************************************/
class CThread
{
private:
	HANDLE m_hThread;
	DWORD m_dwThreadId;
	bool m_Suspended;

	static DWORD CALLBACK ThreadProc(LPVOID lpParameter)
	{
		CThread* pThread = (CThread*)lpParameter;
		if(pThread == NULL)
		{
			return 0;
		}

		pThread->Execute();
		return 0;
	}

protected:
	bool m_Stop;
	virtual void Execute() = 0; /* this pure virtual function should be implemented by child class 
								which inherited from this class
								*/
public:
	CThread(bool pCreateSuspened = true) : m_hThread(NULL), m_dwThreadId(0), m_Stop(false), m_Suspended(false)
	{
		m_hThread = CreateThread(NULL, 0, ThreadProc, (void*)this,
			(pCreateSuspened == true) ? CREATE_SUSPENDED : 0, &m_dwThreadId);
		m_Suspended = true;
	}

	~CThread()
	{
		if(m_hThread != NULL)
		{
			CloseHandle(m_hThread);
		}
	}

	//恢复该线程
	void Resume()
	{
		if(m_hThread != NULL && m_Suspended == true && !m_Stop)
		{
			ResumeThread(m_hThread);
			m_Suspended = false;
		}
	}

	//暂停该线程
	void Suspend()
	{
		if(m_hThread != NULL && m_Suspended == false && !m_Stop)
		{
			SuspendThread(m_hThread);
			m_Suspended = true;
		}
	}

	//判断是否是暂停状态
	bool Suspended() 
	{ 
		return m_Suspended; 
	}

	//停止该线程
	void Stop()
	{
		m_Stop = true;
		if(!Suspended())
		{
			Suspend();
		}
	}

	//等待线程回收资源
	void wait()
	{
		WaitForSingleObject(m_hThread, INFINITE);
	}
};

#endif
