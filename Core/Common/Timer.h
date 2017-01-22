
#ifndef PFE_TIMER_H_
#define PFE_TIMER_H_
#include <sys/time.h>


namespace TTB {
    class Timer {
    public :
        //Constructor
        Timer() ;
        ////Deconstructor
        ~Timer() ;
        //get the duration between 2 call of this method in seconds
        double getDeltaTime();
        //start timer or reset attributs
        void Reset();
        //stop the timer: getDeltaTime will always return 0
        void Stop();
    private :
        bool            m_Started;//if the timer is started
        double           dt;        //delta time :time between 2 getDeltaTime() calls
        struct timeval last_time;   //Current time date
        struct timeval current_time;//time date of the last call
    };
};
#endif // PFE_TIMER_H_
