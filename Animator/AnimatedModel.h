#ifndef _RGP_ANIMATOR_ANIMATEDMODEL_H_
#define _RGP_ANIMATOR_ANIMATEDMODEL_H_
#include "..//Core//Graphics//Model3D.h"
#include "..//Core//BaseActors/Dynamic.h"
#include "..//Core//AdvancedActors//DataType.h"
using namespace RGP_CORE;
namespace RGP_ANIMATOR {
	class AnimatedModel : public Model3D, public Dynamic 
	{
	public:
		AnimatedModel();
		~AnimatedModel();
		void		Destroy();
		void		Render(Camera* selected);
		void		RenderBones(Camera * selected);
		_bool		Init(GLRenderer* renderer);
		Bone*		getBonebyName(_s8b* bonename);
		Bone*		getBonebyIndex(_u32b index);
		Skeleton*	getSkeleton();
		Animation*	getAnimationbyIndex(_u32b index);
		Animation*	getAnimationbyName(_s8b* name);
		_u32b		getNumAnimations();

		Animation*  AddAnimation(_s8b* name);
		void		SelectAnimationbyIndex(_u32b index);
		void		SelectAnimationbyName(_s8b* name);
		_u32b		getSelectedIndex();
		void		RemoveAnimationbyName(_s8b * name);
		void		RemoveAnimationbyIndex(_u32b index);
		void		RemoveAll();

		

	private:
		Skeleton		m_Skeleton;
		Animation*		m_Animations;
		_u32b			m_NumAnimations;
		_u32b			m_SelectedAnimation;
		_u32b			m_Size;
		_u32b			m_BoneRenderingShader;
		_u32b			m_WeighMappingShader;
		_u32b			m_SolidModeShader;
		_bool			m_Initialized;

	};

}

#endif
