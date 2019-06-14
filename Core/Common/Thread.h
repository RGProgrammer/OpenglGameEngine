#ifndef _RGP_THREAD_IMPL_H_
#define _RGP_THREAD_IMPL_H_

///this file representation a crossplatform thread implementation
///since its(thread)  declaration between windows and linux is different.


#include ".//BasePrimitiveTypes.h"
#if defined  _WIN32
#include <Windows.h>
#elif defined _linux_
#include <pthread.h>
#include <semaphore.h>
#endif // WIN32


namespace RGP_CORE {
     enum _ThreadStatus{
		THREAD_ERROR,
        THREAD_ACTIVE,
        THREAD_PAUSED,
        THREAD_STOPPED,
		THREAD_FINISHED
    };
    class Thread {
	public :
		static Thread * Create(void* (*func)(void* arg),void* ptrParameter, _bool Suspended=false);
		static void ThreadSleep(_u32b millisecs);
    private:
        Thread(void* func, void* ptrParameter, _bool Suspended=false);
	public:
        ~Thread();
        _bool            Resume();
        _bool            Pause();
        _bool            Stop();
		_u32b			Join();// wait for the thread to finish and return exitCode
        void            Destroy();
		_s32b			getThreadID();
        _ThreadStatus   getThreadStatus();
    private :
        _ThreadStatus			m_Status ;
		void*					m_ptrFunction;
		_u32b					m_ExitCode;
		_u32b					m_ThreadID;

#if defined  _WIN32
		SECURITY_ATTRIBUTES		m_Attrib;
		HANDLE					m_winThreadHandle;
#endif // WIN32
    };
};
#endif // _RGP_THREAD_IMPL_H_
