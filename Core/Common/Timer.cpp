#include "Timer.h"


RGP_CORE::Timer::Timer(){
#if defined __GNUC__
	gettimeofday(&current_time,NULL);
#elif defined _MSC_VER
	_LUINT Timer_freq;
	_LUINT unused;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Timer_freq);
	frequency = 1.0 / (double)Timer_freq;
	QueryPerformanceCounter((LARGE_INTEGER*)&unused);
	current_time = 0;
#endif
	last_time=current_time ;
	m_Started=false;
};
RGP_CORE::Timer::~Timer(){

};
void RGP_CORE::Timer::Stop(){
    m_Started=false ;
};
void RGP_CORE::Timer::Reset(){
#if defined __GNUC__
	gettimeofday(&current_time, NULL);
#elif defined _MSC_VER
	current_time = 0;
#endif

	last_time=current_time ;
    m_Started=true ;
};
double RGP_CORE::Timer::getDeltaTime(){
    dt=0.0f;
    if(m_Started){
#if defined __GNUC__ 
		if(gettimeofday(&current_time,NULL)==0){
            dt=(float)(current_time.tv_sec-last_time.tv_sec)/(float)1000+(current_time.tv_usec-last_time.tv_usec)/(float)1000000 ;
            last_time=current_time ;
            if(dt<0.0f)
                dt=0.0f;
        }
#elif defined _MSC_VER
		_LUINT curclock;
		if (QueryPerformanceCounter((LARGE_INTEGER*)&current_time)){
			dt=(current_time - last_time)*frequency;
			last_time = current_time;
			if (dt < 0.0)
				dt = 0.0;
		}
#endif
    }
    return dt ;
};

