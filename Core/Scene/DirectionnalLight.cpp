#include ".//DirectionnalLight.h"

TTB::DirectionnalLight::DirectionnalLight(): TTB::LightSource()
{m_Distance =-1.0f ;};
TTB::DirectionnalLight::DirectionnalLight(Vertex3d Pos): TTB::LightSource(Pos)
{m_Distance =-1.0f ;};
TTB::DirectionnalLight::DirectionnalLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):TTB::LightSource(Pos, Dir, Up)
{m_Distance =-1.0f ;};
