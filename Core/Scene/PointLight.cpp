#include ".//PointLight.h"

RGP_CORE::PointLight::PointLight(): RGP_CORE::LightSource()
{
	m_LightViewMtx = (_float*)malloc(96 * sizeof(_float));//contains all the View matrices for a point light
	m_Distance = 4.0f;
};
RGP_CORE::PointLight::PointLight(Vertex3d Pos): RGP_CORE::LightSource(Pos)
{
	m_LightViewMtx = (_float*)malloc(96 * sizeof(_float));
	m_Distance = 4.0f;
};
RGP_CORE::PointLight::PointLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up): RGP_CORE::LightSource(Pos,Dir,Up)
{
	m_LightViewMtx = (_float*)malloc(96 * sizeof(_float));
	m_Distance = 4.0f;
};

void RGP_CORE::PointLight::UpdateLightViewMtx()
{
	Vertex3d Right = Normalize3d(CrossProduct3d(m_Direction, m_Up)), Left = ScaleVertex3d(Right,-1),
		Back = ScaleVertex3d(m_Direction, -1), Bottom = ScaleVertex3d(m_Up, -1);
	float* Adr = m_LightViewMtx;
	FillViewMatrix(m_Position, m_Direction, m_Up       , Adr     );//front
	FillViewMatrix(m_Position, Left		  , m_Up       , Adr +16 );//Right
	FillViewMatrix(m_Position, Back       , m_Up       , Adr +32 );//back
	FillViewMatrix(m_Position, Right      , m_Up       , Adr +48);//Left
	FillViewMatrix(m_Position, m_Up       , Back       , Adr +64);//Up
	FillViewMatrix(m_Position, Bottom     , m_Direction, Adr +80);//Down

};
void RGP_CORE::PointLight::UpdateLightProjectionMtx(Camera* Selected)
{
	FillPersPectiveMatrix(M_PI_2, 1.0f, 1.0, m_Distance*10.0f, m_LightProjectionMtx);
};