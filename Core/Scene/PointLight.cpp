#include ".//PointLight.h"

RGP_CORE::PointLight::PointLight(): RGP_CORE::LightSource()
{};
RGP_CORE::PointLight::PointLight(Vertex3d Pos): RGP_CORE::LightSource(Pos)
{};
RGP_CORE::PointLight::PointLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up): RGP_CORE::LightSource(Pos,Dir,Up)
{};
