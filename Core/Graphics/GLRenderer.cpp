#include ".//GLRenderer.h"

RGP_CORE::GLRenderer::GLRenderer(RenderType Type):m_Target(NULL),m_isInitialized(false) ,
											m_SelectedScene(NULL),

											m_NumFBOs(0),m_FBOs(NULL),m_SelectedFBO(0),
                                            m_AttachmentTextures(NULL),

                                            m_FinalRenderSurface(NULL),
                                            m_FinalRenderProgram(0),

											m_ShadowVectorSize(0),m_NumShadowFBOs(0),
											m_ShadowRenderingProgram(0),
											m_ShadowFBOs(NULL),
											m_ShadowAttachmentTexture(NULL),
											m_CombinedShadowResultFBO(0),
											m_CombinedShadowResultTexture(0),
											m_CombineShadowProgram(0)
											
{
    if(Type==DEFERRED_RENDERING || Type==FORWARD_RENDERING)
        m_Mode=Type ;
    else
        m_Mode=DEFERRED_RENDERING;

	DrawBuff[0] = GL_COLOR_ATTACHMENT0;
	DrawBuff[1] = GL_COLOR_ATTACHMENT1;
	DrawBuff[2] = GL_COLOR_ATTACHMENT2;
	DrawBuff[3] = GL_COLOR_ATTACHMENT3;

};
RGP_CORE::GLRenderer::~GLRenderer(){
    this->Destroy();
};
void RGP_CORE::GLRenderer::Destroy(){
    if(m_FBOs){
        glDeleteFramebuffers(m_NumFBOs,m_FBOs);
        free(m_FBOs);
        m_FBOs=NULL ;
        m_NumFBOs=0 ;
    }
    m_SelectedFBO=0;
    if(m_AttachmentTextures){
        for(_s16b i=0; i < m_NumFBOs;++i){
            if(m_AttachmentTextures[i]==NULL)
                continue ;
            glDeleteTextures(6,m_AttachmentTextures[i]);
            free(m_AttachmentTextures[i]);
        }
        free(m_AttachmentTextures);
        m_AttachmentTextures=NULL ;
    }
	if (m_ShadowFBOs){
		if (m_ShadowAttachmentTexture){
			glDeleteTextures(m_NumShadowFBOs, m_ShadowAttachmentTexture);
			free(m_ShadowAttachmentTexture);
			m_ShadowAttachmentTexture = NULL;
		}
		glDeleteFramebuffers(m_ShadowVectorSize, m_ShadowFBOs);
		free(m_ShadowFBOs);
		m_ShadowFBOs = NULL;
		m_NumShadowFBOs = 0;
		m_ShadowVectorSize = 0;
	}
	if (m_CombinedShadowResultFBO){
		if (m_CombinedShadowResultTexture){
			glDeleteTextures(1, &m_CombinedShadowResultTexture);
			m_CombinedShadowResultTexture = 0;
		}
		glDeleteFramebuffers(1, &m_CombinedShadowResultFBO);
		m_CombinedShadowResultFBO = 0;
	}
	if (m_ShadowRenderingProgram){
		glDeleteProgram(m_ShadowRenderingProgram);
		m_ShadowRenderingProgram = 0;
	}
	if (m_CombineShadowProgram){
		glDeleteProgram(m_CombineShadowProgram);
		m_CombineShadowProgram = 0;
	}

    if(m_FinalRenderSurface){
        if(m_FinalRenderSurface->IndexBuffer){
            glDeleteBuffers(1,&(m_FinalRenderSurface->IndexBuffer));
        }
        if(m_FinalRenderSurface->NormalBuffer){
            glDeleteBuffers(1,&(m_FinalRenderSurface->NormalBuffer));
        }
        if(m_FinalRenderSurface->TexCoords){
            glDeleteBuffers(1,&(m_FinalRenderSurface->TexCoords));
        }
        if(m_FinalRenderSurface->VertexBuffer){
            glDeleteBuffers(1,&(m_FinalRenderSurface->VertexBuffer));
        }
        if(m_FinalRenderSurface->VertexArrayObject){
            glDeleteVertexArrays(1,&(m_FinalRenderSurface->VertexArrayObject));
        }
        free(m_FinalRenderSurface);
        m_FinalRenderSurface=NULL ;
    }
    if(m_FinalRenderProgram){
        glDeleteProgram(m_FinalRenderProgram);
        m_FinalRenderProgram=0 ;
    }
    if(m_Target){
        m_Target->Destroy();
        delete m_Target;
        m_Target=NULL ;
    }
    m_SelectedScene=NULL ;
    m_isInitialized =false;
    glfwTerminate();

};
_bool RGP_CORE::GLRenderer::InitRenderer(gfxConfig Config){

    ///initializinf GLFW and creating window ;
    if(!glfwInit()){
        printf("errro initializing GLFW\n");
        return false ;
    }
    m_Target=Window::CreateWindow(Config.Title,Config.Witdh,Config.Height);
    if(!m_Target){
        printf("error creating Window\n");
        return false ;
    }
    glfwMakeContextCurrent( m_Target->getglfwWindow());
    ///initializing GLEW
    if(glewInit()!=GLEW_OK){
        printf("error init GLEW\n");
        return false ;
    }
	m_Config = Config;
    m_NumFBOs=Config.NumBackBuffers;
     if(!CreateNeededObjects()){
        printf("error Creating FBOs and Render Textures\n");
        return false ;
     }

   //final step to initialize the renderer
    if(!InitFinalPhase()){
        printf("error initializing final phase\n");
        //return false ;
    }
    
    m_isInitialized=true ;
    return true ;
};
_bool RGP_CORE::GLRenderer::CreateNeededObjects(){
    int error;
    ///generating textures(Diffuse,Specular+Roughness(RGBA),Normal,Depth+Stencil,shadow,trancprency)==>6maps;
    m_AttachmentTextures=(GLuint**)malloc(m_NumFBOs*sizeof(GLuint*));
    if(!m_AttachmentTextures){
        printf("allocation error \n");
        return false ;
    }
    for(_s16b i= 0 ; i<m_NumFBOs;++i)
        m_AttachmentTextures[i]=NULL ;
    ///generating textures for each FBO
    for(_s16b i=0; i<m_NumFBOs;++i){
        m_AttachmentTextures[i]=(GLuint*)malloc(6*sizeof(GLuint));
        if(!m_AttachmentTextures[i]){
            printf("error allocation 2 \n");
            return false;
        }
        for(_s16b k=0;k<6;++k){
            m_AttachmentTextures[i][k]=0 ;
        }
        glGetError();
        glGenTextures(6,m_AttachmentTextures[i]);
        if(error=glGetError())
            printf("error generating textures %u\n",i);
        ///Depth texture+stencil
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[i][0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glGetError();
        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,
                         m_Target->getWidth(),
                         m_Target->getHeight(),
                         0,GL_DEPTH_COMPONENT,GL_FLOAT,0);

        ///THE OTHER TEXTURES
        for(_s16b k=1;k<6;++k){
            glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[i][k]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,
                         m_Target->getWidth(),
                         m_Target->getHeight(),
                         0,GL_RGBA ,GL_FLOAT,0);
        }
        glBindTexture(GL_TEXTURE_2D,0);
    }
    ///create FBO and their render Buffer(Textures : Deffuse, Normal, Depth,Sepecular, Transparency(nOT YET) )
        ///generating FBOs
    m_FBOs=(GLuint*)malloc(m_NumFBOs*sizeof(GLuint));
    if(!m_FBOs){
        printf("error allcation memory for FBOs\n");
        return false ;
    }
    glGetError();
    glGenFramebuffers(m_NumFBOs,m_FBOs);
    if(error=glGetError()){
        printf("error generating Framebuffers %d\n",error);
        return false;
    }
	if (!AttachTextures())
		return false;
	int what;
	
	m_ShadowRenderingProgram = CreateGLProgramFromFile("..//Shaders//ShadowMapProgram.vs", "..//Shaders//ShadowMapProgram.fs");
	if (!m_ShadowRenderingProgram){
		printf("error loading Shadow Mapping program");
		scanf("%d", &what);
		return false;
	}
	m_CombineShadowProgram = CreateGLProgramFromFile("..//Shaders//FinalShadow.vs", "..//Shaders//FinalShadow.fs");
	if (!m_CombineShadowProgram){
		printf("error loading Shadow Maps Combiner program");
		scanf("%d", &what);
		return false;
	}
	
	glGenFramebuffers(1, &m_CombinedShadowResultFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,m_CombinedShadowResultFBO);
	glGenTextures(1, &m_CombinedShadowResultTexture);
	glBindTexture(GL_TEXTURE_2D, m_CombinedShadowResultTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		m_Target->getWidth(),
		m_Target->getHeight(),
		0, GL_RGBA, GL_FLOAT, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_CombinedShadowResultTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
    return true ;
};
_bool RGP_CORE::GLRenderer::AttachTextures(){
     int error;
     ///attaching texture to its FBO
    for(_s16b i = 0 ; i < m_NumFBOs ; ++i){
        glGetError();
        glBindFramebuffer(GL_FRAMEBUFFER,m_FBOs[i]);
        if(error=glGetError()){
            printf("error binding framebuffer = %u error %d\n",m_FBOs[i],error);
            continue ;
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_AttachmentTextures[i][DIFFUSE_TEXTURE],0);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,m_AttachmentTextures[i][SPECULAR_TEXTURE],0);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,m_AttachmentTextures[i][NORMAL_TEXTURE],0);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_AttachmentTextures[i][DEPTH_TEXTURE],0);
        error=glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(GL_FRAMEBUFFER_COMPLETE != error){
            printf("frameBuffer attacehment error code %x\n",error);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    return true ;
};
_bool RGP_CORE::GLRenderer::InitFinalPhase(){
    _float vertexBuffer[8]={    -1.0f,1.0f,
                                1.0f,1.0f,
                                1.0f,-1.0f,
                                -1.0f,-1.0f,};
    _float TexCoordBuffer[8]={0.0f,0.0f,
                            1.0f,0.0f,
                            1.0f,1.0f,
                            0.0f,1.0f};
    _u32b IndexBuffer[6]={0,2,1,
                         0,3,2};
    m_FinalRenderSurface=(MeshBuffers*)malloc(sizeof(MeshBuffers));
    if(!m_FinalRenderSurface)
        return false ;
    m_FinalRenderSurface->VertexArrayObject=0;
    m_FinalRenderSurface->VertexBuffer=0;
    m_FinalRenderSurface->NormalBuffer=0;
    m_FinalRenderSurface->TexCoords=0;
    m_FinalRenderSurface->IndexBuffer=0;
    glGetError();
    glGenBuffers(1,&( m_FinalRenderSurface->VertexBuffer));
    if(glGetError())
        return false ;
    glBindBuffer(GL_ARRAY_BUFFER, m_FinalRenderSurface->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,8*sizeof(_float),vertexBuffer,GL_STATIC_DRAW);
    glGetError();
    glGenBuffers(1,&(m_FinalRenderSurface->TexCoords));
    if(glGetError())
        return false ;
    glBindBuffer(GL_ARRAY_BUFFER, m_FinalRenderSurface->TexCoords);
    glBufferData(GL_ARRAY_BUFFER,8*sizeof(_float),TexCoordBuffer,GL_STATIC_DRAW);
    glGetError();
    glGenBuffers(1,&(m_FinalRenderSurface->IndexBuffer));
    if(glGetError())
        return false ;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_FinalRenderSurface->IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(_u32b) , IndexBuffer, GL_STATIC_DRAW);

    glGenVertexArrays(1,&(m_FinalRenderSurface->VertexArrayObject));
    glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER,m_FinalRenderSurface->VertexBuffer);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,m_FinalRenderSurface->TexCoords);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_FinalRenderSurface->IndexBuffer);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	m_FinalRenderProgram = this->CreateGLProgramFromFile("..//Shaders//FinalDeferred.vs", "..//Shaders//FinalDeferred.fs");
	if (!m_FinalRenderProgram){
		printf("error loading render program \n");
		return false;
	}
	

    return true ;
};

_bool RGP_CORE::GLRenderer::isInitialized(){ return m_isInitialized ;};
RGP_CORE::RenderMode  RGP_CORE::GLRenderer::getRenderMode(){ return m_Mode ;};
RGP_CORE::Window*   RGP_CORE::GLRenderer::getTarget(){ return m_Target ;};
void  RGP_CORE::GLRenderer::setScene(GameScene*   Scene){ 
	m_SelectedScene= Scene ;
	if (m_SelectedScene){
		GLuint* tmp = NULL;
		int error;
		_u32b numLights = m_SelectedScene->getNBLights();
		if (numLights > 25)
			numLights = 25;
		if (numLights > m_ShadowVectorSize){
			tmp = (GLuint*)malloc(numLights*sizeof(GLuint));//first we need new FBos
			if (!tmp)
				return;
			for (_u32b i = 0; i < m_NumShadowFBOs; ++i)
				tmp[i] = m_ShadowFBOs[i];
			free(m_ShadowFBOs);
			m_ShadowFBOs = tmp;

			tmp = (GLuint*)malloc(numLights*sizeof(GLuint));//next Attachent texture for eac fbo
			if (!tmp)
				return;
			for (_u32b i = 0; i < m_NumShadowFBOs; ++i)
				tmp[i] = m_ShadowAttachmentTexture[i];
			free(m_ShadowAttachmentTexture);
			m_ShadowAttachmentTexture = tmp;

			//generate and attach
			glGenFramebuffers(numLights - m_NumShadowFBOs, &m_ShadowFBOs[m_NumShadowFBOs]);
			glGenTextures(numLights - m_NumShadowFBOs, &m_ShadowAttachmentTexture[m_NumShadowFBOs]);
			
			for (_u32b i = m_NumShadowFBOs; i < numLights; ++i){
				glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBOs[i]);
				glBindTexture(GL_TEXTURE_2D, m_ShadowAttachmentTexture[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
					m_Config.ShadowResolution,
					m_Config.ShadowResolution,
					0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowAttachmentTexture[i], 0);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (GL_FRAMEBUFFER_COMPLETE != error){
					printf("frameBuffer attacehment error code %x\n", error);
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			m_ShadowVectorSize = numLights;
		}
		m_NumShadowFBOs = numLights;
	}
	else{
		m_NumShadowFBOs = 0;
	}
};


void  RGP_CORE::GLRenderer::RenderCurrentScene(){

	//RENDER SCENE Colors to the SelectedFBO
	this->DrawSceneColors();

	//if should draw shadows
	if (m_Config.EnableShadows==true){
		this->DrawSceneShadows();
	}
    
	///combine results and render to the screen ;
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    RenderToScreen();

	
};
void	RGP_CORE::GLRenderer::LoadShadowProgram()
{
	glUseProgram(m_ShadowRenderingProgram);
};
void	RGP_CORE::GLRenderer::UnloadShadowProgram()
{
	glUseProgram(0);
};
void RGP_CORE::GLRenderer::DrawSceneColors()
{
	_u32b nbActors = 0;
	BaseActor*  actor = NULL;

	glGetError();
	if (m_SelectedScene){
		nbActors = m_SelectedScene->getNBActors();
		///first step render to current FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[m_SelectedFBO]);
		glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
		glDrawBuffers(3, DrawBuff);
		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);*/
		glEnable(GL_DEPTH_TEST);
		///glClear attachements
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (glGetError()){
			printf("WTW Clear \n ");
		}
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		///start searching for renderable objects and rendering
		for (_u32b i = 0; i < nbActors; ++i){
			actor = m_SelectedScene->getActor(i);
			if (actor->getID() & RENDERABLE)
				dynamic_cast<Renderable*>(actor)->Render(m_SelectedScene->getCamera());
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Next frame
		if (m_NumFBOs - 1 == m_SelectedFBO)
			m_SelectedFBO = 0;
		else
			++m_SelectedFBO;
		
	}
};
void RGP_CORE::GLRenderer::DrawSceneShadows()
{
	
	if (m_SelectedScene && m_Config.EnableShadows){
		_u32b NumActors = m_SelectedScene->getNBActors();
		_u32b ShadowIndex = 0;
		_s32b Location1 = -1,Location2=-1, Location3=-1;
		_s8b shaderstring[50] = "";
		LightSource*	Source = NULL;
		Renderable*		actor = NULL;
		glUseProgram(m_ShadowRenderingProgram);
		Location1 = glGetUniformLocation(m_ShadowRenderingProgram, "World");
		if (Location1 == -1)
			printf("error error error");
		Location2 = glGetUniformLocation(m_ShadowRenderingProgram, "View");
		if (Location2 == -1)
			printf("error error error");
		Location3 = glGetUniformLocation(m_ShadowRenderingProgram, "Projection");
		if (Location3 == -1)
			printf("error error error");
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		//for (_u32b i = 0; i < NumActors && ShadowIndex < 25; ++i)
		for(ShadowIndex = 0 ; ShadowIndex < m_NumShadowFBOs ; ++ShadowIndex){
			//if (m_SelectedScene->getActor(i)->getID() & LIGHTSOURCE){
				//Source = dynamic_cast<LightSource*>(m_SelectedScene->getActor(ShadowIndex));
				Source = m_SelectedScene->getLight(ShadowIndex);
				//Multi4x4Mtx(Source->getLightProjectionMtx(), Source->getLightViewMtx(), VPMatrix);
				glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBOs[ShadowIndex]);
				glViewport(0, 0, m_Config.ShadowResolution, m_Config.ShadowResolution);
				glClear(GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				//glDisable(GL_CULL_FACE);
				glUniformMatrix4fv(Location2, 4, GL_FALSE, Source->getLightViewMtx());
				glUniformMatrix4fv(Location3, 4, GL_FALSE, Source->getLightProjectionMtx());
				for (_u32b j = 0; j < NumActors; ++j)
					if (m_SelectedScene->getActor(j)->getID() & RENDERABLE){
						actor = dynamic_cast<Renderable*>(m_SelectedScene->getActor(j));
						glUniformMatrix4fv(Location1, 4, GL_FALSE, actor->getTransMtx());
						actor->CastShadow();
					}

			//	++ShadowIndex;
			//}
		}
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);


		//combine Shadow map to a single one
		glUseProgram(m_CombineShadowProgram);
		glBindFramebuffer(GL_FRAMEBUFFER, m_CombinedShadowResultFBO);
		glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
		ShadowIndex = 0;
		Location1 = glGetUniformLocation(m_CombineShadowProgram, "NumLights");
		glUniform1i(Location1, m_NumShadowFBOs);

		Location1=glGetUniformLocation(m_CombineShadowProgram,"CameraProMtx");
		glUniformMatrix4fv(Location1, 4, GL_FALSE, m_SelectedScene->getCamera()->getProjectionMtx());

		Location1 = glGetUniformLocation(m_CombineShadowProgram, "CameraViewMtx");
		glUniformMatrix4fv(Location1, 4, GL_FALSE, m_SelectedScene->getCamera()->getViewMtx());

		
		Location1 = glGetUniformLocation(m_CombineShadowProgram, "CameraDepthMap");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[m_SelectedFBO][DEPTH_TEXTURE]);
		glUniform1i(Location1, 0);

		//lights uniforms
		//for (_u32b i = 0; i < NumActors && ShadowIndex < m_NumShadowFBOs; ++i){
		for (ShadowIndex = 0; ShadowIndex<m_NumShadowFBOs; ++ShadowIndex) {
			//if (m_SelectedScene->getActor(ShadowIndex)->getID() & LIGHTSOURCE){
				//Source = dynamic_cast<LightSource*>(m_SelectedScene->getActor(ShadowIndex));
				Source = m_SelectedScene->getLight(ShadowIndex);
				sprintf(shaderstring, "Sources[%d].ProjMatrix", ShadowIndex);
				Location1 = glGetUniformLocation(m_CombineShadowProgram, shaderstring);
				glUniformMatrix4fv(Location1, 4, GL_FALSE, Source->getLightProjectionMtx());

				sprintf(shaderstring, "Sources[%d].LightViewMtx", ShadowIndex);
				Location1 = glGetUniformLocation(m_CombineShadowProgram, shaderstring);
				glUniformMatrix4fv(Location1, 4, GL_FALSE, Source->getLightViewMtx());

				sprintf(shaderstring, "Sources[%d].ShadowMap", ShadowIndex);
				Location1 = glGetUniformLocation(m_CombineShadowProgram, shaderstring);
				glActiveTexture(GL_TEXTURE0 + ShadowIndex + 1);
				glBindTexture(GL_TEXTURE_2D, m_ShadowAttachmentTexture[ShadowIndex]);
				glUniform1i(Location1, ShadowIndex + 1);

				//++ShadowIndex;

			//}
		}
		///VAOs
		glBindVertexArray (m_FinalRenderSurface->VertexArrayObject);
		glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//end
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_NONE);
		glUseProgram(0);
	}
};
void RGP_CORE::GLRenderer::RenderToScreen(){
        _s32b location = -1 ;
        _s8b shaderstring[50]="";
        LightSource* Source=NULL ;
        _u32b nbLights=0 ;
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,m_Target->getWidth(),m_Target->getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f,0.5f,0.5f,0.0f);
        ///construct and render the final result
        glUseProgram(m_FinalRenderProgram);
        ///uniforms

        ///adding light code
        if(nbLights=m_SelectedScene->getNBLights()){
			nbLights = (nbLights <= 25) ? nbLights : 25;
            ///init number of lights in the scene
            location=glGetUniformLocation(m_FinalRenderProgram,"numLights");
            glUniform1i(location,nbLights);

            for (_u32b i=0 ; i<nbLights;++i ){
                Source=m_SelectedScene->getLight(i);
                ///initializing world matrix
                sprintf(shaderstring,"Sources[%u].WorldMtx",i);
                location=glGetUniformLocation(m_FinalRenderProgram,shaderstring);
                glUniformMatrix4fv(location,4,GL_FALSE,Source->getTransMtx());

                ///init Diffuse Color ;
                sprintf(shaderstring,"Sources[%u].DiffuseColor",i);
                location=glGetUniformLocation(m_FinalRenderProgram,shaderstring);
                glUniform3fv(location,1,Source->getLightDiffuseColor());

                ///init Specular Color
                sprintf(shaderstring,"Sources[%u].SpecularColor",i);
                location=glGetUniformLocation(m_FinalRenderProgram,shaderstring);
                glUniform3fv(location,1,Source->getLightSpecularColor());

                ///init Attinuation
                sprintf(shaderstring,"Sources[%u].Distance",i);
                location=glGetUniformLocation(m_FinalRenderProgram,shaderstring);
                glUniform1f(location,Source->getLightDistance());

                ///init CutoffAngle
                sprintf(shaderstring,"Sources[%u].CutoffAngle",i);
                location=glGetUniformLocation(m_FinalRenderProgram,shaderstring);
                glUniform1f(location,Source->getLightCutoffAngle());

                ///init Shiness
                sprintf(shaderstring,"Sources[%u].Shininess",i);
                location=glGetUniformLocation(m_FinalRenderProgram,shaderstring);
                glUniform1f(location,Source->getLightShiness());

            }
        }else {
            location=glGetUniformLocation(m_FinalRenderProgram,"numLights");
            glUniform1i(location,0);
        }
        ///other uniforms
        location=glGetUniformLocation(m_FinalRenderProgram,"Diffuse");
        //if(location==-1) printf("WTW Diffuse \n");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][DIFFUSE_TEXTURE]);
        glUniform1i(location,0);

        location=glGetUniformLocation(m_FinalRenderProgram,"Specular");
        // if(location==-1) printf("WTW Specular \n");
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][SPECULAR_TEXTURE]);
        glUniform1i(location,1);

        location=glGetUniformLocation(m_FinalRenderProgram,"Shadow");
         //if(location==-1) printf("WTW Position \n");
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,m_CombinedShadowResultTexture);
        glUniform1i(location,2);

        location=glGetUniformLocation(m_FinalRenderProgram,"Normal");
         //if(location==-1) printf("WTW Normal \n");
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][NORMAL_TEXTURE]);
        glUniform1i(location,3);

        location=glGetUniformLocation(m_FinalRenderProgram,"Depth");
         //if(location==-1) printf("WTW Depth \n");
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][DEPTH_TEXTURE]);
        glUniform1i(location,4);

        location=glGetUniformLocation(m_FinalRenderProgram,"CameraPos");
         //if(location==-1) printf("WTW ProjMtx \n");
        glUniform3f(location,m_SelectedScene->getCamera()->getPosition().x,
                    m_SelectedScene->getCamera()->getPosition().y,
                    m_SelectedScene->getCamera()->getPosition().z);

        location=glGetUniformLocation(m_FinalRenderProgram,"ProjMtx");
         //if(location==-1) printf("WTW ProjMtx \n");
        glUniformMatrix4fv(location,1,GL_FALSE,m_SelectedScene->getCamera()->getProjectionMtx());

        location=glGetUniformLocation(m_FinalRenderProgram,"ViewMtx");
        //if(location==-1) printf("WTW ViewMtx \n");
        glUniformMatrix4fv(location,1,GL_FALSE,m_SelectedScene->getCamera()->getViewMtx());

        ///
        ///VAOs
        glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

        glfwSwapBuffers(m_Target->getglfwWindow());
};
_bool RGP_CORE::GLRenderer::GenBuffers(_u32b numBuffers,GLuint*    target){
    if(!target)
        return false ;
    glGenBuffers(numBuffers,target);
    if(glGetError()){
        printf("error generating Buffers\n");
        return false ;
    }
    return true ;
};
void  RGP_CORE::GLRenderer::DeleteBuffers(_u32b numBuffers,GLuint*    target){
    if(!target)
        return ;
    glDeleteBuffers(numBuffers,target);
    for(_u32b i=0 ; i<numBuffers;++i)
        target[i]=0;
};
void  RGP_CORE::GLRenderer::setBufferData(_u32b Target,_u32b SizeinByte, void* Data, _u32b flag){
    glBufferData(Target,SizeinByte,Data,flag);
    if(glGetError())
        printf("error filling buffer with Data\n");
};
void  RGP_CORE::GLRenderer::BindBuffer(_u32b Bindtype,_u32b BufferID ){
    glBindBuffer(Bindtype,BufferID);
};

///VAOs
_bool RGP_CORE::GLRenderer::GenVertexArrays(_u32b numBuffers,GLuint*    target){
    if(!target)
        return false ;
    glGenVertexArrays(numBuffers,target);
    if(glGetError())
        return false ;
    return true ;
};
void  RGP_CORE::GLRenderer::DeleteVertexArrays(_u32b numBuffers,GLuint*    target){
	if (!target){
		return;
	}
    glDeleteVertexArrays(numBuffers,target);
    for(_u32b i=0; i< numBuffers; ++i)
        target[i]=0;
};
_bool  RGP_CORE::GLRenderer::BindVertexArray(_u32b BufferID){
    glBindVertexArray(BufferID);
    if(glGetError())
        return false ;
    return true ;
};
void RGP_CORE::GLRenderer::DrawElements(GLenum mode,_u32b Count,GLenum Type,void* Offset){
    glDrawElements(mode,Count,Type,Offset);
};
///Textures
_bool RGP_CORE::GLRenderer::GenTextures(_u32b numTextures,GLuint*    target){
    if(!target)
        return false ;
    glGenTextures(numTextures,target);
    if(glGetError()){
        return false ;
    }
    for(_u32b i = 0 ; i< numTextures ; ++i){
        glBindTexture(GL_TEXTURE_2D,target[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }
    glBindTexture(GL_TEXTURE_2D,0);
    return true ;
};
void  RGP_CORE::GLRenderer::DeleteTextures(_u32b numTextures,GLuint*    target){
    if(!target)
        return ;
    glDeleteTextures(numTextures,target);
    for(_u32b i=0 ;i<numTextures;++i)
        target[i]=0;
};
void  RGP_CORE::GLRenderer::SetImageData(RGP_CORE::Image* ImageSource){
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,
                 ImageSource->Width,ImageSource->Height,
                 0,GL_RGBA,GL_UNSIGNED_BYTE,ImageSource->Pixels);

};
void  RGP_CORE::GLRenderer::SetActiveTexture(_u16b index){
    glActiveTexture(GL_TEXTURE0+index);
};
_bool RGP_CORE::GLRenderer::BindTexture(_u32b textureID){
    glBindTexture(GL_TEXTURE_2D,textureID);
    if(glGetError())
        return false ;
    return true ;
};
///ShaderProgramManagement
GLuint   RGP_CORE::GLRenderer::CreateGLProgramFromBuffer(const _s8b* VertexSource,const _s8b* FragmentSource){
    GLuint program=0,vs=0,fs=0;
	GLint error;
    vs=LoadShaderBuffer(GL_VERTEX_SHADER,VertexSource,strlen(VertexSource));
	if (!vs){
		return program;
	}
	if (strlen(FragmentSource) != 0){
		fs = LoadShaderBuffer(GL_FRAGMENT_SHADER, FragmentSource, strlen(FragmentSource));
		if (!fs){
			glDeleteShader(vs);
			return program;
		}
	}
	if (program = glCreateProgram()){
		error = glGetError();
		glAttachShader(program, vs);
		if (error = glGetError()){
			printf("error attaching Vertex shader \n");
		}
		if (fs){
			glAttachShader(program, fs);
			if (error = glGetError()){
				printf("error attaching fragment shader \n");
			}
		}
		glLinkProgram(program);
		if (error = glGetError()){
			printf("error linking error code %d \n", error);
		}
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked == 0){
			printf("error linking shader program %d  %d \n", linked, error);
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			GLchar log[1024];
			glGetProgramInfoLog(program, maxLength, &maxLength, log);
			log[maxLength] = '\0';
			printf("\n%s\n", log);
			glDeleteProgram(program);
			program = 0;
		}
	}
	else{
		printf("cannot create shader program\n");
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
};
GLuint   RGP_CORE::GLRenderer::CreateGLProgramFromFile(const _s8b* VertexFile,const _s8b* FragmentFile){
    GLuint program=0,vs=0,fs=0;
	GLuint error=0;
    vs=LoadShaderFile(GL_VERTEX_SHADER,VertexFile);
    if(!vs)
        return program ;
	if (strlen(FragmentFile) != 0){
		fs = LoadShaderFile(GL_FRAGMENT_SHADER, FragmentFile);
		if (!fs){
			glDeleteShader(vs);
			return program;
		}
	}
	glGetError();
    if(program=glCreateProgram()){
		error=glGetError();
        glAttachShader(program,vs);
		if (error=glGetError()){
			printf("error attaching shader \n");
		}
		if (fs){
			glAttachShader(program, fs);
			if (error = glGetError()){
				printf("error attaching shader \n");
			}
		}
		glLinkProgram(program);
		error=glGetError();
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if(linked==GL_FALSE){
            printf("error linking shader program %d  %d \n",linked, error);
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			GLchar log[1024];
			glGetProgramInfoLog(program, maxLength, &maxLength, log);
			log[maxLength] = '\0';
			printf("\n%s\n", log);
			glDeleteProgram(program);
            program=0 ;
        }
    }else{
        printf("cannot create shader program\n");
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
};
void    RGP_CORE::GLRenderer::DeleteGLProgram(_u32b ProgramID){
    glDeleteProgram(ProgramID);
};
_s32b   RGP_CORE::GLRenderer::GetUniformLocation(_u32b program ,_s8b*   Name){
    return glGetUniformLocation(program,Name);
};
_bool   SetUniformF(_s32b Location,_float data ){
     glUniform1f(Location,data);
     return true ;
};
_bool   RGP_CORE::GLRenderer::SetUniformFv(_s32b Location,_float* data,_u32b numElements ){
    glUniform1fv(Location,numElements,data);
    if(glGetError())
        return false ;
    return true ;
};
_bool   RGP_CORE::GLRenderer::SetUniformvMtx(_s32b Location,_float* Matrix_4x4 ){
    glUniformMatrix4fv(Location,1,GL_FALSE,Matrix_4x4);
    if(glGetError())
        return false ;
    return true ;
};
_bool   RGP_CORE::GLRenderer::SetUniformSample(_s32b Location, _u32b TextureUnit){
    glUniform1i(Location,TextureUnit);
     if(glGetError()){
        //printf("problem \n");
        return false ;
     }
    return true ;
};
_bool   RGP_CORE::GLRenderer::SetVertexAttribPointer(_u32b Index,_u32b NumElemntsPerVertex,
                               _u32b offsetBetweenElements,void* offsetFromFirst)
{
    glVertexAttribPointer(Index,NumElemntsPerVertex,GL_FLOAT,GL_FALSE,offsetBetweenElements,offsetFromFirst);
     if(glGetError())
        return false ;
    return true ;
};
_bool   RGP_CORE::GLRenderer::EnableVertexAttribArray(_u32b index){
    glEnableVertexAttribArray(index);
     if(glGetError())
        return false ;
    return true ;
};
_bool   RGP_CORE::GLRenderer::DisableVertexAttribArray(_u32b index){
    glDisableVertexAttribArray(index);
     if(glGetError())
        return false ;
    return true ;
};
void    RGP_CORE::GLRenderer::SetShaderProgram(GLuint program){
    glUseProgram(program);
};
GLuint RGP_CORE::GLRenderer::LoadShaderFile(GLenum type,const _s8b* filename){
    GLuint Shader=0 ;
    _u32b buffersize=0;
    _s8b* buffer=NULL;
    FILE* Shaderfile=NULL ;
	if (!filename)
		return 0;
    if(!(Shaderfile=fopen(filename,"rb"))){
        printf("shader file %s not found \n",filename);
        return Shader;
    }
    fflush(stdin);
    fseek(Shaderfile,0,SEEK_END);
    buffersize=ftell(Shaderfile);
    if(buffersize==0){
        fclose(Shaderfile);
        return 0;
    }
    fseek(Shaderfile,0,SEEK_SET);
    buffer=(_s8b*)malloc((buffersize+1)*sizeof(_s8b));
    if(!buffer){
        fclose(Shaderfile);
        return Shader;
    }
    fflush(stdin);
    fread(buffer,buffersize,1,Shaderfile);
	buffer[buffersize] = '\0';
    fclose(Shaderfile);
    Shader=LoadShaderBuffer(type,buffer,buffersize);
    free(buffer);
    return Shader ;

};
 GLuint RGP_CORE::GLRenderer::LoadShaderBuffer(GLenum type,const _s8b* Buffer,int buffersize){
     GLuint Shader=0;
	 if (!Buffer || !buffersize){
		 printf("error no shader buffer ");
		 return Shader;
	 }

    Shader=glCreateShader(type);
	glGetError();
    if(Shader){
        glShaderSource(Shader,1,(const _s8b**)(&Buffer),&buffersize);
		if (glGetError())
			printf("error shader source\n");
        glCompileShader(Shader);
        GLint compiled;
        glGetObjectParameterivARB(Shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled){
            printf("error compiling shader\n");
            printf("Source: %s \n",Buffer);
            GLint blen = 0;
            GLsizei slen = 0;

            glGetShaderiv(Shader, GL_INFO_LOG_LENGTH , &blen);
            if (blen > 1)
            {
                GLchar* compiler_log = (GLchar*)malloc(blen);
                glGetInfoLogARB(Shader, blen, &slen, compiler_log);
                printf("compiler_log: %s\n", compiler_log);
                free (compiler_log);
            }
            glDeleteShader(Shader);
        }
    }else{
        printf("Cannot create shader \n");
    }
    return Shader;
 };
