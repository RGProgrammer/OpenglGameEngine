#ifndef _RGP_THREAD_IMPL_H_
#define _RGP_THREAD_IMPL_H_

///this file representation a crossplatform thread implementation
///since its(thread)  declaration between windows and linux is different.


#include ".//BasePrimitiveTypes.h"
#ifdef  WIN32

#elif _linux_

#endif // WIN32


namespace RGP_CORE {
     enum _ThreadStatus{
        THREAD_STARTED,
        THREAD_PAUSED,
        THREAD_STOPED,
        THREAD_WAITING
    };
    class Thread {
	public :
		Thread * CreateThread(void* (*func)(void* arg));
    protected:
        Thread(void* func);
        ~Thread();
        _bool            Resume();
        _bool            Pause();
        _bool            Stop();
        void            Destroy();
        _u8b*           getThreadName();
		_s32b			getThreadID();
        _ThreadStatus   getThreadStatus();
		void*			getResultBuffer();
    private :
        _u8b*           m_ThreadName ;
        _ThreadStatus   m_Status ;
		void*			m_ptrFunction;
		void*			m_Buffer;

    };
};
#endif // _RGP_THREAD_IMPL_H_
