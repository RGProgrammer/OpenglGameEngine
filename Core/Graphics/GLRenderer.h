#ifndef _RGP_GL_RENDERER_H_
#define _RGP_GL_RENDERER_H_
//////////////////////////////////////////////////////////////////////////
/// GL_Renderer is like a transformation of Opengl low level
/// to a high level interface to make adding another renderer
/// type mode easier
//////////////////////////////////////////////////////////////////////////
#include "..//Common//BasePrimitiveTypes.h"
#include "..//Common//StringOP.h"
#include "..//BaseActors//Renderable.h"
#include "..//Scene//GameScene.h"
#include ".//Window.h"
#include <string.h>
///there are more includes here


namespace RGP_CORE{

    typedef struct {
		_s32b		Width;
		_s32b		Height ;
		_u8b*		Pixels ;
	} Image;/// we only accepts RGBA format


	typedef struct{
		Image*			DiffuseMap;
		Image*			SpecularMap;
		Image*			NormalsMap;
		_float			IOR;
	} Material ;

	typedef struct {
        GLuint          DiffuseMap;
		GLuint			SpecularMap;
		GLuint			NormalsMap;
		_float			IOR;
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
		_u32b			AppliedMaterialIndex;
		_u32b			numFaces;
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
		_bool		EnableShadows;
		_s32b		ShadowResolution;
		_bool		useEnvironmentMaps;

    } gfxConfig;


	class GLShaderProgramsManager {
		
	public:
		GLShaderProgramsManager();
		~GLShaderProgramsManager();
		void Destroy();
		_u32b CreateProgramFromOneFile(_s8b* Filename);
		_u32b CreateProgramFromFiles(_s8b* VertexFile, _s8b* FrgamentFile=NULL);
		_u32b CreateProgramFromFiles(_s8b* VertexFile, _s8b* GeometryFile, _s8b* FrgamentFile);
		void  BindProgram(_u32b ID);
		void  DeleteProgram(_u32b ID);
	private:
		_u32b LoadShaderBuffer(GLenum type, const _s8b* Buffer, int buffersize);
		_u32b LoadShaderFile(GLenum Type, _s8b* filename);
		GLuint CreateGLProgram(_s8b* VertexFile, _s8b* GeometryFile, _s8b* FrgamentFile);
		_u32b doExiste(_s8b* VertexFile, _s8b* GeometryFile, _s8b* FrgamentFile);// verify if the shader program existeif it is returns the progran ID
	private:
		friend class GLRenderer;
		struct ShaderProgram {
			_s8b* VertexShaderFileName;//Directory+Filename
			_s8b* GeometryShaderFileName;//Directory+Filename
			_s8b* FragmentShaderFileName;//Directory+Filename
			_s8b* OneForAllFileName;//a file that contains shader code for all files
			_u32b GLProgramID;
		};
		ShaderProgram* getShaderProgram(_u32b programID);
	private:
		ShaderProgram*		m_LoadedProgramsList;
		_u32b				m_NumPrograms;
		_u32b				m_Size;
		_u32b				m_boundProgramID;
	
	};

	class GameScene;

    class GLRenderer{
    private:
		GLenum DrawBuff[6];

        enum TextureOrder {
            DEPTH_TEXTURE=0,
            DIFFUSE_TEXTURE=1,
            SPECULAR_TEXTURE=2,///+roughness in alpha channel
            NORMAL_TEXTURE=3,
			MATERIAL_TEXTURE=4,
			POSITION_TEXTURE=5

        };
	public:
        GLRenderer(RenderMode Type=DEFERRED_RENDERING);
        ~GLRenderer();
        void Destroy();
        _bool InitRenderer(gfxConfig Config);
        _bool isInitialized();
        RenderMode  getRenderMode();
        Window*     getTarget();
        void	setScene(GameScene*   Scene);
		void	RenderScene(_u32b FBO_Target);
        void	RenderCurrentScene();
		void	RenderToTarget(_u32b FBO_Target=0);
		void	RenderSceneColors(_u32b FBO, Camera* camera = NULL);
		void	RenderSceneShadows(_u32b FBO, Camera* camera = NULL);
		void	RenderSceneLightAccum();
		void	LoadShadowProgram();
		void	UnloadShadowProgram();
		void	UpdateEnvironmentMaps();
        ///buffers manager
        ///VBOs
        _bool GenBuffers(_u32b numBuffers,GLuint*    target);
        void  DeleteBuffers(_u32b numBuffers,GLuint*    target);
        void  setBufferData(_u32b Target ,_u32b Size , void* Data, _u32b flag);
        void  BindBuffer(_u32b Bindtype,_u32b BufferID );

        ///VAOs
        _bool GenVertexArrays(_u32b numBuffers,GLuint*    target);
        void  DeleteVertexArrays(_u32b numBuffers,GLuint*    target);
        _bool BindVertexArray(_u32b BufferID);
        ///VBO and VAO
        void DrawElements(GLenum mode,_u32b Count,GLenum type,void* Offset);
        ///Textures
        _bool GenTextures2D(_u32b numTexture,GLuint*    target);
		_bool GenTexturesCube(_u32b numTexture, GLuint* target);
        void  DeleteTextures(_u32b numTexture,GLuint*    target);
        void  SetImageData2D(Image* ImageSource );
		void  SetImageDataCube(Image* right, Image* left, Image* front,
								Image* back, Image* top, Image* bottom);
		void  SetImageDataCube(_s32b Width, _s32b Height);
        void  SetActiveTexture(_u16b index);///starts from GL_TEXTURE0
        _bool BindTexture(_u32b textureID,_bool Texture2D=true);
		
		///FBOs
		_bool GenFrameBuffers(_u32b numFrameBuffers, GLuint* target);
		void DeleteFrameBuffers(_u32b numFrameBuffers, GLuint* target);
		_bool BindFrameBuffer(_u32b BufferID);
		_bool AttachTexturetoFrameBuffer(GLenum AttachementID ,GLenum TextureTarget,_u32b GLTextureID, _s32b Level=0);
		void  setDrawBuffers(GLenum* bufferenum, _s32b numBuffers);
		void  setReadBuffers(GLenum bufferenum);
        ///ShaderProgramManagement
        _u32b  CreateGLProgramFromFile( _s8b* VertexFile, _s8b* FragmentFile=NULL);
		_u32b  CreateGLProgramFromFile( _s8b* VertexFile, _s8b* GeometryFile,  _s8b* FragmentFile);
        void    DeleteGLProgram(_u32b ProgramID);
        _s32b   GetUniformLocation(_u32b programID,_s8b*   Name);
        _bool   SetUniformF(_s32b Location,_float data );
		_bool   SetUniform3F(_s32b Location, _float data1, _float data2, _float data3);
		_bool   SetUniformI(_s32b Location, _u32b data);
        _bool   SetUniformFv(_s32b Location,_float* data ,_u32b numElements );
        _bool   SetUniformvMtx(_s32b Location,_float* Matrix_4x4 );
        _bool   SetUniformSample(_s32b Location, _u32b TextureUnit);
        _bool   SetVertexAttribPointer(_u32b Index,_u32b NumElemntsPerVertex,
                                            _u32b offsetBetweenElements=0,void* offsetFromFirst=0);
        _bool   EnableVertexAttribArray(_u32b index);
        _bool   DisableVertexAttribArray(_u32b index);
        void    SetShaderProgram(_u32b programID);
    private:
        _bool	CreateNeededObjects();//FBOs and Textures
		_bool	AttachColorsTextures();
		_bool	CreateColorsObjects();
		_bool	CreateShadowsObjects();
		_bool	CreateLightObjects();
        
        _bool InitFinalPhase();
		
        

    private :
        Window*						m_Target ;
        RenderMode					m_Mode;
		gfxConfig					m_Config;
		MeshBuffers*				m_FinalRenderSurface;
		GLuint						m_FinalRenderProgram;
		GLShaderProgramsManager*	m_ShaderManager;
        _bool						m_isInitialized ;

		//scene to render
		GameScene*					m_SelectedScene;
		

		//colors
        _s16b						m_NumFBOs;
        GLuint*						m_FBOs;
        GLuint**					m_AttachmentTextures;
        _s16b						m_SelectedFBO;

		//light
		_u32b						m_LightAccumProgram;
		GLuint						m_LightAccumBuffer;
		GLuint						m_LightAccumDiffuseTexture;
		GLuint						m_LightAccumSpecularTexture;

		//Shadows
		_u32b						m_ShadowVectorSize;
		_u32b						m_NumShadowFBOs;
		_u32b						m_ShadowRenderingProgram;
		GLuint*						m_ShadowFBOs;
		GLuint*						m_ShadowAttachmentTexture;
		_u32b						m_ShadowAccumProgram;
		GLuint						m_ShadowAccumBuffer;
		GLuint						m_ShadowAccumTexture;
		

    };
};
#endif // _RGP_GL_RENDERER_H_
