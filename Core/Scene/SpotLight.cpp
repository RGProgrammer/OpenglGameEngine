#include ".//SpotLight.h"
RGP_CORE::SpotLight::SpotLight():LightSource()
{m_CutoffAngle=M_PI_4/2.0f;};
RGP_CORE::SpotLight::SpotLight(Vertex3d Pos):LightSource(Pos)
{m_CutoffAngle=M_PI_4/2.0f;};
RGP_CORE::SpotLight::SpotLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):LightSource(Pos,Dir,Up)
{m_CutoffAngle=M_PI_4/2.0f;};
