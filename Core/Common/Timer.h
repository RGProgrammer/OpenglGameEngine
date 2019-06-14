
#ifndef _RGP_CORE_TIMER_H_
#define _RGP_CORE_TIMER_H_

#include "..//Common//BasePrimitiveTypes.h"
#include <time.h>
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
        _bool				m_Started;//if the timer is started
		_float				dt;        //delta time :time between 2 getDeltaTime() calls
		clock_t				last_time;
		clock_t             current_time;
    };
};
#endif // PFE_TIMER_H_
