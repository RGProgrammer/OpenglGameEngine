#include "./AnimatedModel.h"

RGP_ANIMATOR::AnimatedModel::AnimatedModel() : Dynamic(),Model3D(), m_Animations(0), m_NumAnimations(0),
												m_SelectedAnimation(0), m_Skeleton({ 0,0 }), m_BoneRenderingShader(0),
												m_SolidModeShader(0),m_WeighMappingShader(0), m_Initialized(false)
{
	
};
RGP_ANIMATOR::AnimatedModel::~AnimatedModel()
{
	this->Destroy();
};
void RGP_ANIMATOR::AnimatedModel::Destroy()
{
	RemoveAll();
	if (m_Skeleton.Parts) {
		for (_u32b i = 0; i < m_Skeleton.NumParts; ++i) {
			if (m_Skeleton.Parts[i].Name) {
				free(m_Skeleton.Parts[i].Name);
			}
			if (m_Skeleton.Parts[i].Indices) {
				free(m_Skeleton.Parts[i].Indices);
			}
			if (m_Skeleton.Parts[i].Factor) {
				free(m_Skeleton.Parts[i].Factor);
			}
			if (m_Skeleton.Parts[i].ChildrenIndices) {
				free(m_Skeleton.Parts[i].ChildrenIndices);
			}
		}
		free(m_Skeleton.Parts);
		m_Skeleton.Parts = NULL;
		m_Skeleton.NumParts = 0;

	}
	if (m_GLRenderer) {
		if (m_BoneRenderingShader) {
			m_GLRenderer->DeleteGLProgram(m_BoneRenderingShader);
			m_BoneRenderingShader = 0;
		}
		if (m_SolidModeShader) {
			m_GLRenderer->DeleteGLProgram(m_SolidModeShader);
			m_SolidModeShader = 0;
		}
		if (m_WeighMappingShader) {
			m_GLRenderer->DeleteGLProgram(m_WeighMappingShader);
			m_WeighMappingShader = 0;
		}
	}
	m_Initialized = false;
	Model3D::Destroy();
};
void RGP_ANIMATOR::AnimatedModel::Render(Camera* selected)
{

};

Bone*		RGP_ANIMATOR::AnimatedModel::getBonebyName(_s8b* bonename)
{
	for (_u32b i = 0; i < m_Skeleton.NumParts; ++i) {
		if (strcmp(bonename, m_Skeleton.Parts[i].Name) == 0)
			return &(m_Skeleton.Parts[i]);
	}
	return NULL;
};
Bone*			RGP_ANIMATOR::AnimatedModel::getBonebyIndex(_u32b index)
{
	if(index>0 && index<=m_Skeleton.NumParts)
		return &(m_Skeleton.Parts[index-1]);
	return NULL;
};
Skeleton*	RGP_ANIMATOR::AnimatedModel::getSkeleton()
{
	return &m_Skeleton;
};
Animation*	RGP_ANIMATOR::AnimatedModel::getAnimationbyIndex(_u32b index)
{
	if(index>0 && index <= m_NumAnimations)
		return &(m_Animations[index-1]);
	return NULL;
};
Animation*	RGP_ANIMATOR::AnimatedModel::getAnimationbyName(_s8b* name)
{
	for (_u32b i = 0; i < m_NumAnimations; ++i) {
		if (strcmp(name,m_Animations[i].Name) == 0)
			return &(m_Animations[i]);
	}
	return NULL;
};
_u32b		RGP_ANIMATOR::AnimatedModel::getNumAnimations()
{
	return m_NumAnimations;
};

Animation*  RGP_ANIMATOR::AnimatedModel::AddAnimation(_s8b* name)
{
	Animation* tmp = NULL;
	if (m_NumAnimations == m_Size) {
		tmp = (Animation*)malloc((m_Size + 5) * sizeof(Animation));
		if (!tmp)
			return NULL;
		for (_u32b i = 0; i < m_NumAnimations; ++i) {
			tmp[i] = m_Animations[i];
		}
		free(m_Animations);
		m_Animations = tmp;
	}
	if (!(m_Animations[m_NumAnimations].Name = CreateStringCopy(name)))
		return NULL;
	m_NumAnimations++;
	return &(m_Animations[m_NumAnimations - 1]);


};
void		 RGP_ANIMATOR::AnimatedModel::SelectAnimationbyIndex(_u32b index)
{
	if (index > 0 && index <= m_NumAnimations)
		m_SelectedAnimation = index;

};
void		 RGP_ANIMATOR::AnimatedModel::SelectAnimationbyName(_s8b* name)
{
	for (_u32b i = 0; i < m_NumAnimations; ++i) 
		if (strcmp(name, m_Animations[i].Name) == 0){
			m_SelectedAnimation = i + 1;
			return;
		}
};
_u32b		 RGP_ANIMATOR::AnimatedModel::getSelectedIndex()
{
	return m_SelectedAnimation;
};
void		RGP_ANIMATOR::AnimatedModel::RemoveAnimationbyName(_s8b * name)
{
	for(_u32b i =0 ; i< m_NumAnimations ; ++i)
		if (strcmp(m_Animations[i].Name, name) == 0) {
			for (_u32b j = i; j < m_NumAnimations - 1; ++j)
				m_Animations[j] = m_Animations[j + 1];
			m_NumAnimations--;
		}
};
void		RGP_ANIMATOR::AnimatedModel::RemoveAnimationbyIndex(_u32b index)
{
	if (index > 0 && index <= m_NumAnimations) {
		for (_u32b j = index; j < m_NumAnimations - 1; ++j)
			m_Animations[j] = m_Animations[j + 1];
		m_NumAnimations--;
	}
};
void		RGP_ANIMATOR::AnimatedModel::RemoveAll()
{
	if(m_Animations) {
		for (_u32b i = 0; i < m_NumAnimations; ++i) {
			if (m_Animations[i].Name)
				free(m_Animations[i].Name);
			if (m_Animations[i].Poses) {
				for (_u32b j = 0; j < m_Animations[i].NumPoses; ++j) {
					if (m_Animations[i].Poses[j].Keys) {
						for (_u32b k = 0; k < m_Animations[i].Poses[j].numKeys; ++k) {
							if (m_Animations[i].Poses[j].Keys[k].Name)
								free(m_Animations[i].Poses[j].Keys[k].Name);
						}
						free(m_Animations[i].Poses[j].Keys);
					}
				}
				free(m_Animations[i].Poses);

			}
		}
		free(m_Animations);
		m_Animations = NULL;
		m_NumAnimations = 0;
		m_SelectedAnimation = 0;
	}
};