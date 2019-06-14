#include ".//LevelBuilder.h"
#include ".//InitDatabase.h"
using namespace RGP_LEVELBUILDER;

LevelBuilder *instance = NULL;

int main(void)
{
	instance = LevelBuilder::Create();
	if (!instance) {
		printf("Error init the program \n");
		return 0;
	}
	instance->StartLoop();
	LevelBuilder::Release();
	return 0;

}
