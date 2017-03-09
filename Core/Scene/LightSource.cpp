#include ".//LightSource.h"

TTB::LightSource::LightSource(): BaseActor(),
                                    m_DiffuseColor{1.0,1.0,1.0},m_SpecularColor{1.0,1.0,1.0},m_AmbientColor{1.0,1.0,1.0},
                                    m_Distance(10.0f),m_Shiness(100.0f),m_CutoffAngle(-1.0f)
{ m_ID |= LIGHTSOURCE ;};

TTB::LightSource::LightSource(Vertex3d Pos): BaseActor(Pos),
                                    m_DiffuseColor{1.0,1.0,1.0},m_SpecularColor{1.0,1.0,1.0},m_AmbientColor{1.0,1.0,1.0},
                                    m_Distance(10.0f),m_Shiness(100.0f),m_CutoffAngle(-1.0f)
{ m_ID |= LIGHTSOURCE ;};

TTB::LightSource::LightSource(Vertex3d Pos,Vertex3d Dir,Vertex3d Up): BaseActor(Pos,Dir,Up),
                                    m_DiffuseColor{1.0,1.0,1.0},m_SpecularColor{1.0,1.0,1.0},m_AmbientColor{1.0,1.0,1.0},
                                    m_Distance(10.0f),m_Shiness(100.0f),m_CutoffAngle(-1.0f)
{ m_ID |= LIGHTSOURCE ;};

TTB::LightSource::~LightSource(){
    this->Destroy();
};
void TTB::LightSource::setLightDiffuseColor(_float red, _float green, _float blue){
    float Sum=red+green+blue ;
    m_DiffuseColor[0]=red/Sum ;
    m_DiffuseColor[1]=green/Sum ;
    m_DiffuseColor[2]=blue/Sum ;
};
void TTB::LightSource::setLightDiffuseColor(Vertex3d color){
    float Sum=color.x+color.y+color.z ;
    m_DiffuseColor[0]=color.x/Sum;
    m_DiffuseColor[1]=color.y/Sum;
    m_DiffuseColor[2]=color.z/Sum;
};
void TTB::LightSource::setLightDiffuseColor(_u8b red, _u8b green, _u8b blue){
    m_DiffuseColor[0]=red/255.0f;
    m_DiffuseColor[1]=red/255.0f;
    m_DiffuseColor[2]=red/255.0f;
};
void TTB::LightSource::setLightSpecularColor(_float red, _float green, _float blue){
    float Sum=red+green+blue ;
    m_SpecularColor[0]=red/Sum ;
    m_SpecularColor[1]=green/Sum ;
    m_SpecularColor[2]=blue/Sum ;
};
void TTB::LightSource::setLightSpecularColor(Vertex3d color){
    float Sum=color.x+color.y+color.z ;
    m_SpecularColor[0]=color.x/Sum;
    m_SpecularColor[1]=color.y/Sum;
    m_SpecularColor[2]=color.z/Sum;
};
void TTB::LightSource::setLightSpecularColor(_u8b red, _u8b green, _u8b blue){
    m_SpecularColor[0]=red/255.0f;
    m_SpecularColor[1]=red/255.0f;
    m_SpecularColor[2]=red/255.0f;
};
void TTB::LightSource::setLightAmbientColor(_float red, _float green, _float blue){
    float Sum=red+green+blue ;
    m_AmbientColor[0]=red/Sum ;
    m_AmbientColor[1]=green/Sum ;
    m_AmbientColor[2]=blue/Sum ;
};
void TTB::LightSource::setLightAmbientColor(Vertex3d color){
    float Sum=color.x+color.y+color.z ;
    m_AmbientColor[0]=color.x/Sum;
    m_AmbientColor[1]=color.y/Sum;
    m_AmbientColor[2]=color.z/Sum;
};
void TTB::LightSource::setLightAmbientColor(_u8b red, _u8b green, _u8b blue){
    m_AmbientColor[0]=red/255.0f;
    m_AmbientColor[1]=red/255.0f;
    m_AmbientColor[2]=red/255.0f;
};
void TTB::LightSource::setLightDistance(_float Distance){
    m_Distance=Distance ;
};
void TTB::LightSource::setLightStrength(_float Strength){
    m_Shiness=Strength ;
};
const float* TTB::LightSource::getLightDiffuseColor(){ return m_DiffuseColor;};
const float* TTB::LightSource::getLightSpecularColor(){ return m_SpecularColor;};
const float* TTB::LightSource::getLightAmbientColor(){return m_AmbientColor ;};
_float   TTB::LightSource::getLightDistance(){ return m_Distance ;};
_float   TTB::LightSource::getLightAttinuation(){ return 1.0f / m_Distance ;};
_float   TTB::LightSource::getLightCutoffAngle(){ return m_CutoffAngle ;};
_float   TTB::LightSource::getLightShiness(){return m_Shiness ;};
