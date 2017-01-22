#include "GameScene.h"


TTB::GameScene::GameScene(): Size(10) ,m_NBActors(0) ,v_Actors(NULL) ,
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
        for(_u32b i=0;i<m_NBActors ;i++){
            tmp=v_Actors[i];
            tmp->Destroy();
            delete tmp;
        }
        free(v_Actors);
    }
    v_Actors=NULL ;
    m_NBActors=0 ;
    Size=10;
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
void TTB::GameScene::RemoveAt(_u32b index){
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
void TTB::GameScene::setCamera(Camera* cam){ m_Camera=cam ;};
TTB::Camera* TTB::GameScene::getCamera(){ return m_Camera ;};
/*TTB::Player* TTB::GameScene::getPlayer(){ return m_PlayerRef ; };
void TTB::GameScene::setPlayer(TTB::Player* player){ m_PlayerRef=player ;};*/
void TTB::GameScene::setGravity(Vertex3d force){ m_Gravity= force ;};
Vertex3d TTB::GameScene::getGravity(){ return m_Gravity ;};
