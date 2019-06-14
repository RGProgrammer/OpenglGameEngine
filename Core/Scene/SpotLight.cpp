#include ".//SpotLight.h"
RGP_CORE::SpotLight::SpotLight():LightSource()
{
	m_CutoffAngle=0.7f;
};
RGP_CORE::SpotLight::SpotLight(Vertex3d Pos):LightSource(Pos)

{
	m_CutoffAngle = 0.7f;
};
RGP_CORE::SpotLight::SpotLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):LightSource(Pos,Dir,Up)

{
	m_CutoffAngle = 0.7f;
};

void  RGP_CORE::SpotLight::UpdateLightViewMtx()
{
	FillViewMatrix(m_Position, m_Direction, m_Up, m_LightViewMtx);
};

void RGP_CORE::SpotLight::UpdateLightProjectionMtx(Camera* Selected)
{
	FillPersPectiveMatrix(m_CutoffAngle*2.0f,1.0f , 1.0f,m_Distance*m_Distance , this->m_LightProjectionMtx);
};