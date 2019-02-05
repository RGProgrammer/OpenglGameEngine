/////////////////////////////////////////////////////////////////////////////////////
//                          GameScene                                              //
//the GameScene could be considered as a handler of the Game-objects'reference     //
//all the game Objects that could be placed in the scene are 'Actor' based Objects //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////
#ifndef PFE_GAMESCENE_H_
#define PFE_GAMESCENE_H_

#include "..//Common//BasePrimitiveTypes.h"
#include "..//AdvancedActors//EnvMapProbe.h"
#include "..//BaseActors//Renderable.h"
#include ".//LightSource.h"
#include ".//Camera.h"

namespace RGP_CORE {
	class EnvMapProbe;
	class Renderable;

	class GameScene {
	public:
    	GameScene();
    	virtual ~GameScene() ;
    	void						FreeVector();
    	virtual void				Destroy() ;
		_u32b						getNumActors();
		BaseActor*					getActor(_u32b index);
    	virtual _s16b				AddActor(BaseActor* actor);		
		virtual _bool				RemoveActorAt(_u32b index);
		virtual _bool				RemoveActor(BaseActor* actor);

    	/*_s16b						AddLight(LightSource* Source);
    	LightSource*				getLight(_u32b index);
    	_u32b						getNumLights();
    	void						RemoveLightAt(_u32b index);
		*/
		_s16b						AddEnvMapProbe(LightSource* Source);
		EnvMapProbe*				getEnvMapProbe(_u32b index);
		_u32b						getNumEnvMapProbe();
		void						RemoveEnvMapProbeAt(_u32b index);

		_s16b						AddUIComponent(Renderable* Source);
		Renderable*					getUICompoenent(_u32b index);
		_u32b						getNumUIComponents();
		void						RemoveUIComponentAt(_u32b index);

    	void						setCamera(Camera*);
    	Camera*						getCamera();
    	
    	void						setGravity(Vertex3d force);
    	Vertex3d					getGravity();
	protected:
    	_u32b        		Size ;
	private :
    	_u32b        		m_NumActors ;
    	BaseActor**         v_Actors ;

    	/*_u32b               m_NumLights ;
    	LightSource**       m_LightSources ;*/

		_u32b				m_NumEnvMap;
		EnvMapProbe**		m_EnvMaps;

		_u32b				m_NumUILayerComponents;
		Renderable**		m_UILayerCompoenents;

    	Camera*             m_Camera ;
    	Vertex3d 			m_Gravity ;
	};
};

#endif // PFE_GAMESCENE_H_
