#ifndef _RGP_INIT_DATABASE_H_
#define _RGP_INIT_DATABASE_H_

#include "..//Core//ClassesDB//ClassesDB.h"
#include "..//Core//Graphics//Model3D.h"
//#include "..//Core//Scene//PointLight.h"
#include "..//Core//Scene//DirectionnalLight.h"
//#include "..//Core//Scene//SpotLight.h"



_bool InitClassesDatabase()
{
	RGP_CORE::Class_DB::addClass("Model3D", (void*(*)(void** args)) (&RGP_CORE::Model3D::Create), 2);
	//RGP_CORE::Class_DB::addClass("PointLight", (void*(*)(void** args)) (&RGP_CORE::PointLight::Create), 2);
	//RGP_CORE::Class_DB::addClass("SpotLight", (void*(*)(void** args)) (&RGP_CORE::SpotLight::Create), 2);
	RGP_CORE::Class_DB::addClass("DirectionnalLight", (void*(*)(void** args)) (&RGP_CORE::DirectionnalLight::Create), 2);

	//all newly created classes (child of BaseActor) should be added to the Database ;
	return true;

};
#endif // !_RGP_INIT_DATABASE_H_

