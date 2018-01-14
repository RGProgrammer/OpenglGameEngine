#include ".//DirectionnalLight.h"

RGP_CORE::DirectionnalLight::DirectionnalLight(): RGP_CORE::LightSource()
{m_Distance =-1.0f ;
m_LightViewMtx = (_float*)malloc(16 * sizeof(_float));
};
RGP_CORE::DirectionnalLight::DirectionnalLight(Vertex3d Pos): RGP_CORE::LightSource(Pos)
{m_Distance =-1.0f ;
m_LightViewMtx = (_float*)malloc(16 * sizeof(_float));
};
RGP_CORE::DirectionnalLight::DirectionnalLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):RGP_CORE::LightSource(Pos, Dir, Up)
{m_Distance =-1.0f ;
m_LightViewMtx = (_float*)malloc(16 * sizeof(_float));
};


void RGP_CORE::DirectionnalLight:: UpdateLightViewMtx()
{
	FillViewMatrix(m_Position, m_Direction, m_Up, m_LightViewMtx);
};
void RGP_CORE::DirectionnalLight::UpdateLightProjectionMtx(Camera * Selected)
{
	FillOrthographicsMatrix(-100.0f, 100.0f, 100.0f, -100.0f, 1.0, 5000.0f, m_LightProjectionMtx);
};