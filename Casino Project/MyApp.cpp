#include "MyApp.h"
#include "ObjParser.h"
#include "SDL_GLDebugMessageCallback.h"
#include<iostream>

#include <imgui.h>

CMyApp::CMyApp()
{
}

CMyApp::~CMyApp()
{
}

void CMyApp::SetupDebugCallback()
{
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
}

void CMyApp::InitShaders()
{
	m_programID = glCreateProgram();
	AttachShader(m_programID, GL_VERTEX_SHADER, "Shaders/Vert_PosNormTex.vert");
	AttachShader(m_programID, GL_FRAGMENT_SHADER, "Shaders/Frag_ZH.frag");
	LinkProgram(m_programID);
}

void CMyApp::CleanShaders()
{
	glDeleteProgram(m_programID);
}

MeshObject<Vertex> createQuad()
{
	MeshObject<Vertex> mesh;

	mesh.vertexArray = {
		{{-0.5f, 0.0f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, 0.0f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.0f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, 0.0f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 0.5f, 0.0f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.0f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
	};

	mesh.indexArray = { 0, 1, 2, 2, 3, 0, 4, 6, 5, 6, 4, 7 };

	return mesh;
}

void CMyApp::InitGeometry()
{
	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{0, offsetof(Vertex, position), 3, GL_FLOAT},
		{1, offsetof(Vertex, normal), 3, GL_FLOAT},
		{2, offsetof(Vertex, texcoord), 2, GL_FLOAT},
	};

	m_quadGPU = CreateGLObjectFromMesh(createQuad(), vertexAttribList);

	MeshObject<Vertex> mesh;

	mesh = ObjParser::parse("Assets/hand.obj");
	m_handGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/pokerchip.obj");
	m_pokerChipGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);


	for (int i = 0; i < 5; i++)
	{
		glm::vec3 position = glm::vec3((2 * i) - 4, 0, 0);
		Card card = { position, i };
		cards.push_back(card);
	}

}

void CMyApp::CleanGeometry()
{
	CleanOGLObject(m_quadGPU);
}

void CMyApp::InitTextures()
{
	glCreateSamplers(1, &m_SamplerID);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	ImageRGBA image = ImageFromFile("Assets/card_back.png");
	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
	glTextureStorage2D(m_TextureID, NumberOfMIPLevels(image), GL_RGBA8, image.width, image.height);
	glTextureSubImage2D(m_TextureID, 0, 0, 0, image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	glGenerateTextureMipmap(m_TextureID);

	ImageRGBA imageCardBack = ImageFromFile("Assets/card_back.png");
	glCreateTextures(GL_TEXTURE_2D, 1, &m_CardBackTextureID);
	glTextureStorage2D(m_CardBackTextureID, NumberOfMIPLevels(imageCardBack), GL_RGBA8, imageCardBack.width, imageCardBack.height);
	glTextureSubImage2D(m_CardBackTextureID, 0, 0, 0, imageCardBack.width, imageCardBack.height, GL_RGBA, GL_UNSIGNED_BYTE, imageCardBack.data());
	glGenerateTextureMipmap(m_CardBackTextureID);

	ImageRGBA imageCards = ImageFromFile("Assets/cards.png");
	glCreateTextures(GL_TEXTURE_2D, 1, &m_CardsTextureID);
	glTextureStorage2D(m_CardsTextureID, NumberOfMIPLevels(imageCards), GL_RGBA8, imageCards.width, imageCards.height);
	glTextureSubImage2D(m_CardsTextureID, 0, 0, 0, imageCards.width, imageCards.height, GL_RGBA, GL_UNSIGNED_BYTE, imageCards.data());
	glGenerateTextureMipmap(m_CardsTextureID);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_AnotherCardsTextureID);


	ImageRGBA imageGreenFabric  = ImageFromFile("Assets/green_fabric.jpg");
	glCreateTextures(GL_TEXTURE_2D, 1, &m_GreenFabricTextureID);
	glTextureStorage2D(m_GreenFabricTextureID, NumberOfMIPLevels(imageGreenFabric), GL_RGBA8, imageGreenFabric.width, imageGreenFabric.height);
	glTextureSubImage2D(m_GreenFabricTextureID, 0, 0, 0, imageGreenFabric.width, imageGreenFabric.height, GL_RGBA, GL_UNSIGNED_BYTE, imageGreenFabric.data());
	glGenerateTextureMipmap(m_GreenFabricTextureID);

	ImageRGBA imageHand = ImageFromFile("Assets/hand.png");
	glCreateTextures(GL_TEXTURE_2D, 1, &m_HandTextureID);
	glTextureStorage2D(m_HandTextureID, NumberOfMIPLevels(imageHand), GL_RGBA8, imageHand.width, imageHand.height);
	glTextureSubImage2D(m_HandTextureID, 0, 0, 0, imageHand.width, imageHand.height, GL_RGBA, GL_UNSIGNED_BYTE, imageHand.data());
	glGenerateTextureMipmap(m_HandTextureID);

	ImageRGBA imagePokerChip = ImageFromFile("Assets/pokerchip.jpg");
	glCreateTextures(GL_TEXTURE_2D, 1, &m_PokerChipTextureID);
	glTextureStorage2D(m_PokerChipTextureID, NumberOfMIPLevels(imagePokerChip), GL_RGBA8, imagePokerChip.width, imagePokerChip.height);
	glTextureSubImage2D(m_PokerChipTextureID, 0, 0, 0, imagePokerChip.width, imagePokerChip.height, GL_RGBA, GL_UNSIGNED_BYTE, imagePokerChip.data());
	glGenerateTextureMipmap(m_PokerChipTextureID);
}

void CMyApp::CleanTextures()
{
	glDeleteSamplers(1, &m_SamplerID);
	glDeleteTextures(1, &m_TextureID);
	glDeleteTextures(1, &m_CardBackTextureID);
	glDeleteTextures(1, &m_CardsTextureID);
	glDeleteTextures(1, &m_GreenFabricTextureID);
	glDeleteTextures(1, &m_HandTextureID);
	glDeleteTextures(1, &m_PokerChipTextureID);
}

bool CMyApp::Init()
{
	SetupDebugCallback();

	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	InitShaders();
	InitGeometry();
	InitTextures();


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	// Camera
	m_camera.SetView(
		glm::vec3(0.0, 30.0, 25.0),  //  - eye
		glm::vec3(0.0, 0.0, 0.0),  //  - at
		glm::vec3(0.0, 1.0, 0.0)); //  - up

	m_cameraManipulator.SetCamera(&m_camera);

	return true;
}

void CMyApp::Clean()
{
	CleanShaders();
	CleanGeometry();
	CleanTextures();
}

static bool HitPlane(const Ray& ray, const glm::vec3& planeQ, const glm::vec3& planeI, const glm::vec3& planeJ, Intersection& result)
{
	// palneQ + u * planeI + v * planeJ
	glm::mat3 A(-ray.direction, planeI, planeJ);
	glm::vec3 B = ray.origin - planeQ;

	if (fabsf(glm::determinant(A)) < 1e-6) return false;
	glm::vec3 X = glm::inverse(A) * B;

	if (X.x < 0.0) {
		return false;
	}
	result.t = X.x;
	result.uv.x = X.y;
	result.uv.y = X.z;

	return true;

}



static bool HitSphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius, float& t)
{
	glm::vec3 p_m_c = rayOrigin - sphereCenter;
	float a = glm::dot(rayDir, rayDir);
	float b = 2.0f * glm::dot(rayDir, p_m_c);
	float c = glm::dot(p_m_c, p_m_c) - sphereRadius * sphereRadius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0.0f)
	{
		return false;
	}

	float sqrtDiscriminant = sqrtf(discriminant);

	
	float t0 = (-b - sqrtDiscriminant) / (2.0f * a);
	float t1 = (-b + sqrtDiscriminant) / (2.0f * a);

	if (t1 < 0.0f)
	{
		return false;
	}

	if (t0 < 0.0f)
	{
		t = t1;
	}
	else
	{
		t = t0;
	}

	return true;
}

Ray CMyApp::CalculatePixelRay(glm::vec2 pixel) const
{
	glm::vec4 ndcNear = glm::vec4(
		2.0f * (pixel.x + 0.5f) / m_windowSize.x - 1.0f,
		1.0f - 2.0f * (pixel.y + 0.5f) / m_windowSize.y,
		-1.0f,  // z = -1 for near plane in NDC
		1.0f);

	glm::vec4 ndcFar = glm::vec4(ndcNear.x, ndcNear.y, 1.0f, 1.0f); // z = 1 for far plane

	glm::mat4 invVP = glm::inverse(m_camera.GetViewProj());

	glm::vec4 worldNear = invVP * ndcNear;
	glm::vec4 worldFar = invVP * ndcFar;

	worldNear /= worldNear.w;
	worldFar /= worldFar.w;

	Ray ray;
	ray.origin = glm::vec3(worldNear);
	ray.direction = glm::normalize(glm::vec3(worldFar - worldNear));

	return ray;
}


bool CMyApp::intersectTableTop(const Ray& ray, const glm::vec3& tablePos, const glm::vec3& tableSize, glm::vec3& result)
{
	// plane
	glm::vec3 planeQ = tablePos + 0.5f * glm::vec3(-tableSize.x, tableSize.y, -tableSize.z);
	glm::vec3 planeI = glm::vec3(tableSize.x, 0.0f, 0.0f);
	glm::vec3 planeJ = glm::vec3(0.0f, 0.0f, tableSize.z);
	Intersection inter;
	// if there is an intersection, and it is within the square
	if (HitPlane(ray, planeQ, planeI, planeJ, inter)
		&& (inter.uv.x >= 0 && inter.uv.x <= 1)
		&& (inter.uv.y >= 0 && inter.uv.y <= 1)
		) {
		const glm::vec2& uv = inter.uv;
		result = planeQ + planeI * uv.x + planeJ * uv.y;
		return true;
	}
	else {
		return false;
	}
}


void CMyApp::Update(const SUpdateInfo& updateInfo)
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;

	if (m_IsPicking) {
		// starting a ray through the clicked pixel
		Ray ray = CalculatePixelRay(glm::vec2(m_PickedPixel.x, m_PickedPixel.y));
		// intersecting with the table
		glm::vec3 desiredPos;
		if (intersectTableTop(ray, TABLE_POS, TABLE_SIZE, desiredPos)) {

			int gridX = static_cast<int>(std::round(desiredPos.x / 2.0f)) * 2;
			int gridZ = static_cast<int>(std::round(desiredPos.z / 3.0f)) * 3;


			Card newCard;
			newCard.position = glm::vec3(gridX, 0.0f, gridZ);
			newCard.type = m_selectedType;

			cards.push_back(newCard);
			m_cardsCount++;
		}
	
		
        m_IsPicking = false;
	}

	float effectiveTime = m_ElapsedTimeInSec - m_timeOffset;

	if (m_isLampStopped) {
		effectiveTime = m_pauseTime - m_timeOffset;
	}

	float t = sin(effectiveTime * 2 * PI / 2) * 0.5f + 0.5;
	m_lightPos = glm::vec4(-15 + 30 * t, 10 - 20 * t + 20 * t * t, 0, 1);

	m_cameraManipulator.Update(updateInfo.DeltaTimeInSec);
}

void CMyApp::SetCommonUniforms()
{
	glProgramUniformMatrix4fv(m_programID, ul(m_programID, "viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));

    glProgramUniform3fv(m_programID, ul(m_programID, "cameraPos"), 1, glm::value_ptr(m_camera.GetEye()));
	glProgramUniform4fv(m_programID, ul(m_programID, "lightPos"), 1, glm::value_ptr(m_lightPos));
	
	glProgramUniform3fv(m_programID, ul(m_programID, "La"), 1, glm::value_ptr(m_La));
	glProgramUniform3fv(m_programID, ul(m_programID, "Ld"), 1, glm::value_ptr(m_Ld));
	glProgramUniform3fv(m_programID, ul(m_programID, "Ls"), 1, glm::value_ptr(m_Ls));
	
	glProgramUniform1f(m_programID, ul(m_programID, "lightConstantAttenuation"), m_lightConstantAttenuation);
	glProgramUniform1f(m_programID, ul(m_programID, "lightLinearAttenuation"), m_lightLinearAttenuation);
	glProgramUniform1f(m_programID, ul(m_programID, "lightQuadraticAttenuation"), m_lightQuadraticAttenuation);


	glProgramUniform1f(m_programID, ul(m_programID, "isLampOn"), m_isLampOn);
	
	glProgramUniform1i(m_programID, ul(m_programID, "state"), m_state);
	glProgramUniform1i(m_programID, ul(m_programID, "chipCol"), m_chipCol);

	glProgramUniform1i(m_programID, ul(m_programID, "cardCount"), m_cardsCount);
	glProgramUniform1i(m_programID, ul(m_programID, "cardType"), 0);
}


void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetCommonUniforms();

	glUseProgram(m_programID);

	RenderTable();
	RenderCards();
	RenderHand();
	RenderChips();

	glBindTextureUnit(0, 0);
	glBindSampler(0, 0);

	glBindVertexArray(0);
	// shader
	glUseProgram(0);
}


void CMyApp::RenderTable() {
	glUniform1i(ul("texImage"), 0);
	glBindTextureUnit(0, m_GreenFabricTextureID);
	glBindSampler(0, m_SamplerID);

	m_tableTransform = glm::translate(TABLE_POS)
		* glm::scale(TABLE_SIZE);

	DrawObject(m_quadGPU, m_tableTransform);

	glBindVertexArray(0);
}

void CMyApp::RenderChips() {
	glUniform1i(ul("texImage"), 0);
	glBindTextureUnit(0, m_PokerChipTextureID);
	glBindSampler(0, m_SamplerID);

	glProgramUniform1i(m_programID, ul(m_programID, "state"), CHIP_SHADER);

	for (int i = 0; i < m_chipsCountN; i++)
	{
		m_chipCol = i % 6;
		glProgramUniform1i(m_programID, ul(m_programID, "chipCol"), m_chipCol);

		glm::mat4 chipTransform = glm::translate(glm::vec3(-10, i * 0.15f, 5)) * glm::scale(glm::vec3(1));
		DrawObject(m_pokerChipGPU, chipTransform);
	}

	static const float PI = glm::pi<float>();
	float baseAngle = 2 * PI / m_chipsCountK;
	for (int i = 0; i < m_chipsCountK; i++)
	{
		m_chipCol = i % 6;
		glProgramUniform1i(m_programID, ul(m_programID, "chipCol"), m_chipCol);
		float fi = m_ElapsedTimeInSec + i * baseAngle;
		float radius = 4.0f;
		float x = radius * cos(fi);
		float z = radius * sin(fi);
		float y_angle = atan2(z, x);
		glm::mat4 chipTransform = glm::translate(glm::vec3(-10, 0, 5))
				* glm::translate(glm::vec3(x, 0.f, z))
				* glm::translate(glm::vec3(0, 1, 0))
				* glm::rotate(y_angle, glm::vec3(0, -1, 0)) //rotate Y
				* glm::rotate(m_ElapsedTimeInSec, glm::vec3(1, 0, 0)) //rotate X
				* glm::rotate(glm::radians(90.f), glm::vec3(0, 0, 1)); //rotate Z
		DrawObject(m_pokerChipGPU, chipTransform);
	}


	glProgramUniform1i(m_programID, ul(m_programID, "state"), DEFAULT_SHADER);

	glBindVertexArray(0);
}

void CMyApp::RenderHand() {
	glUniform1i(ul("texImage"), 0);
	glBindTextureUnit(0, m_HandTextureID);
	glBindSampler(0, m_SamplerID);

	m_tableTransform = glm::translate(glm::vec3(-3, 2, 12)) * glm::rotate(m_handRot, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(3));

	DrawObject(m_handGPU, m_tableTransform);

	glBindVertexArray(0);
}

void CMyApp::RenderCards() {




	glUniform1i(ul("texImage"), 0);
	if (!m_isPressed) {
		glBindTextureUnit(0, m_CardsTextureID);
	}
	else {
		glBindTextureUnit(0, m_AnotherCardsTextureID);
	}
	glBindSampler(0, m_SamplerID);


	glProgramUniform1i(m_programID, ul(m_programID, "state"), CARD_SHADER);


	for (int i = 0; i < cards.size(); i++)
	{
		Card card = cards[i];
		card.position = cards[i].position;
		card.type = cards[i].type;
		glProgramUniform1i(m_programID, ul(m_programID, "cardType"), card.type);
		
		glm::mat4 cardTransform =  glm::translate(card.position) * glm::scale(CARD_SIZE);
		DrawObject(m_quadGPU, cardTransform);
	}


	glProgramUniform1i(m_programID, ul(m_programID, "state"), DEFAULT_SHADER);

	glBindVertexArray(0);
}

void CMyApp::DrawObject(OGLObject& obj, const glm::mat4& world) {
	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(world));
	glUniformMatrix4fv(ul("worldIT"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(world))));
	glBindVertexArray(obj.vaoID);
	glDrawElements(GL_TRIANGLES, obj.count, GL_UNSIGNED_INT, nullptr);
}

void CMyApp::RenderGUI()
{
	ImGui::SliderAngle("Hand Rotation", &m_handRot, -45, 45);
	ImGui::SliderInt("Chips N", &m_chipsCountN, 1, 20);
	ImGui::SliderInt("Chips K", &m_chipsCountK, 1, 20);
	ImGui::Checkbox("Enable light", &m_isLampOn);
	if (ImGui::Checkbox("Stop light", &m_isLampStopped)) {
		if (m_isLampStopped) {
			m_pauseTime = m_ElapsedTimeInSec;
		}
		else {
			m_timeOffset += (m_ElapsedTimeInSec - m_pauseTime);
		}
	}
	ImGui::ColorEdit3("c1", glm::value_ptr(m_c1));
	ImGui::ColorEdit3("c2", glm::value_ptr(m_c2));
	if (ImGui::Button("Update Card Texture")) {
		UpdateCardTextureWithColors(m_c1, m_c2);
		m_isPressed = true;
	}

}


void CMyApp::UpdateCardTextureWithColors(const glm::vec3& c1, const glm::vec3& c2) {
	
	ImageRGBA imageCards = ImageFromFile("Assets/cards.png");
	

	int cardWidth = imageCards.width / 5;
	int height = imageCards.height;
	
	for (int y = 0; y < height; ++y) {
		float t = float(y) / height;
		glm::vec3 interp = (1.0f - t) * c1 + t * c2;
		glm::u8vec4 color = glm::u8vec4(interp * 255.0f, 255);
		for (int x = 0; x < cardWidth; ++x) {
			imageCards.SetTexel(x, y, color);
		}
	}


	for (int card = 1; card <= 3; card+=2) {
		for (int y = 0; y < height; ++y) {
			for (int x = card * cardWidth; x < (card + 1) * cardWidth; ++x) {
				glm::u8vec4 texel = imageCards.GetTexel(x, y);
				if (texel.r == 255 && texel.g == 255 && texel.b == 255) { // white
					imageCards.SetTexel(x, y, glm::u8vec4(c1 * 255.f, 255));
				}
			}
		}
	}

	for (int card = 2; card <= 4; card += 2) {
		for (int y = 0; y < height; ++y) {
			for (int x = card * cardWidth; x < (card + 1) * cardWidth; ++x) {
				glm::u8vec4 texel = imageCards.GetTexel(x, y);
				if (texel.r == 255 && texel.g == 255 && texel.b == 255) { // white
					imageCards.SetTexel(x, y, glm::u8vec4(c2 * 255.f, 255));
				}
			}
		}
	}

	if (m_AnotherCardsTextureID != 0) {
		glDeleteTextures(1, &m_AnotherCardsTextureID);
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_AnotherCardsTextureID);
	glTextureStorage2D(m_AnotherCardsTextureID, NumberOfMIPLevels(imageCards), GL_RGBA8, imageCards.width, imageCards.height);
	glTextureSubImage2D(m_AnotherCardsTextureID, 0, 0, 0, imageCards.width, imageCards.height, GL_RGBA, GL_UNSIGNED_BYTE, imageCards.data());
	glGenerateTextureMipmap(m_AnotherCardsTextureID);
}


void CMyApp::KeyboardDown(const SDL_KeyboardEvent& key)
{
	if (key.repeat == 0)
	{
		if (key.keysym.sym == SDLK_F5 && key.keysym.mod & KMOD_CTRL)
		{
			CleanShaders();
			InitShaders();
		}
		if (key.keysym.sym == SDLK_F1)
		{
			GLint polygonModeFrontAndBack[2] = {};
			
			glGetIntegerv(GL_POLYGON_MODE, polygonModeFrontAndBack);
			GLenum polygonMode = (polygonModeFrontAndBack[0] != GL_FILL ? GL_FILL : GL_LINE);
			glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
		}

		if (key.keysym.sym == SDLK_LCTRL || key.keysym.sym == SDLK_RCTRL)
		{
			m_IsCtrlDown = true;
		}

		if (key.keysym.sym == SDLK_0)
		{
			m_selectedType = 0;
		}

		if (key.keysym.sym == SDLK_1)
		{
			m_selectedType = 1;
		}

		if (key.keysym.sym == SDLK_2)
		{
			m_selectedType = 2;
		}

		if (key.keysym.sym == SDLK_3)
		{
			m_selectedType = 3;
		}

		if (key.keysym.sym == SDLK_4)
		{
			m_selectedType = 4;
		}
	}
	m_cameraManipulator.KeyboardDown(key);
}

void CMyApp::KeyboardUp(const SDL_KeyboardEvent& key)
{
	m_cameraManipulator.KeyboardUp(key);
	if (key.keysym.sym == SDLK_LCTRL || key.keysym.sym == SDLK_RCTRL)
	{
		m_IsCtrlDown = false;
	}
}


void CMyApp::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	m_cameraManipulator.MouseMove(mouse);
}

void CMyApp::MouseDown(const SDL_MouseButtonEvent& mouse)
{
	if ( m_IsCtrlDown )
	{
		m_IsPicking = true;
	}
	m_PickedPixel = { mouse.x, mouse.y };
}

void CMyApp::MouseUp(const SDL_MouseButtonEvent& mouse)
{
    
}

void CMyApp::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
	m_cameraManipulator.MouseWheel(wheel);
}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	m_windowSize = glm::uvec2(_w, _h);
	m_camera.SetAspect(static_cast<float>(_w) / _h);
}

void CMyApp::OtherEvent(const SDL_Event& ev)
{

}
