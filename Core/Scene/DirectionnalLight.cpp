#include ".//DirectionnalLight.h"

RGP_CORE::DirectionnalLight::DirectionnalLight(): RGP_CORE::LightSource()
{m_Distance =-1.0f ;};
RGP_CORE::DirectionnalLight::DirectionnalLight(Vertex3d Pos): RGP_CORE::LightSource(Pos)
{m_Distance =-1.0f ;};
RGP_CORE::DirectionnalLight::DirectionnalLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):RGP_CORE::LightSource(Pos, Dir, Up)
{m_Distance =-1.0f ;};
