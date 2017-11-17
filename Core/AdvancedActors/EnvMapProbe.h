#ifndef _RGP_ENVIRONMENT_MAP_PROBE_
#define _RGP_ENVIRONMENT_MAP_PROBE_

/*Env Map or Environment map is a class made to be used by object that reflect or refract light 
 this type of object need to be attached to be effective*/


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

	private:
		_bool			m_isStatic;
		GLuint			m_FBO;
		GLuint			m_TextureCubeMap;
		GLRenderer*		m_Renderer;
		GameScene*		m_Scene;


	private :
		static _bool	InitStaticAttributs();
	};

	static _float		ViewMatices[96];//6* 4*4
	static _float		ProjMatix[16];
	static _bool		AttributsInitialized = false;//to check if the matrices are initialized
	
	
}



#endif
