#include "..//Animator//Animator.h"


using namespace RGP_ANIMATOR;

Animator* a=NULL ;

int main(void)
{
	a = Animator::CreateAnimator();
	if (!a) {
		printf("fatal error");
		system("PAUSE");
		return EXIT_FAILURE;
	}
	a->Start();
	a->Destroy();
	delete a;
	a = NULL;
	
	return EXIT_SUCCESS;
}