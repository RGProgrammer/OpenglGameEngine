#include "emissive.h"

EmissiveObject * EmissiveObject::Create(GLRenderer *renderer)
{
	
	EmissiveObject* instance = new EmissiveObject();
	instance->setRenderer(renderer);
	if (!instance->init()) {
		delete instance;
		return NULL;
	}
	return instance;
}

EmissiveObject::EmissiveObject():Model3D()
{
}

EmissiveObject::~EmissiveObject()
{
	this->Destroy();
}

void EmissiveObject::Destroy()
{
	Model3D::Destroy();
}

_bool EmissiveObject::init()
{
	if (!m_GLRenderer || !m_GLRenderer->isInitialized())
		return false;
	//get the default material index and use it
	_u32b index = m_GLRenderer->GetMaterialIndex("DefaultMaterial");
	//load a cube from file : 
	this->LoadModelFromFile("../Resources/Sphere.obj"); //todo
	this->ApplyMaterialToAll(index);
	this->m_DoesCastShadow = false;
	return true;

}
