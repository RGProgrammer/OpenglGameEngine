
#ifndef _RGP_CORE_TIMER_H_
#define _RGP_CORE_TIMER_H_

#if defined __GNUC__
	#include <sys/time.h>
#elif defined _MSC_VER
	#include <time.h>
	#include <Windows.h>
#endif

#include "..//Common//BasePrimitiveTypes.h"

#if defined _MSC_VER
	#if defined _WIN32
		typedef _u32b _LUINT;
	#elif defined _WIN64
		typedef _u64b _LUINT;
	#endif
#endif

namespace RGP_CORE {
    class Timer {
    public :
        //Constructor
        Timer() ;
        ////Deconstructor
        ~Timer() ;
        //get the duration between 2 call of this method in seconds
        _float getDeltaTime();
        //start timer or reset attributs
        void Reset();
        //stop the timer: getDeltaTime will always return 0
        void Stop();
    private :
        _bool            m_Started;//if the timer is started
		_float           dt;        //delta time :time between 2 getDeltaTime() calls
#if defined __GNUC__
		struct timeval last_time;   //Current time date
        struct timeval current_time;//time date of the last call
#elif defined _MSC_VER
		//_LUINT			last_time;
		clock_t				last_time;
		clock_t             current_time;
		//_LUINT			current_time;
		//_float			frequency;
#endif
    };
};
#endif // PFE_TIMER_H_
