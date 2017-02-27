#include ".//PointLight.h"

TTB::PointLight::PointLight(): TTB::LightSource()
{};
TTB::PointLight::PointLight(Vertex3d Pos): TTB::LightSource(Pos)
{};
TTB::PointLight::PointLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up): TTB::LightSource(Pos,Dir,Up)
{};
