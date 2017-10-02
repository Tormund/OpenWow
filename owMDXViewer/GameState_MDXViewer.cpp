#include "stdafx.h"

// General
#include "GameState_MDXViewer.h"

// FIXME DELETE ME
GLint currentColor;

bool GameState_MDXViewer::Init()
{
	_Input->AddInputListener(this);

	_ModulesMgr->LoadModule(_ModelsMgr, true);

	enableFreeCamera = false;
	cameraSprint = false;

	window = new UIWindow();
	window->Init(VECTOR_ZERO, _Settings->GetWindowSize(), nullptr);
	_UIMgr->Attach(window);

	auto camera = new Camera;
	_PipelineGlobal->SetCamera(camera);
	_PipelineGlobal->SetProjection(45.0f, _Settings->aspectRatio, 0.1f, 10000.0f);

	currentModel = nullptr;
	if (_Engine->GetArgument(0).empty())
	{
		Debug::Error("Not model name!");
		return false;
	}

	currentModel = new MDX(_Engine->GetArgument(0));
	currentModel->m_IsBillboard = true;

	_Camera->Position = vec3(30, 30, 30);
	_Camera->Yaw = -135;
	_Camera->Pitch = -45;
	_Camera->Update();
	return true;
}

void GameState_MDXViewer::Destroy()
{
	//window->Delete();

	m_inited = false;

	_Input->DeleteInputListener(this);
}

void GameState_MDXViewer::InputPhase(double t, double dt)
{
	float delta = PI / 60.0f;
	float speed = 1.5f * (cameraSprint ? 2.0f : 0.5f);

	if (_Input->IsKeyPressed(GLFW_KEY_W))
	{
		_Camera->ProcessKeyboard(FORWARD, speed);
	}

	if (_Input->IsKeyPressed(GLFW_KEY_S))
	{
		_Camera->ProcessKeyboard(BACKWARD, speed);
	}

	if (_Input->IsKeyPressed(GLFW_KEY_A))
	{
		_Camera->ProcessKeyboard(LEFT, speed);
	}

	if (_Input->IsKeyPressed(GLFW_KEY_D))
	{
		_Camera->ProcessKeyboard(RIGHT, speed);
	}
}

void GameState_MDXViewer::UpdatePhase(double t, double dt)
{
	if (currentModel)
	{
		currentModel->updateEmitters(dt);
	}
}

void GameState_MDXViewer::Render(double t, double dt)
{
	_Camera->Update();

	if (currentModel != nullptr)
	{
		vec4 la(0.1f, 0.1f, 0.1f, 1);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(la));

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor4f(1, 1, 1, 1);
		for (int i = 0; i < 8; i++)
		{
			GLuint light = GL_LIGHT0 + i;
			glLightf(light, GL_CONSTANT_ATTENUATION, 0);
			glLightf(light, GL_LINEAR_ATTENUATION, 0.7f);
			glLightf(light, GL_QUADRATIC_ATTENUATION, 0.03f);
			glEnable(light);
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_LIGHTING);

		//currentModel->m_ModelCamera->setup(globalTime);
		currentModel->draw();
	}
}

void GameState_MDXViewer::RenderUI(double t, double dt)
{
	_Render->RenderText(vec2(5, _Settings->GetWindowSize().y - 22), "Cam:" + to_string(_Camera->Position.x) + "," + to_string(_Camera->Position.y) + "," + to_string(_Camera->Position.z));
	_Render->RenderText(vec2(5, _Settings->GetWindowSize().y - 44), "CamDir:" + to_string(_Camera->Direction.x) + "," + to_string(_Camera->Direction.y) + "," + to_string(_Camera->Direction.z));
	_Render->RenderText(vec2(5, _Settings->GetWindowSize().y - 66), "CamDirYawPitch:" + to_string(_Camera->Yaw) + "," + to_string(_Camera->Pitch));
}

//

#pragma region Input functional

MOUSE_MOVED_(GameState_MDXViewer)
{
	if (enableFreeCamera)
	{
		vec2 mouseDelta = (_mousePos - lastMousePos) / _Settings->GetWindowSize();

		_Camera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

		_GLFW->SetMousePosition(lastMousePos);
	}
}

MOUSE_PRESSED(GameState_MDXViewer)
{
	if (_button == GLFW_MOUSE_BUTTON_LEFT)
	{
		enableFreeCamera = true;
		lastMousePos = _mousePos;
		_GLFW->HideCursor();
		return true;
	}

	return false;
}

MOUSE_RELEASE(GameState_MDXViewer)
{
	if (_button == GLFW_MOUSE_BUTTON_LEFT)
	{
		enableFreeCamera = false;
		lastMousePos = VECTOR_ZERO;
		_GLFW->ShowCursor();
		return true;
	}

	return false;
}

MOUSE_WHEEL(GameState_MDXViewer)
{
	return false;
}

KEYBD_PRESSED(GameState_MDXViewer)
{
	if (_key == GLFW_KEY_ESCAPE)
	{
		Debug::Exit(0);
	}

	if (_key == GLFW_KEY_X)
	{
		cameraSprint = true;
		return true;
	}

	if (_key == GLFW_KEY_L)
	{
		_Settings->lighting = !_Settings->lighting;
		return true;
	}

	if (_key == GLFW_KEY_F5)
	{
		_Settings->drawmodels = !_Settings->drawmodels;
		return true;
	}
	if (_key == GLFW_KEY_F6)
	{
		_Settings->draw_map_wmo_doodads = !_Settings->draw_map_wmo_doodads;
		return true;
	}
	if (_key == GLFW_KEY_F7)
	{
		_Settings->drawterrain = !_Settings->drawterrain;
		return true;
	}
	if (_key == GLFW_KEY_F8)
	{
		_Settings->drawwmo = !_Settings->drawwmo;
		return true;
	}

	if (_key == GLFW_KEY_C)
	{
		_Settings->drawColors = !_Settings->drawColors;
		return true;
	}

	if (_key == GLFW_KEY_H)
	{
		_Settings->drawhighres = !_Settings->drawhighres;
		return true;
	}

	if (_key == GLFW_KEY_F)
	{
		_Settings->drawfog = !_Settings->drawfog;
		return true;
	}

	if (_key == GLFW_KEY_1)
	{
		currentColor = GL_COLOR_ATTACHMENT0;
		return true;
	}

	if (_key == GLFW_KEY_2)
	{
		currentColor = GL_COLOR_ATTACHMENT1;
		return true;
	}

	if (_key == GLFW_KEY_3)
	{
		currentColor = GL_COLOR_ATTACHMENT2;
		return true;
	}

	if (_key == GLFW_KEY_4)
	{
		currentColor = GL_COLOR_ATTACHMENT3;
		return true;
	}

	if (_key == GLFW_KEY_5)
	{
		currentColor = GL_COLOR_ATTACHMENT4;
		return true;
	}

	if (_key == GLFW_KEY_6)
	{
		currentColor = GL_COLOR_ATTACHMENT5;
		return true;
	}

	return false;
}

KEYBD_RELEASE(GameState_MDXViewer)
{
	if (_key == GLFW_KEY_X)
	{
		cameraSprint = false;
		return true;
	}

	return false;
}

#pragma endregion

void GameState_MDXViewer::randBackground()
{
	if (currentModel != nullptr)
	{
		delete currentModel;
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	char* ui[] = {"MainMenu", "NightElf", "Human", "Dwarf", "Orc", "Tauren", "Scourge"};

	char* randui = ui[Random::GenerateMax(7)];
	char path[256];
	sprintf_s(path, "Interface\\Glues\\Models\\UI_%s\\UI_%s.m2", randui, randui);

	currentModel = new MDX(path);
	currentModel->m_IsBillboard = true;
}