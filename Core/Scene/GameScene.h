/////////////////////////////////////////////////////////////////////////////////////
//                          GameScene                                              //
//the GameScene could be considered as a handler of the Game-objects'reference     //
//all the game Objects that could be placed in the scene are 'Actor' based Objects //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////
#ifndef PFE_GAMESCENE_H_
#define PFE_GAMESCENE_H_

#include "..//Common//BasePrimitiveTypes.h"
//#include "..//BaseActors//BaseActor.h"
#include ".//LightSource.h"
#include ".//Camera.h"

namespace RGP_CORE {
	class GameScene {
	public:
    	GameScene();
    	~GameScene() ;
    	void FreeVector();
    	void Destroy() ;
    	_s16b AddActor(BaseActor* actor);
    	BaseActor*  getActor(_u32b index);
    	_u32b getNBActors();
    	void RemoveActorAt(_u32b index);
    	_s16b AddLight(LightSource* Source);
    	LightSource*  getLight(_u32b index);
    	_u32b getNBLights();
    	void RemoveLightAt(_u32b index);
    	void setCamera(Camera*);
    	Camera* getCamera();
    	//Player* getPlayer();
    	//void setPlayer(Player* player);
    	void setGravity(Vertex3d force);
    	Vertex3d getGravity();
	private:
    	_u32b        		Size ;
    	_u32b        		m_NBActors ;
    	BaseActor**         v_Actors ;
    	_u32b               m_NBLights ;
    	LightSource**       m_LightSources ;
    	Camera*             m_Camera ;
    	//Player*             m_PlayerRef
    	Vertex3d 			m_Gravity ;
	};
};
#endif // PFE_GAMESCENE_H_
