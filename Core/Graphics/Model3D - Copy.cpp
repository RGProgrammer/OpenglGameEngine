#include ".//Model3D.h"

TTB::Model3D::Model3D():Model3D({0.0f,0.0f,0.0f}){
};
TTB::Model3D::Model3D(Vertex3d Pos): Model3D(Pos,{0.0f,0.0f,1.0f},{0.0f,1.0f,0.0f}){
};
TTB::Model3D::Model3D(Vertex3d Pos, Vertex3d Dir, Vertex3d Up): Renderable(Pos,Dir, Up),
															m_nbMeshes(0) ,v_Meshes(NULL),v_Buffers(NULL),
															v_Materials(NULL),v_oglMaterials(NULL),m_nbMaterials(0),
															m_FileDirectory(NULL),m_appliedShaderProgram(0){
};
TTB::Model3D::~Model3D(){
	Renderable::Destroy();
	this->Destroy();
};




void TTB::Model3D::Destroy(){
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
            if(v_Materials[i].LightMap){
                if(v_Materials[i].LightMap->Pixels){
                    free(v_Materials[i].LightMap->Pixels);
                    v_Materials[i].LightMap->Pixels=NULL;
                }
                free(v_Materials[i].LightMap);
                v_Materials[i].LightMap=NULL ;
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
        if(v_oglMaterials){
         for(_u32b i=0;i< m_nbMaterials;i++){
            if(v_oglMaterials[i].DiffuseMap)
                glDeleteTextures(1,&(v_oglMaterials[i].DiffuseMap));
            if(v_oglMaterials[i].SpecularMap)
                glDeleteTextures(1,&(v_oglMaterials[i].SpecularMap));
            if(v_oglMaterials[i].LightMap)
                glDeleteTextures(1,&(v_oglMaterials[i].LightMap));
            if(v_oglMaterials[i].NormalsMap)
                glDeleteTextures(1,&(v_oglMaterials[i].NormalsMap));
         }
         free(v_oglMaterials);
         v_oglMaterials=NULL ;
        }
        m_nbMaterials=0 ;
	}
	if(v_Buffers){
        for(_u32b i=0; i< m_nbMeshes ;i++){
            if(v_Buffers[i].VertexBuffer){
                glDeleteBuffers(1,&(v_Buffers[i].VertexBuffer));
            }
            if(v_Buffers[i].NormalBuffer){
                glDeleteBuffers(1,&(v_Buffers[i].NormalBuffer));
            }
            if(v_Buffers[i].IndexBuffer){
                glDeleteBuffers(1,&(v_Buffers[i].IndexBuffer));
            }
            if(v_Buffers[i].TexCoords){
                glDeleteBuffers(1,&(v_Buffers[i].TexCoords));
            }
            if(v_Buffers[i].VertexArrayObject){
                glDeleteVertexArrays(1,&(v_Buffers[i].VertexArrayObject));
            }
        }
        free(v_Buffers);
        v_Buffers=0 ;
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
	m_nbMeshes=0 ;
	}
	if(m_appliedShaderProgram){
        glDeleteProgram(m_appliedShaderProgram);
        m_appliedShaderProgram=0;
	}
	if(m_FileDirectory){
        free(m_FileDirectory);
        m_FileDirectory=NULL;
	}

};





_s16b TTB::Model3D::LoadModelFromFile(char* filename){
	this->Destroy();
	const aiScene* Scene=aiImportFile(filename,aiProcess_GenSmoothNormals | aiProcess_Triangulate |
                                                 aiProcess_GenUVCoords | aiProcess_PreTransformVertices|
                                                 aiProcess_RemoveRedundantMaterials | aiProcess_FlipUVs);
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
    if(!LoadShaderProg(".//Shaders//Default2.vs",".//Shaders//Default2.fs"))
        printf("error loading shader program\n");
    InitVAOs();
    return 1;
};



_s16b   TTB::Model3D::LoadShaderProg(char* VS_File,char* FS_File){
    ///Code should be changed here








};




_s16b   TTB::Model3D::InitVAOs(){
    if(!m_appliedShaderProgram)
        return 0;
    for(_u32b i=0 ;i<m_nbMeshes;++i){
        glGenVertexArrays(1,&(v_Buffers[i].VertexArrayObject));
        glBindVertexArray(v_Buffers[i].VertexArrayObject);
        if(v_Buffers[i].VertexBuffer){
            glBindBuffer(GL_ARRAY_BUFFER,v_Buffers[i].VertexBuffer);
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
            glEnableVertexAttribArray(0);
        }
        if(v_Buffers[i].TexCoords){
            glBindBuffer(GL_ARRAY_BUFFER,v_Buffers[i].TexCoords);
            glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
            glEnableVertexAttribArray(1);
        }else{
            glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
            glEnableVertexAttribArray(1);
        }
        if(v_Buffers[i].IndexBuffer){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,v_Buffers[i].IndexBuffer);
        }
    }
    glBindVertexArray(0);
    return 1 ;
};




void TTB::Model3D::Render(Camera* Selected){
    _float MV[16];
    _float MVP[16];
    int location =-1;
    if(m_nbMeshes){


        if(!m_appliedShaderProgram){
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                Multi4x4Mtx(Selected->getViewMtx(),this->getTransMtx(),MV);
                //Multi4x4Mtx(Selected->getProjectionMtx(),MV,MVP);
                glLoadMatrixf(MV);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45.0f,800.0f/600.0f,0.0f,1000.0f);
            for (_u32b i=0 ; i< m_nbMeshes;++i){



                glBegin(GL_TRIANGLES);
                glColor3f(1.0f,0.0f,0.0f);
                for(_u32b j=0 ; j<v_Meshes[i].nbFaces;++j){
                    glVertex3f(v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3  ]*3  ],
                                v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3  ]*3+1],
                                v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3  ]*3+2]);

                    glVertex3f(v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3+1]*3  ],
                                v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3+1]*3+1],
                                v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3+1]*3+2]);

                    glVertex3f(v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3+2]*3  ],
                                v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3+2]*3+1],
                                v_Meshes[i].VertexBuffer[v_Meshes[i].IndexBuffer[j*3+2]*3+2]);
                }
                glEnd();
            }
        }else{

            glUseProgram(m_appliedShaderProgram);
            ///these Uniforms are for Default1.vs and Default1.fs
            /*
            glUniformMatrix4fv(glGetUniformLocation(m_appliedShaderProgram,"MV"),4,GL_FALSE,MV);
            glUniformMatrix4fv(glGetUniformLocation(m_appliedShaderProgram,"MVP"),4,GL_FALSE,MVP);*/
            /// these uniforms are for Default2.vs and Default2.fs
            location=glGetUniformLocation(m_appliedShaderProgram,"Model");
            if(location==-1)
                printf("error uniform location Model matrix %d \n",location);
            glUniformMatrix4fv(location,4,GL_FALSE,this->getTransMtx());
            /*if(glGetError()){
                printf("error defining uniform model matrix %d \n",location);
            }*/

            location=glGetUniformLocation(m_appliedShaderProgram,"View");
            if(location==-1)
                printf("error uniform location View matrix\n");
            glUniformMatrix4fv(location,4,GL_FALSE,Selected->getViewMtx());
             /*if(glGetError()){
                printf("error defining uniform view matrix\n");
            }*/

            location=glGetUniformLocation(m_appliedShaderProgram,"Proj");
            if(location==-1)
                printf("error uniform location Proj matrix\n");
            glUniformMatrix4fv(location,4,GL_FALSE,Selected->getProjectionMtx());
             /*if(glGetError()){
                printf("error defining uniform projection matrix\n");
            }*/
            for (_u32b i=0 ; i< m_nbMeshes;++i){
                    location=glGetUniformLocation(m_appliedShaderProgram,"diffusemap");
                    if(location==-1)
                        printf("error uniform location texture\n");
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, v_oglMaterials[v_Meshes[i].AppliedMaterial].DiffuseMap);
                    glUniform1i(location,0);
                     /*if(glGetError()){
                        printf("error defining uniform texture \n");
                    }*/

                glBindVertexArray(v_Buffers[i].VertexArrayObject);
                glDrawElements(
                    GL_TRIANGLES,      // mode
                    v_Meshes[i].nbFaces*3,    // count
                    GL_UNSIGNED_INT,   // type
                    (void*)0    );       // element array buffer offset
             };
             glBindVertexArray(0);
             glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
             glBindBuffer(GL_ARRAY_BUFFER,0);
             glUseProgram(0);

        }

    }
};



_u16b TTB::Model3D::ProcessNode(aiNode* Node,const aiScene* Scene){
    /// process current Node
    for(_u16b i=0;i<Node->mNumMeshes ;i++){
        AddMesh(Scene->mMeshes[Node->mMeshes[i]]->mName.C_Str(),Scene->mMeshes[Node->mMeshes[i]]->mMaterialIndex);
        CopyVertices(Scene->mMeshes[Node->mMeshes[i]]->mVertices,Scene->mMeshes[Node->mMeshes[i]]->mNumVertices);
        CopyNormals(Scene->mMeshes[Node->mMeshes[i]]->mNormals,Scene->mMeshes[Node->mMeshes[i]]->mNumVertices);
        CopyFaces(Scene->mMeshes[Node->mMeshes[i]]->mFaces,Scene->mMeshes[Node->mMeshes[i]]->mNumFaces );
        CopyTextureCoords(Scene->mMeshes[Node->mMeshes[i]]->mTextureCoords,Scene->mMeshes[Node->mMeshes[i]]->mNumVertices);
    }

    ///process ChildNodes
    for(_u16b i=0; i< Node->mNumChildren;i++ ){
        return ProcessNode(Node->mChildren[i],Scene);
    }
    return 1;
};




_u16b TTB::Model3D::AddMesh(const char* Name, _u16b MaterialID){
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
    v_Meshes[m_nbMeshes-1].Name=(char*)malloc(strlen(Name)*sizeof(char)) ;
    if(v_Meshes[m_nbMeshes-1].Name)
        strcpy(v_Meshes[m_nbMeshes-1].Name,Name);
    v_Meshes[m_nbMeshes-1].IndexBuffer=NULL;
    v_Meshes[m_nbMeshes-1].nbFaces=0;
    v_Meshes[m_nbMeshes-1].VertexBuffer=NULL ;
    v_Meshes[m_nbMeshes-1].nbVertices=0;
    v_Meshes[m_nbMeshes-1].NormalBuffer=NULL;
    v_Meshes[m_nbMeshes-1].nbNormals=0;
    v_Meshes[m_nbMeshes-1].AppliedMaterial=MaterialID ;
};
_u16b TTB::Model3D::CopyVertices(const aiVector3D*   buffer,_u32b nbVertices){
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
    }
    return 1 ;
};
_u16b TTB::Model3D::CopyNormals(const aiVector3D*   buffer,_u32b nbVertices){
    ///copy Normalbuffer to the last mesh
    if(buffer && nbVertices> 0){
        v_Meshes[m_nbMeshes-1].NormalBuffer=(_float*)malloc(nbVertices*3*sizeof(_float));
        if(!(v_Meshes[m_nbMeshes-1].NormalBuffer))
            return 0;
        v_Meshes[m_nbMeshes-1].nbNormals=nbVertices ;
        for(_u32b i=0 ; i<nbVertices;i++){
            v_Meshes[m_nbMeshes-1].NormalBuffer[i*3  ]=buffer[i].x;
            v_Meshes[m_nbMeshes-1].NormalBuffer[i*3+1]=buffer[i].y;
            v_Meshes[m_nbMeshes-1].NormalBuffer[i*3+2]=buffer[i].z;
        }
    }
    return 1 ;
};
_u16b TTB::Model3D::CopyFaces(const aiFace* Faces, _u32b nbFaces){
    ///copy Faces
    if(Faces && nbFaces> 0){
        v_Meshes[m_nbMeshes-1].IndexBuffer=(_u32b*)malloc(nbFaces*3*sizeof(_u32b));
        if(!(v_Meshes[m_nbMeshes-1].IndexBuffer))
            return 0;
        v_Meshes[m_nbMeshes-1].nbFaces=nbFaces ;
        for(_u32b i=0 ; i<nbFaces;i++){
            v_Meshes[m_nbMeshes-1].IndexBuffer[i*3  ]=Faces[i].mIndices[0];
            v_Meshes[m_nbMeshes-1].IndexBuffer[i*3+1]=Faces[i].mIndices[1];
            v_Meshes[m_nbMeshes-1].IndexBuffer[i*3+2]=Faces[i].mIndices[2];
        }
    }
    return 1 ;
};
_u16b TTB::Model3D::CopyTextureCoords(aiVector3D** TexCoords, _u32b nbTexCoords){
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
        }
        v_Meshes[m_nbMeshes-1].TexCoords=(_float*)malloc(nbTexCoords*2*sizeof(_float));
        if(!(v_Meshes[m_nbMeshes-1].TexCoords))
            return 0;
        v_Meshes[m_nbMeshes-1].nbTexCoords=nbTexCoords ;
        for(_u32b i=0 ; i<nbTexCoords;i++){
            v_Meshes[m_nbMeshes-1].TexCoords[i*2  ]= TexCoords[0][i].x;
            v_Meshes[m_nbMeshes-1].TexCoords[i*2+1]= TexCoords[0][i].y;
        }
    }
    return 1 ;
};
_u16b TTB::Model3D::LoadMaterial(const aiScene* Scene){
    if(!LoadMaterialstoMemory(Scene))
        return 0 ;
    if(!GenerateOGLMaterials())
        return 0;
    return 1 ;

};
_u16b TTB::Model3D::LoadMaterialstoMemory(const aiScene* Scene){
    m_nbMaterials=Scene->mNumMaterials;
    _bool hasDiffusemap;
    this->v_Materials=(Material*)malloc(m_nbMaterials*sizeof(Material));
    if(!v_Materials){
        return 0 ;
    }
    for(_u32b i=0 ; i<m_nbMaterials;++i){
        v_Materials[i].Color={0.0f,0.0f,1.0f};
        v_Materials[i].LightMap=NULL;
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
                            TTB::CatStrings(m_FileDirectory,path.C_Str(),&filename);
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
                            TTB::CatStrings(m_FileDirectory,path.C_Str(),&filename);
                            v_Materials[i].SpecularMap->Pixels=SOIL_load_image(filename,(int*)(&(v_Materials[i].SpecularMap->Width)),
                                                                     (int*)(&(v_Materials[i].SpecularMap->Height)),NULL,SOIL_LOAD_RGBA);
                            free(filename);
                        }else{
                            v_Materials[i].SpecularMap->Pixels=SOIL_load_image(path.C_Str(),(int*)(&(v_Materials[i].SpecularMap->Width)),
                                                                     (int*)(&(v_Materials[i].SpecularMap->Height)),NULL,SOIL_LOAD_RGBA);
                        }
                    }
                }else if(Scene->mMaterials[i]->mProperties[j]->mSemantic==aiTextureType_LIGHTMAP){
                    v_Materials[i].LightMap=(Image*)malloc(sizeof(Image));
                    v_Materials[i].LightMap->Height=0 ;
                    v_Materials[i].LightMap->Width=0;
                    v_Materials[i].LightMap->Pixels=NULL;
                    if(Scene->mMaterials[i]->mProperties[j]->mIndex!=0){
                        CopyTextureData(Scene->mTextures[Scene->mMaterials[i]->mProperties[j]->mIndex-1],v_Materials[i].LightMap);
                    }else if(Scene->mMaterials[i]->mProperties[j]->mType==aiPTI_String){
                        aiString path ;
                        Scene->mMaterials[i]->GetTexture(aiTextureType_LIGHTMAP,0,&path);
                        char* filename=NULL ;
                        if(m_FileDirectory){
                            TTB::CatStrings(m_FileDirectory,path.C_Str(),&filename);
                            v_Materials[i].LightMap->Pixels=SOIL_load_image(filename,(int*)(&(v_Materials[i].LightMap->Width)),
                                                                     (int*)(&(v_Materials[i].LightMap->Height)),NULL,SOIL_LOAD_RGBA);
                           free(filename);
                        }else{
                            v_Materials[i].LightMap->Pixels=SOIL_load_image(path.C_Str(),(int*)(&(v_Materials[i].LightMap->Width)),
                                                                     (int*)(&(v_Materials[i].LightMap->Height)),NULL,SOIL_LOAD_RGBA);
                        }
                    }

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
                            TTB::CatStrings(m_FileDirectory,path.C_Str(),&filename);
                            v_Materials[i].NormalsMap->Pixels=SOIL_load_image(filename,(int*)(&(v_Materials[i].NormalsMap->Width)),
                                                                     (int*)(&(v_Materials[i].NormalsMap->Height)),NULL,SOIL_LOAD_RGBA);
                            free(filename);
                        }else
                            v_Materials[i].NormalsMap->Pixels=SOIL_load_image(path.C_Str(),(int*)(&(v_Materials[i].NormalsMap->Width)),
                                                                     (int*)(&(v_Materials[i].NormalsMap->Height)),NULL,SOIL_LOAD_RGBA);
                    }
                }
        }
        if(!hasDiffusemap){
            v_Materials[i].DiffuseMap=(Image*)malloc(sizeof(Image));
            v_Materials[i].DiffuseMap->Height=1 ;
            v_Materials[i].DiffuseMap->Width=1;
            v_Materials[i].DiffuseMap->Pixels=(_u8b*)malloc(4*sizeof(_u8b));
            v_Materials[i].DiffuseMap->Pixels[0]=255 ;
            v_Materials[i].DiffuseMap->Pixels[1]=0 ;
            v_Materials[i].DiffuseMap->Pixels[2]=255 ;
            v_Materials[i].DiffuseMap->Pixels[3]=0 ;
        }
    }
    return 1;
}
_u16b TTB::Model3D::GenerateOGLMaterials(){
    if(!v_Materials)
        return 0 ;
    this->v_oglMaterials=(OGLMaterial*)malloc(m_nbMaterials*sizeof(OGLMaterial));
    if(!this->v_oglMaterials)
        return 0 ;
    glEnable(GL_TEXTURE_2D);
	for(_u32b i=0 ;i<m_nbMaterials;i++){
		v_oglMaterials[i].Color={0.0f,0.0f,1.0f};
        v_oglMaterials[i].LightMap=0;
        v_oglMaterials[i].NormalsMap=0 ;
        v_oglMaterials[i].SpecularMap=0;
        v_oglMaterials[i].DiffuseMap=0;
		if(v_Materials[i].DiffuseMap && v_Materials[i].DiffuseMap->Pixels){
                            glGenTextures(1,&(this->v_oglMaterials[i].DiffuseMap));
                            glBindTexture(GL_TEXTURE_2D,v_oglMaterials[i].DiffuseMap);
                            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
                            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,
                                    v_Materials[i].DiffuseMap->Width,
                                    v_Materials[i].DiffuseMap->Height,
                                    0,GL_RGBA,GL_UNSIGNED_BYTE,
                                    v_Materials[i].DiffuseMap->Pixels);
                            glBindTexture(GL_TEXTURE_2D,0);
        }
		if( v_Materials[i].SpecularMap && v_Materials[i].SpecularMap->Pixels){
                            glGenTextures(1,&(v_oglMaterials[i].SpecularMap));
                            glBindTexture(GL_TEXTURE_2D,v_oglMaterials[i].SpecularMap);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
                                    v_Materials[i].SpecularMap->Width,
                                    v_Materials[i].SpecularMap->Height,
                                    0,GL_RGBA,GL_UNSIGNED_BYTE,
                                    v_Materials[i].SpecularMap->Pixels);
                            glBindTexture(GL_TEXTURE_2D,0);
               glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);          }
		if( v_Materials[i].NormalsMap && v_Materials[i].NormalsMap->Pixels){
                            glGenTextures(1,&(v_oglMaterials[i].NormalsMap));
                            glBindTexture(GL_TEXTURE_2D,v_oglMaterials[i].NormalsMap);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
                                    v_Materials[i].NormalsMap->Width,
                                    v_Materials[i].NormalsMap->Height,
                                    0,GL_RGBA,GL_UNSIGNED_BYTE,
                                    v_Materials[i].NormalsMap->Pixels);
                            glBindTexture(GL_TEXTURE_2D,0);
                        }
        if( v_Materials[i].LightMap && v_Materials[i].LightMap->Pixels){
                            glGenTextures(1,&(v_oglMaterials[i].LightMap));
                            glBindTexture(GL_TEXTURE_2D,v_oglMaterials[i].LightMap);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
                                    v_Materials[i].LightMap->Width,
                                    v_Materials[i].LightMap->Height,
                                    0,GL_RGBA,GL_UNSIGNED_BYTE,
                                    v_Materials[i].LightMap->Pixels);
                            glBindTexture(GL_TEXTURE_2D,0);
                        }

	}
    glDisable(GL_TEXTURE_2D);
    return 1 ;
}
_u16b TTB::Model3D::GenerateBuffers(){

    v_Buffers=(MeshBuffers*)malloc(m_nbMeshes*sizeof(MeshBuffers));
    if(v_Buffers==NULL){
        return 0 ;}
    for(_u32b i=0; i< m_nbMeshes;++i){
        v_Buffers[i].VertexArrayObject=0;
        v_Buffers[i].IndexBuffer=0;
        v_Buffers[i].VertexBuffer=0;
        v_Buffers[i].NormalBuffer=0;
        v_Buffers[i].TexCoords=0;
    };
    for(_u32b i=0; i< m_nbMeshes;++i){

        ///generating vertices buffer
        glGenBuffers(1,&(v_Buffers[i].VertexBuffer));
        if(!v_Buffers[i].VertexBuffer)
            return 0 ;
        ///filling buffer with data code here
        glBindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,v_Meshes[i].nbVertices*3*sizeof(_float),v_Meshes[i].VertexBuffer,GL_STATIC_DRAW);
        ///........



        ///generating Normals buffer
        glGenBuffers(1,&(v_Buffers[i].NormalBuffer));
        if(!v_Buffers[i].NormalBuffer)
            return 0 ;
        ///filling buffer with data code here
        glBindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].NormalBuffer);
        glBufferData(GL_ARRAY_BUFFER, v_Meshes[i].nbNormals*3*sizeof(_float),v_Meshes[i].NormalBuffer,GL_STATIC_DRAW);
        ///........


        ///generating texture coordinates buffer
        if(v_Meshes[i].TexCoords){
            glGenBuffers(1,&(v_Buffers[i].TexCoords));
            if(!v_Buffers[i].TexCoords)
                return 0 ;
            ///filling buffer with data code here
            glBindBuffer(GL_ARRAY_BUFFER, v_Buffers[i].TexCoords);
            glBufferData(GL_ARRAY_BUFFER,v_Meshes[i].nbTexCoords*2,v_Meshes[i].TexCoords,GL_STATIC_DRAW);
        };

        ///generating indices buffer
        glGenBuffers(1,&(v_Buffers[i].IndexBuffer));
        if(!v_Buffers[i].IndexBuffer)
            return 0 ;
        ///filling buffer with data code here
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_Buffers[i].IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, v_Meshes[i].nbFaces*3*sizeof(_u32b) , v_Meshes[i].IndexBuffer, GL_STATIC_DRAW);
        ///........

    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    return 1;
};
 _u16b TTB::Model3D::CopyTextureData(aiTexture* Texture, Image* Dest){
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


