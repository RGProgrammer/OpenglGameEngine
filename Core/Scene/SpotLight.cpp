#include ".//SpotLight.h"
RGP_CORE::SpotLight::SpotLight():LightSource()
{m_CutoffAngle=0.5f;};
RGP_CORE::SpotLight::SpotLight(Vertex3d Pos):LightSource(Pos)
{m_CutoffAngle=0.5f;};
RGP_CORE::SpotLight::SpotLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):LightSource(Pos,Dir,Up)
{m_CutoffAngle=0.5f;};


void RGP_CORE::SpotLight::UpdateLightProjectionMtx()
{
	FillPersPectiveMatrix(RadiustoDegree(m_CutoffAngle*2.0f),m_CutoffAngle*2.0f , 0.0, m_Distance, m_LightProjectionMtx);
};