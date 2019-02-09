#ifndef _LEVEL_BUILDER_CUSTOM_SCENE_H_
#define _LEVEL_BUILDER_CUSTOM_SCENE_H_

#include "..//Core//Scene//GameScene.h"

using namespace RGP_CORE;
namespace RGP_LEVELBUILDER {
	class CustomScene : public GameScene {
	public: 
		CustomScene();
		~CustomScene();
		void Destroy();
		_bool isActorSelected(_u32b index);
		_bool isActorSelected(const _s8b* actorName);
		_bool setActorAsSelected(_u32b index, _bool Selected = true);
		_bool setActorAsSelected(const _s8b* nameactor, _bool Selected = true);
		_bool* getMemoryCase(_u32b index);

		 _u32b						AddActor(BaseActor* actor);
		 _bool						RemoveActorAt(_u32b index);
		 _bool						RemoveActor(BaseActor* actor);
		 void						RemoveSelectedActors();
		 void						UnselectAll();

	private:
		_bool*			m_isSelected;
		_u32b			m_NumElements;
	};

}
#endif