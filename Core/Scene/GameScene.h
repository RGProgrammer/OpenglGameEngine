/////////////////////////////////////////////////////////////////////////////////////
//                          GameScene                                              //
//the GameScene could be considered as a handler of the Game-objects'reference     //
//all the game Objects that could be placed in the scene are 'Actor' based Objects //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////
#ifndef PFE_GAMESCENE_H_
#define PFE_GAMESCENE_H_

#include "..//Common//BasePrimitiveTypes.h"
#include "..//BaseActors//BaseActor.h"
#include ".//Camera.h"


namespace TTB {
	class GameScene {
	public:
    	GameScene();
    	~GameScene() ;
    	_s16b AddActor(BaseActor* actor);
    	void FreeVector();
    	void Destroy() ;
    	BaseActor*  getActor(_u32b index);
    	_u32b getNBActors();
    	void setCamera(Camera*);
    	//Player* getPlayer();
    	//void setPlayer(Player* player);
    	Camera* getCamera();
    	void RemoveAt(_u32b index);
    	void setGravity(Vertex3d force);
    	Vertex3d getGravity();
	private:
    	_u32b        		Size ;
    	_u32b        		m_NBActors ;
    	BaseActor**         v_Actors ;
    	Camera*             m_Camera ;
    	//Player*             m_PlayerRef
    	Vertex3d 			m_Gravity ;
	};
};
#endif // PFE_GAMESCENE_H_
