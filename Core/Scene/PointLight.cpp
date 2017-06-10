#include ".//PointLight.h"

RGP_CORE::PointLight::PointLight(): RGP_CORE::LightSource()
{
	m_LightViewMtx = (_float*)malloc(96 * sizeof(_float));//contains all the View matrices for a point light

};
RGP_CORE::PointLight::PointLight(Vertex3d Pos): RGP_CORE::LightSource(Pos)
{
	m_LightViewMtx = (_float*)malloc(96 * sizeof(_float));
};
RGP_CORE::PointLight::PointLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up): RGP_CORE::LightSource(Pos,Dir,Up)
{
	m_LightViewMtx = (_float*)malloc(96 * sizeof(_float));
};

void RGP_CORE::PointLight::UpdateLightViewMtx()
{
	
};
void RGP_CORE::PointLight::UpdateLightProjectionMtx()
{

};