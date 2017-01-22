#include ".//GLRenderer.h"

TTB::GLRenderer::GLRenderer(RenderType Type):m_Target(NULL),m_isInitialized(false) ,
                                            m_NumFBOs(0),m_FBOs(NULL),m_SelectedFBO(0),
                                            m_SelectedScene (NULL),//m_RenderBuffers(NULL),
                                            m_AttachmentTextures(NULL),
                                            m_FinalRenderSurface(NULL),
                                            m_FinalRenderProgram(0)
{
    if(Type==DEFERRED_RENDERING || Type==FORWARD_RENDERING)
        m_Mode=Type ;
    else
        m_Mode=DEFERRED_RENDERING;
};
TTB::GLRenderer::~GLRenderer(){
    this->Destroy();
};
void TTB::GLRenderer::Destroy(){
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
_bool TTB::GLRenderer::InitRenderer(gfxConfig Config){

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
    m_NumFBOs=Config.NumBackBuffers;
     printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    glEnable(GL_TEXTURE_2D);
     if(!CreateNeededObjects()){
        printf("error Creating FBOs and Render Textures\n");
        return false ;
     }

    if(!AttachTextures()){
        printf("error Creating FBOs and Render Textures\n");
        return false ;
    }
    ///preparing a surface for the final render
    if(!InitFinalPhase()){
        printf("error initializing final phase\n");
        //return false ;
    }
    m_FinalRenderProgram=this->CreateGLProgramFromFile(".//Shaders//FinalDeferred.vs",".//Shaders//FinalDeferred.fs");
    if(!m_FinalRenderProgram){
        printf("error loading render program \n");
        return false ;
    }
    m_isInitialized=true ;
    printf("Done Initialization \n");
    return true ;
};
_bool TTB::GLRenderer::CreateNeededObjects(){
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
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glGetError();
        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,
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
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,
                         m_Target->getWidth(),
                         m_Target->getHeight(),
                         0,GL_RGBA,GL_UNSIGNED_BYTE,0);
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
    return true ;
};
_bool TTB::GLRenderer::AttachTextures(){
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
_bool TTB::GLRenderer::InitFinalPhase(){
    _float vertexBuffer[12]={-1.0f,1.0f,-5.0f,
                           1.0f,1.0f,-5.0f,
                          1.0f,-1.0f,-5.0f,
                          -1.0f,-1.0f,-5.0f};
    _float TexCoordBuffer[8]={0.0f,0.0f,
                            1.0f,0.0f,
                            1.0f,1.0f,
                            0.0f,1.0f};
    _u32b IndexBuffer[6]={0,2,1,
                         0,2,3};
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
    glBufferData(GL_ARRAY_BUFFER,12*sizeof(_float),vertexBuffer,GL_STATIC_DRAW);
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
    printf("VAO of the final render %u\n",m_FinalRenderSurface->VertexArrayObject);
    glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER,m_FinalRenderSurface->VertexBuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,m_FinalRenderSurface->TexCoords);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_FinalRenderSurface->IndexBuffer);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    return true ;
};

_bool TTB::GLRenderer::isInitialized(){ return m_isInitialized ;};
TTB::RenderMode  TTB::GLRenderer::getRenderMode(){ return m_Mode ;};
TTB::Window*   TTB::GLRenderer::getTarget(){ return m_Target ;};
void  TTB::GLRenderer::setScene(GameScene*   Scene){ m_SelectedScene= Scene ;};


void  TTB::GLRenderer::RenderCurrentScene(){
/// this methods do all the process to render the scene
    _u32b nbActors=0 ;
    BaseActor*  actor=NULL ;

    glGetError();
    if(m_SelectedScene){
        nbActors=m_SelectedScene->getNBActors();
        ///first step render to current FBO
        glBindFramebuffer(GL_FRAMEBUFFER,m_FBOs[m_SelectedFBO]);
        glViewport(0,0,m_Target->getWidth(),m_Target->getHeight());
        glDrawBuffers(6,DrawBuff);

        ///glClear attachements
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        ///start searching for renderable objects and rendering
        for(_u32b i=0 ; i< nbActors;++i){
            actor=m_SelectedScene->getActor(i);
            if(actor->getID() & RENDERABLE)
                dynamic_cast<Renderable*>(actor)->Render(m_SelectedScene->getCamera());
        }

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        ///after render ;
        RenderToScreen();

        ///go to the next framebuffer
        if(m_NumFBOs-1==m_SelectedFBO)
            m_SelectedFBO=0;
        else
            ++m_SelectedFBO;

        glfwSwapBuffers(m_Target->getglfwWindow());

    }
};
void TTB::GLRenderer::RenderToScreen(){
        _s32b location = -1 ;
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,m_Target->getWidth(),m_Target->getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f,0.5f,0.5f,0.0f);
        ///construct and render the final result
        glUseProgram(m_FinalRenderProgram);
        ///uniforms
        location=glGetUniformLocation(m_FinalRenderProgram,"Diffuse");
        //printf("Diffuse location = %d \n",location);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][DIFFUSE_TEXTURE]);
        glUniform1i(location,0);

        location=glGetUniformLocation(m_FinalRenderProgram,"Specular");
        //printf("Specular location = %d \n",location);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][SPECULAR_TEXTURE]);
        glUniform1i(location,1);

        location=glGetUniformLocation(m_FinalRenderProgram,"Normal");
        //printf("Normal location = %d \n",location);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][NORMAL_TEXTURE]);
        glUniform1i(location,2);

        location=glGetUniformLocation(m_FinalRenderProgram,"Depth");
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[m_SelectedFBO][DEPTH_TEXTURE]);
        glUniform1i(location,3);

        location=glGetUniformLocation(m_FinalRenderProgram,"ProjMtx");
        glUniformMatrix4fv(location,1,GL_FALSE,m_SelectedScene->getCamera()->getProjectionMtx());
        ///VAOs
        glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
};
_bool TTB::GLRenderer::GenBuffers(_u32b numBuffers,GLuint*    target){
    if(!target)
        return false ;
    glGenBuffers(numBuffers,target);
    if(glGetError()){
        return false ;
    }
    return true ;
};
void  TTB::GLRenderer::DeleteBuffers(_u32b numBuffers,GLuint*    target){
    if(!target)
        return ;
    glDeleteBuffers(numBuffers,target);
    for(_u32b i=0 ; i<numBuffers;++i)
        target[i]=0;
};
void  TTB::GLRenderer::setBufferData(_u32b Target,_u32b SizeinByte, void* Data, _u32b flag){
    glBufferData(Target,SizeinByte,Data,flag);
};
void  TTB::GLRenderer::BindBuffer(_u32b BufferID,_u32b Bindtype ){
    glBindBuffer(Bindtype,BufferID);
};

///VAOs
_bool TTB::GLRenderer::GenVertexArrays(_u32b numBuffers,GLuint*    target){
    if(!target)
        return false ;
    glGenVertexArrays(numBuffers,target);
    if(glGetError())
        return false ;
    return true ;
};
void  TTB::GLRenderer::DeleteVertexArrays(_u32b numBuffers,GLuint*    target){
    if(!target)
        return ;
    glDeleteVertexArrays(numBuffers,target);
    for(_u32b i=0; i<=numBuffers; ++i)
        target[i]=0;
};
_bool  TTB::GLRenderer::BindVertexArray(_u32b BufferID){
    glBindVertexArray(BufferID);
    if(glGetError())
        return false ;
    return true ;
};
void TTB::GLRenderer::DrawElements(GLenum mode,_u32b Count,GLenum Type,void* Offset){
    glDrawElements(mode,Count,Type,Offset);
};
///Textures
_bool TTB::GLRenderer::GenTextures(_u32b numTextures,GLuint*    target){
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
    }
    glBindTexture(GL_TEXTURE_2D,0);
    return true ;
};
void  TTB::GLRenderer::DeleteTextures(_u32b numTextures,GLuint*    target){
    if(!target)
        return ;
    glDeleteTextures(numTextures,target);
    for(_u32b i=0 ;i<numTextures;++i)
        target[i]=0;
};
void  TTB::GLRenderer::SetImageData(TTB::Image* ImageSource){
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,
                 ImageSource->Width,ImageSource->Height,
                 0,GL_RGBA,GL_UNSIGNED_BYTE,ImageSource->Pixels);

};
void  TTB::GLRenderer::SetActiveTexture(_u16b index){
    glActiveTexture(GL_TEXTURE0+index);
};
_bool TTB::GLRenderer::BindTexture(_u32b textureID){
    glBindTexture(GL_TEXTURE_2D,textureID);
    if(glGetError())
        return false ;
    return true ;
};
///ShaderProgramManagement
GLuint   TTB::GLRenderer::CreateGLProgramFromBuffer(_s8b* VertexSource,_s8b* FragmentSource){
    GLuint program=0,vs=0,fs=0;
    vs=LoadShaderBuffer(GL_VERTEX_SHADER,VertexSource,strlen(VertexSource));
    if(!vs)
        return program ;
    fs=LoadShaderBuffer(GL_FRAGMENT_SHADER,FragmentSource,strlen(FragmentSource));
    if(!fs){
        glDeleteShader(vs);
        return program ;
    }
    if(program=glCreateProgram()){
        glAttachShader(program,vs);
        glAttachShader(program,fs);
        glLinkProgram(program);
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if(!linked){
            printf("error linking shader program\n");
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
GLuint   TTB::GLRenderer::CreateGLProgramFromFile(_s8b* VertexFile,_s8b* FragmentFile){
    GLuint program=0,vs=0,fs=0;
    vs=LoadShaderFile(GL_VERTEX_SHADER,VertexFile);
    if(!vs)
        return program ;
    fs=LoadShaderFile(GL_FRAGMENT_SHADER,FragmentFile);
    if(!fs){
        glDeleteShader(vs);
        return program ;
    }
    if(program=glCreateProgram()){
        glAttachShader(program,vs);
        glAttachShader(program,fs);
        glLinkProgram(program);
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if(!linked){
            printf("error linking shader program\n");
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
void    TTB::GLRenderer::DeleteGLProgram(_u32b ProgramID){
    glDeleteProgram(ProgramID);
};
_s32b   TTB::GLRenderer::GetUniformLocation(_u32b program ,_s8b*   Name){
    return glGetUniformLocation(program,Name);
};
_bool   SetUniformF(_s32b Location,_float data ){
     glUniform1f(Location,data);
     return true ;
};
_bool   TTB::GLRenderer::SetUniformFv(_s32b Location,_float* data,_u32b numElements ){
    glUniform1fv(Location,numElements,data);
    if(glGetError())
        return false ;
    return true ;
};
_bool   TTB::GLRenderer::SetUniformvMtx(_s32b Location,_float* Matrix_4x4 ){
    glUniformMatrix4fv(Location,1,GL_FALSE,Matrix_4x4);
    if(glGetError())
        return false ;
    return true ;
};
_bool   TTB::GLRenderer::SetUniformSample(_s32b Location, _u32b TextureID){
    glUniform1i(Location,TextureID);
     if(glGetError())
        return false ;
    return true ;
};
_bool   TTB::GLRenderer::SetVertexAttribPointer(_u32b Index,_u32b NumElemntsPerVertex,
                               _u32b offsetBetweenElements,void* offsetFromFirst)
{
    glVertexAttribPointer(Index,NumElemntsPerVertex,GL_FLOAT,GL_FALSE,offsetBetweenElements,offsetFromFirst);
     if(glGetError())
        return false ;
    return true ;
};
_bool   TTB::GLRenderer::EnableVertexAttribArray(_u32b index){
    glEnableVertexAttribArray(index);
     if(glGetError())
        return false ;
    return true ;
};
_bool   TTB::GLRenderer::DisableVertexAttribArray(_u32b index){
    glDisableVertexAttribArray(index);
     if(glGetError())
        return false ;
    return true ;
};
void    TTB::GLRenderer::SetShaderProgram(GLuint program){
    glUseProgram(program);
};
GLuint TTB::GLRenderer::LoadShaderFile(GLenum type,_s8b* filename){
    GLuint Shader=0 ;
    _u32b buffersize=0;
    _s8b* buffer=NULL;
    FILE* Shaderfile=NULL ;

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
    buffer=(_s8b*)malloc(buffersize*sizeof(_s8b));
    if(!buffer){
        fclose(Shaderfile);
        return Shader;
    }
    fflush(stdin);
    fread(buffer,buffersize,1,Shaderfile);
    fclose(Shaderfile);
    Shader=LoadShaderBuffer(type,buffer,buffersize);
    free(buffer);
    return Shader ;

};
 GLuint TTB::GLRenderer::LoadShaderBuffer(GLenum type,_s8b* Buffer,int buffersize){
     GLuint Shader=0;
    if(!Buffer || !buffersize)
        return Shader ;

    Shader=glCreateShader(type);
    if(Shader){
        glShaderSource(Shader,1,(const _s8b**)(&Buffer),&buffersize);
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
