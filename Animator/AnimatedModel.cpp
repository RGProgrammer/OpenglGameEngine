#include "./AnimatedModel.h"



RGP_ANIMATOR::AnimatedModel*	RGP_ANIMATOR::AnimatedModel::CreateFromFile(GLRenderer* renderer, _s8b* filename)
{
	if (!renderer || !(renderer->isInitialized()))
		return NULL;
	AnimatedModel*	 am = new AnimatedModel();
	if (!am)
		return NULL;
	if (!am->Init(renderer, filename)) {
		delete am;
		return NULL;
	}
	const aiScene* Scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Fast );
	if (Scene->HasAnimations()) {
		//copy bones data :
		if (!am->ProcessBones(Scene->mRootNode,Scene)) {
			delete am;
			printf("error loading bones data\n");
			return NULL;
		}
		if (!am->ProcessAnimation(Scene)) {
			delete am;
			printf("error loading animation data\n");
			return NULL;
		}
	}
	else {
		printf("no animation found \n");
	}
	aiReleaseImport(Scene);
	return am;
};


RGP_ANIMATOR::AnimatedModel::AnimatedModel() : Dynamic(),Model3D(), m_Animations(0), m_NumAnimations(0),
												m_SelectedAnimation(0), m_Skeleton({ 0,0 }), m_Initialized(false),m_Size(0),
												m_BoneRenderingShader(0), m_BonesVBO(0), m_BonesVAO(0),
												m_WeighMappingShader(0),m_WeighMappingModeVAO(0),m_WeighMappingVBO(0),
												m_Mode(0), m_Cursor(0.0),m_Status(STOP)
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
			if (m_Skeleton.Parts[i].Weighs) {
				free(m_Skeleton.Parts[i].Weighs);
			}
			if (m_Skeleton.Parts[i].ChildrenIndices) {
				free(m_Skeleton.Parts[i].ChildrenIndices);
			}
		}
		free(m_Skeleton.Parts);
		m_Skeleton.Parts = NULL;
		m_Skeleton.NumParts = 0;

		m_Cursor = 0.0;
		m_Status = STOP;

	}
	if (m_GLRenderer) {
		if (m_BoneRenderingShader) {
			m_GLRenderer->DeleteGLProgram(m_BoneRenderingShader);
			m_BoneRenderingShader = 0;
		}
		if (m_WeighMappingShader) {
			m_GLRenderer->DeleteGLProgram(m_WeighMappingShader);
			m_WeighMappingShader = 0;
		}
		if (m_BonesVBO) {
			m_GLRenderer->DeleteBuffers(1, &(m_BonesVBO));
			m_BonesVBO = 0;
		}
		if (m_BonesVAO) {
			m_GLRenderer->DeleteVertexArrays(1, &(m_BonesVAO));
			m_BonesVAO = 0;
		}
		if (m_WeighMappingModeVAO) {
			m_GLRenderer->DeleteVertexArrays(1, &(m_WeighMappingModeVAO));
			m_WeighMappingModeVAO = 0;
		}
		if (m_WeighMappingVBO) {
			m_GLRenderer->DeleteBuffers(1, &(m_WeighMappingVBO));
			m_WeighMappingVBO = 0;
		}
	}
	m_Initialized = false;
	Model3D::Destroy();
};

static _u32b meshindex = 0;

_bool		RGP_ANIMATOR::AnimatedModel::ProcessBones(aiNode* node,const aiScene* scene)
{
	_bool ret = true;
	aiBone* ptrBone = NULL;
	_u32b	numBones = 0;
	Bone bone;
	bone.Name=NULL;
	bone.MeshID= meshindex;
	bone.NumIndices=0;
	bone.Indices=0;// index of each affected vertex
	bone.Weighs=0;
	bone.ParentIndex=0;
	bone.NumChildren=0;
	bone.ChildrenIndices=0;
	bone.LocalPostion = { 0.0f,0.0f,0.0f };
	bone.LocalDirection = { 0.0f,0.0f,0.0f };
	bone.LocalUp = { 0.0f,0.0f,0.0f };
	//copy node content
	for (_u16b i = 0; i < node->mNumMeshes; ++i) {
		numBones = scene->mMeshes[node->mMeshes[i]]->mNumBones;
		for (_u32b b = 0; i <numBones; ++b) {
			ptrBone = scene->mMeshes[node->mMeshes[i]]->mBones[b];
			bone.Name = CreateStringCopy(ptrBone->mName.C_Str());
			bone.MeshID = meshindex;
			bone.NumIndices = ptrBone->mNumWeights;
			bone.Indices =(_u32b*)malloc(bone.NumIndices*sizeof(_u32b));
			if (!bone.Indices) {
				free(bone.Name);
				return false;
			}
			bone.Weighs = (_float*)malloc(bone.NumIndices * sizeof(_float));
			if (!bone.Weighs) {
				free(bone.Name);
				free(bone.Indices);
				return false;
			}
			for (_u32b j = 0; j < bone.NumIndices; ++j) {
				bone.Indices[j] = ptrBone->mWeights[j].mVertexId;
				bone.Weighs[j] = ptrBone->mWeights[j].mWeight;
			}
			bone.LocalPostion = { ptrBone->mOffsetMatrix.a4 ,ptrBone->mOffsetMatrix.b4,ptrBone->mOffsetMatrix.c4 };
			bone.LocalDirection = { ptrBone->mOffsetMatrix.a3 ,ptrBone->mOffsetMatrix.b3,ptrBone->mOffsetMatrix.c3 };
			bone.LocalUp = { ptrBone->mOffsetMatrix.a2 ,ptrBone->mOffsetMatrix.b2,ptrBone->mOffsetMatrix.c2 };
			if(!AddBone(bone)){
				free(bone.Name);
				free(bone.Indices);
				free(bone.Weighs);
				return false;
			}
		}
		meshindex++;
	}
	//process children nodes
	for (_u16b i = 0; i< node->mNumChildren && ret ; i++) {
		ret*=ProcessNode(node->mChildren[i], scene);
	}

	meshindex = 0;
	return ret;
};
_bool		RGP_ANIMATOR::AnimatedModel::ProcessAnimation(const aiScene* Scene)
{
	Key  key;
	Animation* ani;
	_double cursor;
	Key newkey;
	Key*  tmp;
	_u32b p = 0, r = 0, s = 0;
	for (_u32b a = 0; a < Scene->mNumAnimations; ++a) {
		//creating animation container
		ani=this->AddAnimation((_s8b*)(Scene->mAnimations[a]->mName.C_Str()));
		if (!ani)
			return false;
		ani->numChannels = 0;
		ani->channels = NULL;
		ani->channels = (Channel*)malloc(Scene->mAnimations[a]->mNumChannels * sizeof(Channel));
		if (!ani->channels)
			return false;
		ani->numChannels = Scene->mAnimations[a]->mNumChannels;
		for (_u32b c = 0; c < ani->numChannels; ++c) {
			ani->channels[c].boneName = NULL;
			ani->channels[c].Keys = NULL;
			ani->channels[c].numKeys = 0;
		}
		for (_u32b c = 0; c < ani->numChannels; ++c) {
			ani->channels[c].boneName = CreateStringCopy(Scene->mAnimations[a]->mChannels[c]->mNodeName.C_Str());
			cursor = 1000.0f;
		
			for (p = 0, r = 0, s = 0;
					p < Scene->mAnimations[a]->mChannels[c]->mNumPositionKeys ||
					r < Scene->mAnimations[a]->mChannels[c]->mNumRotationKeys ||
					s < Scene->mAnimations[a]->mChannels[c]->mNumScalingKeys;
				) {
				//determine which is the time for the next key
				if (p < Scene->mAnimations[a]->mChannels[c]->mNumPositionKeys) {
					cursor = MIN(Scene->mAnimations[a]->mChannels[c]->mPositionKeys[p].mTime,cursor);
				}
				if (r < Scene->mAnimations[a]->mChannels[c]->mNumRotationKeys) {
					cursor = MIN(Scene->mAnimations[a]->mChannels[c]->mRotationKeys[r].mTime, cursor);
				}
				if (s < Scene->mAnimations[a]->mChannels[c]->mNumScalingKeys) {
					cursor = MIN(Scene->mAnimations[a]->mChannels[c]->mScalingKeys[s].mTime, cursor);
				}
		
				//Create a new key
				newkey.Instance = cursor;
				newkey.Position = { 0.0f,0.0f,0.0f };
				newkey.Rotation = { 0.0f,0.0f,0.0f };
				newkey.Scaling  = { 1.0f,1.0f,1.0f };
				//////
				if (p < Scene->mAnimations[a]->mChannels[c]->mNumPositionKeys) {
					if (Scene->mAnimations[a]->mChannels[c]->mPositionKeys[p].mTime == cursor) {
						//copy Values
						newkey.Position.x = Scene->mAnimations[a]->mChannels[c]->mPositionKeys[p].mValue.x;
						newkey.Position.y = Scene->mAnimations[a]->mChannels[c]->mPositionKeys[p].mValue.y;
						newkey.Position.z = Scene->mAnimations[a]->mChannels[c]->mPositionKeys[p].mValue.z;
						//increment
						++p;
					}
				}
				if (r < Scene->mAnimations[a]->mChannels[c]->mNumRotationKeys) {
					if (Scene->mAnimations[a]->mChannels[c]->mRotationKeys[r].mTime == cursor) {
						//copy Values
						newkey.Rotation.x = Scene->mAnimations[a]->mChannels[c]->mRotationKeys[r].mValue.x;
						newkey.Rotation.y = Scene->mAnimations[a]->mChannels[c]->mRotationKeys[r].mValue.y;
						newkey.Rotation.z = Scene->mAnimations[a]->mChannels[c]->mRotationKeys[r].mValue.z;
						//increment
						++r;
					}
				}
				if (s < Scene->mAnimations[a]->mChannels[c]->mNumScalingKeys) {
					if (Scene->mAnimations[a]->mChannels[c]->mScalingKeys[s].mTime == cursor) {
						//copy Values
						newkey.Position.x = Scene->mAnimations[a]->mChannels[c]->mScalingKeys[s].mValue.x;
						newkey.Position.y = Scene->mAnimations[a]->mChannels[c]->mScalingKeys[s].mValue.y;
						newkey.Position.z = Scene->mAnimations[a]->mChannels[c]->mScalingKeys[s].mValue.z;
						//increment
						++s;
					}
				}
				//add the key 
				//Dynamic allocation and copy old keys content 
				tmp = (Key*)malloc((ani->channels[c].numKeys + 1) * sizeof(Key));
				if (!tmp)
					return false;
				for (_u32b i = 0; i < ani->channels[c].numKeys; ++i) {
					tmp[i] = ani->channels[c].Keys[i];
				}
				free(ani->channels[c].Keys);
				ani->channels[c].numKeys++;
				ani->channels[c].Keys = tmp;
				
			}
		}
	}
	return true;
};



void	RGP_ANIMATOR::AnimatedModel::Update(_float dt)
{

	//the behaviour of the object depends on its status( PLAYING ANIMATION, PAUSED or STOPPED)

};

void		RGP_ANIMATOR::AnimatedModel::Play()
{
};
void		RGP_ANIMATOR::AnimatedModel::Pause()
{
};
void		RGP_ANIMATOR::AnimatedModel::Stop()
{
};

void		RGP_ANIMATOR::AnimatedModel::Render(Camera* selected)
{
	if (m_Mode == 0)
		Model3D::Render(selected);
	else
		RenderWeighMap(selected);
	//RenderBones(selected);
	
};
void		RGP_ANIMATOR::AnimatedModel::RenderBones(Camera * selected) 
{
	if (m_GLRenderer) {
		m_GLRenderer->SetShaderProgram(m_BoneRenderingShader);
		m_GLRenderer->GetUniformLocation(m_BoneRenderingShader, "Radius");
		m_GLRenderer->BindVertexArray(m_BonesVAO);
		m_GLRenderer->DrawArrays(GL_POINTS, 0, m_Skeleton.NumParts);
	}
};
void		RGP_ANIMATOR::AnimatedModel::RenderWeighMap(Camera* selected)
{

};
_bool		RGP_ANIMATOR::AnimatedModel::Init(GLRenderer* renderer, _s8b* modelfilename)
{
	//defining renderer
	if (!(renderer && renderer->isInitialized()))
		return false;
	this->setRenderer(renderer);
	//Init bone related objects
	if (!(m_BoneRenderingShader = m_GLRenderer->CreateGLProgramFromFile("..//Animator//shaders//bone.vs",
		"..//Animator//shaders//bone.gs",
		"..//Animator//shaders//bone.fs")))
		return false;
	m_GLRenderer->GenVertexArrays(1, &(m_BonesVAO));
	m_GLRenderer->BindVertexArray(m_BonesVAO);
	m_GLRenderer->GenBuffers(1,&(m_BonesVBO));
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_BonesVBO);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, MAX_NUM_BONES * 3 * sizeof(_float), NULL, GL_DYNAMIC_DRAW);
	m_GLRenderer->SetVertexAttribPointer(0, 3, 0, 0);
	m_GLRenderer->EnableVertexAttribArray(0);

	m_GLRenderer->BindVertexArray(m_BonesVAO);



	if(modelfilename)
		if (!LoadModelFromFile(modelfilename,false))
			return false;

	return true;
};


_bool	RGP_ANIMATOR::AnimatedModel::AddBone(Bone bone)
{
	if (!(bone.Name))
		return false;
	if (getBonebyName(bone.Name))
		return false;
	Bone* tmp = (Bone*)malloc((m_Skeleton.NumParts+1) * sizeof(Bone));
	if (!tmp)
		return false;
	for (_u32b i = 0; i < m_Skeleton.NumParts; ++i)
		tmp[i] = m_Skeleton.Parts[i];
	if (m_Skeleton.Parts)
		free(m_Skeleton.Parts);
	m_Skeleton.Parts = tmp;
	m_Skeleton.Parts[m_Skeleton.NumParts].ChildrenIndices = bone.ChildrenIndices;
	m_Skeleton.Parts[m_Skeleton.NumParts].Weighs = bone.Weighs;
	m_Skeleton.Parts[m_Skeleton.NumParts].Indices = bone.Indices;
	m_Skeleton.Parts[m_Skeleton.NumParts].LocalDirection = bone.LocalDirection;
	m_Skeleton.Parts[m_Skeleton.NumParts].LocalPostion = bone.LocalPostion;
	m_Skeleton.Parts[m_Skeleton.NumParts].LocalUp = bone.LocalUp;
	m_Skeleton.Parts[m_Skeleton.NumParts].MeshID = bone.MeshID;
	m_Skeleton.Parts[m_Skeleton.NumParts].Name = bone.Name;
	m_Skeleton.Parts[m_Skeleton.NumParts].NumChildren = bone.NumChildren;
	m_Skeleton.Parts[m_Skeleton.NumParts].NumIndices = bone.NumIndices;
	m_Skeleton.Parts[m_Skeleton.NumParts].ParentIndex = bone.ParentIndex;
	m_Skeleton.NumParts++;

	// updating VBO ;
	_float vertices[MAX_NUM_BONES * 3];
	for (_u32b i = 0; i < m_Skeleton.NumParts; ++i) {
		vertices[i * 3  ] = m_Skeleton.Parts[i].LocalPostion.x;
		vertices[i * 3+1] = m_Skeleton.Parts[i].LocalPostion.y;
		vertices[i * 3+2] = m_Skeleton.Parts[i].LocalPostion.z;
	}
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_BonesVBO);
	m_GLRenderer->setBufferSubData(GL_ARRAY_BUFFER, 0, m_Skeleton.NumParts * 3 * sizeof(_float), vertices);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
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
			if (m_Animations[i].channels) {
				for (_u32b j = 0; j < m_Animations[i].numChannels; ++j) {
					if (m_Animations[i].channels[j].Keys) {
						free(m_Animations[i].channels[j].Keys);
					}
					if (m_Animations[i].channels[j].boneName){
						free(m_Animations[i].channels[j].boneName);
					}
				}
				free(m_Animations[i].channels);

			}
		}
		free(m_Animations);
		m_Animations = NULL;
		m_NumAnimations = 0;
		m_SelectedAnimation = 0;
		m_Size = 0;
	}
};