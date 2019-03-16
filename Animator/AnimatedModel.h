#ifndef _RGP_ANIMATOR_ANIMATEDMODEL_H_
#define _RGP_ANIMATOR_ANIMATEDMODEL_H_
#include "..//Core//Graphics//Model3D.h"
#include "..//Core//BaseActors/Dynamic.h"
#include "..//Core//AdvancedActors//DataType.h"

#define MAX_NUM_BONES		50
using namespace RGP_CORE;
namespace RGP_ANIMATOR {
	class AnimatedModel :  public Model3D, public Dynamic 
	{
		enum Status {
			STOP=0,
			PLAY=1,
			PAUSE=2
		};
	public:
		static AnimatedModel*	CreateFromFile(GLRenderer* renderer, _s8b* filename);
		AnimatedModel();
		~AnimatedModel();
		void		Destroy();
		void		Update(_float dt);
		void		Play();
		void		Pause();
		void		Stop();
		void		Render(Camera* selected);
		void		RenderBones(Camera * selected);
		void		RenderWeighMap(Camera* selected);
		_bool		Init(GLRenderer* renderer,_s8b* modelfilename);
		_bool		AddBone(Bone bone);
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
		_bool		ProcessBones(aiNode* node,const aiScene* scene);
		_bool		ProcessAnimation(const aiScene* Scene);
		void		InterpolateBonesTransformation();

		

	private:
		_bool			m_Initialized;
		Skeleton		m_Skeleton;
		Animation*		m_Animations;
		_u32b			m_NumAnimations;
		_u32b			m_SelectedAnimation;
		_u32b			m_Size;
		_u32b			m_BoneRenderingShader;
		_u32b			m_BonesVBO;
		_u32b			m_BonesVAO;

		_u32b			m_WeighMappingShader;
		_u32b			m_WeighMappingModeVAO;
		_u32b			m_WeighMappingVBO;

		_u8b			m_Mode;//0 solid , 1 weighmap

		_double			m_Cursor;
		Status			m_Status;

	};

}

#endif
