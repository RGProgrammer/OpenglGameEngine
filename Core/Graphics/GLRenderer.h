#ifndef _TTB_GL_RENDERER_H_
#define _TTB_GL_RENDERER_H_
//////////////////////////////////////////////////////////////////////////
/// GL_Renderer is like a transformation of Opengl low level
/// to a high level interface to make adding another renderer
/// type mode easier
//////////////////////////////////////////////////////////////////////////
#include "..//Common//BasePrimitiveTypes.h"
#include "..//BaseActors//Renderable.h"
#include "..//Scene//GameScene.h"
#include ".//Window.h"
#include <string.h>
///there are more includes here


namespace TTB{

    typedef struct {
		_s32b		Width;
		_s32b		Height ;
		_u8b*		Pixels ;
	} Image;/// we only accepts RGBA format


	typedef struct{
		Image*			DiffuseMap;
		Image*			SpecularMap;
		Image*			NormalsMap;
	} Material ;

	typedef struct {
        GLuint          DiffuseMap;
		GLuint			SpecularMap;
		GLuint			NormalsMap;
	} OGLMaterial ;

    ///mesh structure for openGL
	typedef struct {
	    GLuint          VertexArrayObject ;
        GLuint          VertexBuffer ;
		GLuint		    NormalBuffer;
		GLuint          TangentBuffer ;
		GLuint          BitangentBuffer ;
		GLuint          TexCoords ;
		GLuint          IndexBuffer ;
    } MeshBuffers ;

    ///OpenGL Renderer
    typedef enum RenderType {
        FORWARD_RENDERING,
        DEFERRED_RENDERING
    } RenderMode;
    typedef struct {
        _s8b*       Title ;
        _s32b       Witdh;
        _s32b       Height ;
        _u16b       NumBackBuffers;///max 5

    } gfxConfig;
    class GLRenderer{
    public:
        const GLenum DrawBuff[5]={GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,
                                    GL_COLOR_ATTACHMENT4};
        enum TextureOrder {
            DEPTH_TEXTURE=0,
            DIFFUSE_TEXTURE=1,
            SPECULAR_TEXTURE=2,///+roughness in alpha channel
            NORMAL_TEXTURE=3,
            SHADOW_TEXTURE=4

        };
        GLRenderer(RenderMode Type=DEFERRED_RENDERING);
        ~GLRenderer();
        void Destroy();
        _bool InitRenderer(gfxConfig Config);
        _bool isInitialized();
        RenderMode  getRenderMode();
        Window*     getTarget();
        void  setScene(GameScene*   Scene);
        void  RenderCurrentScene();
        ///buffers manager
        ///VBOs
        _bool GenBuffers(_u32b numBuffers,GLuint*    target);
        void  DeleteBuffers(_u32b numBuffers,GLuint*    target);
        void  setBufferData(_u32b Target ,_u32b Size , void* Data, _u32b flag);
        void  BindBuffer(_u32b BufferID,_u32b Bindtype );

        ///VAOs
        _bool GenVertexArrays(_u32b numBuffers,GLuint*    target);
        void  DeleteVertexArrays(_u32b numBuffers,GLuint*    target);
        _bool BindVertexArray(_u32b BufferID);
        ///VBO and VAO
        void DrawElements(GLenum mode,_u32b Count,GLenum type,void* Offset);
        ///Textures
        _bool GenTextures(_u32b numTexture,GLuint*    target);
        void  DeleteTextures(_u32b numTexture,GLuint*    target);
        void  SetImageData(Image* ImageSource );
        void  SetActiveTexture(_u16b index);///starts from GL_TEXTURE0
        _bool BindTexture(_u32b textureID);

        ///ShaderProgramManagement
        GLuint  CreateGLProgramFromBuffer(_s8b* VertexSource,_s8b* FragmentSource);
        GLuint  CreateGLProgramFromFile(_s8b* VertexFile,_s8b* FragmentFile);
        void    DeleteGLProgram(_u32b ProgramID);
        _s32b   GetUniformLocation(_u32b program,_s8b*   Name);
        _bool   SetUniformF(_s32b Location,_float data );
        _bool   SetUniformFv(_s32b Location,_float* data ,_u32b numElements );
        _bool   SetUniformvMtx(_s32b Location,_float* Matrix_4x4 );
        _bool   SetUniformSample(_s32b Location, _u32b TextureID);
        _bool   SetVertexAttribPointer(_u32b Index,_u32b NumElemntsPerVertex,
                                            _u32b offsetBetweenElements=0,void* offsetFromFirst=0);
        _bool   EnableVertexAttribArray(_u32b index);
        _bool   DisableVertexAttribArray(_u32b index);
        void    SetShaderProgram(GLuint program);
    private:
        _bool CreateNeededObjects();//FBOs and Textures
        _bool AttachTextures();
        _bool InitFinalPhase();
        void RenderToScreen();
        static inline GLuint LoadShaderFile(GLenum type,_s8b* filename);
        static inline GLuint LoadShaderBuffer(GLenum type,_s8b* Buffer,int buffersize);

    private :
        Window*                 m_Target ;
        RenderMode              m_Mode;
        _bool                   m_isInitialized ;
        _s16b                   m_NumFBOs;
        GLuint*                 m_FBOs;
        //GLuint*                 m_RenderBuffers;
        GLuint**                m_AttachmentTextures;
        _s16b                   m_SelectedFBO;

        GameScene*              m_SelectedScene ;

        MeshBuffers*            m_FinalRenderSurface ;
        GLuint                  m_FinalRenderProgram ;

    };
};
#endif // _TTB_GL_RENDERER_H_
