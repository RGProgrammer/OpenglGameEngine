#include "GameScene.h"


RGP_CORE::GameScene::GameScene(): Size(10) ,m_NumActors(0) ,v_Actors(NULL) ,
								m_NumEnvMap(0),m_EnvMaps(NULL),m_Camera(NULL) ,m_Gravity({0.0f,0.0f,0.0f}),
								m_NumUILayerComponents(0), m_UILayerCompoenents(NULL )
							
{
};
RGP_CORE::GameScene::~GameScene() {
    this->Destroy();
};
_u32b RGP_CORE::GameScene::AddActor(BaseActor* actor){
    if(actor==NULL)
        return 0 ;
    if(v_Actors==NULL){
        v_Actors=(BaseActor**)malloc((Size)*sizeof(BaseActor*));
        if(v_Actors==NULL)
            return 0 ;
    }
    if(m_NumActors==Size){
        BaseActor** tmp=v_Actors ;
        // allocation of bigger vetor
        v_Actors=(BaseActor**)malloc((Size+10)*sizeof(BaseActor*));
        if(v_Actors==NULL){
            v_Actors=tmp;
            return 0;
        }
        //coping old vector data
        Size+=10 ;
        for(_u32b i=0 ; i<m_NumActors;i++)
            v_Actors[i]=tmp[i];
        free(tmp);
        }
        v_Actors[m_NumActors]=actor ;
        m_NumActors++;
        return m_NumActors ;
};
void RGP_CORE::GameScene::FreeVector(){
    BaseActor* tmp=NULL ;
    if(v_Actors){
        for(_u32b i=0;i< m_NumActors ;++i){
            tmp=v_Actors[i];
            tmp->Destroy();
            delete tmp;
        }
        free(v_Actors);
    }
    v_Actors=NULL ;
    m_NumActors=0 ;
    Size=10;
 

	if (m_EnvMaps) {
		for (_u32b i = 0; i<m_NumEnvMap; ++i) {
			m_EnvMaps[i]->Destroy();
			delete (m_EnvMaps[i]);
		}
		free(m_EnvMaps);
		m_EnvMaps = NULL;
		m_NumEnvMap = 0;
	}
	if (m_UILayerCompoenents) {
		for (_u32b i = 0; i<m_NumUILayerComponents; ++i) {
			m_UILayerCompoenents[i]->Destroy();
			delete (m_UILayerCompoenents[i]);
		}
		free(m_UILayerCompoenents);
		m_UILayerCompoenents = NULL;
		m_NumUILayerComponents = 0;
	}

};
void RGP_CORE::GameScene::Destroy() {
    this->FreeVector();
    m_Gravity={0.0f,0.0f,0.0f};
	m_Camera = NULL;

};
RGP_CORE::BaseActor*  RGP_CORE::GameScene::getActor(_u32b index){
    if(index>=0 && index< m_NumActors){
        return v_Actors[index];
    }else 
		return NULL ;
};
_bool  RGP_CORE::GameScene::RemoveActorAt(_u32b index){
    BaseActor* tmp =NULL ;
    if(index>=0 && index< m_NumActors){
        tmp=v_Actors[index] ;
        tmp->Destroy();
        delete tmp ;
		for(_u32b i=index; i<m_NumActors-1 ;i++)
            v_Actors[i]=v_Actors[i+1];
        m_NumActors-- ;
		return true;
    }
	return false;
};
_bool RGP_CORE::GameScene::RemoveActor(BaseActor* actor)
{
	for (_u32b i = 0; i < m_NumActors ; i++)
		if (v_Actors[i] == actor) {
			delete v_Actors[i];
			for (; i < m_NumActors - 1;++i) {
				v_Actors[i] = v_Actors[i + 1];
			}
			m_NumActors--;
			return true;
		}
	return  false;
};

_u32b RGP_CORE::GameScene::getNumActors(){ return m_NumActors ;};

void RGP_CORE::GameScene::setCamera(Camera* cam){ m_Camera=cam ;};
RGP_CORE::Camera* RGP_CORE::GameScene::getCamera(){ return m_Camera ;};
void RGP_CORE::GameScene::setGravity(Vertex3d force){ m_Gravity= force ;};
Vertex3d RGP_CORE::GameScene::getGravity(){ return m_Gravity ;};

_s16b	RGP_CORE::GameScene::AddUIComponent(Renderable* component)
{
	if (!component)
		return 0;
	RGP_CORE::Renderable** tmp = (RGP_CORE::Renderable**)malloc((m_NumUILayerComponents + 1) * sizeof(RGP_CORE::Renderable*));
	if (!tmp)
		return 0;
	for (_u32b i = 0; i< m_NumUILayerComponents; ++i) {
		tmp[i] = m_UILayerCompoenents[i];
	}
	if (m_UILayerCompoenents)
		free(m_UILayerCompoenents);
	m_UILayerCompoenents = tmp;
	m_UILayerCompoenents[m_NumUILayerComponents] = component;
	++m_NumUILayerComponents;
	return 1;
};
RGP_CORE::Renderable*	RGP_CORE::GameScene::getUICompoenent(_u32b index)
{
	if (index >= 0 && index <m_NumUILayerComponents)
		return m_UILayerCompoenents[index];
	else
		return NULL;
};
_u32b	RGP_CORE::GameScene::getNumUIComponents()
{
	return m_NumUILayerComponents;
};
void	RGP_CORE::GameScene::RemoveUIComponentAt(_u32b index)
{
	if (!m_UILayerCompoenents)
		if (index >= 0 && index < m_NumUILayerComponents) {
			m_UILayerCompoenents[index]->Destroy();
			delete m_UILayerCompoenents[index];
			for (_u32b i = 0; i < m_NumUILayerComponents - 1; ++i)
				m_UILayerCompoenents[i] = m_UILayerCompoenents[i + 1];
		}
};