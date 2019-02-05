#include ".//LevelBuilder.h"
#ifdef _WIN32	
#define	GLFW_EXPOSE_NATIVE_WIN32
#include "..//tools//glfw//glfw3native.h"
#endif




_s8b Title[] = "LEVEL DEIGN TOOL";
static RGP_LEVELBUILDER::LevelBuilder* _Instance=NULL ;
RGP_LEVELBUILDER::LevelBuilder* RGP_LEVELBUILDER::LevelBuilder::Create()
{
	if (_Instance)
		return _Instance;
	_Instance = new LevelBuilder();
	if (!_Instance)
		return NULL;
	if (!_Instance->Init()) {
		delete _Instance;
		_Instance = NULL;
		return NULL;
	}
	return _Instance;
};
void		RGP_LEVELBUILDER::LevelBuilder::Release()
{
	if (_Instance) {
		delete _Instance;
		_Instance = NULL; 
	}
};

void RGP_LEVELBUILDER::LevelBuilder::StartLoop()
{
	if (!m_isInitilized)
		return;
	while (true) {
		glfwPollEvents();
		if (glfwWindowShouldClose(m_RendererInstance->getTarget()->getglfwWindow()))
			break;
		ReactToEvents();
		//TODO : implement select item function
		//TODO : add select item window


		m_RendererInstance->RenderCurrentScene();
		ImGui_ImplGlfwGL3_NewFrame();
		this->ActorsListingToolBox();
		this->BaseActorToolBox();
		this->SelectFileBox();
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		
		m_RendererInstance->SwapBuffers();
	}
};

RGP_LEVELBUILDER::LevelBuilder::LevelBuilder():m_isInitilized(false),
												m_RendererInstance(NULL), m_PhysicsEngineInstance(NULL),
												m_SoundEngineInstance(NULL), m_SceneInstance(NULL),
												m_RenderThread(NULL), m_Camera(NULL),
												m_CurrentCommand(COMMAND_NONE), m_SelectedAxis(TRANSFORM_ON_ALL),
												m_SelectedPosition{0.0f,0.0f,0.0f},
												m_SelectedRotation{0.0f,0.0f,0.0f},
												m_SelectedScale{0.0f,0.0f,0.0f},
												m_FilenameUse(USE_NOTHING)
{

};
RGP_LEVELBUILDER::LevelBuilder::~LevelBuilder()
{
	this->Destroy();
};
void				RGP_LEVELBUILDER::LevelBuilder::Destroy()
{
	m_isInitilized = false;
	m_RendererInstance=NULL ;
	m_PhysicsEngineInstance=NULL ;
	m_SoundEngineInstance=NULL ; 
	if (m_SceneInstance) {
		delete m_SceneInstance;
		m_SceneInstance = NULL;
	}
	if (m_RenderThread) {
		delete m_RenderThread;
		m_RenderThread = NULL;
	}
	if (m_Camera) {
		delete m_Camera;
		m_Camera = NULL;
	}
	ImGui_ImplGlfwGL3_Shutdown();
	if (m_RendererInstance) {
		delete m_RendererInstance;
		m_RendererInstance = NULL;
	}
};
_bool				RGP_LEVELBUILDER::LevelBuilder::Init()
{
	if (!InitClassesDatabase()) {
		printf("error init classes database \n");
		return false;
	}
	m_SceneInstance = new CustomScene();
	if (!m_SceneInstance) {
		this->Destroy();
		return false;
	}
	m_Camera = new PerspCamera(M_PI_2, 1200.0f / 900.0f, 0.1f, 5000.0f);
	m_SceneInstance->setCamera(m_Camera);
	m_Camera->setPosition({ 0.0f,12.0f,-10.0f });
	m_Camera->setOrientation({ 0.0f, -0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f });
	m_RendererInstance = new RGP_CORE::GLRenderer();
	if (!m_RendererInstance->InitRenderer({ Title,1200,900,false ,1024,false })) {
		return false;
	}
	m_RendererInstance->setScene(m_SceneInstance);

	m_PhysicsEngineInstance = new PhysicsEngine();
	m_PhysicsEngineInstance->Init(m_SceneInstance);
	InitClassesDatabase();
	LoadDefaultScene();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(m_RendererInstance->getTarget()->getglfwWindow(), true);
	ImGui::StyleColorsDark();

	m_isInitilized = true;

	return true;
};

_bool				RGP_LEVELBUILDER::LevelBuilder::isInitialized() { return m_isInitilized; };
BaseActor*			RGP_LEVELBUILDER::LevelBuilder::addActor(_s8b* Actortype, _s8b* instanceName) 
{
	BaseActor* instance = NULL;
	_u32b index =
	RGP_CORE::Class_DB::find(Actortype);
	if (!index)
		return false;
	BaseActor* (*ptr)(void**) = ((BaseActor*(*)(void** args))
		RGP_CORE::Class_DB::m_Classes[index - 1].createInstanceMethod) ;
	instance = (*ptr)(NULL);
	m_SceneInstance->AddActor(instance);
	return instance;
};

BaseActor*			RGP_LEVELBUILDER::LevelBuilder::getActorByIndex(_u32b Actorindex)
{
	if (m_SceneInstance)
		return m_SceneInstance->getActor(Actorindex);
	else
		return NULL;
};
void				RGP_LEVELBUILDER::LevelBuilder::removeActor(BaseActor* ptrActor)
{
	if (m_SceneInstance) {
		m_SceneInstance->RemoveActor(ptrActor);
	}
};
void				RGP_LEVELBUILDER::LevelBuilder::removeActor(_u32b index)
{
	if (m_SceneInstance)
		m_SceneInstance->RemoveActorAt(index);
};


_bool				RGP_LEVELBUILDER::LevelBuilder::LoadDefaultScene()
{
	BaseActor* Actor = NULL;
	BaseActor* (*ptr)(void**) = ((BaseActor*(*)(void** args))RGP_CORE::Class_DB::getCreateMethod("Model3D"));
	if (ptr) {
		
		void* args[2] = { m_RendererInstance,(void*)"..//test//Samples//Plane.obj" };
		Actor = (*ptr)(args);
		Actor->setName("Plane");
		m_SceneInstance->AddActor(Actor);
	}
	else
		return false;
	ptr = ((BaseActor*(*)(void** args))RGP_CORE::Class_DB::getCreateMethod("DirectionnalLight"));
	if (ptr) {
		Actor = NULL;
		Actor = (*ptr)(NULL);
		Actor->setName("light");
		m_SceneInstance->AddActor(Actor);
	}
	else
		return false;
	m_RendererInstance->reRegisterLightSources();

	return true;

};
_bool				RGP_LEVELBUILDER::LevelBuilder::ImportScene(const _s8b* filename)
{

	return false;

};
_bool				RGP_LEVELBUILDER::LevelBuilder::ExportScene(const _s8b* filename)
{
	return false;
};
_bool				RGP_LEVELBUILDER::LevelBuilder::ImportStaticModel(const _s8b* filename)
{
	BaseActor* (*ptr)(void**) = ((BaseActor*(*)(void** args))RGP_CORE::Class_DB::getCreateMethod("Model3D"));
	if (ptr) {
		BaseActor* model = NULL;
		void* args[2] = { m_RendererInstance,(void*)filename };
		model = (*ptr)(args);
		m_SceneInstance->AddActor(model);
	}
	else {
		printf("null pointer \n");
	}
	return true;
};
_bool				RGP_LEVELBUILDER::LevelBuilder::ImportDynamicModel(const _s8b* filename)
{
	return false;
};

void				RGP_LEVELBUILDER::LevelBuilder::TranslateSelected(Vertex3d translation)
{
	for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
		if(m_SceneInstance->isActorSelected(i))
			m_SceneInstance->getActor(i)->Translate(translation);
	}
};
void				RGP_LEVELBUILDER::LevelBuilder::RotateSelected(Vertex3d rotation)
{
	for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
		if (m_SceneInstance->isActorSelected(i)){
			m_SceneInstance->getActor(i)->RotateViaDirection(rotation.z);
			m_SceneInstance->getActor(i)->RotateViaUp(rotation.y);
			m_SceneInstance->getActor(i)->RotateViaSide(rotation.x);
		}
	}
}; 


void	 RGP_LEVELBUILDER::LevelBuilder::ActorsListingToolBox()
{
	char name[50] = "";
	ImGui::Begin("Scene");
	ImGui::SetWindowSize(ImVec2(200, 600));
	if (m_SceneInstance->getNumActors()) {
		for (_u32b i = 1; i < m_SceneInstance->getNumActors(); ++i) {
			ImGui::Selectable(m_SceneInstance->getActor(i )->getName(),m_SceneInstance->getMemoryCase(i));
		}
	}
	else {
		ImGui::Text("Empty");
	}

	ImGui::End();
};
void	RGP_LEVELBUILDER::LevelBuilder::BaseActorToolBox()
{
	_bool showbox = false;
	BaseActor* actor = NULL;
	Vertex4d quat;
	for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
		if (m_SceneInstance->isActorSelected(i)) {
			showbox = true;
			break;
		}
	}
	if (showbox) {
		ImGui::Begin("Transform");
		ImGui::SetWindowSize(ImVec2(200, 350));
		ImGui::Text("Position :", NULL);
		//todo: calculate average postion
		for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
			if (m_SceneInstance->isActorSelected(i)) {
				actor = m_SceneInstance->getActor(i);
				break;
			}
		}
		if (actor) {
			m_SelectedPosition[0] = actor->getPosition().x;
			m_SelectedPosition[1] = actor->getPosition().y;
			m_SelectedPosition[2] = actor->getPosition().z;
			quat = actor->getQuaternion();
			m_SelectedRotation[0] = quat.x;
			m_SelectedRotation[1] = quat.y;
			m_SelectedRotation[2] = quat.z;

		}
		
		if (ImGui::InputFloat3(" ", m_SelectedPosition)) {
			actor->setPosition({ m_SelectedPosition[0],m_SelectedPosition[2], m_SelectedPosition[2] });
		}
		ImGui::Separator();
		ImGui::Text("Rotation :", NULL);
		if (ImGui::InputFloat3(" ", m_SelectedRotation)) {
			actor->setQuaternion({ m_SelectedRotation[0],m_SelectedRotation[2], m_SelectedRotation[2],1.0f });
		}
		ImGui::Separator();
		ImGui::Text("Scale :", NULL);
		ImGui::InputFloat3(" ", m_SelectedScale);

		ImGui::End();
	}

};
void RGP_LEVELBUILDER::LevelBuilder::SelectFileBox()
{
	//temporary code
	_s8b					FileName[100];
	if (m_CurrentCommand== COMMAND_FILESELECT) {
		ImGui::Begin("Input");
		ImGui::SetWindowSize(ImVec2(800, 150));
		ImGui::Text("File :", NULL);
		ImGui::InputText("", FileName, IM_ARRAYSIZE(FileName));
		if (ImGui::Button("Load")) {
			if (m_FilenameUse == USE_IMPORT_STATIC)
				this->ImportStaticModel(FileName);
			else if (m_FilenameUse == USE_IMPORT_DYNAMIC)
				this->ImportDynamicModel(FileName);
			else if (m_FilenameUse == USE_IMPORT_SCENE)
				this->ImportScene(FileName);
			else if (m_FilenameUse == USE_EXPORT_SCENE)
				this->ExportScene(FileName);
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			m_CurrentCommand == COMMAND_NONE;
			m_FilenameUse = USE_NOTHING;
		}
		ImGui::End();
	}

};



void RGP_LEVELBUILDER::LevelBuilder::ReactToEvents()
{
	
	_double CursorPosx; 
	_double CursorPosy;
	Vertex3d ver = { 0.0f,0.0f,0.0f };
	Vertex3d transformHelper;
	RGP_CORE::Window* window= this->m_RendererInstance->getTarget();
	glfwGetCursorPos(window->getglfwWindow(), &CursorPosx, &CursorPosy);
	ver.x = ((CursorPosx - m_CursorPos.u) / window->getWidth());
	ver.y = ((CursorPosy - m_CursorPos.v) / window->getHeight());

	
	
	if (m_CurrentCommand == COMMAND_NONE) {//if no command 
		//Mouse Events(+shift)
		if (glfwGetMouseButton(window->getglfwWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			
			if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS ||
				glfwGetKey(window->getglfwWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {//shift key is pressed
				ver.x *= CONSTSTEP;
				ver.y *= CONSTSTEP;
				m_SceneInstance->getCamera()->TranslateViaSide(ver.x);
				m_SceneInstance->getCamera()->TranslateViaUp(ver.y);

			}
			else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS ||
				glfwGetKey(window->getglfwWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
				ver.y *= CONSTSTEP*3.0f;
				m_SceneInstance->getCamera()->TranslateViaDirection(ver.y*(1.0f));

			}
			else {
				m_SceneInstance->getCamera()->RotateViaUp(ver.x*(-0.2f));
				m_SceneInstance->getCamera()->RotateViaSide(ver.y*0.2f);
			}
		}
		
		//Keyboard shortcuts
		if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_G) == GLFW_PRESS) {
			m_CurrentCommand = COMMAND_GRAB;

		}
		else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_R) == GLFW_PRESS) {
			m_CurrentCommand = COMMAND_ROTATE;
		}
		else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_S) == GLFW_PRESS) {
			m_CurrentCommand = COMMAND_SCALE;
		}
		else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_I) == GLFW_PRESS &&
			(glfwGetKey(window->getglfwWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window->getglfwWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)) {
			m_CurrentCommand = COMMAND_FILESELECT;
			m_FilenameUse = USE_IMPORT_STATIC;
		}
	}
	else {
		if (glfwGetMouseButton(window->getglfwWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && m_CurrentCommand != COMMAND_FILESELECT) {
			m_CurrentCommand = COMMAND_NONE;
			m_SelectedAxis = TRANSFORM_ON_ALL;
			m_FilenameUse = USE_NOTHING;
			return;
		}

		if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			m_CurrentCommand = COMMAND_NONE;
			m_SelectedAxis = TRANSFORM_ON_ALL;
			m_FilenameUse = USE_NOTHING;
			return;
		}
		else {
			if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_X) == GLFW_PRESS) {
				if (m_SelectedAxis != TRANSFORM_ON_X)
					m_SelectedAxis = TRANSFORM_ON_X;
				else
					m_SelectedAxis = TRANSFORM_ON_ALL;
			}
			if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_Y) == GLFW_PRESS) {
				if (m_SelectedAxis != TRANSFORM_ON_Y)
					m_SelectedAxis = TRANSFORM_ON_Y;
				else
					m_SelectedAxis = TRANSFORM_ON_ALL;
			}
			if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_Z) == GLFW_PRESS) {
				if (m_SelectedAxis != TRANSFORM_ON_Z)
					m_SelectedAxis = TRANSFORM_ON_Z;
				else
					m_SelectedAxis = TRANSFORM_ON_ALL;
			}
		}
	}
	
	
	if(m_CurrentCommand==COMMAND_GRAB){// if current command Grab(translate)
		
		//grab Command reaction
		ver.x *= CONSTSTEP;
		ver.y *= CONSTSTEP;
		if (m_SelectedAxis == TRANSFORM_ON_ALL) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = ScaleVertex3d(m_SceneInstance->getCamera()->getSide(), -ver.x);
					transformHelper = AddVertex3d(transformHelper, ScaleVertex3d(m_SceneInstance->getCamera()->getUp(), -ver.y));
					m_SceneInstance->getActor(i)->Translate(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_X) {

			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = ScaleVertex3d(m_SceneInstance->getActor(i)->getSide(), ver.x);
					m_SceneInstance->getActor(i)->Translate(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Y) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = ScaleVertex3d(m_SceneInstance->getActor(i)->getUp(), -ver.y);
					m_SceneInstance->getActor(i)->Translate(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Z) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = ScaleVertex3d(m_SceneInstance->getActor(i)->getDirection(), ver.x);
					m_SceneInstance->getActor(i)->Translate(transformHelper);
				}
			}
		}
	}
	else if (m_CurrentCommand == COMMAND_SCALE) {
		//SCale command reaction
		if (m_SelectedAxis == TRANSFORM_ON_ALL) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					m_SceneInstance->getActor(i)->ScaleUniformAdd(ver.y);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_X) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = m_SceneInstance->getActor(i)->getScale();
					transformHelper.x += ver.x;
					m_SceneInstance->getActor(i)->Scale(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Y) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = m_SceneInstance->getActor(i)->getScale();
					transformHelper.y += ver.y;
					m_SceneInstance->getActor(i)->Scale(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Z) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = m_SceneInstance->getActor(i)->getScale();
					transformHelper.z += ver.x;
					m_SceneInstance->getActor(i)->Scale(transformHelper);
				}
			}
		}
	}
	else if (m_CurrentCommand == COMMAND_ROTATE) {
		//Rotation Command Reaction
		 if (m_SelectedAxis == TRANSFORM_ON_X) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					m_SceneInstance->getActor(i)->RotateViaSide(ver.x);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Y) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					m_SceneInstance->getActor(i)->RotateViaUp(ver.y);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Z) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					m_SceneInstance->getActor(i)->RotateViaDirection(ver.x);
				}
			}
		}

	}
	
	glfwGetCursorPos(window->getglfwWindow(), &CursorPosx, &CursorPosy);
	m_CursorPos.u = (_float)CursorPosx;
	m_CursorPos.v = (_float)CursorPosy;
	
};
















//this part is just copy and paste from the example files


// OpenGL3 Render function.
// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so. 
void ImGui_ImplGlfwGL3_RenderDrawData(ImDrawData* draw_data)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Backup GL state
	GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
	glActiveTexture(GL_TEXTURE0);
	GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
	GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
	GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
	GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
	GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
	GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
	GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Setup viewport, orthographic projection matrix
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	const float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
	{ 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
	{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
	{ -1.0f,                  1.0f,                   0.0f, 1.0f },
	};
	glUseProgram(g_ShaderHandle);
	glUniform1i(g_AttribLocationTex, 0);
	glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
	glBindSampler(0, 0); // Rely on combined texture/sampler state.

						 // Recreate the VAO every time 
						 // (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)
	GLuint vao_handle = 0;
	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);
	glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
	glEnableVertexAttribArray(g_AttribLocationPosition);
	glEnableVertexAttribArray(g_AttribLocationUV);
	glEnableVertexAttribArray(g_AttribLocationColor);
	glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

	// Draw
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}
	glDeleteVertexArrays(1, &vao_handle);

	// Restore modified GL state
	glUseProgram(last_program);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindSampler(0, last_sampler);
	glActiveTexture(last_active_texture);
	glBindVertexArray(last_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
	if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
	if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
	if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
	if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

static const char* ImGui_ImplGlfwGL3_GetClipboardText(void* user_data)
{
	return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void ImGui_ImplGlfwGL3_SetClipboardText(void* user_data, const char* text)
{
	glfwSetClipboardString((GLFWwindow*)user_data, text);
}

void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow*, int button, int action, int mods)
{
	if (action == GLFW_PRESS && button >= 0 && button < 3)
		g_MouseJustPressed[button] = true;
}

void ImGui_ImplGlfw_ScrollCallback(GLFWwindow*, double xoffset, double yoffset)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += (float)xoffset;
	io.MouseWheel += (float)yoffset;
}

void ImGui_ImplGlfw_KeyCallback(GLFWwindow*, int key, int, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
		io.KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;

	(void)mods; // Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void ImGui_ImplGlfw_CharCallback(GLFWwindow*, unsigned int c)
{
	ImGuiIO& io = ImGui::GetIO();
	if (c > 0 && c < 0x10000)
		io.AddInputCharacter((unsigned short)c);
}

bool ImGui_ImplGlfwGL3_CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

															  // Upload texture to graphics system
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &g_FontTexture);
	glBindTexture(GL_TEXTURE_2D, g_FontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

	// Restore state
	glBindTexture(GL_TEXTURE_2D, last_texture);

	return true;
}

bool ImGui_ImplGlfwGL3_CreateDeviceObjects()
{
	// Backup GL state
	GLint last_texture, last_array_buffer, last_vertex_array;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

	const GLchar *vertex_shader =
		"#version 150\n"
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
		"#version 150\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
		"}\n";

	g_ShaderHandle = glCreateProgram();
	g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
	g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
	glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
	glCompileShader(g_VertHandle);
	glCompileShader(g_FragHandle);
	glAttachShader(g_ShaderHandle, g_VertHandle);
	glAttachShader(g_ShaderHandle, g_FragHandle);
	glLinkProgram(g_ShaderHandle);

	g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
	g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
	g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
	g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
	g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

	glGenBuffers(1, &g_VboHandle);
	glGenBuffers(1, &g_ElementsHandle);

	ImGui_ImplGlfwGL3_CreateFontsTexture();

	// Restore modified GL state
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindVertexArray(last_vertex_array);

	return true;
}

void    ImGui_ImplGlfwGL3_InvalidateDeviceObjects()
{
	if (g_VboHandle) glDeleteBuffers(1, &g_VboHandle);
	if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);
	g_VboHandle = g_ElementsHandle = 0;

	if (g_ShaderHandle && g_VertHandle) glDetachShader(g_ShaderHandle, g_VertHandle);
	if (g_VertHandle) glDeleteShader(g_VertHandle);
	g_VertHandle = 0;

	if (g_ShaderHandle && g_FragHandle) glDetachShader(g_ShaderHandle, g_FragHandle);
	if (g_FragHandle) glDeleteShader(g_FragHandle);
	g_FragHandle = 0;

	if (g_ShaderHandle) glDeleteProgram(g_ShaderHandle);
	g_ShaderHandle = 0;

	if (g_FontTexture)
	{
		glDeleteTextures(1, &g_FontTexture);
		ImGui::GetIO().Fonts->TexID = 0;
		g_FontTexture = 0;
	}
}

static void ImGui_ImplGlfw_InstallCallbacks(GLFWwindow* window)
{
	glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
	glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
	glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
	glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
}

bool    ImGui_ImplGlfwGL3_Init(GLFWwindow* window, bool install_callbacks)
{
	g_Window = window;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                         // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	io.SetClipboardTextFn = ImGui_ImplGlfwGL3_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ImplGlfwGL3_GetClipboardText;
	io.ClipboardUserData = g_Window;
#ifdef _WIN32
	io.ImeWindowHandle = glfwGetWin32Window(g_Window);
#endif

	// Load cursors
	// FIXME: GLFW doesn't expose suitable cursors for ResizeAll, ResizeNESW, ResizeNWSE. We revert to arrow cursor for those.
	g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

	if (install_callbacks)
		ImGui_ImplGlfw_InstallCallbacks(window);

	return true;
}

void ImGui_ImplGlfwGL3_Shutdown()
{
	// Destroy GLFW mouse cursors
	for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
		glfwDestroyCursor(g_MouseCursors[cursor_n]);
	memset(g_MouseCursors, 0, sizeof(g_MouseCursors));

	// Destroy OpenGL objects
	ImGui_ImplGlfwGL3_InvalidateDeviceObjects();
}

void ImGui_ImplGlfwGL3_NewFrame()
{
	if (!g_FontTexture)
		ImGui_ImplGlfwGL3_CreateDeviceObjects();

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	int display_w, display_h;
	glfwGetWindowSize(g_Window, &w, &h);
	glfwGetFramebufferSize(g_Window, &display_w, &display_h);
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Setup time step
	double current_time = glfwGetTime();
	io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	g_Time = current_time;

	// Setup inputs
	// (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
	if (glfwGetWindowAttrib(g_Window, GLFW_FOCUSED))
	{
		if (io.WantMoveMouse)
		{
			glfwSetCursorPos(g_Window, (double)io.MousePos.x, (double)io.MousePos.y);   // Set mouse position if requested by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
		}
		else
		{
			double mouse_x, mouse_y;
			glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
			io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
		}
	}
	else
	{
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	}

	for (int i = 0; i < 3; i++)
	{
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
		g_MouseJustPressed[i] = false;
	}

	// Update OS/hardware mouse cursor if imgui isn't drawing a software cursor
	ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
	if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None)
	{
		glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		glfwSetCursor(g_Window, g_MouseCursors[cursor] ? g_MouseCursors[cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
		glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// Gamepad navigation mapping [BETA]
	memset(io.NavInputs, 0, sizeof(io.NavInputs));
	if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)
	{
		// Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { if (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS) io.NavInputs[NAV_NO] = 1.0f; }
#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); if (v > 1.0f) v = 1.0f; if (io.NavInputs[NAV_NO] < v) io.NavInputs[NAV_NO] = v; }
		int axes_count = 0, buttons_count = 0;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
		MAP_BUTTON(ImGuiNavInput_Activate, 0);     // Cross / A
		MAP_BUTTON(ImGuiNavInput_Cancel, 1);     // Circle / B
		MAP_BUTTON(ImGuiNavInput_Menu, 2);     // Square / X
		MAP_BUTTON(ImGuiNavInput_Input, 3);     // Triangle / Y
		MAP_BUTTON(ImGuiNavInput_DpadLeft, 13);    // D-Pad Left
		MAP_BUTTON(ImGuiNavInput_DpadRight, 11);    // D-Pad Right
		MAP_BUTTON(ImGuiNavInput_DpadUp, 10);    // D-Pad Up
		MAP_BUTTON(ImGuiNavInput_DpadDown, 12);    // D-Pad Down
		MAP_BUTTON(ImGuiNavInput_FocusPrev, 4);     // L1 / LB
		MAP_BUTTON(ImGuiNavInput_FocusNext, 5);     // R1 / RB
		MAP_BUTTON(ImGuiNavInput_TweakSlow, 4);     // L1 / LB
		MAP_BUTTON(ImGuiNavInput_TweakFast, 5);     // R1 / RB
		MAP_ANALOG(ImGuiNavInput_LStickLeft, 0, -0.3f, -0.9f);
		MAP_ANALOG(ImGuiNavInput_LStickRight, 0, +0.3f, +0.9f);
		MAP_ANALOG(ImGuiNavInput_LStickUp, 1, +0.3f, +0.9f);
		MAP_ANALOG(ImGuiNavInput_LStickDown, 1, -0.3f, -0.9f);
#undef MAP_BUTTON
#undef MAP_ANALOG
	}

	// Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
	ImGui::NewFrame();
}