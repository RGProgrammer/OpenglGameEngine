#ifndef _TTB_THREAD_IMPL_H_
#define _TTB_THREAD_IMPL_H_

///this file representation a crossplatform thread implementation
///since its(thread)  declaration between windows and linux is different.


#include ".//BasePrimitiveTypes.h"
#ifdef  WIN32

#elif _linux_

#endif // WIN32


namespace TTB {
    class enum _ThreadStatus{
        THREAD_STARTED,
        THREAD_PAUSED,
        THREAD_STOPED,
        THREAD_WAITING
    };
    class Thread {
    public:
        Thread();
        ~Thread();
        virtual void    Function()=0;
        void            Start();
        void            Pause();
        void            Stop();
        void            Destroy();
        _u8b*           getThreadName();
        _ThreadStatus   getThreadStatus();

    protected :
    private :
        _u8b*           m_ThreadName ;
        _ThreadStatus   m_Status ;
        ///some other attributs to add

    };
};
#endif // _TTB_THREAD_IMPL_H_
