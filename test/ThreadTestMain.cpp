#include "../Core//Common//Thread.h"
#include <iostream>

using namespace RGP_CORE;
void* func(void* arg) {
	std::cout << "Hello i am in the thread" << std::endl;
	Thread::ThreadSleep(10000);
	return (void*)5;
}
int main(void) {
	_u32b ret = 0;
	Thread* th = Thread::Create(func, NULL);
	if (th)
		ret=th->Join();
	std::cout << "thread exit with code " << ret << std:: endl;
	system("PAUSE");;
	delete th;
	return 0;
}