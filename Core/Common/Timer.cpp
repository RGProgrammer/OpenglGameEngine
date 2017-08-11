#include "Timer.h"


RGP_CORE::Timer::Timer():m_Started(false) ,dt(0.0) 
{
	current_time = clock();
	last_time = current_time;
};

RGP_CORE::Timer::~Timer(){

};
void RGP_CORE::Timer::Stop(){
    m_Started=false ;
};
void RGP_CORE::Timer::Reset(){
	current_time = clock();
	last_time = current_time;
    m_Started=true ;
};
_float RGP_CORE::Timer::getDeltaTime(){
    dt=0.0f;
    if(m_Started){
		current_time = clock();
		if (current_time <= last_time) {
			dt = 0.0;
		}
		else {
			dt = (_float)(current_time - last_time) / (_float)(CLOCKS_PER_SEC);
			last_time = current_time ;
		}
	}

    return dt ;
};

