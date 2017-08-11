#include "Timer.h"

#if defined _MSC_VER

RGP_CORE::Timer::Timer():m_Started(false) ,dt(0.0) 
{
	//_LUINT Timer_freq;
	//_LUINT unused;
	//QueryPerformanceFrequency((LARGE_INTEGER*)&Timer_freq);
	//frequency = 1.0 / (_float)Timer_freq;
	//QueryPerformanceCounter((LARGE_INTEGER*)&unused);
	printf("%u \n", _MSC_VER);
	frequency = 0.0001;
	current_time = 0;
		last_time = current_time;
};
#elif defined __GNUC__

RGP_CORE::Timer::Timer() :m_Started(false), dt(0.0)
{

	gettimeofday(&current_time, NULL);
	last_time = current_time;
};

#endif
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
_float RGP_CORE::Timer::getDeltaTime(){
    dt=0.0f;
    if(m_Started){
#if defined __GNUC__
		if(gettimeofday(&current_time,NULL)==0){
            dt=(_float)(current_time.tv_sec-last_time.tv_sec)/(_float)1000+(current_time.tv_usec-last_time.tv_usec)/(_float)1000000 ;
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

