#include "PhysicalModel.h"
RGP_CORE::PhysicalModel::PhysicalModel(_float mass): Model3D(), Physical(NULL,mass)
{};
RGP_CORE::PhysicalModel::PhysicalModel(Vertex3d Pos, _float mass ):Model3D(Pos), Physical(Pos,NULL,mass)
{};
RGP_CORE::PhysicalModel::PhysicalModel(Vertex3d Pos, Vertex3d Dir, Vertex3d Up, _float mass ):Model3D(Pos,Dir,Up), Physical(Pos,Dir, Up,NULL,mass)
{};
RGP_CORE::PhysicalModel::~PhysicalModel() 
{
	this->Destroy();
};
void RGP_CORE::PhysicalModel::Destroy()
{
	Model3D::Destroy();
	Physical::Destroy();
};