#ifndef _RGP_MODEL_3D_H_
#define _RGP_MODEL_3D_H_



#include "..//..//tools//assimp//include//scene.h"
#include "..//..//tools//assimp//include//cimport.h"
#include "..//..//tools//assimp//include//postprocess.h"
#include "..//Common//Common.h"
#include "..//BaseActors//Renderable.h"
#include ".//GLRenderer.h"


namespace RGP_CORE {

	///Mesh structure
    typedef struct {
		char*			Name;
		_float*		    VertexBuffer ;//each 3 floats form a vertex(x,y,z)
		_float*		    NormalBuffer;
		_float*         TangentBuffer ;
		_float*         BitangentBuffer ;
		_float*         TexCoords;//each 2 floats form a vertex (x,y) or (u,v)
		_u32b* 			IndexBuffer ;
		_u32b			nbVertices;
		_u32b			nbFaces ;
		_u32b   		AppliedMaterial ;
	} Mesh , *pMesh ;


	class Model3D : public virtual Renderable {
	public:
		static BaseActor*	Create(void ** args);//args is the list of argment needed for the object
		Model3D(const _s8b* name = "Actor");
		Model3D(Vertex3d Pos, const _s8b* name = "Actor");
		Model3D(Vertex3d Pos, Vertex3d Dir, Vertex3d Up, const _s8b* name = "Actor");
		virtual ~Model3D() ;
		virtual void 	Destroy() ;
		void			DestroyBuffers();
		void			ClearModelLoadedData();
		_s16b			LoadModelFromFile(char* filename,_bool ClearDataAfterLoad=true);
		virtual _s16b   LoadShaderProg(char* VS_File,char* FS_File);
		virtual _s16b   InitVAOs();
		virtual void 	Render(Camera* Selected);
		virtual void	CastShadow();
		const pMesh		getMesh(_u32b index );
		_u32b			getNumMeshes();
		void			setReflectionProbe(EnvMapProbe* Probe);
		EnvMapProbe*	getReflectionProbe(EnvMapProbe* Probe);
	
	protected:
		_u16b ProcessNode(aiNode* Node,const aiScene* Scene);/// currently this function copy only a static model
        _u16b AddMesh(const char* Name,_u32b MaterialID);
        _u16b CopyVertices(const aiVector3D*   buffer,_u32b nbVertices);
        _u16b CopyNormals(const aiVector3D*   buffer,_u32b nbVertices);
        _u16b CopyTangents(const aiVector3D*   Tbuffer,const aiVector3D*   Bibuffer,_u32b nbVertices);
        _u16b CopyFaces(const aiFace* Faces, _u32b nbFaces);
        _u16b CopyTextureCoords(aiVector3D** TexCoords, _u32b nbTexCoords);
        _u16b LoadMaterial(const aiScene* Scene);
        _u16b CopyTextureData(aiTexture* Texture, Image* Dest);
        _u16b LoadMaterialstoMemory(const aiScene* Scene);
        _u16b GenerateGPUMaterials();
        _u16b GenerateVerticesBuffers();
		_u16b GenerateCommandBuffer();
		_u16b FillBuffers();
	protected:
		_s8b*					m_FileDirectory;
		pMesh					m_Meshes ;
		_u32b					m_NumMeshes;
		Material*               m_Materials;
		_u32b                   m_NumMaterials;
		_u32b*					m_MaterialsWrappers;
		_u32b				    m_ShaderProgram;
		_u32b					m_RenderingVAO;
		_u32b					m_RenderingCommandsBuffer;
		_bool					m_ClearAfterLoad;
		_s32b					m_WorldMtxLocation;
		DrawElementsIndirectCommand* m_DrawCommands;
		//renderer dependent attributs
		MeshBuffers				m_Buffer ;
		EnvMapProbe*			m_ReflectionProbe;
		
		
	};

};

#endif
