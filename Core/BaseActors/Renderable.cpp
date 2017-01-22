#include ".//Renderable.h"


TTB::Renderable::Renderable():BaseActor(),m_Visible(true),m_GLRenderer(NULL){
    m_ID |=RENDERABLE ;
};
TTB::Renderable::Renderable(Vertex3d Pos):BaseActor(Pos),
                                            m_GLRenderer(NULL),m_Visible(true){
    m_ID |=RENDERABLE ;
};
TTB::Renderable::Renderable(Vertex3d Pos,Vertex3d Dir,Vertex3d Up):BaseActor(Pos,Dir,Up),
                                                                    m_Visible(true),m_GLRenderer(NULL){
    m_ID |=RENDERABLE ;
};
TTB::Renderable::~Renderable(){
    this->Destroy();
};
void TTB::Renderable::Destroy(){
    m_GLRenderer=NULL ;
};
void TTB::Renderable::setRenderer(GLRenderer* renderer){
    m_GLRenderer=renderer ;
};
void TTB::Renderable::setVisible(_bool visible){
    m_Visible=visible ;
};
_bool TTB::Renderable::isVisible(){
    return m_Visible ;
};
