#ifndef _RGP_GL_RENDERER_H_
#define _RGP_GL_RENDERER_H_


//////////////////////////////////////////////////////////////////////////////////
//  GL_Renderer is like a transformation of Opengl low level					//
//  to a high level interface to make adding another renderer					//
//  type mode easier															//
// PS: this is not the final code structure(mames , atributs and methodes)		//
//////////////////////////////////////////////////////////////////////////////////



#include ".//MyWindow.h"
#include "..//Common//BasePrimitiveTypes.h"
#include "..//Common//Common.h"
#include "..//BaseActors//Renderable.h"
#include "..//Scene//GameScene.h"
#include "../Common/Timer.h"
#include "..//..//tools//freeimage//include//FreeImage.h"
#include <string.h>


#define MAXNUMAMTERIALS 100
#define MAXNUMLIGHTSOURCES  100 

namespace RGP_CORE{

    typedef struct {
		_s32b		Width;
		_s32b		Height ;
		_u8b*		Pixels ;
	} Image;/// we only accepts RGBA format

	Image* LoadImageFromFile(const _s8b* filename);


	typedef struct{
		_s8b*			Name;
		Image*			DiffuseMap;
		Image*			SpecularMap;
		Image*			NormalsMap;
		_float			IOR;
		_float			Opacity;
	} Material ;

	typedef struct {
		_s8b*			Name;
        _u32b			DiffuseMap;
		_u32b			SpecularMap;
		_u32b			NormalsMap;
		_float			IOR;
		_float			Opacity;
	} OGLMaterial ;

	typedef struct {
		
		_u64b			DiffuseBindless;
		_u64b			SpecularBindless;
		_u64b			NormalBindless;
		_float			IOR;
		_float			Opacity;
		
		
	} GPUMaterial;

    ///mesh structure for openGL
	typedef struct {
		_u32b			Wrappers[7];
		_u32b			VertexArrayObject ;
		_u32b			numFaces;
    } MeshBuffers ;

	
    ///OpenGL Renderer
	
    typedef enum RenderType {
        FORWARD_RENDERING,
        DEFERRED_RENDERING
    } RenderMode;

	//renderer configuration
    typedef struct {
        _s8b*       Title ;
        _s32b       Witdh;
        _s32b       Height ;
		_bool		EnableShadows;
		_s32b		ShadowResolution;
		_bool		useEnvironmentMaps;


    } gfxConfig;

	//Indirect render Command struct
	typedef  struct {
		_u32b  count;
		_u32b  instanceCount;
		_u32b  first;
		_u32b  baseInstance;
	} DrawArraysIndirectCommand;
	

	typedef  struct {
		_u32b  count;
		_u32b  instanceCount;
		_u32b  firstIndex;
		_u32b  baseVertex;
		_u32b  baseInstance;
	} DrawElementsIndirectCommand;

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
		_u32b CreateGLProgram(_s8b* VertexFile, _s8b* GeometryFile, _s8b* FrgamentFile);
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
		GLenum DrawBuff[7];

        enum TextureOrder {
            DEPTH_TEXTURE		= 0,
            DIFFUSE_TEXTURE		= 1, //Base color with transparent material 
            SPECULAR_TEXTURE	= 2,
            NORMAL_TEXTURE		= 3,
			MATERIAL_TEXTURE	= 4,
			POSITION_TEXTURE	= 5,
			TRANSPARENCY_TEXTURE= 6 //define transparent spots in the scene

        };
	public:
        GLRenderer(RenderMode Type=DEFERRED_RENDERING);
        ~GLRenderer();
        void Destroy();
        _bool InitRenderer(gfxConfig Config);
        _bool isInitialized();
        RenderMode  getRenderMode();
        Window*     getTarget();
		void		MakeContext();
        void	setScene(GameScene*   Scene);
		_bool	reRegisterLightSources();
		void	RenderScene(_u32b FBO_Target,Camera* camera=NULL);
        void	RenderCurrentScene();
		void	RenderToTarget(_u32b FBO_Target=0);
		void	RenderSceneColors(_u32b FBO, Camera* camera = NULL);
		void	RenderSceneShadows(_u32b FBO, Camera* camera = NULL);
		void	RenderSceneLightAccum(Camera* camera=NULL);
		void	SwapBuffers() { glfwSwapBuffers(m_Target->getglfwWindow()); };
		void	LoadShadowProgram();
		void	UnloadShadowProgram();
		void	UpdateEnvironmentMaps();
		void	SwitchNoLightMode();
		_u32b	getCameratransformsUBO();
		_u32b   getMaterialsUBO();
		_u32b	getCurrentShaderProgram();
		_bool	DoesSupportBindlessTexture();
		void	printExtension();

		//todo material
		_u32b			CreateMaterial(Material material); //return material index (>=1)
		_u32b			GetMaterialIndex(const _s8b* Name);
		OGLMaterial*	GetMaterial(_u32b index);
		_bool			RemoveMaterial(const _s8b* materialname);
		_bool			RemoveMaterialAt(_u32b index);
		void			ClearMaterials();// remove all created materials
		_u32b			getNumMetrials();



		void           Color(_float red, _float green, _float blue);


        ///buffers manager
        ///Buffer Objects
        _bool GenBuffers(_u32b numBuffers,_u32b*    target);
        void  DeleteBuffers(_u32b numBuffers, _u32b*    target);
        void  setBufferData(_u32b Target ,_u32b Size , void* Data, _u32b flag,_u32b bufferwrapper=0);
		void  setBufferSubData(_u32b Target, _u32b Offset, _u32b Size, void* Data , _u32b bufferwrapper = 0);
        void  BindBuffer(_u32b Bindtype,_u32b BufferID );
		void  BindBufferBase(_u32b Target, _u32b index, _u32b BufferID);

        ///VAOs
        _bool GenVertexArrays(_u32b numBuffers,_u32b*    target);
        void  DeleteVertexArrays(_u32b numBuffers,_u32b*    target);
        _bool BindVertexArray(_u32b BufferID);
        ///VBO and VAO
        void DrawElements(_u32b mode,_u32b Count,GLenum type,void* Offset,_u32b numInstances=1);
		void DrawArrays(_u32b mode, _u32b first, _u32b Count,  _u32b numInstances=1);
		_bool DrawElementsIndirect(_u32b mode, _u32b Type, const DrawElementsIndirectCommand* command);
		_bool DrawArraysIndirect(_u32b mode, const DrawArraysIndirectCommand * command);

		_bool MultiDrawElementsIndirect(_u32b mode, _u32b Type,const void* commands, _u32b count, _u32b stride=0);
		_bool MultiDrawArraysIndirect(_u32b mode,const void *indirect, _u32b drawcount, _u32b stride);



        ///Textures
        _bool GenTextures2D(_u32b numTexture,_u32b*    target);
		_bool GenTexturesCube(_u32b numTexture, _u32b* target);
        void  DeleteTextures(_u32b numTexture,_u32b*    target);
        void  SetImageData2D(Image* ImageSource );
		void  SetImageDataCube(Image* right, Image* left, Image* front,
								Image* back, Image* top, Image* bottom);
		void  SetImageDataCube(_s32b Width, _s32b Height);
        void  SetActiveTexture(_u16b index);///starts from GL_TEXTURE0
        _bool BindTexture(_u32b textureID,_u32b unit=0,_bool Texture2D=true);
		_u64b GetTextureHandle(_u32b textureID);
		void  MakeTextureHandleResidant(_u64b texHandle, _bool makeResidant = true);
		
		///FBOs
		_bool GenFrameBuffers(_u32b numFrameBuffers, _u32b* target);
		void DeleteFrameBuffers(_u32b numFrameBuffers, _u32b* target);
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
		_bool	SetUniform64I(_s32b Location, _u64b data);
        _bool   SetUniformFv(_s32b Location,_float* data ,_u32b numElements );
        _bool   SetUniformvMtx(_s32b Location,_float* Matrix_4x4 );
        _bool   SetUniformSample(_s32b Location, _u32b TextureUnit);
        _bool   SetVertexAttribPointerF(_u32b Index,_u32b NumElemntsPerVertex,
                                            _u32b offsetBetweenElements=0,void* offsetFromFirst=0);
		_bool   SetVertexAttribPointerI(_u32b Index, _u32b NumElemntsPerVertex,
											_u32b offsetBetweenElements = 0, void* offsetFromFirst = 0);
		_bool	SetUniformHandleu64(_s32b Location, _u64b Handle);
		_bool	SetUniformHandleu64v(_s32b Location, _u32b Count , _u64b* Handle);
        _bool   EnableVertexAttribArray(_u32b index);
        _bool   DisableVertexAttribArray(_u32b index);
        void    SetShaderProgram(_u32b programID);
    private:
        _bool	CreateNeededObjects();//FBOs and Textures
		_bool	AttachColorsTextures();
		_bool	CreateColorsObjects();
		_bool	CreateShadowsObjects();
		_bool	CreateLightObjects();
		_bool	CreateDefaultMaterial();
        _bool   InitFinalPhase();
		_bool   UpdateCameraMtxUBO(); //TODO
		_bool   UpdateLightDataUBO();
	public:
		_bool   UpdateMaterialsUBO();

		
        

    private :
        Window*						m_Target ;
        RenderMode					m_Mode;
		gfxConfig					m_Config;
		MeshBuffers*				m_FinalRenderSurface;
		_u32b						m_FinalRenderProgram;
		GLShaderProgramsManager*	m_ShaderManager;
        _bool						m_isInitialized ;
		Material					defaultMAaterial;
		//scene to render
		GameScene*					m_SelectedScene;

		//colors
        _u32b						m_FBO;
        _u32b*						m_AttachmentTextures;
        

		//light
		_u32b						m_LightAccumProgram;
		_u32b						m_LightAccumBuffer;
		_u32b						m_LightAccumDiffuseTexture;
		_u32b						m_LightAccumSpecularTexture;

		//Shadows
		_u32b						m_ShadowVectorSize;
		_u32b						m_NumShadowFBOs;
		_u32b						m_ShadowRenderingProgram;
		_u32b*						m_ShadowFBOs;
		_u32b*						m_ShadowAttachmentTexture;
		_u32b						m_ShadowAccumProgram;
		_u32b						m_ShadowAccumBuffer;
		_u32b						m_ShadowAccumTexture;
		_bool						m_noLightMode;
		
		OGLMaterial*				m_Materials; //all the needed materials will be stored here (not in the scene actors);
		GPUMaterial*				m_GPUMaterials;
		_u32b						m_NumMaterials;
		_u32b						m_SizeofMaterialVector;

		//UBOs
		_u32b						m_CurrentShaderProgram;
		_u32b						m_AllMaterialUBO;
		_u32b						m_CameraMtxUBO;//TODO
		_u32b						m_LightDataUBO;
		_u32b						m_NumRegisteredLights;
		Timer						m_Timer;
		_double						m_Time;
		_u32b						num_exts_i;
		_s8b**						exts_i;
		
		
    };
};
#endif // _RGP_GL_RENDERER_H_
