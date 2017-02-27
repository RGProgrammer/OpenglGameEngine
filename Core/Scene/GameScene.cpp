#include "GameScene.h"


TTB::GameScene::GameScene(): Size(10) ,m_NBActors(0) ,v_Actors(NULL) ,m_NBLights(0),m_LightSources(NULL),
                            m_Camera(NULL) ,m_Gravity({0.0f,0.0f,0.0f})//,m_PlayerRef(NULL)
{
};
TTB::GameScene::~GameScene() {
    this->Destroy();
};
_s16b TTB::GameScene::AddActor(BaseActor* actor){
    if(actor==NULL)
        return 0 ;
    if(v_Actors==NULL){
        v_Actors=(BaseActor**)malloc((Size)*sizeof(BaseActor*));
        if(v_Actors==NULL)
            return 0 ;
    }
    if(m_NBActors==Size){
        BaseActor** tmp=v_Actors ;
        // allocation of bigger vetor
        v_Actors=(BaseActor**)malloc((Size+10)*sizeof(BaseActor*));
        if(v_Actors==NULL){
            v_Actors=tmp;
            return 0;
        }
        //coping old vector data
        Size+=10 ;
        for(_u32b i=0 ; i<m_NBActors;i++)
            v_Actors[i]=tmp[i];
        free(tmp);
        }
        v_Actors[m_NBActors]=actor ;
        m_NBActors++;
        return 1 ;
};
void TTB::GameScene::FreeVector(){
    BaseActor* tmp=NULL ;
    if(v_Actors){
        for(_u32b i=0;i<m_NBActors ;++i){
            tmp=v_Actors[i];
            tmp->Destroy();
            delete tmp;
        }
        free(v_Actors);
    }
    v_Actors=NULL ;
    m_NBActors=0 ;
    Size=10;
    if(m_LightSources){
        for(_u32b i=0;i<m_NBLights ;++i){
            m_LightSources[i]->Destroy();
            delete (m_LightSources[i]);
        }
        free(m_LightSources);
        m_LightSources=NULL ;
        m_NBLights=0 ;
    }
};
void TTB::GameScene::Destroy() {
    this->FreeVector();
    m_Camera=NULL ;
    //m_PlayerRef=NULL;
    m_Gravity={0.0f,0.0f,0.0f};

};
TTB::BaseActor*  TTB::GameScene::getActor(_u32b index){
    if(index>=0 && index< m_NBActors){
        return v_Actors[index];
    }else return NULL ;
};
void TTB::GameScene::RemoveActorAt(_u32b index){
    BaseActor* tmp =NULL ;
    if(index>=0 && index< m_NBActors){
        tmp=v_Actors[index] ;
        tmp->Destroy();
        delete tmp ;
        for(_u32b i=index; i<m_NBActors-1 ;i++)
            v_Actors[i]=v_Actors[i+1];
        m_NBActors-- ;
    }
};

_u32b TTB::GameScene::getNBActors(){ return m_NBActors ;};
_s16b TTB::GameScene::AddLight(TTB::LightSource* Source){
    if(!Source)
        return 0 ;
    TTB::LightSource** tmp=(TTB::LightSource**)malloc((m_NBLights+1)*sizeof(TTB::LightSource*));
    if(!tmp)
        return 0 ;
    for(_u32b i=0; i< m_NBLights;++i){
        tmp[i]=m_LightSources[i];
    }
    if(m_LightSources)
        free(m_LightSources);
    m_LightSources=tmp;
    m_LightSources[m_NBLights]=Source ;
    ++m_NBLights ;
    return 1 ;
};
void TTB::GameScene::RemoveLightAt(_u32b index){
    if(!m_LightSources)
        if(index>=0 && index< m_NBLights){
            m_LightSources[index]->Destroy();
            delete m_LightSources[index];
            for(_u32b i=0 ; i< m_NBLights-1 ;++i)
                m_LightSources[i]=m_LightSources[i+1];
        }
};
TTB::LightSource*  TTB::GameScene::getLight(_u32b index){
    if(index>=0 && index <m_NBLights)
        return m_LightSources[index];
    else
        return NULL ;

};
_u32b TTB::GameScene::getNBLights(){ return m_NBLights ;};
void TTB::GameScene::setCamera(Camera* cam){ m_Camera=cam ;};
TTB::Camera* TTB::GameScene::getCamera(){ return m_Camera ;};
void TTB::GameScene::setGravity(Vertex3d force){ m_Gravity= force ;};
Vertex3d TTB::GameScene::getGravity(){ return m_Gravity ;};
