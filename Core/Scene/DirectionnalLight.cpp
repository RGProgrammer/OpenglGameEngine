#include ".//DirectionnalLight.h"

TTB::DirectionnalLight::DirectionnalLight(): LightSource()
{m_Distance =-1.0f ;};
TTB::DirectionnalLight::DirectionnalLight(Vertex3d Pos): LightSource(Pos)
{m_Distance =-1.0f ;};
TTB::DirectionnalLight::DirectionnalLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):LightSource(Pos, Dir, Up)
{m_Distance =-1.0f ;};
