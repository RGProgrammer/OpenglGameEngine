#include ".//Model3D.h"

RGP_CORE::Model3D::Model3D():Model3D({0.0f,0.0f,0.0f}){
};
RGP_CORE::Model3D::Model3D(Vertex3d Pos): Model3D(Pos,{0.0f,0.0f,-1.0f},{0.0f,1.0f,0.0f}){
};
RGP_CORE::Model3D::Model3D(Vertex3d Pos, Vertex3d Dir, Vertex3d Up): Renderable(Pos,Dir, Up),
															m_nbMeshes(0), v_Meshes(NULL), v_Buffers(NULL),
															m_VAOforShadowcasting(NULL),
															v_Materials(NULL),v_oglMaterials(NULL),m_nbMaterials(0),
															m_FileDirectory(NULL),m_ShaderProgram(0)
{
	m_DoesCastShadow = true;
};
RGP_CORE::Model3D::~Model3D(){
	this->Destroy();
};


void RGP_CORE::Model3D::Destroy(){
	if(v_Materials){
        for(_u32b i=0;i< m_nbMaterials;i++){
            if(v_Materials[i].DiffuseMap){
                if(v_Materials[i].DiffuseMap->Pixels){
                    free(v_Materials[i].DiffuseMap->Pixels);
                    v_Materials[i].DiffuseMap->Pixels=NULL;
                }
                free(v_Materials[i].DiffuseMap);
                v_Materials[i].DiffuseMap=NULL ;
            }
            if(v_Materials[i].SpecularMap){
                if(v_Materials[i].SpecularMap->Pixels){
                    free(v_Materials[i].SpecularMap->Pixels);
                    v_Materials[i].SpecularMap->Pixels=NULL;
                }
                free(v_Materials[i].SpecularMap);
                v_Materials[i].SpecularMap=NULL ;
            }
            if(v_Materials[i].NormalsMap){
                if(v_Materials[i].NormalsMap->Pixels){
                    free(v_Materials[i].NormalsMap->Pixels);
                    v_Materials[i].NormalsMap->Pixels=NULL;
                }
                free(v_Materials[i].NormalsMap);
                v_Materials[i].NormalsMap=NULL ;
            }
        }
        free(v_Materials);
        v_Materials=NULL ;
	}
        if(v_oglMaterials){
         for(_u32b i=0;i< m_nbMaterials;i++){
            if(v_oglMaterials[i].DiffuseMap)
                m_GLRenderer->DeleteTextures(1,&(v_oglMaterials[i].DiffuseMap));
            if(v_oglMaterials[i].SpecularMap)
                m_GLRenderer->DeleteTextures(1,&(v_oglMaterials[i].SpecularMap));
            if(v_oglMaterials[i].NormalsMap)
                m_GLRenderer->DeleteTextures(1,&(v_oglMaterials[i].NormalsMap));
         }
         free(v_oglMaterials);
         v_oglMaterials=NULL ;
         m_nbMaterials=0 ;
    }

	if(v_Buffers){
        for(_u32b i=0; i< m_nbMeshes ;i++){
            if(v_Buffers[i].VertexBuffer){
                m_GLRenderer->DeleteBuffers(1,&(v_Buffers[i].VertexBuffer));
            }
            if(v_Buffers[i].NormalBuffer){
                m_GLRenderer->DeleteBuffers(1,&(v_Buffers[i].NormalBuffer));
            }
            if(v_Buffers[i].TangentBuffer){
                m_GLRenderer->DeleteBuffers(1,&(v_Buffers[i].TangentBuffer));
            }
            if(v_Buffers[i].BitangentBuffer){
                m_GLRenderer->DeleteBuffers(1,&(v_Buffers[i].BitangentBuffer));
            }
            if(v_Buffers[i].IndexBuffer){
                m_GLRenderer->DeleteBuffers(1,&(v_Buffers[i].IndexBuffer));
            }
            if(v_Buffers[i].TexCoords){
                m_GLRenderer->DeleteBuffers(1,&(v_Buffers[i].TexCoords));
            }
            if(v_Buffers[i].VertexArrayObject){
                m_GLRenderer->DeleteVertexArrays(1,&(v_Buffers[i].VertexArrayObject));
            }
        }
        free(v_Buffers);
        v_Buffers=NULL ;
	}
	if (m_VAOforShadowcasting){
		m_GLRenderer->DeleteVertexArrays(m_nbMeshes, m_VAOforShadowcasting);
		free(m_VAOforShadowcasting);
		m_VAOforShadowcasting = NULL;
	}
	if(v_Meshes){
		for(_u32b i=0; i< m_nbMeshes ;i++){
			if(v_Meshes[i].Name){
				free(v_Meshes[i].Name);
				v_Meshes[i].Name=NULL ;
			}
			if(v_Meshes[i].VertexBuffer){
				free(v_Meshes[i].VertexBuffer=NULL);
				v_Meshes[i].VertexBuffer=NULL ;
			}
			if(v_Meshes[i].TangentBuffer){
                free(v_Meshes[i].TangentBuffer);
                free(v_Meshes[i].BitangentBuffer);
                v_Meshes[i].TangentBuffer=NULL ;
                v_Meshes[i].BitangentBuffer=NULL ;
			}
			if(v_Meshes[i].NormalBuffer){
				free(v_Meshes[i].NormalBuffer);
				v_Meshes[i].NormalBuffer=NULL ;
			}
			if(v_Meshes[i].IndexBuffer){
				free(v_Meshes[i].IndexBuffer);
				v_Meshes[i].IndexBuffer=NULL ;
			}
			if(v_Meshes[i].TexCoords){
                free(v_Meshes[i].TexCoords);
				v_Meshes[i].TexCoords=NULL ;
			}
		}
		free(v_Meshes);
		v_Meshes=NULL ;
	}
	m_nbMeshes = 0;
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

_s16b RGP_CORE::Model3D::LoadModelFromFile(char* filename){
	if(!m_GLRenderer)
        return 0 ;
	const aiScene* Scene=aiImportFile(filename,aiProcess_Triangulate |
                                                 aiProcess_GenUVCoords | aiProcess_PreTransformVertices|
                                                 aiProcess_RemoveRedundantMaterials /*| aiProcess_FlipUVs*/ );
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
  	if(!GenerateBuffers()){
        printf("could not generate vertices buffers \n");
        return 0;
    }
	if (!LoadShaderProg("..//Shaders//Model_Deferred.vs", "..//Shaders//Model_Deferred.fs")){
		printf("error loading shader program\n");
		return 0;
	}
    InitVAOs();
    return 1;
};



_s16b   RGP_CORE::Model3D::LoadShaderProg(char* VS_File,char* FS_File){
    ///Code should be changed here
    if(!m_GLRenderer)
        return 0 ;
    m_ShaderProgram=m_GLRenderer->CreateGLProgramFromFile(VS_File,FS_File);
    if(!m_ShaderProgram)
        return 0 ;
    return 1 ;
};

_s16b   RGP_CORE::Model3D::InitVAOs(){
    if(!m_ShaderProgram)
        return 0;
    glGetError();
    for(_u32b i=0 ;i<m_nbMeshes;++i){
        m_GLRenderer->GenVertexArrays(1,&(v_Buffers[i].VertexArrayObject));
        m_GLRenderer->BindVertexArray(v_Buffers[i].VertexArrayObject);

        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,v_Buffers[i].VertexBuffer);
        m_GLRenderer->SetVertexAttribPointer(0,3,0,0);
        m_GLRenderer->EnableVertexAttribArray(0);

        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,v_Buffers[i].NormalBuffer);
        m_GLRenderer->SetVertexAttribPointer(1,3,0,0);
        m_GLRenderer->EnableVertexAttribArray(1);

        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,v_Buffers[i].TexCoords);
        m_GLRenderer->SetVertexAttribPointer(2,2,0,0);
        m_GLRenderer->EnableVertexAttribArray(2);

        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,v_Buffers[i].TangentBuffer);
        m_GLRenderer->SetVertexAttribPointer(3,3,0,0);
        m_GLRenderer->EnableVertexAttribArray(3);

        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,v_Buffers[i].BitangentBuffer);
        m_GLRenderer->SetVertexAttribPointer(4,3,0,0);
        m_GLRenderer->EnableVertexAttribArray(4);

        m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER,v_Buffers[i].IndexBuffer);
    }
	m_GLRenderer->BindVertexArray(0);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, 0);
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_GLRenderer->GenVertexArrays(m_nbMeshes, m_VAOforShadowcasting);
	for (_u32b i = 0; i < m_nbMeshes; ++i){
		m_GLRenderer->BindVertexArray(m_VAOforShadowcasting[i]);
		m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].VertexBuffer);
		m_GLRenderer->SetVertexAttribPointer(0, 3, 0, 0);
		m_GLRenderer->EnableVertexAttribArray(0);
		m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_Buffers[i].IndexBuffer);
	}
	m_GLRenderer->BindVertexArray(0);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, 0);
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return 1 ;
};




void RGP_CORE::Model3D::Render(Camera* Selected){
    if(isVisible() && Selected && m_GLRenderer){
        ///new rendering code here using the high level opengl interface(m_GLRenderer)
        ///set the program to use
        int Location ;
        m_GLRenderer->SetShaderProgram(m_ShaderProgram);
        ///setup uniform variable
        ///Matrices
        m_GLRenderer->SetUniformvMtx(m_GLRenderer->GetUniformLocation(m_ShaderProgram,"WorldMtx"),BaseActor::getTransMtx());
        m_GLRenderer->SetUniformvMtx(m_GLRenderer->GetUniformLocation(m_ShaderProgram,"ViewMtx"),Selected->getViewMtx());
        m_GLRenderer->SetUniformvMtx(m_GLRenderer->GetUniformLocation(m_ShaderProgram,"ProjMtx"),Selected->getProjectionMtx());
        for(_u32b i = 0 ;i<m_nbMeshes;++i){
            ///Texture
            m_GLRenderer->SetActiveTexture(0);
            m_GLRenderer->BindTexture(v_oglMaterials[v_Meshes[i].AppliedMaterial].DiffuseMap);
            Location=m_GLRenderer->GetUniformLocation(m_ShaderProgram,"Diffusemap");
            m_GLRenderer->SetUniformSample(Location,0);

            m_GLRenderer->SetActiveTexture(1);
            m_GLRenderer->BindTexture(v_oglMaterials[v_Meshes[i].AppliedMaterial].NormalsMap);
            Location=m_GLRenderer->GetUniformLocation(m_ShaderProgram,"Normalmap");
            m_GLRenderer->SetUniformSample(Location,1);

            m_GLRenderer->SetActiveTexture(2);
            m_GLRenderer->BindTexture(v_oglMaterials[v_Meshes[i].AppliedMaterial].SpecularMap);
            Location=m_GLRenderer->GetUniformLocation(m_ShaderProgram,"Specularmap");
            m_GLRenderer->SetUniformSample(Location,2);



            ///Bind the VAO
            m_GLRenderer->BindVertexArray(v_Buffers[i].VertexArrayObject);
            m_GLRenderer->DrawElements(GL_TRIANGLES,v_Meshes[i].nbFaces*3,GL_UNSIGNED_INT,(void*)0  );
        }
        ///after rendering
        m_GLRenderer->BindVertexArray(0);
        m_GLRenderer->SetShaderProgram(0);
        m_GLRenderer->SetActiveTexture(GL_TEXTURE0);
        m_GLRenderer->BindTexture(0);
        m_GLRenderer->SetActiveTexture(GL_TEXTURE1);
        m_GLRenderer->BindTexture(0);
        m_GLRenderer->SetActiveTexture(GL_TEXTURE2);
        m_GLRenderer->BindTexture(0);
		m_GLRenderer->SetActiveTexture(GL_NONE);

    }
};
void	RGP_CORE::Model3D::CastShadow()
{
	 
	if (!m_DoesCastShadow || !m_GLRenderer){
		return;
	}
	//the shadow pogram is already loaded and the need FBO is Bound
	//just need to define shader program input paramters(only VAOs)
	for (_u32b i = 0; i < m_nbMeshes; ++i){
		//binding need VAO
		m_GLRenderer->BindVertexArray(m_VAOforShadowcasting[i]);
		//Draw
		m_GLRenderer->DrawElements(GL_TRIANGLES, v_Meshes[i].nbFaces * 3, GL_UNSIGNED_INT, (void*)0);
	}
	m_GLRenderer->BindVertexArray(0);
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
        return ProcessNode(Node->mChildren[i],Scene);
    }
    return 1;
};

_u16b RGP_CORE::Model3D::AddMesh(const char* Name, _u16b MaterialID){
    ///add a mesh structure to the mesh vector
    ///initilize attibutes to 0 or NULL and init Material index
    pMesh tmp=(pMesh)malloc((m_nbMeshes+1)*sizeof(Mesh));
    if(!tmp)
        return 0;
    for(_u32b i=0; i<m_nbMeshes;i++)
        tmp[i]=v_Meshes[i];
    free(v_Meshes);
    v_Meshes=tmp;
    m_nbMeshes++;
	if (Name){
		printf("%s\n", Name);
		v_Meshes[m_nbMeshes - 1].Name = (char*)malloc((strlen(Name)+1)*sizeof(char));
		if (v_Meshes[m_nbMeshes - 1].Name){
			strcpy(v_Meshes[m_nbMeshes - 1].Name, Name);
		}
	}
	else
		v_Meshes[m_nbMeshes - 1].Name = NULL;

    v_Meshes[m_nbMeshes-1].IndexBuffer=NULL;
    v_Meshes[m_nbMeshes-1].nbFaces=0;
    v_Meshes[m_nbMeshes-1].VertexBuffer=NULL ;
    v_Meshes[m_nbMeshes-1].nbVertices=0;
    v_Meshes[m_nbMeshes-1].NormalBuffer=NULL;
    v_Meshes[m_nbMeshes-1].TangentBuffer=NULL;
    v_Meshes[m_nbMeshes-1].BitangentBuffer=NULL;
    v_Meshes[m_nbMeshes-1].nbNormals=0;
    v_Meshes[m_nbMeshes-1].AppliedMaterial=MaterialID ;
	return 1;
};
_u16b RGP_CORE::Model3D::CopyVertices(const aiVector3D*   buffer,_u32b nbVertices){
    ///copy vertexbuffer to the last mesh
    if(buffer && nbVertices> 0){
        v_Meshes[m_nbMeshes-1].VertexBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        if(!(v_Meshes[m_nbMeshes-1].VertexBuffer))
            return 0;
        v_Meshes[m_nbMeshes-1].nbVertices=nbVertices ;
        for(_u32b i=0 ; i<nbVertices;i++){
            v_Meshes[m_nbMeshes-1].VertexBuffer[i*3  ]=buffer[i].x;
            v_Meshes[m_nbMeshes-1].VertexBuffer[i*3+1]=buffer[i].y;
            v_Meshes[m_nbMeshes-1].VertexBuffer[i*3+2]=buffer[i].z;
        }
        return 1 ;
    }
    return 0 ;

};
_u16b RGP_CORE::Model3D::CopyNormals(const aiVector3D*   buffer,_u32b nbVertices){
    ///copy Normalbuffer to the last mesh
    if(buffer && nbVertices> 0){
        v_Meshes[m_nbMeshes-1].NormalBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        if(!(v_Meshes[m_nbMeshes-1].NormalBuffer))
            return 0;
        v_Meshes[m_nbMeshes-1].nbNormals=nbVertices ;
        for(_u32b i=0 ; i<nbVertices;i++){
            v_Meshes[m_nbMeshes-1].NormalBuffer[i*3  ]=buffer[i].x+0.0f;
            v_Meshes[m_nbMeshes-1].NormalBuffer[i*3+1]=buffer[i].y+0.0f;
            v_Meshes[m_nbMeshes-1].NormalBuffer[i*3+2]=buffer[i].z+0.0f ;
        }
        return 1 ;
    }
    return 0 ;

};
_u16b RGP_CORE::Model3D::CopyTangents(const aiVector3D*   Tbuffer,const aiVector3D*   Bibuffer,_u32b nbVertices){
    if(Tbuffer && Bibuffer){
        v_Meshes[m_nbMeshes-1].TangentBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        v_Meshes[m_nbMeshes-1].BitangentBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        if(!(v_Meshes[m_nbMeshes-1].TangentBuffer && v_Meshes[m_nbMeshes-1].BitangentBuffer))
            return 0;
        for(_u32b i=0 ; i<nbVertices;i++){
            v_Meshes[m_nbMeshes-1].TangentBuffer[i*3  ]=Tbuffer[i].x;
            v_Meshes[m_nbMeshes-1].TangentBuffer[i*3+1]=Tbuffer[i].y;
            v_Meshes[m_nbMeshes-1].TangentBuffer[i*3+2]=Tbuffer[i].z;

            v_Meshes[m_nbMeshes-1].BitangentBuffer[i*3  ]=Bibuffer[i].x;
            v_Meshes[m_nbMeshes-1].BitangentBuffer[i*3+1]=Bibuffer[i].y;
            v_Meshes[m_nbMeshes-1].BitangentBuffer[i*3+2]=Bibuffer[i].z;
        }
        return 1 ;
    }
    return 0 ;
};
_u16b RGP_CORE::Model3D::CopyFaces(const aiFace* Faces, _u32b nbFaces){
    ///copy Faces
    if(Faces && nbFaces> 0){
        v_Meshes[m_nbMeshes-1].IndexBuffer=(_u32b*)malloc(nbFaces*3*sizeof(_u32b));
        if(!(v_Meshes[m_nbMeshes-1].IndexBuffer))
            return 0;
        v_Meshes[m_nbMeshes-1].nbFaces=nbFaces ;
        for(_u32b i=0 ; i<nbFaces;i++){
            /*v_Meshes[m_nbMeshes-1].IndexBuffer[i*3  ]=Faces[i].mIndices[0];
            v_Meshes[m_nbMeshes-1].IndexBuffer[i*3+1]=Faces[i].mIndices[1];
            v_Meshes[m_nbMeshes-1].IndexBuffer[i*3+2]=Faces[i].mIndices[2];*/
            v_Meshes[m_nbMeshes-1].IndexBuffer[i*3  ]=Faces[i].mIndices[0];
            v_Meshes[m_nbMeshes-1].IndexBuffer[i*3+1]=Faces[i].mIndices[2];
            v_Meshes[m_nbMeshes-1].IndexBuffer[i*3+2]=Faces[i].mIndices[1];
        }
        return 1 ;
    }
    return 0 ;
};
_u16b RGP_CORE::Model3D::CopyTextureCoords(aiVector3D** TexCoords, _u32b nbTexCoords){
    ///copy tecture coords
    if(TexCoords && nbTexCoords> 0){
        if(!TexCoords[0]){
            v_Meshes[m_nbMeshes-1].TexCoords=(_float*)malloc(nbTexCoords*2*sizeof(_float));
            if(!(v_Meshes[m_nbMeshes-1].TexCoords))
                return 0;
            v_Meshes[m_nbMeshes-1].nbTexCoords=nbTexCoords ;
            for(_u32b i=0 ; i<nbTexCoords;i++){
                v_Meshes[m_nbMeshes-1].TexCoords[i*2  ]= 0.0f;
                v_Meshes[m_nbMeshes-1].TexCoords[i*2+1]= 0.0f;
            }
            return 1 ;
        }else{
            v_Meshes[m_nbMeshes-1].TexCoords=(_float*)malloc(nbTexCoords*2*sizeof(_float));
            if(!(v_Meshes[m_nbMeshes-1].TexCoords))
                return 0;
            v_Meshes[m_nbMeshes-1].nbTexCoords=nbTexCoords ;
            for(_u32b i=0 ; i<nbTexCoords;i++){
                v_Meshes[m_nbMeshes-1].TexCoords[i*2  ]= TexCoords[0][i].x;
                v_Meshes[m_nbMeshes-1].TexCoords[i*2+1]= TexCoords[0][i].y;
            }
            return 1 ;
        }
    }
    return 0 ;
};
_u16b RGP_CORE::Model3D::LoadMaterial(const aiScene* Scene){
    if(!LoadMaterialstoMemory(Scene))
        return 0 ;
    if(!GenerateOGLMaterials())
        return 0;
    return 1 ;

};
_u16b RGP_CORE::Model3D::LoadMaterialstoMemory(const aiScene* Scene){
    m_nbMaterials=Scene->mNumMaterials;
    _bool hasDiffusemap=false;
	_bool hasNormalmap = false;
	_bool hasSpecularmap = false;
    this->v_Materials=(Material*)malloc(m_nbMaterials*sizeof(Material));
    if(!v_Materials){
        return 0 ;
    }
    for(_u32b i=0 ; i<m_nbMaterials;++i){
        v_Materials[i].NormalsMap=NULL ;
        v_Materials[i].SpecularMap=NULL;
        v_Materials[i].DiffuseMap=NULL;
        hasDiffusemap=false;
        for(_u16b j=0 ;j< Scene->mMaterials[i]->mNumProperties;j++){
                if(Scene->mMaterials[i]->mProperties[j]->mSemantic==aiTextureType_DIFFUSE){
                    v_Materials[i].DiffuseMap=(Image*)malloc(sizeof(Image));
                    v_Materials[i].DiffuseMap->Height=0 ;
                    v_Materials[i].DiffuseMap->Width=0;
                    v_Materials[i].DiffuseMap->Pixels=NULL;

                    if(Scene->mMaterials[i]->mProperties[j]->mIndex!=0){
                        ///if the texture is embedded
                        CopyTextureData(Scene->mTextures[Scene->mMaterials[i]->mProperties[j]->mIndex-1],v_Materials[i].DiffuseMap);
                    }else if(Scene->mMaterials[i]->mProperties[j]->mType==aiPTI_String){
                        aiString path ;
                        Scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE,0,&path);
                        char* filename=NULL ;
                        if(m_FileDirectory){
                            RGP_CORE::CatStrings(m_FileDirectory,path.C_Str(),&filename);
                            v_Materials[i].DiffuseMap->Pixels=SOIL_load_image(filename,(int*)(&(v_Materials[i].DiffuseMap->Width)),
                                                                     (int*)(&(v_Materials[i].DiffuseMap->Height)),NULL,SOIL_LOAD_RGBA);
                            free(filename);
                        }else{
                            v_Materials[i].DiffuseMap->Pixels=SOIL_load_image(path.C_Str(),(int*)(&(v_Materials[i].DiffuseMap->Width)),
                                                                     (int*)(&(v_Materials[i].DiffuseMap->Height)),NULL,SOIL_LOAD_RGBA);
                        }
                    }
                    hasDiffusemap=true ;
                }else if(Scene->mMaterials[i]->mProperties[j]->mSemantic==aiTextureType_SPECULAR){
                    v_Materials[i].SpecularMap=(Image*)malloc(sizeof(Image));
                    v_Materials[i].SpecularMap->Height=0 ;
                    v_Materials[i].SpecularMap->Width=0;
                    v_Materials[i].SpecularMap->Pixels=NULL;
                    if(Scene->mMaterials[i]->mProperties[j]->mIndex!=0){
                        CopyTextureData(Scene->mTextures[Scene->mMaterials[i]->mProperties[j]->mIndex-1],v_Materials[i].SpecularMap);
                    }else if(Scene->mMaterials[i]->mProperties[j]->mType==aiPTI_String){
                        aiString path ;
                        Scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR,0,&path);
                        char* filename=NULL ;
                        if(m_FileDirectory){
                            RGP_CORE::CatStrings(m_FileDirectory,path.C_Str(),&filename);
                            v_Materials[i].SpecularMap->Pixels=SOIL_load_image(filename,(int*)(&(v_Materials[i].SpecularMap->Width)),
                                                                     (int*)(&(v_Materials[i].SpecularMap->Height)),NULL,SOIL_LOAD_RGBA);
                            free(filename);
                        }else{
                            v_Materials[i].SpecularMap->Pixels=SOIL_load_image(path.C_Str(),(int*)(&(v_Materials[i].SpecularMap->Width)),
                                                                     (int*)(&(v_Materials[i].SpecularMap->Height)),NULL,SOIL_LOAD_RGBA);
                        }
                    }
                    hasSpecularmap=true ;
                }else if(Scene->mMaterials[i]->mProperties[j]->mSemantic==aiTextureType_NORMALS){
                    v_Materials[i].NormalsMap=(Image*)malloc(sizeof(Image));
                    v_Materials[i].NormalsMap->Height=0 ;
                    v_Materials[i].NormalsMap->Width=0;
                    v_Materials[i].NormalsMap->Pixels=NULL;
                    if(Scene->mMaterials[i]->mProperties[j]->mIndex!=0){
                        CopyTextureData(Scene->mTextures[Scene->mMaterials[i]->mProperties[j]->mIndex-1],v_Materials[i].NormalsMap);
                    }else if(Scene->mMaterials[i]->mProperties[j]->mType==aiPTI_String){
                        aiString path ;
                        Scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS,0,&path);
                        char* filename=NULL ;
                        if(m_FileDirectory){
                            RGP_CORE::CatStrings(m_FileDirectory,path.C_Str(),&filename);
                            v_Materials[i].NormalsMap->Pixels=SOIL_load_image(filename,(int*)(&(v_Materials[i].NormalsMap->Width)),
                                                                     (int*)(&(v_Materials[i].NormalsMap->Height)),NULL,SOIL_LOAD_RGBA);
                            free(filename);
                        }else
                            v_Materials[i].NormalsMap->Pixels=SOIL_load_image(path.C_Str(),(int*)(&(v_Materials[i].NormalsMap->Width)),
                                                                     (int*)(&(v_Materials[i].NormalsMap->Height)),NULL,SOIL_LOAD_RGBA);
                    }
                    hasNormalmap=true ;
                }
        }
        if(!hasDiffusemap){
            v_Materials[i].DiffuseMap=(Image*)malloc(sizeof(Image));
            v_Materials[i].DiffuseMap->Height=1 ;
            v_Materials[i].DiffuseMap->Width=1;
            v_Materials[i].DiffuseMap->Pixels=(_u8b*)malloc(4*sizeof(_u8b));
            v_Materials[i].DiffuseMap->Pixels[0]=128 ;
            v_Materials[i].DiffuseMap->Pixels[1]=128 ;
            v_Materials[i].DiffuseMap->Pixels[2]=128 ;
            v_Materials[i].DiffuseMap->Pixels[3]=0 ;
        }
        if(!hasNormalmap){
            v_Materials[i].NormalsMap=(Image*)malloc(sizeof(Image));
            v_Materials[i].NormalsMap->Height=1 ;
            v_Materials[i].NormalsMap->Width=1;
            v_Materials[i].NormalsMap->Pixels=(_u8b*)malloc(4*sizeof(_u8b));
            v_Materials[i].NormalsMap->Pixels[0]=128 ;
            v_Materials[i].NormalsMap->Pixels[1]=127 ;
            v_Materials[i].NormalsMap->Pixels[2]=255 ;
            v_Materials[i].NormalsMap->Pixels[3]=0 ;
        }
        if(!hasSpecularmap){
            v_Materials[i].SpecularMap=(Image*)malloc(sizeof(Image));
            v_Materials[i].SpecularMap->Height=1 ;
            v_Materials[i].SpecularMap->Width=1;
            v_Materials[i].SpecularMap->Pixels=(_u8b*)malloc(4*sizeof(_u8b));
            v_Materials[i].SpecularMap->Pixels[0]=127 ;
            v_Materials[i].SpecularMap->Pixels[1]=127 ;
            v_Materials[i].SpecularMap->Pixels[2]=127 ;
            v_Materials[i].SpecularMap->Pixels[3]=0 ;
        }
    }
    return 1;
}
_u16b RGP_CORE::Model3D::GenerateOGLMaterials(){
    if(!v_Materials)
        return 0 ;
    this->v_oglMaterials=(OGLMaterial*)malloc(m_nbMaterials*sizeof(OGLMaterial));
    if(!this->v_oglMaterials)
        return 0 ;
    glEnable(GL_TEXTURE_2D);
	for(_u32b i=0 ;i<m_nbMaterials;i++){
        v_oglMaterials[i].NormalsMap=0 ;
        v_oglMaterials[i].SpecularMap=0;
        v_oglMaterials[i].DiffuseMap=0;
		if(v_Materials[i].DiffuseMap && v_Materials[i].DiffuseMap->Pixels){
                            m_GLRenderer->GenTextures(1,&(this->v_oglMaterials[i].DiffuseMap));
                            m_GLRenderer->BindTexture(v_oglMaterials[i].DiffuseMap);
                            m_GLRenderer->SetImageData(v_Materials[i].DiffuseMap);
                            m_GLRenderer->BindTexture(0);
        }
		if( v_Materials[i].SpecularMap && v_Materials[i].SpecularMap->Pixels){
                            m_GLRenderer->GenTextures(1,&(v_oglMaterials[i].SpecularMap));
                            m_GLRenderer->BindTexture(v_oglMaterials[i].SpecularMap);
                            m_GLRenderer->SetImageData(v_Materials[i].SpecularMap);
                            m_GLRenderer->BindTexture(0);
        }
		if( v_Materials[i].NormalsMap && v_Materials[i].NormalsMap->Pixels){
                            m_GLRenderer->GenTextures(1,&(v_oglMaterials[i].NormalsMap));
                            m_GLRenderer->BindTexture(v_oglMaterials[i].NormalsMap);
                            m_GLRenderer->SetImageData(v_Materials[i].NormalsMap);
                            m_GLRenderer->BindTexture(0);
        }
	}
    glDisable(GL_TEXTURE_2D);
    return 1 ;
}
_u16b RGP_CORE::Model3D::GenerateBuffers(){

    v_Buffers=(MeshBuffers*)malloc(m_nbMeshes*sizeof(MeshBuffers));
    if(v_Buffers==NULL){
        return 0 ;
	}
	m_VAOforShadowcasting = (GLuint*)malloc(m_nbMeshes*sizeof(GLuint));
	if (!m_VAOforShadowcasting)
		return 0;
    for(_u32b i=0; i< m_nbMeshes;++i){
        v_Buffers[i].VertexArrayObject=0;
        v_Buffers[i].IndexBuffer=0;
        v_Buffers[i].VertexBuffer=0;
        v_Buffers[i].NormalBuffer=0;
        v_Buffers[i].TangentBuffer=0;
        v_Buffers[i].BitangentBuffer=0 ;
        v_Buffers[i].TexCoords=0;
		m_VAOforShadowcasting[i] = 0;
    };

    for(_u32b i=0; i< m_nbMeshes;++i){

        ///generating vertices buffer
        m_GLRenderer->GenBuffers(1,&(v_Buffers[i].VertexBuffer));
        if(!v_Buffers[i].VertexBuffer)
            return 0 ;
        ///filling buffer with data code here
        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].VertexBuffer);
        m_GLRenderer->setBufferData(GL_ARRAY_BUFFER,v_Meshes[i].nbVertices*3*sizeof(_float),v_Meshes[i].VertexBuffer,GL_STATIC_DRAW);
        ///........

        ///generating Normals buffer
        glGenBuffers(1,&(v_Buffers[i].NormalBuffer));
        if(!v_Buffers[i].NormalBuffer)
            return 0 ;
        ///filling buffer with data code here
        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].NormalBuffer);
        m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, v_Meshes[i].nbNormals*3*sizeof(_float),v_Meshes[i].NormalBuffer,GL_STATIC_DRAW);
        ///........

        ///Generating Tangent and Bitangent
        m_GLRenderer->GenBuffers(1,&(v_Buffers[i].TangentBuffer));
        m_GLRenderer->GenBuffers(1,&(v_Buffers[i].BitangentBuffer));
        if(!(v_Buffers[i].TangentBuffer && v_Buffers[i].BitangentBuffer))
            return 0 ;
        ///filling buffer with data
        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].TangentBuffer);
        m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, v_Meshes[i].nbNormals*3*sizeof(_float),v_Meshes[i].TangentBuffer,GL_STATIC_DRAW);
        m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].BitangentBuffer);
        m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, v_Meshes[i].nbNormals*3*sizeof(_float),v_Meshes[i].BitangentBuffer,GL_STATIC_DRAW);
        /// //////

        ///generating texture coordinates buffer
        if(v_Meshes[i].TexCoords){
            m_GLRenderer->GenBuffers(1,&(v_Buffers[i].TexCoords));
            if(!v_Buffers[i].TexCoords)
                return 0 ;
            ///filling buffer with data code here
            m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].TexCoords);
            m_GLRenderer->setBufferData(GL_ARRAY_BUFFER,v_Meshes[i].nbTexCoords*2,v_Meshes[i].TexCoords,GL_STATIC_DRAW);
        };

        ///generating indices buffer
        m_GLRenderer->GenBuffers(1,&(v_Buffers[i].IndexBuffer));
        if(!v_Buffers[i].IndexBuffer)
            return 0 ;
        ///filling buffer with data code here
        m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_Buffers[i].IndexBuffer);
        m_GLRenderer->setBufferData(GL_ELEMENT_ARRAY_BUFFER, v_Meshes[i].nbFaces*3*sizeof(_u32b) , v_Meshes[i].IndexBuffer, GL_STATIC_DRAW);
        ///........

    }
    m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,0);
    m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    return 1;
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


