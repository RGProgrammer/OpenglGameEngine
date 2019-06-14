#ifndef _RGP_ENVIRONMENT_MAP_PROBE_
#define _RGP_ENVIRONMENT_MAP_PROBE_

/*Env Map or Environment map is a class made to be used by object that reflect or refract light 
 this type of object need to be attached to be effective*/

#include "..//Scene//Camera.h"
#include "..//Graphics//GLRenderer.h"
#include "..//Scene//GameScene.h"




namespace RGP_CORE {
	class GLRenderer;
	class GameScene;

	class EnvMapProbe: public BaseActor {
	public:
		EnvMapProbe();
		EnvMapProbe(Vertex3d Pos);
		~EnvMapProbe();
		void			Destroy();
		_bool			Init(GLRenderer* Renderer, GameScene* Scene);
		void			GenerateEnvMap();
		_bool			isStatic();
		void			setStatic(_bool make=true);
		GLuint			getEnvMap();
		void			setPosition(Vertex3d Pos);
		GLuint			getTexture();
	private:
		GLRenderer *	m_Renderer;
		GameScene*		m_Scene;
		PerspCamera*	m_PerspCameras[6];
		_bool			m_isInitialized;
		_bool			m_isStatic;
		_u32b			m_FBO;
		_u32b			m_TextureCubeMap;
		

	};

	
	
}



#endif
