#include "./Thread.h"

RGP_CORE::Thread * RGP_CORE::Thread::Create(void* (*func)(void* arg),void* ptrParametr,_bool Suspended)
{
	Thread* th = new Thread(func, ptrParametr, Suspended);
	if (!th)
		return NULL;
	if (th->getThreadStatus() != THREAD_ERROR)
		return th;
	else {
		delete th;
		return NULL;
	}

};

RGP_CORE::Thread::~Thread()
{
	this->Destroy();
};

RGP_CORE::_ThreadStatus   RGP_CORE::Thread::getThreadStatus()
{
	return m_Status;
};

_s32b			RGP_CORE::Thread::getThreadID()
{
	return m_ThreadID;
};

#ifdef  _WIN32
RGP_CORE::Thread::Thread(void* func, void* ptrParameter,_bool Suspended) :m_Status(THREAD_ERROR), m_ptrFunction(NULL), m_ExitCode(0), m_winThreadHandle(NULL)
{
	m_Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	m_Attrib.lpSecurityDescriptor = NULL;
	m_Attrib.bInheritHandle = false;
	
	if (!Suspended) {
		if (m_winThreadHandle = CreateThread(&m_Attrib, 0, (LPTHREAD_START_ROUTINE)func, ptrParameter, 0, &m_ThreadID)) {
			m_ptrFunction = func;
			m_Status = THREAD_ACTIVE;
		}
	}else {
		if (m_winThreadHandle = CreateThread(&m_Attrib, 0, (LPTHREAD_START_ROUTINE)func, ptrParameter, CREATE_SUSPENDED, &m_ThreadID)) {
			m_ptrFunction = func;
			m_Status = THREAD_PAUSED;
		}
	}

};

void  RGP_CORE::Thread::ThreadSleep(_u32b millisecs)
{
	Sleep(millisecs);
};

_bool            RGP_CORE::Thread::Resume()
{
	BOOL ret;
	if (m_Status != THREAD_PAUSED || m_Status == THREAD_ERROR)
		return false;
	if (ResumeThread(m_winThreadHandle) == -1) {
		ret = GetExitCodeThread(m_winThreadHandle, &m_ExitCode);
		if (m_ExitCode != STILL_ACTIVE) {
			m_Status = THREAD_FINISHED;
		}
		return false;
	}
	m_Status = THREAD_ACTIVE;
	return true;
	
};
_bool            RGP_CORE::Thread::Pause()
{
	BOOL ret;
	if (m_Status != THREAD_ACTIVE || m_Status == THREAD_ERROR)
		return false;
	if (SuspendThread(m_winThreadHandle) == -1) {
		ret = GetExitCodeThread(m_winThreadHandle, &m_ExitCode);
		if (m_ExitCode != STILL_ACTIVE) {
			m_Status = THREAD_FINISHED;
		}
		return false;
	}
	m_Status = THREAD_PAUSED;
	return true;

};
_bool            RGP_CORE::Thread::Stop()
{
	BOOL ret;
	if (m_Status == THREAD_FINISHED || m_Status != THREAD_ERROR || m_Status==THREAD_STOPPED)
		return false;
	if (TerminateThread(m_winThreadHandle,m_ExitCode) == -1) {
		ret = GetExitCodeThread(m_winThreadHandle, &m_ExitCode);
		if (m_ExitCode != STILL_ACTIVE) {
			m_Status = THREAD_FINISHED;
		}
		return false;
	}
	m_Status = THREAD_STOPPED;
	return true;
};
_u32b			RGP_CORE::Thread::Join()
{
	if (m_Status == THREAD_FINISHED) {
		return m_ExitCode;
	}else{
		WaitForSingleObject(m_winThreadHandle, INFINITE);
		GetExitCodeThread(m_winThreadHandle, &m_ExitCode);
		return m_ExitCode;
	}

};
void            RGP_CORE::Thread::Destroy()
{
	this->Stop();
	if (m_winThreadHandle) {
		CloseHandle(m_winThreadHandle);
		m_winThreadHandle = NULL;
		m_ThreadID = 0;
		m_Status = THREAD_ERROR;
		m_ptrFunction = NULL;
		m_ExitCode = 0;
	}


};
#elif defined _linux_

#endif


