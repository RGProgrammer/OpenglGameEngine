#include ".//GLRenderer.h"



RGP_CORE::GLShaderProgramsManager::GLShaderProgramsManager(): m_LoadedProgramsList(NULL) , m_NumPrograms(0), m_Size(0),
															m_boundProgramID(0)
{};
RGP_CORE::GLShaderProgramsManager::~GLShaderProgramsManager()
{
	this->Destroy();
};
void RGP_CORE::GLShaderProgramsManager::Destroy()
{
	if (m_LoadedProgramsList) {
		for (_u32b i = 0; i < m_NumPrograms; ++i) {
			if (m_LoadedProgramsList[i].VertexShaderFileName)
				free(m_LoadedProgramsList[i].VertexShaderFileName);
			if (m_LoadedProgramsList[i].GeometryShaderFileName)
				free(m_LoadedProgramsList[i].GeometryShaderFileName);
			if (m_LoadedProgramsList[i].FragmentShaderFileName)
				free(m_LoadedProgramsList[i].FragmentShaderFileName);
			if (m_LoadedProgramsList[i].OneForAllFileName)
				free(m_LoadedProgramsList[i].OneForAllFileName);
			if(m_LoadedProgramsList[i].GLProgramID)
				glDeleteProgram(m_LoadedProgramsList[i].GLProgramID);
		}
		free(m_LoadedProgramsList);
		m_LoadedProgramsList = NULL;
	}
	m_NumPrograms = 0;
	m_Size = 0;
};
_u32b RGP_CORE::GLShaderProgramsManager::CreateProgramFromOneFile(_s8b* Filename)
{
	return 0;
};
_u32b RGP_CORE::GLShaderProgramsManager::CreateProgramFromFiles(_s8b* VertexFile, _s8b* FrgamentFile)
{
	return CreateProgramFromFiles(VertexFile, NULL, FrgamentFile);
};
_u32b RGP_CORE::GLShaderProgramsManager::CreateProgramFromFiles(_s8b* VertexFile, _s8b* GeometryFile, _s8b* FragmentFile)
{
	if (VertexFile==NULL)
		return 0;
	_u32b ID=0;
	if (ID = doExiste(VertexFile, GeometryFile, FragmentFile))
		return ID;
	if (m_Size == m_NumPrograms) {
		ShaderProgram* tmp = (ShaderProgram*)malloc((m_Size + 1) * sizeof(ShaderProgram));
		if (!tmp)
			return 0;
		for (_u32b i = 0; i < m_NumPrograms; ++i) 
			tmp[i] = m_LoadedProgramsList[i];
		free(m_LoadedProgramsList);
		m_LoadedProgramsList = tmp;
		++m_Size;
	}
	m_LoadedProgramsList[m_NumPrograms].OneForAllFileName = NULL;
	m_LoadedProgramsList[m_NumPrograms].VertexShaderFileName = NULL;
	m_LoadedProgramsList[m_NumPrograms].GeometryShaderFileName = NULL;
	m_LoadedProgramsList[m_NumPrograms].FragmentShaderFileName = NULL;
	m_LoadedProgramsList[m_NumPrograms].GLProgramID = 0;
	++m_NumPrograms;
	m_LoadedProgramsList[m_NumPrograms-1].VertexShaderFileName = (_s8b*)malloc((strlen(VertexFile) + 1) * sizeof(_s8b));
	if (!m_LoadedProgramsList[m_NumPrograms - 1].VertexShaderFileName) {
		return 0;
	}
	else {
		strcpy(m_LoadedProgramsList[m_NumPrograms - 1].VertexShaderFileName, VertexFile);
	}
	if (GeometryFile) {
		m_LoadedProgramsList[m_NumPrograms - 1].GeometryShaderFileName = (_s8b*)malloc((strlen(GeometryFile) + 1) * sizeof(_s8b));
		if (!m_LoadedProgramsList[m_NumPrograms - 1].GeometryShaderFileName) {
			this->DeleteProgram(m_NumPrograms - 1);
			return 0;
		}
		else {
			strcpy(m_LoadedProgramsList[m_NumPrograms - 1].GeometryShaderFileName, GeometryFile);
		}
	}
	if (FragmentFile) {
		m_LoadedProgramsList[m_NumPrograms - 1].FragmentShaderFileName = (_s8b*)malloc((strlen(FragmentFile) + 1) * sizeof(_s8b));
		if (!m_LoadedProgramsList[m_NumPrograms - 1].FragmentShaderFileName) {
			this->DeleteProgram(m_NumPrograms);
			return 0;
		}
		else {
			strcpy(m_LoadedProgramsList[m_NumPrograms - 1].FragmentShaderFileName, FragmentFile);
		}
	}
	m_LoadedProgramsList[m_NumPrograms-1].GLProgramID = CreateGLProgram(VertexFile, GeometryFile, FragmentFile);
	if (!m_LoadedProgramsList[m_NumPrograms - 1].GLProgramID) {
		this->DeleteProgram(m_NumPrograms);
		return 0;
	}
	return m_NumPrograms;
};
void  RGP_CORE::GLShaderProgramsManager::BindProgram(_u32b ID)
{
	if (ID != m_boundProgramID) {
		if (ID == 0) {
			glUseProgram(0);
			m_boundProgramID = 0;
			return;
		}
		if (ID <= m_NumPrograms) {
			glUseProgram(m_LoadedProgramsList[ID - 1].GLProgramID);
			m_boundProgramID = ID;
		}
	}
};
void  RGP_CORE::GLShaderProgramsManager::DeleteProgram(_u32b ID)
{
	if (ID > 0 && ID <= m_NumPrograms) {
		
		if (m_LoadedProgramsList[ID - 1].FragmentShaderFileName) {
			free(m_LoadedProgramsList[ID - 1].FragmentShaderFileName);
			m_LoadedProgramsList[ID - 1].FragmentShaderFileName = NULL;
		}
		if (m_LoadedProgramsList[ID - 1].GeometryShaderFileName) {
			free(m_LoadedProgramsList[ID - 1].GeometryShaderFileName);
			m_LoadedProgramsList[ID - 1].GeometryShaderFileName = NULL;
		}
		if (m_LoadedProgramsList[ID - 1].VertexShaderFileName) {
			free(m_LoadedProgramsList[ID - 1].VertexShaderFileName);
			m_LoadedProgramsList[ID - 1].VertexShaderFileName = NULL;
		}
		if (m_LoadedProgramsList[ID - 1].OneForAllFileName) {
			free(m_LoadedProgramsList[ID - 1].OneForAllFileName);
			m_LoadedProgramsList[ID - 1].OneForAllFileName = NULL;
		}
		if (m_LoadedProgramsList[ID - 1].GLProgramID) {
			glDeleteProgram(m_LoadedProgramsList[ID - 1].GLProgramID);
			m_LoadedProgramsList[ID - 1].GLProgramID = 0;
		}
		for (_u32b i = ID - 1; i < m_NumPrograms-1; ++i) {
			m_LoadedProgramsList[i] = m_LoadedProgramsList[i + 1];
		}
		if (m_boundProgramID == ID) {
			m_boundProgramID = 0;
			glUseProgram(0);
		}
		else if (m_boundProgramID > ID)
			--m_boundProgramID; 
		--m_NumPrograms;
		m_LoadedProgramsList[m_NumPrograms].FragmentShaderFileName = NULL;
		m_LoadedProgramsList[m_NumPrograms].GeometryShaderFileName = NULL;
		m_LoadedProgramsList[m_NumPrograms].VertexShaderFileName = NULL;
		m_LoadedProgramsList[m_NumPrograms].OneForAllFileName = NULL;
		m_LoadedProgramsList[m_NumPrograms].GLProgramID = 0;
	}
};
_u32b RGP_CORE::GLShaderProgramsManager::LoadShaderBuffer(GLenum type, const _s8b* Buffer, int buffersize) {
	GLuint Shader = 0;
	if (!Buffer || !buffersize) {
		printf("error no shader buffer ");
		return Shader;
	}

	Shader = glCreateShader(type);
	if (Shader) {
		glShaderSource(Shader, 1, (const _s8b**)(&Buffer), &buffersize);
		glCompileShader(Shader);
		GLint compiled;
		glGetObjectParameterivARB(Shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			printf("error compiling shader\n");
			printf("Source: %s \n", Buffer);
			GLint blen = 0;
			GLsizei slen = 0;

			glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &blen);
			if (blen > 1)
			{
				GLchar* compiler_log = (GLchar*)malloc(blen);
				glGetInfoLogARB(Shader, blen, &slen, compiler_log);
				printf("compiler_log: %s\n", compiler_log);
				free(compiler_log);
			}
			glDeleteShader(Shader);
		}
	}
	else {
		printf("Cannot create shader \n");
	}
	return Shader;
};
_u32b RGP_CORE::GLShaderProgramsManager::LoadShaderFile(GLenum Type, _s8b* filename)
{
	GLuint Shader = 0;
	_u32b buffersize = 0;
	_s8b* buffer = NULL;
	FILE* Shaderfile = NULL;
	if (!filename)
		return 0;
	fflush(stdin);
	if (!(Shaderfile = fopen(filename, "rb"))) {
		printf("shader file %s not found \n", filename);
		return Shader;
	}
	
	fseek(Shaderfile, 0, SEEK_END);
	buffersize = ftell(Shaderfile);
	if (buffersize == 0) {
		printf("empty file\n");
		fclose(Shaderfile);
		return 0;
	}
	fseek(Shaderfile, 0, SEEK_SET);
	buffer = (_s8b*)malloc((buffersize + 1) * sizeof(_s8b));
	if (!buffer) {
		fclose(Shaderfile);
		return Shader;
	}
	fflush(stdin);
	fread(buffer, buffersize, 1, Shaderfile);
	buffer[buffersize] = '\0';
	fclose(Shaderfile);
	Shader = LoadShaderBuffer(Type, buffer, buffersize);
	free(buffer);
	return Shader;
};

GLuint RGP_CORE::GLShaderProgramsManager::CreateGLProgram(_s8b* VertexFile, _s8b* GeometryFile, _s8b* FragmentFile)
{
	GLuint program = 0, vs = 0, gs = 0, fs = 0;
	GLuint error = 0;
	vs = LoadShaderFile(GL_VERTEX_SHADER, VertexFile);
	if (!vs) {
		printf("error loading vertex shader\n");
		return program;
	}
	if (GeometryFile) {
		if (strlen(GeometryFile) != 0) {
			gs = LoadShaderFile(GL_GEOMETRY_SHADER, GeometryFile);
			if (!gs) {
				glDeleteShader(vs);
				return program;
			}
		}
	}
	if (FragmentFile)
		if (strlen(FragmentFile) != 0) {
			fs = LoadShaderFile(GL_FRAGMENT_SHADER, FragmentFile);
			if (!fs) {
				glDeleteShader(vs);
				if (gs) {
					glDeleteShader(gs);
				}
				return program;
			}
		}
	glGetError();
	if (program = glCreateProgram()) {
		error = glGetError();
		glAttachShader(program, vs);
		if (error = glGetError()) {
			printf("error attaching shader \n");
		}
		if (gs) {
			glAttachShader(program, gs);
			if (error = glGetError()) {
				printf("error attaching shader \n");
			}
		}
		if (fs) {
			glAttachShader(program, fs);
			if (error = glGetError()) {
				printf("error attaching shader \n");
			}
		}
		glLinkProgram(program);
		error = glGetError();
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE) {
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
	else {
		printf("cannot create shader program\n");
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
	return program;
};

_u32b RGP_CORE::GLShaderProgramsManager::doExiste(_s8b* VertexFile, _s8b* GeometryFile, _s8b* FragmentFile)
{
	//IF THE SHADER PROGRM IS LOADED
	for (_u32b i = 0; i < m_NumPrograms; i++)
		if (strcmp(VertexFile, m_LoadedProgramsList[i].VertexShaderFileName) == 0)
			return i + 1;
	return 0;
};
RGP_CORE::GLShaderProgramsManager::ShaderProgram* 
				RGP_CORE::GLShaderProgramsManager::getShaderProgram(_u32b programID)
{
	if (programID > 0 && programID <= m_NumPrograms)
		return &m_LoadedProgramsList[programID-1];
	return 0;
};


_u32b	RGP_CORE::GLRenderer::getLastFrameTexture()
{
	if (m_SelectedFBO - 1 < 0)
		return m_AttachmentTextures[0][DIFFUSE_TEXTURE];
	else
		return m_AttachmentTextures[m_SelectedFBO - 1][DIFFUSE_TEXTURE];
};



RGP_CORE::GLRenderer::GLRenderer(RenderMode Type):m_Target(NULL),
											m_isInitialized(false) ,
											m_SelectedScene(NULL),m_ShaderManager(NULL),
											m_NumFBOs(0),m_FBOs(NULL),m_SelectedFBO(0),
                                            m_AttachmentTextures(NULL),

                                            m_FinalRenderSurface(NULL),
                                            m_FinalRenderProgram(0),
											
											m_LightAccumProgram(0),
											m_LightAccumBuffer(0),
											m_LightAccumDiffuseTexture(0),
											m_LightAccumSpecularTexture(0),

											m_ShadowVectorSize(0),m_NumShadowFBOs(0),
											m_ShadowRenderingProgram(0),
											m_ShadowFBOs(NULL),
											m_ShadowAttachmentTexture(NULL),
											m_ShadowAccumBuffer(0),
											m_ShadowAccumTexture(0),
											m_ShadowAccumProgram(0),
											m_noLightMode(false)
											
{
    if(Type==DEFERRED_RENDERING || Type==FORWARD_RENDERING)
        m_Mode=Type ;
    else
        m_Mode=DEFERRED_RENDERING;

	DrawBuff[0] = GL_COLOR_ATTACHMENT0;
	DrawBuff[1] = GL_COLOR_ATTACHMENT1;
	DrawBuff[2] = GL_COLOR_ATTACHMENT2;
	DrawBuff[3] = GL_COLOR_ATTACHMENT3;
	DrawBuff[4] = GL_COLOR_ATTACHMENT4;

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
	if (m_LightAccumProgram) {
		glDeleteProgram(m_LightAccumProgram);
		m_LightAccumProgram = 0;
	}
	if (m_LightAccumBuffer) {
		glDeleteFramebuffers(1, &m_LightAccumBuffer);
		m_LightAccumBuffer = 0;
	}
	if (m_LightAccumDiffuseTexture) {
		glDeleteTextures(1, &m_LightAccumDiffuseTexture);
		m_LightAccumDiffuseTexture = 0;
	}
	if (m_LightAccumSpecularTexture) {
		glDeleteTextures(1, &m_LightAccumSpecularTexture);
		m_LightAccumSpecularTexture = 0;
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
	if (m_ShadowAccumBuffer){
		if (m_ShadowAccumTexture){
			glDeleteTextures(1, &m_ShadowAccumTexture);
			m_ShadowAccumTexture = 0;
		}
		glDeleteFramebuffers(1, &m_ShadowAccumBuffer);
		m_ShadowAccumBuffer = 0;
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
	if (m_ShaderManager) {
		delete m_ShaderManager;
		m_ShaderManager = NULL;
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
    m_Target=Window::Create(Config.Title,Config.Witdh,Config.Height);
    if(!m_Target){
        printf("error creating Window\n");
        return false ;
    }
	this->MakeContext();
    ///initializing GLEW
    if(glewInit()!=GLEW_OK){
        printf("error init GLEW\n");
        return false ;
	}
	glfwSwapInterval(0);
	m_ShaderManager = new GLShaderProgramsManager();
	if (!m_ShaderManager) {
		printf("Fatal error\n");
		return false;
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
        return false ;
    }
    //init rendering thread 
	
    m_isInitialized=true ;
    return true ;
};
_bool RGP_CORE::GLRenderer::CreateNeededObjects(){
	if (!CreateColorsObjects())
		return false;
	if (!CreateLightObjects())
		return false;
	if (!CreateShadowsObjects())
		return false;
	return true;
};

_bool	RGP_CORE::GLRenderer::CreateColorsObjects()
{
	int error;
	///generating textures(Diffuse,Specular+Roughness(RGBA),Normal,Depth+Stencil,shadow,trancprency)==>6maps;
	m_AttachmentTextures = (GLuint**)malloc(m_NumFBOs * sizeof(GLuint*));
	if (!m_AttachmentTextures) {
		printf("allocation error \n");
		return false;
	}
	for (_s16b i = 0; i<m_NumFBOs; ++i)
		m_AttachmentTextures[i] = NULL;
	///generating textures for each FBO
	for (_s16b i = 0; i<m_NumFBOs; ++i) {
		m_AttachmentTextures[i] = (GLuint*)malloc(6 * sizeof(GLuint));
		if (!m_AttachmentTextures[i]) {
			printf("error allocation 2 \n");
			return false;
		}
		for (_s16b k = 0; k<6; ++k) {
			m_AttachmentTextures[i][k] = 0;
		}
		glGetError();
		glGenTextures(6, m_AttachmentTextures[i]);
		if (error = glGetError())
			printf("error generating textures %u\n", i);
		///Depth texture+stencil
		glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[i][0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGetError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
			m_Target->getWidth(),
			m_Target->getHeight(),
			0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		///THE OTHER TEXTURES
		for (_s16b k = 1; k<6; ++k) {
			glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[i][k]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
				m_Target->getWidth(),
				m_Target->getHeight(),
				0, GL_RGBA, GL_FLOAT, 0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	///create FBO and their render Buffer(Textures : Deffuse, Normal, Depth,Sepecular, Transparency(nOT YET) )
	///generating FBOs
	m_FBOs = (GLuint*)malloc(m_NumFBOs * sizeof(GLuint));
	if (!m_FBOs) {
		printf("error allcation memory for FBOs\n");
		return false;
	}
	glGetError();
	glGenFramebuffers(m_NumFBOs, m_FBOs);
	if (error = glGetError()) {
		printf("error generating Framebuffers %d\n", error);
		return false;
	}
	for (int m = 0; m < m_NumFBOs; m++)
		if (m_FBOs[m] == 0)
			printf("error geerating Frame bufferss \n");
	if (!AttachColorsTextures())
		return false;
	return true;
};

_bool RGP_CORE::GLRenderer::AttachColorsTextures() {
	int error;
	///attaching texture to its FBO
	for (_s16b i = 0; i < m_NumFBOs; ++i) {
		glGetError();
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[i]);
		if (error = glGetError()) {
			printf("error binding framebuffer = %u error %d\n", m_FBOs[i], error);
			continue;
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_AttachmentTextures[i][DIFFUSE_TEXTURE], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_AttachmentTextures[i][SPECULAR_TEXTURE], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AttachmentTextures[i][NORMAL_TEXTURE], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_AttachmentTextures[i][MATERIAL_TEXTURE], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_AttachmentTextures[i][POSITION_TEXTURE], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_AttachmentTextures[i][DEPTH_TEXTURE], 0);
		error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (GL_FRAMEBUFFER_COMPLETE != error) {
			printf("frameBuffer attacehment error code %x\n", error);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
};

_bool	RGP_CORE::GLRenderer::CreateShadowsObjects()
{

	m_ShadowRenderingProgram = CreateGLProgramFromFile("..//Shaders//ShadowMapProgram.vs", NULL);
	if (!m_ShadowRenderingProgram) {
		printf("error loading Shadow Mapping program");
		return false;
	}
	m_ShadowAccumProgram = CreateGLProgramFromFile("..//Shaders//ShadowAccumProgram.vs", "..//Shaders//ShadowAccumProgram.fs");
	if (!m_ShadowAccumProgram) {
		printf("error loading Shadow Maps Combiner program");
		return false;
	}

	glGenFramebuffers(1, &m_ShadowAccumBuffer);
	if (m_ShadowAccumBuffer == 0) {
		printf("error generating FBO for Shadows \n");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowAccumBuffer);
	glGenTextures(1, &m_ShadowAccumTexture);
	glBindTexture(GL_TEXTURE_2D, m_ShadowAccumTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		m_Target->getWidth(),
		m_Target->getHeight(),
		0, GL_RGBA, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ShadowAccumTexture, 0);
	glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
};
_bool	RGP_CORE::GLRenderer::CreateLightObjects()
{
	int error;
	m_LightAccumProgram = CreateGLProgramFromFile("..//Shaders//LightAccumProgram.vs", "..//Shaders//LightAccumProgram.fs");
	if (!m_LightAccumProgram) {
		printf("error loading Light program");
		return false;
	}
	glGenFramebuffers(1, &m_LightAccumBuffer);
	if (m_LightAccumBuffer == 0) {
		printf("error generating FBO for light accumulation \n");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_LightAccumBuffer);
	glGenTextures(1, &m_LightAccumDiffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_LightAccumDiffuseTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		m_Target->getWidth(),
		m_Target->getHeight(),
		0, GL_RGBA, GL_FLOAT, 0);
	glGenTextures(1, &m_LightAccumSpecularTexture);
	glBindTexture(GL_TEXTURE_2D, m_LightAccumSpecularTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		m_Target->getWidth(),
		m_Target->getHeight(),
		0, GL_RGBA, GL_FLOAT, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_LightAccumDiffuseTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_LightAccumSpecularTexture, 0);
	error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != error) {
		printf("frameBuffer attacehment error code %x\n", error);
	}
	glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0,0.0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
};
_bool RGP_CORE::GLRenderer::InitFinalPhase(){
    _float vertexBuffer[8]={    -1.0f,1.0f,
                                1.0f,1.0f,
                                1.0f,-1.0f,
                                -1.0f,-1.0f,};
    _float TexCoordBuffer[8]={0.0f,1.0f,
                            1.0f,1.0f,
                            1.0f,0.0f,
                            0.0f,0.0f};
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
    glBindBuffer(GL_ARRAY_BUFFER, m_FinalRenderSurface->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,8*sizeof(_float),vertexBuffer,GL_STATIC_DRAW);


    glGenBuffers(1,&(m_FinalRenderSurface->TexCoords));
    glBindBuffer(GL_ARRAY_BUFFER, m_FinalRenderSurface->TexCoords);
    glBufferData(GL_ARRAY_BUFFER,8*sizeof(_float),TexCoordBuffer,GL_STATIC_DRAW);


    glGenBuffers(1,&(m_FinalRenderSurface->IndexBuffer));
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


void		RGP_CORE::GLRenderer::MakeContext()
{
	glfwMakeContextCurrent(m_Target->getglfwWindow());
};


void  RGP_CORE::GLRenderer::setScene(GameScene*   Scene){ 
	m_SelectedScene= Scene ;
	_u32b NumShadowmaps = 0;
	GLuint* tmp = NULL;
	LightSource* Source = NULL;
	if (m_SelectedScene){
		NumShadowmaps= m_SelectedScene->getNumLights();
		
		int error;
		_u32b numLights = m_SelectedScene->getNumLights();
		for (_u32b i = 0; i < numLights; ++i) {
			Source=m_SelectedScene->getLight(i);
			if (Source->getLightCutoffAngle() < 0.0)
				NumShadowmaps += 5;
		}

		if (NumShadowmaps > m_ShadowVectorSize){
			tmp = (GLuint*)malloc(NumShadowmaps *sizeof(GLuint));//first we need new FBos
			if (!tmp)
				return;
			for (_u32b i = 0; i < m_NumShadowFBOs; ++i)
				tmp[i] = m_ShadowFBOs[i];
			free(m_ShadowFBOs);
			m_ShadowFBOs = tmp;

			tmp = (GLuint*)malloc(NumShadowmaps *sizeof(GLuint));//next Attachent texture for eac fbo
			if (!tmp)
				return;
			for (_u32b i = 0; i < m_NumShadowFBOs; ++i)
				tmp[i] = m_ShadowAttachmentTexture[i];
			free(m_ShadowAttachmentTexture);
			m_ShadowAttachmentTexture = tmp;

			//generate and attach
			glGenFramebuffers(NumShadowmaps - m_NumShadowFBOs, &m_ShadowFBOs[m_NumShadowFBOs]);
			glGenTextures(NumShadowmaps - m_NumShadowFBOs, &m_ShadowAttachmentTexture[m_NumShadowFBOs]);
			
			for (_u32b i = m_NumShadowFBOs; i < NumShadowmaps; ++i){
				glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBOs[i]);
				glBindTexture(GL_TEXTURE_2D, m_ShadowAttachmentTexture[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
					m_Config.ShadowResolution,
					m_Config.ShadowResolution,
					0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowAttachmentTexture[i],0);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (GL_FRAMEBUFFER_COMPLETE != error){
					printf("frameBuffer attacehment error code %x\n", error);
				}
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			m_ShadowVectorSize = NumShadowmaps;
		}
		m_NumShadowFBOs = NumShadowmaps;
	}
};

void	RGP_CORE::GLRenderer::RenderScene(_u32b FBO_Target,Camera* camera)
{

	//RENDER SCENE Colors to the SelectedFBO
	this->RenderSceneColors(m_FBOs[m_SelectedFBO],camera);
	if (!m_noLightMode) {
		////if should draw shadows
		if (m_Config.EnableShadows == true) {
			this->RenderSceneShadows(m_ShadowAccumBuffer, camera);
		}
		this->RenderSceneLightAccum();
	}
	///combine results and render to the screen ;
	this->RenderToTarget(FBO_Target);
	//this->RenderSceneUI(FBO_Target);
	//Next frame
	if (m_NumFBOs - 1 == m_SelectedFBO)
		m_SelectedFBO = 0;
	else
		++m_SelectedFBO;

};

void  RGP_CORE::GLRenderer::RenderCurrentScene()
{
	this->RenderScene(0);
};
void	RGP_CORE::GLRenderer::LoadShadowProgram()
{
	m_ShaderManager->BindProgram(m_ShadowRenderingProgram);
};
void	RGP_CORE::GLRenderer::UnloadShadowProgram()
{
	m_ShaderManager->BindProgram(0);
};
void RGP_CORE::GLRenderer::RenderSceneColors(_u32b FBO,Camera *camera)
{
	_u32b nbActors = 0;
	BaseActor*  actor = NULL;
	Camera* Eye=NULL;
	
	glGetError();
	if (m_SelectedScene){
		if (camera) {
			Eye = camera;
		}
		else {
			Eye = m_SelectedScene->getCamera();
		}

		nbActors = m_SelectedScene->getNumActors();
		///first step render to current FBO
		glGetError();
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
		glDrawBuffers(5, DrawBuff);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		///glClear attachements
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		///start searching for renderable objects and rendering
		for (_u32b i = 0; i < nbActors; ++i){
			actor = m_SelectedScene->getActor(i);
			if (actor->getID() & RENDERABLE) {
				dynamic_cast<Renderable*>(actor)->Render(Eye);
			}
		}
		//glFinish();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_SMOOTH);
		glDisable(GL_BLEND);
		
	}
};
void RGP_CORE::GLRenderer::RenderSceneShadows(_u32b FBO, Camera* camera)
{
	Camera* Eye = NULL ;
	
	if (m_SelectedScene && m_Config.EnableShadows){
		_u32b NumActors = m_SelectedScene->getNumActors();
		_u32b ShadowIndex = 0, numLights = m_SelectedScene->getNumLights();
		_s32b Location1 = -1, Location2 = -1, Location3 = -1;
		LightSource*	Source = NULL;
		Renderable*		actor = NULL;
		if (numLights == 0)
			return;
		if (camera) {
			Eye = camera;
		}
		else {
			Eye = m_SelectedScene->getCamera();
		}
		
		m_ShaderManager->BindProgram(m_ShadowRenderingProgram);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		Location1 = this->GetUniformLocation(m_ShadowRenderingProgram, "World");
		Location2 = this->GetUniformLocation(m_ShadowRenderingProgram, "View");
		Location3 = this->GetUniformLocation(m_ShadowRenderingProgram, "Projection");
	
		for(_u32b Index = 0 ; Index < numLights; ++Index){
				Source = m_SelectedScene->getLight(Index);
					glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBOs[ShadowIndex]);
					glDrawBuffer(GL_NONE);
					glReadBuffer(GL_NONE);
					glViewport(0, 0, m_Config.ShadowResolution, m_Config.ShadowResolution);
					glClear(GL_DEPTH_BUFFER_BIT);
					this->SetUniformvMtx(Location2, Source->getLightViewMtx());
					this->SetUniformvMtx(Location3, Source->getLightProjectionMtx(Eye));

					for (_u32b j = 0; j < NumActors; ++j)
						if (m_SelectedScene->getActor(j)->getID() & RENDERABLE) {
							actor = dynamic_cast<Renderable*>(m_SelectedScene->getActor(j));
							this->SetUniformvMtx(Location1, actor->getTransMtx());
							actor->CastShadow();
						}
					++ShadowIndex;
					if (Source->getLightCutoffAngle() < 0.0) {
						for (_u8b k = 1; k <6; ++k) {
							glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBOs[ShadowIndex]);
							glDrawBuffer(GL_NONE);
							glReadBuffer(GL_NONE);
							glViewport(0, 0, m_Config.ShadowResolution, m_Config.ShadowResolution);
							glClear(GL_DEPTH_BUFFER_BIT);
							this->SetUniformvMtx(Location2, Source->getLightViewMtx()+k*16);
							for (_u32b j = 0; j < NumActors; ++j)
								if (m_SelectedScene->getActor(j)->getID() & RENDERABLE) {
									actor = dynamic_cast<Renderable*>(m_SelectedScene->getActor(j));
									this->SetUniformvMtx(Location1, actor->getTransMtx());
									actor->CastShadow();
								}
							++ShadowIndex;
						}
					}

		}
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);


		//combine Shadow map to a single one
		m_ShaderManager->BindProgram(m_ShadowAccumProgram);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f,0.0f);
		glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
		glEnable(GL_BLEND);
		glBlendEquation(GL_MAX);
		ShadowIndex = 0;
		Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "PositionMap");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[m_SelectedFBO][POSITION_TEXTURE]);
		glUniform1i(Location1, 0);

		for (_u32b Index = 0; Index < numLights; ++Index) {

			Source = m_SelectedScene->getLight(Index);

			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "lighttype");
			if (Source->getLightDistance() < 0.0f)//Directionnal light
				glUniform1i(Location1, 1);
			else if (Source->getLightCutoffAngle()<0.0f)// Point light
				glUniform1i(Location1, 2);
			else                              //Spot light
				glUniform1i(Location1, 3);
			

			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "shadowStrengh");
			this->SetUniformF(Location1, Source->getShadowStrengh());

			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "LightProjMatrix");
			this->SetUniformvMtx(Location1, Source->getLightProjectionMtx(Eye));

			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "LightViewMtx");
			this->SetUniformvMtx(Location1, Source->getLightViewMtx());

			
			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "ShadowMap");
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_ShadowAttachmentTexture[ShadowIndex]);
			glUniform1i(Location1, 1);


			///VAOs
			glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			++ShadowIndex;
			if (Source->getLightCutoffAngle() < 0.0) {
				for (_u8b k = 1; k < 6; ++k) {
					Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "LightViewMtx");
					this->SetUniformvMtx(Location1, Source->getLightViewMtx() + k * 16);

					Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "ShadowMap");
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, m_ShadowAttachmentTexture[ShadowIndex]);
					glUniform1i(Location1, 1);

					///VAOs
					glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
					++ShadowIndex;
				}
			}
		}

		//end
		glDisable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_NONE);
		m_ShaderManager->BindProgram(0);
	}
};
void	RGP_CORE::GLRenderer:: RenderSceneLightAccum(Camera* camera)
{
	GLint Location = -1;
	LightSource* Source = NULL;
	Camera *Eye = camera;
	if (!Eye)
		Eye = m_SelectedScene->getCamera();
	if (m_SelectedScene) {
		if (m_SelectedScene->getNumLights() == 0) {
			return;
		}
		this->BindFrameBuffer(m_LightAccumBuffer);
		this->SetShaderProgram(m_LightAccumProgram);
		glDrawBuffers(2, DrawBuff);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);
		glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		
		
		//common uniform variables
		Location = this->GetUniformLocation(m_LightAccumProgram, "NormalMap");
		this->SetActiveTexture(0);
		this->BindTexture(m_AttachmentTextures[m_SelectedFBO][NORMAL_TEXTURE]);
		this->SetUniformSample(Location, 0);
		Location = this->GetUniformLocation(m_LightAccumProgram, "Depth");
		this->SetActiveTexture(1);
		this->BindTexture(m_AttachmentTextures[m_SelectedFBO][DEPTH_TEXTURE]);
		this->SetUniformSample(Location, 1);
		Location = this->GetUniformLocation(m_LightAccumProgram, "CameraViewMtx");
		this->SetUniformvMtx(Location, Eye->getViewMtx());
		Location = this->GetUniformLocation(m_LightAccumProgram, "CameraProjMtx");
		this->SetUniformvMtx(Location, Eye->getProjectionMtx());
		Location = this->GetUniformLocation(m_LightAccumProgram, "CameraPos");
		this->SetUniform3F(Location, Eye->getPosition().x,
			Eye->getPosition().y,
			Eye->getPosition().z);


		//for each light
		for (_u32b i = 0; i<m_SelectedScene->getNumLights(); ++i) {
			Source = m_SelectedScene->getLight(i);
			//specific light uniform variables
			///initializing world matrix
			Location = this->GetUniformLocation(m_LightAccumProgram, "Source.WorldMtx");
			this->SetUniformvMtx(Location, Source->getTransMtx());
			
			///init Diffuse Color ;
			Location = this->GetUniformLocation(m_LightAccumProgram, "Source.DiffuseColor");
			glUniform3fv(Location, 1, Source->getLightDiffuseColor());

			///init Specular Color
			Location = this->GetUniformLocation(m_LightAccumProgram, "Source.SpecularColor");
			glUniform3fv(Location, 1, Source->getLightSpecularColor());

			///init Attinuation
			Location = this->GetUniformLocation(m_LightAccumProgram, "Source.Distance");
			glUniform1f(Location, Source->getLightDistance());

			///init CutoffAngle
			Location = this->GetUniformLocation(m_LightAccumProgram, "Source.CutoffAngle");
			glUniform1f(Location, Source->getLightCutoffAngle());

			///init Shiness
			Location = this->GetUniformLocation(m_LightAccumProgram, "Source.Shininess");
			glUniform1f(Location, Source->getLightShiness());
			//render
			glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}


		glDisable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glActiveTexture(GL_NONE);
		m_ShaderManager->BindProgram(0);

	}
};

void RGP_CORE::GLRenderer::RenderToTarget(_u32b FBO_Target){
        _s32b location = -1 ;
        _s8b shaderstring[50]="";
		glGetError();
        glBindFramebuffer(GL_FRAMEBUFFER, FBO_Target);

		glDrawBuffers(1, DrawBuff);
        glViewport(0,0,m_Target->getWidth(),m_Target->getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8f,0.8f,0.8f,0.0f);
        ///construct and render the final result
		m_ShaderManager->BindProgram(m_FinalRenderProgram);

		
        ///other uniforms
        location= this->GetUniformLocation(m_FinalRenderProgram,"Diffuse");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][DIFFUSE_TEXTURE]);
        glUniform1i(location,0);

        location= this->GetUniformLocation(m_FinalRenderProgram,"Specular");
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][SPECULAR_TEXTURE]);
        glUniform1i(location,1);


        location= this->GetUniformLocation(m_FinalRenderProgram,"Depth");
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][DEPTH_TEXTURE]);
        glUniform1i(location,2);

		location = this->GetUniformLocation(m_FinalRenderProgram, "Shadow");
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_ShadowAccumTexture);
		glUniform1i(location,3);

		location = this->GetUniformLocation(m_FinalRenderProgram, "LightDiffuse");
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_LightAccumDiffuseTexture);
		glUniform1i(location, 4);

		location = this->GetUniformLocation(m_FinalRenderProgram, "LightSpecular");
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_LightAccumSpecularTexture);
		glUniform1i(location, 5);


		location = this->GetUniformLocation(m_FinalRenderProgram, "noLight");
		if (m_noLightMode)
			glUniform1i(location, 1);
		else
			glUniform1i(location, 0);
        ///
        ///VAOs
        glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


        glBindVertexArray(0);
		m_ShaderManager->BindProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

};


void	RGP_CORE::GLRenderer::UpdateEnvironmentMaps()
{

};


void	RGP_CORE::GLRenderer::SwitchNoLightMode()
{
	if (m_noLightMode)
		m_noLightMode = false;
	else
		m_noLightMode = true;
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
void   RGP_CORE::GLRenderer::setBufferSubData(_u32b Target, _u32b Offset, _u32b Size, void* Data)
{
	int error;
	glGetError();
	glBufferSubData(Target, Offset, Size, Data);
	if (error = glGetError())
		printf("error filling buffer with sub Data %x\n", error);
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
void RGP_CORE::GLRenderer::DrawArrays(GLenum mode, _u32b first, _u32b count)
{
	glDrawArrays(mode, first, count);
};
///Textures
_bool RGP_CORE::GLRenderer::GenTextures2D(_u32b numTextures,GLuint*    target){
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
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glBindTexture(GL_TEXTURE_2D,0);
    return true ;
};
_bool RGP_CORE::GLRenderer::GenTexturesCube(_u32b numTextures, GLuint*    target)
{
	if (!target)
		return false;
	glGenTextures(numTextures, target);
	if (glGetError()) {
		return false;
	}
	for (_u32b i = 0; i< numTextures; ++i) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, target[i]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
};
void  RGP_CORE::GLRenderer::DeleteTextures(_u32b numTextures,GLuint*    target){
    if(!target)
        return ;
    glDeleteTextures(numTextures,target);
    for(_u32b i=0 ;i<numTextures;++i)
        target[i]=0;
};
void  RGP_CORE::GLRenderer::SetImageData2D(RGP_CORE::Image* ImageSource){
    if(ImageSource)
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,
                 ImageSource->Width,ImageSource->Height,
                 0,GL_RGBA,GL_UNSIGNED_BYTE,ImageSource->Pixels);

};
void  RGP_CORE::GLRenderer::SetImageDataCube(RGP_CORE::Image* right , RGP_CORE::Image* left, RGP_CORE::Image* front,
													RGP_CORE::Image* back, RGP_CORE::Image* top, RGP_CORE::Image* bottom) 
{
		
};
void  RGP_CORE::GLRenderer::SetImageDataCube(_s32b Width, _s32b Height) 
{
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X  , 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
};
void  RGP_CORE::GLRenderer::SetActiveTexture(_u16b index)
{
    glActiveTexture(GL_TEXTURE0+index);
};
_bool RGP_CORE::GLRenderer::BindTexture(_u32b textureID,_bool Texture2D){
    if(Texture2D)
		glBindTexture(GL_TEXTURE_2D,textureID);
	else
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    if(glGetError())
        return false ;
    return true ;
};

//FBOs
_bool RGP_CORE::GLRenderer::GenFrameBuffers(_u32b numFrameBuffers, GLuint* target)
{
	if (!target)
		return false;
	int error =glGetError();
	glGenFramebuffers(numFrameBuffers, target);
	if (error = glGetError()) {
		
		return false;
	}
	return true;
};
void RGP_CORE::GLRenderer::DeleteFrameBuffers(_u32b numFrameBuffers, GLuint* target)
{
	if (target) {
		glDeleteFramebuffers(numFrameBuffers, target);
	}
};
_bool RGP_CORE::GLRenderer::BindFrameBuffer(_u32b BufferID)
{
	int error = glGetError();
	glBindFramebuffer(GL_FRAMEBUFFER, BufferID);
	if (error = glGetError()) {
		printf("error binding FBO\n");
		return false;
	}
	return true;
};
_bool RGP_CORE::GLRenderer::AttachTexturetoFrameBuffer(GLenum AttachementID, GLenum TextureTarget, _u32b TextureID, _s32b Level)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, AttachementID, TextureTarget, TextureID, Level);
	//try this  
	//glFrameBufferTexture3D
	return true;
};

void  RGP_CORE::GLRenderer::setDrawBuffers(GLenum* bufferenum, _s32b numBuffers)
{
	if (numBuffers == 0)
		glDrawBuffer(GL_NONE);
	else if(numBuffers > 0)
		glDrawBuffers(numBuffers, bufferenum);
};
void  RGP_CORE::GLRenderer::setReadBuffers(GLenum bufferenum) 
{
	glReadBuffer(bufferenum);
};

///ShaderProgramManagement

_u32b   RGP_CORE::GLRenderer::CreateGLProgramFromFile(_s8b* VertexFile, _s8b* FragmentFile){
	return CreateGLProgramFromFile(VertexFile, NULL, FragmentFile);
};
_u32b  RGP_CORE::GLRenderer::CreateGLProgramFromFile( _s8b* VertexFile,  _s8b* GeometryFile,  _s8b* FragmentFile)
{
	if (m_ShaderManager) {
		return m_ShaderManager->CreateProgramFromFiles(VertexFile, GeometryFile, FragmentFile);
	}
	return 0;
};
void    RGP_CORE::GLRenderer::DeleteGLProgram(_u32b ProgramID){
		m_ShaderManager->DeleteProgram(ProgramID);
};
_s32b   RGP_CORE::GLRenderer::GetUniformLocation(_u32b programID ,_s8b*  Name){
	GLShaderProgramsManager::ShaderProgram* program = NULL;
	program = m_ShaderManager->getShaderProgram(programID);
	GLint location;
	if (program) {
		location= glGetUniformLocation(program->GLProgramID, Name);
//		printf("%s location %d \n",Name, location);
		return location;
	}
	return -1;
};
_bool   RGP_CORE::GLRenderer::SetUniformF(_s32b Location,_float data )
{
     glUniform1f(Location,data);
     return true ;
};
_bool    RGP_CORE::GLRenderer::SetUniform3F(_s32b Location, _float data1, _float data2, _float data3)
{
	glUniform3f(Location, data1, data2, data3);
	return true;
};
_bool   RGP_CORE::GLRenderer::SetUniformI(_s32b Location, _u32b data)
{
	glUniform1i(Location, data);
	return true;
};
_bool   RGP_CORE::GLRenderer::SetUniformFv(_s32b Location,_float* data,_u32b numElements )
{
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
void    RGP_CORE::GLRenderer::SetShaderProgram(_u32b programID){
	m_ShaderManager->BindProgram(programID);
};

 
