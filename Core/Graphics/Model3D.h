#ifndef _RGP_MODEL_3D_H_
#define _RGP_MODEL_3D_H_


#include "..//..//tools//SOIL//SOIL.h"
#include "..//..//tools//assimp//scene.h"
#include "..//..//tools//assimp//cimport.h"
#include "..//..//tools//assimp//postprocess.h"
#include "..//Common//StringOP.h"
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
		_float*         TexCoords ;//each 2 floats form a vertex (x,y) or (u,v)
		_u32b* 			IndexBuffer ;
		_u32b			nbVertices;
		_u32b			nbNormals;
		_u32b			nbFaces ;
		_u32b           nbTexCoords;
		_u32b   		AppliedMaterial ;
	} Mesh , *pMesh ;

	class Model3D : public virtual Renderable {
	public:
		Model3D();
		Model3D(Vertex3d Pos);
		Model3D(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
		virtual ~Model3D() ;
		virtual void 	Destroy() ;
		_s16b			LoadModelFromFile(char* filename);
		virtual _s16b   LoadShaderProg(char* VS_File,char* FS_File);
		virtual _s16b   InitVAOs();
		virtual void 	Render(Camera* Selected);
		virtual void	CastShadow();
		_bool			isEnvMap() { return false; };
	protected:
		_u16b ProcessNode(aiNode* Node,const aiScene* Scene);/// currently this function copy only a static model
        _u16b AddMesh(const char* Name,_u16b MaterialID);
        _u16b CopyVertices(const aiVector3D*   buffer,_u32b nbVertices);
        _u16b CopyNormals(const aiVector3D*   buffer,_u32b nbVertices);
        _u16b CopyTangents(const aiVector3D*   Tbuffer,const aiVector3D*   Bibuffer,_u32b nbVertices);
        _u16b CopyFaces(const aiFace* Faces, _u32b nbFaces);
        _u16b CopyTextureCoords(aiVector3D** TexCoords, _u32b nbTexCoords);
        _u16b LoadMaterial(const aiScene* Scene);
        _u16b CopyTextureData(aiTexture* Texture, Image* Dest);
        _u16b LoadMaterialstoMemory(const aiScene* Scene);
        _u16b GenerateOGLMaterials();
        _u16b GenerateBuffers();
	protected:
		pMesh					v_Meshes ;
		MeshBuffers*            v_Buffers ;
		GLuint*					m_VAOforShadowcasting;
		_u32b					m_nbMeshes;
		Material*               v_Materials;
		OGLMaterial*            v_oglMaterials;
		_u32b                   m_nbMaterials;
		_s8b*					m_FileDirectory;
		_u32b				    m_ShaderProgram ;
	};

};

#endif
