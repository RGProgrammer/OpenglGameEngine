#include ".//Model3D.h"

RGP_CORE::BaseActor*	RGP_CORE::Model3D::Create(void ** args) 
{

	Model3D* model = new Model3D();
	if (!model)
		return NULL;
	model->setRenderer((GLRenderer*)args[0]);
	if (!model->LoadModelFromFile((_s8b*)args[1])) {
		delete model;
		return NULL;
	}
	return model;
}

RGP_CORE::Model3D::Model3D(const _s8b* name):Model3D({0.0f,0.0f,0.0f},name){
};
RGP_CORE::Model3D::Model3D(Vertex3d Pos, const _s8b* name): Model3D(Pos,{0.0f,0.0f,-1.0f},{0.0f,1.0f,0.0f},name){
};
RGP_CORE::Model3D::Model3D(Vertex3d Pos, Vertex3d Dir, Vertex3d Up, const _s8b* name): Renderable(Pos,Dir, Up,name),
															m_NumMeshes(0), m_Meshes(NULL),
															m_Buffer { {0,0,0,0,0,0},0,0,0,0 },
															m_ReflectionProbe(NULL),
															m_Materials(NULL),m_NumMaterials(0),
															m_FileDirectory(NULL),m_ShaderProgram(0), 
															m_MaterialsWrappers(NULL),m_MaterialUBO(0),
															m_RenderingVAO(0),m_RenderingCommandsBuffer(0),
															m_ClearAfterLoad(true), m_WorldMtxLocation(-1)
{
	m_DoesCastShadow = true;

};
RGP_CORE::Model3D::~Model3D(){
	this->Destroy();
};


void RGP_CORE::Model3D::Destroy(){
	
	this->ClearModelLoadedData();
	this->DestroyBuffers();
	
	m_NumMeshes = 0;
	if (m_ShaderProgram){

        m_GLRenderer->DeleteGLProgram(m_ShaderProgram);
        m_ShaderProgram=0;
	}
	if(m_FileDirectory){
        free(m_FileDirectory);
        m_FileDirectory=NULL;
	}
	Renderable::Destroy();
};
void RGP_CORE::Model3D::DestroyBuffers()
{
	
	m_GLRenderer->DeleteBuffers(5, m_Buffer.Wrappers);
	if (m_Buffer.VertexArrayObject) {
		m_GLRenderer->DeleteVertexArrays(1, &(m_Buffer.VertexArrayObject));
	}
	m_Buffer = { {0,0,0,0,0,0},0,0,0,0 };
	
};
void RGP_CORE::Model3D::ClearModelLoadedData()
{
	if (m_Materials) {
		for (_u32b i = 0; i< m_NumMaterials; i++) {
			if (m_Materials[i].DiffuseMap) {
				if (m_Materials[i].DiffuseMap->Pixels) {
					free(m_Materials[i].DiffuseMap->Pixels);
					m_Materials[i].DiffuseMap->Pixels = NULL;
				}
				free(m_Materials[i].DiffuseMap);
				m_Materials[i].DiffuseMap = NULL;
			}
			if (m_Materials[i].SpecularMap) {
				if (m_Materials[i].SpecularMap->Pixels) {
					free(m_Materials[i].SpecularMap->Pixels);
					m_Materials[i].SpecularMap->Pixels = NULL;
				}
				free(m_Materials[i].SpecularMap);
				m_Materials[i].SpecularMap = NULL;
			}
			if (m_Materials[i].NormalsMap) {
				if (m_Materials[i].NormalsMap->Pixels) {
					free(m_Materials[i].NormalsMap->Pixels);
					m_Materials[i].NormalsMap->Pixels = NULL;
				}
				free(m_Materials[i].NormalsMap);
				m_Materials[i].NormalsMap = NULL;
			}
			
		}
		free(m_Materials);
		m_Materials = NULL;
	}
	if (m_Meshes) {
		for (_u32b i = 0; i< m_NumMeshes; i++) {
			if (m_Meshes[i].Name) {
				free(m_Meshes[i].Name);
				m_Meshes[i].Name = NULL;
			}
			if (m_Meshes[i].VertexBuffer) {
				free(m_Meshes[i].VertexBuffer = NULL);
				m_Meshes[i].VertexBuffer = NULL;
			}
			if (m_Meshes[i].TangentBuffer) {
				free(m_Meshes[i].TangentBuffer);
				free(m_Meshes[i].BitangentBuffer);
				m_Meshes[i].TangentBuffer = NULL;
				m_Meshes[i].BitangentBuffer = NULL;
			}
			if (m_Meshes[i].NormalBuffer) {
				free(m_Meshes[i].NormalBuffer);
				m_Meshes[i].NormalBuffer = NULL;
			}
			if (m_Meshes[i].IndexBuffer) {
				free(m_Meshes[i].IndexBuffer);
				m_Meshes[i].IndexBuffer = NULL;
			}
			if (m_Meshes[i].TexCoords) {
				free(m_Meshes[i].TexCoords);
				m_Meshes[i].TexCoords = NULL;
			}
		}
		free(m_Meshes);
		m_Meshes = NULL;
	}

};


_s16b RGP_CORE::Model3D::LoadModelFromFile(char* filename, _bool ClearDataAfterLoad ){
	if(!m_GLRenderer)
        return 0 ;
	const aiScene* Scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality |aiProcess_FlipUVs );
	//if failed
	if(!Scene){
        printf("error loading file\n");
		return 0 ;
	}
  	if(Scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || Scene->mRootNode==NULL){
        printf("error laoding scene\n");
        aiReleaseImport(Scene);
        return 0 ;
  	}
  	//else succes
  	///init m_FileDirectory value
  	m_FileDirectory=ExtractDirectory(filename);
  	//some other code here(copying useful contents)
    if(!LoadMaterial(Scene)){
        printf("Could not load material \n");
        aiReleaseImport(Scene);
        return 0 ;
    }
  	 if(!ProcessNode(Scene->mRootNode,Scene)){
        printf("Could not load Meshes \n");
        aiReleaseImport(Scene);
        return 0 ;
  	 }
  	// We're done. Release all resources associated with this import
  	aiReleaseImport(Scene);
  	if(!GenerateVerticesBuffers()){
        printf("could not generate vertices buffers \n");
        return 0;
	}
	else
		FillBuffers();
	this->GenerateCommandBuffer();

	if (!LoadShaderProg("..//Shaders//Model_Deferred.vs", "..//Shaders//Model_Deferred.fs")){
		printf("error loading shader program\n");
		return 0;
	}
	m_WorldMtxLocation = m_GLRenderer->GetUniformLocation(m_ShaderProgram, "WorldMtx");
	if (!InitVAOs())
		return 0;
	if (ClearDataAfterLoad)
		ClearModelLoadedData();
    return 1;
};



_s16b   RGP_CORE::Model3D::LoadShaderProg(char* VS_File,char* FS_File){
    ///Code should be changed here
    if(!m_GLRenderer)
        return 0 ;
	if (m_ShaderProgram) {
		m_GLRenderer->DeleteGLProgram(m_ShaderProgram);
	}
	m_ShaderProgram=m_GLRenderer->CreateGLProgramFromFile(VS_File,FS_File);
    if(!m_ShaderProgram)
        return 0 ;
    return 1 ;
};

_s16b   RGP_CORE::Model3D::InitVAOs(){
    if(!m_ShaderProgram)
        return 0;
   
	//TODO
	m_GLRenderer->GenVertexArrays(1, &m_Buffer.VertexArrayObject);
	m_GLRenderer->BindVertexArray(m_Buffer.VertexArrayObject);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[0]);
	m_GLRenderer->SetVertexAttribPointer(0, 3);
	m_GLRenderer->EnableVertexAttribArray(0);

	m_GLRenderer->BindVertexArray(m_Buffer.VertexArrayObject);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[1]);
	m_GLRenderer->SetVertexAttribPointer(1, 3);
	m_GLRenderer->EnableVertexAttribArray(1);

	m_GLRenderer->BindVertexArray(m_Buffer.VertexArrayObject);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[2]);
	m_GLRenderer->SetVertexAttribPointer(2, 2);
	m_GLRenderer->EnableVertexAttribArray(2);

	m_GLRenderer->BindVertexArray(m_Buffer.VertexArrayObject);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[3]);
	m_GLRenderer->SetVertexAttribPointer(3, 3);
	m_GLRenderer->EnableVertexAttribArray(3);

	m_GLRenderer->BindVertexArray(m_Buffer.VertexArrayObject);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[4]);
	m_GLRenderer->SetVertexAttribPointer(4, 3);
	m_GLRenderer->EnableVertexAttribArray(4);

	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.Wrappers[5]);
	m_GLRenderer->BindBuffer(GL_DRAW_INDIRECT_BUFFER, m_RenderingCommandsBuffer);

	m_GLRenderer->BindVertexArray(0);

	return 1 ;
};




void RGP_CORE::Model3D::Render(Camera* Selected){
    if(isVisible() && Selected && m_GLRenderer && m_GLRenderer->isInitialized() ){
        ///new rendering code here using the high level opengl interface(m_GLRenderer)
        //set the program to use
		if (m_GLRenderer->getCurrentShaderProgram() != m_ShaderProgram) {

			m_GLRenderer->SetShaderProgram(m_ShaderProgram);
			m_GLRenderer->BindBufferBase(GL_UNIFORM_BUFFER, 1, m_GLRenderer->getCameratransformsUBO());
			m_GLRenderer->BindBufferBase(GL_UNIFORM_BUFFER, 2, m_MaterialUBO);
		}
		m_GLRenderer->SetUniformvMtx(m_WorldMtxLocation, this->getTransMtx());
		//TODO :providing attached reflection probe to the shader program
		
		
		m_GLRenderer->BindVertexArray(m_Buffer.VertexArrayObject);
		m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.Wrappers[5]);
		m_GLRenderer->BindBuffer(GL_DRAW_INDIRECT_BUFFER,m_RenderingCommandsBuffer);//Bind MultiDraw Indirect Buffer
		//Draw Command
		m_GLRenderer->MultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, m_NumMeshes);
		m_GLRenderer->BindVertexArray(0);
		m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_GLRenderer->BindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	}
	
};
void	RGP_CORE::Model3D::CastShadow()
{
	 
	if (!m_DoesCastShadow || !m_GLRenderer){
		return;
	}
	
	m_GLRenderer->BindVertexArray(m_Buffer.VertexArrayObject);
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.Wrappers[5]);
	m_GLRenderer->BindBuffer(GL_DRAW_INDIRECT_BUFFER, m_RenderingCommandsBuffer);//Bind MultiDraw Indirect Buffer
	//Draw Command
	m_GLRenderer->MultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, m_NumMeshes);
	m_GLRenderer->BindVertexArray(0);
	m_GLRenderer->BindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
};

void RGP_CORE::Model3D::setReflectionProbe(EnvMapProbe* Probe)
{
	m_ReflectionProbe = Probe;
};
RGP_CORE::EnvMapProbe*	RGP_CORE::Model3D::getReflectionProbe(EnvMapProbe* Probe)
{
	return m_ReflectionProbe;
};

const RGP_CORE::pMesh		RGP_CORE::Model3D::getMesh(_u32b index)
{
	if (!m_Meshes) {
		printf("not initialized\n");
		return NULL;
	}
	if (index >= 0 && index < m_NumMeshes)
		return &(m_Meshes[index]);
	else {
		printf("out of range \n");
		return NULL;
	}
};
_u32b			RGP_CORE::Model3D::getNumMeshes()
{
	return m_NumMeshes;
};


_u16b RGP_CORE::Model3D::ProcessNode(aiNode* Node,const aiScene* Scene){
    /// process current Node
   
	for(_u16b i=0;i<Node->mNumMeshes ;i++){
		AddMesh(Scene->mMeshes[Node->mMeshes[i]]->mName.C_Str(),Scene->mMeshes[Node->mMeshes[i]]->mMaterialIndex);
		CopyVertices(Scene->mMeshes[Node->mMeshes[i]]->mVertices,Scene->mMeshes[Node->mMeshes[i]]->mNumVertices);
		CopyNormals(Scene->mMeshes[Node->mMeshes[i]]->mNormals,Scene->mMeshes[Node->mMeshes[i]]->mNumVertices);
		CopyTangents(Scene->mMeshes[Node->mMeshes[i]]->mTangents,Scene->mMeshes[Node->mMeshes[i]]->mBitangents,
		Scene->mMeshes[Node->mMeshes[i]]->mNumVertices);
		CopyFaces(Scene->mMeshes[Node->mMeshes[i]]->mFaces,Scene->mMeshes[Node->mMeshes[i]]->mNumFaces );
		CopyTextureCoords(Scene->mMeshes[Node->mMeshes[i]]->mTextureCoords,Scene->mMeshes[Node->mMeshes[i]]->mNumVertices);
	}
    ///process ChildNodes
   for(_u16b i=0; i< Node->mNumChildren;i++ ){
        ProcessNode(Node->mChildren[i],Scene);
   }
    return 1;
};

_u16b RGP_CORE::Model3D::AddMesh(const char* Name, _u16b MaterialID){
    ///add a mesh structure to the mesh vector
    ///initilize attibutes to 0 or NULL and init Material index
    pMesh tmp=(pMesh)malloc((m_NumMeshes+1)*sizeof(Mesh));
    if(!tmp)
        return 0;
    for(_u32b i=0; i<m_NumMeshes;i++)
        tmp[i]=m_Meshes[i];
    free(m_Meshes);
    m_Meshes=tmp;
    m_NumMeshes++;
	if (Name){
		
		m_Meshes[m_NumMeshes - 1].Name = CreateStringCopy(Name);
	}
	else
		m_Meshes[m_NumMeshes - 1].Name = NULL;

    m_Meshes[m_NumMeshes-1].IndexBuffer=NULL;
    m_Meshes[m_NumMeshes-1].nbFaces=0;
    m_Meshes[m_NumMeshes-1].VertexBuffer=NULL ;
    m_Meshes[m_NumMeshes-1].nbVertices=0;
    m_Meshes[m_NumMeshes-1].NormalBuffer=NULL;
    m_Meshes[m_NumMeshes-1].TangentBuffer=NULL;
    m_Meshes[m_NumMeshes-1].BitangentBuffer=NULL;
	m_Meshes[m_NumMeshes - 1].TexCoords = NULL;
    m_Meshes[m_NumMeshes-1].AppliedMaterial=MaterialID ;
	return 1;
};
_u16b RGP_CORE::Model3D::CopyVertices(const aiVector3D*   buffer,_u32b nbVertices){
    ///copy vertexbuffer to the last mesh
    if(buffer && nbVertices> 0){
        m_Meshes[m_NumMeshes-1].VertexBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        if(!(m_Meshes[m_NumMeshes-1].VertexBuffer))
            return 0;
        m_Meshes[m_NumMeshes-1].nbVertices=nbVertices ;
        for(_u32b i=0 ; i<nbVertices;i++){
            m_Meshes[m_NumMeshes-1].VertexBuffer[i*3  ]=buffer[i].x;
            m_Meshes[m_NumMeshes-1].VertexBuffer[i*3+1]=buffer[i].y;
            m_Meshes[m_NumMeshes-1].VertexBuffer[i*3+2]=buffer[i].z;
        }
        return 1 ;
    }
    return 0 ;

};
_u16b RGP_CORE::Model3D::CopyNormals(const aiVector3D*   buffer,_u32b nbVertices){
    ///copy Normalbuffer to the last mesh
    if(buffer && nbVertices> 0){
        m_Meshes[m_NumMeshes-1].NormalBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        if(!(m_Meshes[m_NumMeshes-1].NormalBuffer))
            return 0;
        for(_u32b i=0 ; i<nbVertices;i++){
            m_Meshes[m_NumMeshes-1].NormalBuffer[i*3  ]=buffer[i].x;
            m_Meshes[m_NumMeshes-1].NormalBuffer[i*3+1]=buffer[i].y;
            m_Meshes[m_NumMeshes-1].NormalBuffer[i*3+2]=buffer[i].z;
        }
        return 1 ;
    }
    return 0 ;

};
_u16b RGP_CORE::Model3D::CopyTangents(const aiVector3D*   Tbuffer,const aiVector3D*   Bibuffer,_u32b nbVertices){
    if(Tbuffer && Bibuffer){
        m_Meshes[m_NumMeshes-1].TangentBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        m_Meshes[m_NumMeshes-1].BitangentBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        if(!(m_Meshes[m_NumMeshes-1].TangentBuffer && m_Meshes[m_NumMeshes-1].BitangentBuffer))
            return 0;
        for(_u32b i=0 ; i<nbVertices;i++){
            m_Meshes[m_NumMeshes-1].TangentBuffer[i*3  ]=Tbuffer[i].x;
            m_Meshes[m_NumMeshes-1].TangentBuffer[i*3+1]=Tbuffer[i].y;
            m_Meshes[m_NumMeshes-1].TangentBuffer[i*3+2]=Tbuffer[i].z;

            m_Meshes[m_NumMeshes-1].BitangentBuffer[i*3  ]=Bibuffer[i].x;
            m_Meshes[m_NumMeshes-1].BitangentBuffer[i*3+1]=Bibuffer[i].y;
            m_Meshes[m_NumMeshes-1].BitangentBuffer[i*3+2]=Bibuffer[i].z;
        }
        return 1 ;
    }
    return 0 ;
};
_u16b RGP_CORE::Model3D::CopyFaces(const aiFace* Faces, _u32b nbFaces){
    ///copy Faces
    if(Faces && nbFaces> 0){
        m_Meshes[m_NumMeshes-1].IndexBuffer=(_u32b*)malloc(nbFaces*3*sizeof(_u32b));
        if(!(m_Meshes[m_NumMeshes-1].IndexBuffer))
            return 0;
        m_Meshes[m_NumMeshes-1].nbFaces=nbFaces ;
        for(_u32b i=0 ; i<nbFaces;i++){
            m_Meshes[m_NumMeshes-1].IndexBuffer[i*3  ]=Faces[i].mIndices[0];
            m_Meshes[m_NumMeshes-1].IndexBuffer[i*3+1]=Faces[i].mIndices[1];
            m_Meshes[m_NumMeshes-1].IndexBuffer[i*3+2]=Faces[i].mIndices[2];
        }
        return 1 ;
    }
    return 0 ;
};
_u16b RGP_CORE::Model3D::CopyTextureCoords(aiVector3D** TexCoords, _u32b nbTexCoords){
    ///copy tecture coords
    if(TexCoords && nbTexCoords> 0){
        if(!TexCoords[0]){
            m_Meshes[m_NumMeshes-1].TexCoords=(_float*)malloc(nbTexCoords*2*sizeof(_float));
            if(!(m_Meshes[m_NumMeshes-1].TexCoords))
                return 0;
            for(_u32b i=0 ; i<nbTexCoords;i++){
                m_Meshes[m_NumMeshes-1].TexCoords[i*2  ]= 0.0f;
                m_Meshes[m_NumMeshes-1].TexCoords[i*2+1]= 0.0f;
            }
            return 1 ;
        }else{
            m_Meshes[m_NumMeshes-1].TexCoords =(_float*)malloc(nbTexCoords*2*sizeof(_float));
            if(!(m_Meshes[m_NumMeshes-1].TexCoords))
                return 0;

            for(_u32b i=0 ; i<nbTexCoords;i++){
                m_Meshes[m_NumMeshes-1].TexCoords[i*2  ]= TexCoords[0][i].x;
                m_Meshes[m_NumMeshes-1].TexCoords[i*2+1]= TexCoords[0][i].y;
            }
            return 1 ;
        }
    }
    return 0 ;
};
_u16b RGP_CORE::Model3D::LoadMaterial(const aiScene* Scene){
    if(!LoadMaterialstoMemory(Scene))
        return 0 ;
    if(!GenerateGPUMaterials())
        return 0;
    return 1 ;

};
_u16b RGP_CORE::Model3D::LoadMaterialstoMemory(const aiScene* Scene){
    m_NumMaterials=Scene->mNumMaterials;
    _bool hasDiffusemap=false;
	_bool hasNormalmap = false;
	_bool hasSpecularmap = false;
	aiString materialname;
    this->m_Materials=(Material*)malloc(m_NumMaterials*sizeof(Material));
    if(!m_Materials){
        return 0 ;
    }
    for(_u32b i=0 ; i<m_NumMaterials;++i){
		m_Materials[i].Name = NULL;
        m_Materials[i].NormalsMap=NULL ;
        m_Materials[i].SpecularMap=NULL;
        m_Materials[i].DiffuseMap=NULL;
		aiGetMaterialString(Scene->mMaterials[i],AI_MATKEY_NAME,&materialname);
		aiGetMaterialFloat(Scene->mMaterials[i],AI_MATKEY_REFRACTI,&(m_Materials[i].IOR));
		aiGetMaterialFloat(Scene->mMaterials[i], AI_MATKEY_OPACITY, &(m_Materials[i].Opacity));
        hasDiffusemap=false;
		hasNormalmap = false;
		hasSpecularmap = false;
		m_Materials[i].Name = CreateStringCopy(materialname.C_Str());
        for(_u16b j=0 ;j< Scene->mMaterials[i]->mNumProperties;j++){
			
            if(Scene->mMaterials[i]->mProperties[j]->mSemantic==aiTextureType_DIFFUSE){
                    m_Materials[i].DiffuseMap=(Image*)malloc(sizeof(Image));
                    m_Materials[i].DiffuseMap->Height=0 ;
                    m_Materials[i].DiffuseMap->Width=0;
                    m_Materials[i].DiffuseMap->Pixels=NULL;

                    if(Scene->mMaterials[i]->mProperties[j]->mIndex!=0){
                        ///if the texture is embedded
                        CopyTextureData(Scene->mTextures[Scene->mMaterials[i]->mProperties[j]->mIndex-1],m_Materials[i].DiffuseMap);
                    }else if(Scene->mMaterials[i]->mProperties[j]->mType==aiPTI_String){
                        aiString path ;
                        Scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE,0,&path);
                        char* filename=NULL ;
						if (m_FileDirectory) {
							RGP_CORE::CatStrings(m_FileDirectory, path.C_Str(), &filename);
							m_Materials[i].DiffuseMap= LoadImageFromFile(filename);
							free(filename);
							
                        }else{
                            m_Materials[i].DiffuseMap= LoadImageFromFile(path.C_Str());
                        }
				
                    }
                    hasDiffusemap=true ;
             }else if(Scene->mMaterials[i]->mProperties[j]->mSemantic==aiTextureType_SPECULAR){
                    m_Materials[i].SpecularMap=(Image*)malloc(sizeof(Image));
                    m_Materials[i].SpecularMap->Height=0 ;
                    m_Materials[i].SpecularMap->Width=0;
                    m_Materials[i].SpecularMap->Pixels=NULL;
                    if(Scene->mMaterials[i]->mProperties[j]->mIndex!=0){
                        CopyTextureData(Scene->mTextures[Scene->mMaterials[i]->mProperties[j]->mIndex-1],m_Materials[i].SpecularMap);
                    }else if(Scene->mMaterials[i]->mProperties[j]->mType==aiPTI_String){
                        aiString path ;
                        Scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR,0,&path);
                        char* filename=NULL ;
                        if(m_FileDirectory){
                            RGP_CORE::CatStrings(m_FileDirectory,path.C_Str(),&filename);
                            m_Materials[i].SpecularMap=LoadImageFromFile(filename);
                            free(filename);
                        }else{
                            m_Materials[i].SpecularMap = LoadImageFromFile(path.C_Str());
                        }
                    }
                    hasSpecularmap=true ;
             }else if(Scene->mMaterials[i]->mProperties[j]->mSemantic==aiTextureType_NORMALS){
                    m_Materials[i].NormalsMap=(Image*)malloc(sizeof(Image));
                    m_Materials[i].NormalsMap->Height=0 ;
                    m_Materials[i].NormalsMap->Width=0;
                    m_Materials[i].NormalsMap->Pixels=NULL;
                    if(Scene->mMaterials[i]->mProperties[j]->mIndex!=0){
                        CopyTextureData(Scene->mTextures[Scene->mMaterials[i]->mProperties[j]->mIndex-1],m_Materials[i].NormalsMap);
                    }else if(Scene->mMaterials[i]->mProperties[j]->mType==aiPTI_String){
                        aiString path ;
                        Scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS,0,&path);
                        char* filename=NULL ;
                        if(m_FileDirectory){
                            RGP_CORE::CatStrings(m_FileDirectory,path.C_Str(),&filename);
                            m_Materials[i].NormalsMap = LoadImageFromFile(filename);
                            free(filename);
                        }else
                            m_Materials[i].NormalsMap = LoadImageFromFile(path.C_Str());
                    }
                    hasNormalmap=true ;
                }
        }
        if(!hasDiffusemap){
            m_Materials[i].DiffuseMap=(Image*)malloc(sizeof(Image));
            m_Materials[i].DiffuseMap->Height=1 ;
            m_Materials[i].DiffuseMap->Width=1;
            m_Materials[i].DiffuseMap->Pixels=(_u8b*)malloc(4*sizeof(_u8b));
            m_Materials[i].DiffuseMap->Pixels[0]=128 ;
            m_Materials[i].DiffuseMap->Pixels[1]=128 ;
            m_Materials[i].DiffuseMap->Pixels[2]=128 ;
            m_Materials[i].DiffuseMap->Pixels[3]=0 ;
        }
        if(!hasNormalmap){
            m_Materials[i].NormalsMap=(Image*)malloc(sizeof(Image));
            m_Materials[i].NormalsMap->Height=1 ;
            m_Materials[i].NormalsMap->Width=1;
            m_Materials[i].NormalsMap->Pixels=(_u8b*)malloc(4*sizeof(_u8b));
            m_Materials[i].NormalsMap->Pixels[0]=128 ;
            m_Materials[i].NormalsMap->Pixels[1]=127 ;
            m_Materials[i].NormalsMap->Pixels[2]=255 ;
            m_Materials[i].NormalsMap->Pixels[3]=0 ;
        }
        if(!hasSpecularmap){
            m_Materials[i].SpecularMap=(Image*)malloc(sizeof(Image));
            m_Materials[i].SpecularMap->Height=1 ;
            m_Materials[i].SpecularMap->Width=1;
            m_Materials[i].SpecularMap->Pixels=(_u8b*)malloc(4*sizeof(_u8b));
            m_Materials[i].SpecularMap->Pixels[0]=120 ;
            m_Materials[i].SpecularMap->Pixels[1]=120 ;
            m_Materials[i].SpecularMap->Pixels[2]=120 ;
            m_Materials[i].SpecularMap->Pixels[3]=0 ;
        }
    }
    return 1;
}
_u16b RGP_CORE::Model3D::GenerateGPUMaterials(){
    if(!m_Materials)
        return 0 ;
    this->m_MaterialsWrappers=(_u32b*)malloc(m_NumMaterials*sizeof(_u32b));
    if(!this->m_MaterialsWrappers)
        return 0 ;
    glEnable(GL_TEXTURE_2D);
	for (_u32b i = 0; i < m_NumMaterials; i++) {
		m_MaterialsWrappers[i] = m_GLRenderer->CreateMaterial(m_Materials[i]);
	}
    glDisable(GL_TEXTURE_2D);
    return 1 ;
}
_u16b RGP_CORE::Model3D::GenerateVerticesBuffers()
{

	_u32b numVertices = 0;
	_u32b numFaces = 0;
	for (_u32b i = 0; i < m_NumMeshes; ++i) {
		numVertices += m_Meshes[i].nbVertices;
		numFaces += m_Meshes[i].nbFaces;
	}
	m_Buffer.numFaces = numFaces;
	m_GLRenderer->GenBuffers(6, m_Buffer.Wrappers);
	
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[0]);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(_float), NULL, GL_DYNAMIC_DRAW);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[1]);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(_float), NULL, GL_DYNAMIC_DRAW);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[2]);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(_float), NULL, GL_DYNAMIC_DRAW);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[3]);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(_float), NULL, GL_DYNAMIC_DRAW);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[4]);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(_float), NULL, GL_DYNAMIC_DRAW);
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.Wrappers[5]);
	m_GLRenderer->setBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(_u32b), NULL, GL_DYNAMIC_DRAW);


    return 1;
}
_u16b RGP_CORE::Model3D::GenerateCommandBuffer()
{
	DrawElementsIndirectCommand* drawCommands = NULL;
	_u32b baseVertex = 0;
	_u32b numfaces = 0;
	m_GLRenderer->GenBuffers(1, &m_RenderingCommandsBuffer);
	//Generate Command Buffer and Copy to GPU
	drawCommands = (DrawElementsIndirectCommand*)malloc(m_NumMeshes * sizeof(DrawElementsIndirectCommand));
	if (!drawCommands) {
		printf("error generating rendering commands\n");
		return 0;
	}
	for (_u32b i = 0; i < m_NumMeshes; baseVertex+=m_Meshes[i].nbVertices, numfaces+= m_Meshes[i].nbFaces,++i) {
		drawCommands[i].count = m_Meshes[i].nbFaces * 3;
		drawCommands[i].instanceCount = 1;
		drawCommands[i].firstIndex = numfaces * 3 * sizeof(_u32b);
		drawCommands[i].baseVertex = baseVertex;
		drawCommands[i].baseInstance = m_Meshes[i].AppliedMaterial;

	}
	m_GLRenderer->BindBuffer(GL_DRAW_INDIRECT_BUFFER, m_RenderingCommandsBuffer);
	m_GLRenderer->setBufferData(GL_DRAW_INDIRECT_BUFFER, m_NumMeshes * sizeof(DrawElementsIndirectCommand), drawCommands, GL_STATIC_DRAW);
	return 1;
}
;
_u16b RGP_CORE::Model3D::FillBuffers()
{
	//TODO
	_u32b numVertices = 0;
	_u32b numfaces = 0;
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[0]);
	for (_u32b i = 0 , numVertices = 0; i < m_NumMeshes; ++i) {
		m_GLRenderer->setBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(_float), m_Meshes[i].nbVertices * 3 * sizeof(_float), m_Meshes[i].VertexBuffer);
		numVertices += m_Meshes[i].nbVertices;
	
	}

	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[1]);
	for (_u32b i = 0 , numVertices = 0; i < m_NumMeshes; ++i) {
		m_GLRenderer->setBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(_float), m_Meshes[i].nbVertices * 3 * sizeof(_float), m_Meshes[i].NormalBuffer);
		numVertices += m_Meshes[i].nbVertices;
		
	}

	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[2]);
	for (_u32b i = 0, numVertices = 0; i < m_NumMeshes; ++i) {
		m_GLRenderer->setBufferSubData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(_float), m_Meshes[i].nbVertices * 2 * sizeof(_float), m_Meshes[i].TexCoords);
		numVertices += m_Meshes[i].nbVertices;
		
	}

	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[3]);
	for (_u32b i = 0, numVertices = 0; i < m_NumMeshes; ++i) {
		m_GLRenderer->setBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(_float), m_Meshes[i].nbVertices * 3 * sizeof(_float), m_Meshes[i].TangentBuffer);
		numVertices += m_Meshes[i].nbVertices;
		
	}

	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[4]);
	for (_u32b i = 0, numVertices = 0; i < m_NumMeshes; ++i) {
		m_GLRenderer->setBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(_float), m_Meshes[i].nbVertices * 3 * sizeof(_float), m_Meshes[i].BitangentBuffer);
		numVertices += m_Meshes[i].nbVertices;
		
	}

	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_Buffer.Wrappers[5]);
	for (_u32b i = 0; i < m_NumMeshes; ++i) {
		m_GLRenderer->setBufferSubData(GL_ELEMENT_ARRAY_BUFFER, numfaces * 3 * sizeof(_u32b), m_Meshes[i].nbFaces * 3 * sizeof(_u32b), m_Meshes[i].IndexBuffer);
		numfaces += m_Meshes[i].nbFaces;
	}

	return true;
};
 _u16b RGP_CORE::Model3D::CopyTextureData(aiTexture* Texture, Image* Dest){
    if(!Texture || ! Dest)
        return 0 ;
    Dest->Pixels=(_u8b*)malloc(Texture->mHeight*Texture->mWidth*4);
    if(!Dest->Pixels)
        return 0 ;
    for(_u32b i=0; i <Texture->mHeight*Texture->mWidth;i++){
        Dest->Pixels[i*4  ]= Texture->pcData[i].r;
        Dest->Pixels[i*4+1]= Texture->pcData[i].g;
        Dest->Pixels[i*4+2]= Texture->pcData[i].b;
        Dest->Pixels[i*4+3]= Texture->pcData[i].a;
    }
    Dest->Height=Texture->mHeight;
    Dest->Width=Texture->mWidth;
    return 1 ;
 };


