#pragma once

#include <vector>


// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Utils
#include "Camera.h"
#include "CameraManipulator.h"
#include "GLUtils.hpp"

struct SUpdateInfo
{
	float ElapsedTimeInSec = 0.0f;
	float DeltaTimeInSec = 0.0f;
};

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

struct Intersection
{
	glm::vec2 uv;
	float t;
};

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void CMyApp::UpdateCardTextureWithColors(const glm::vec3& c1, const glm::vec3& c2);
	bool CMyApp::intersectTableTop(const Ray& ray, const glm::vec3& tablePos, const glm::vec3& tableSize, glm::vec3& result);

	void Update(const SUpdateInfo&);
	void Render();
	void RenderTable();
	void RenderCards();
	void RenderHand();
	void RenderChips();
	void RenderGUI();



	void KeyboardDown(const SDL_KeyboardEvent&);
	void KeyboardUp(const SDL_KeyboardEvent&);
	void MouseMove(const SDL_MouseMotionEvent&);
	void MouseDown(const SDL_MouseButtonEvent&);
	void MouseUp(const SDL_MouseButtonEvent&);
	void MouseWheel(const SDL_MouseWheelEvent&);
	void Resize(int, int);

	void OtherEvent(const SDL_Event&);

protected:
	void SetupDebugCallback();



	static constexpr float PI = glm::pi<float>();
	float m_ElapsedTimeInSec = 0.0f;
	
	//Shader
	static const int DEFAULT_SHADER = 0;
	static const int CHIP_SHADER = 1;
	static const int CARD_SHADER = 2;
	int m_state = DEFAULT_SHADER;
	int m_chipCol = 0;


	//Table
	glm::mat4 m_tableTransform;

	static constexpr glm::vec3 TABLE_SIZE = glm::vec3(30.0f, 1, 21.0f);
	static constexpr glm::vec3 TABLE_POS = glm::vec3(0, -0.1, 0);

	//Card
	
	int m_selectedType = 0;
	glm::vec3 m_c1 = glm::vec3(0);
	glm::vec3 m_c2 = glm::vec3(0);
	bool m_isPressed = false;

	

	struct Card {
		glm::vec3 position;
		int type;
	};

	static constexpr glm::vec3 CARD_SIZE = glm::vec3(1.6f, 1, 2.48f);

	std::vector<Card> cards;
	int m_cardsCount = 5;

	
	
	//Hand
	float m_handRot = 0;


	//Chips
	int m_chipsCountN = 10;
	int m_chipsCountK = 5;

	//Light
	glm::vec4 m_lightPos = glm::vec4(0, 10, 0, 1);
	glm::vec3 m_La = glm::vec3(0.1);
	glm::vec3 m_Ld = glm::vec3(1);
	glm::vec3 m_Ls = glm::vec3(1);

	float m_lightConstantAttenuation = 1.0;
	float m_lightLinearAttenuation = 0.0;
	float m_lightQuadraticAttenuation = 0.001;

	bool m_isLampOn = true;
	bool m_isLampStopped = false;
	float m_pauseTime = 0;
	float m_timeOffset = 0;
	// Picking

	glm::ivec2 m_PickedPixel = glm::ivec2( 0, 0 );
	bool m_IsPicking = false;
	bool m_IsCtrlDown = false;

	glm::uvec2 m_windowSize = glm::uvec2(0, 0);

	Ray CalculatePixelRay(glm::vec2 pickerPos) const;


	// Camera
	Camera m_camera;
	CameraManipulator m_cameraManipulator;


	
	GLuint m_programID = 0;


	void InitShaders();
	void CleanShaders();


	void SetCommonUniforms();
	void DrawObject(OGLObject& obj, const glm::mat4& world);


	OGLObject m_quadGPU = {};
	OGLObject m_handGPU = {};
	OGLObject m_pokerChipGPU = {};

	void InitGeometry();
	void CleanGeometry();

	GLuint m_SamplerID = 0;

	GLuint m_TextureID = 0;
	GLuint m_CardBackTextureID = 0;
	GLuint m_CardsTextureID = 0;
	GLuint m_AnotherCardsTextureID = 0;
	GLuint m_GreenFabricTextureID = 0;
	GLuint m_HandTextureID = 0;
	GLuint m_PokerChipTextureID = 0;
	
	void InitTextures();
	void CleanTextures();
};
