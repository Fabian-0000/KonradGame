#include "MainLoop.hpp"
#include "Renderer.hpp"
#include "DarkMode.hpp"
#include "MainView.hpp"

#include "Object.hpp"
#include "Engine.hpp"

#include "Input.hpp"
#include "Level.hpp"
#include "Elevator.hpp"
#include "State.hpp"

#include "Time.hpp"

#include "Particle.hpp"

#include "SaveData.hpp"

#include "Objects/MovingPlatform.hpp"

#include <SDL3/SDL.h>

MainLoop::MainLoop()
	: m_Window(sf::VideoMode(1920, 1080), "Game", 7U, sf::ContextSettings(0, 0, 7, 1, 1, 0, false)),
	  m_Renderer(m_Window)
{
	ActivateDarkMode(m_Window);

	m_Window.setVerticalSyncEnabled(true);
}

void fullscreenWindow(sf::RenderWindow& window, bool fullscreen) {
	window.close();

	sf::VideoMode mode;
	unsigned int style;

	if (fullscreen) {
		mode = sf::VideoMode::getDesktopMode();
		style = sf::Style::None;
	}
	else {
		mode = sf::VideoMode(1920, 1080);
		style = sf::Style::Default;
	}

	window.create(mode, "Game", style);
	window.setVerticalSyncEnabled(true);

	if (!fullscreen) ActivateDarkMode(window);
}

void MainLoop::Run() {
	m_Renderer.GetCamera().SetStartRect(FloatRect(-8, -9, 16, 9));
	m_Renderer.GetCamera().SetSize(vec2(16, 9));
	m_Renderer.GetCamera().StayInRect(true);
	m_Renderer.GetCamera().Update();

	m_Renderer.GetPlayerTextures().resize(17);
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::IdleBody  ].loadFromFile("../res/sprites/player/idle_body.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::IdleLegs  ].loadFromFile("../res/sprites/player/idle_legs.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::WalkBody  ].loadFromFile("../res/sprites/player/walk_body.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::WalkLegs  ].loadFromFile("../res/sprites/player/walk_legs.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::SprintBody].loadFromFile("../res/sprites/player/sprint_body.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::SprintLegs].loadFromFile("../res/sprites/player/sprint_legs.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::JumpBody  ].loadFromFile("../res/sprites/player/jump_body.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::JumpLegs  ].loadFromFile("../res/sprites/player/jump_legs.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::DashBody  ].loadFromFile("../res/sprites/player/dash_body.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::DashLegs  ].loadFromFile("../res/sprites/player/dash_legs.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::Sneak     ].loadFromFile("../res/sprites/player/sneak.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::Shoot     ].loadFromFile("../res/sprites/player/shoot.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::Shot      ].loadFromFile("../res/sprites/player/shot.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::ShotDown  ].loadFromFile("../res/sprites/player/shot_down.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::Climb     ].loadFromFile("../res/sprites/player/climb.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::DoubleJump].loadFromFile("../res/sprites/player/double_jump.png");
	m_Renderer.GetPlayerTextures()[Renderer::PlayerTextures::Overlay   ].loadFromFile("../res/sprites/overlay/overlay.png");

	m_Renderer.GetEnemyTextures().resize(10);
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedIdleBody ].loadFromFile("../res/sprites/enemies/gonraed/idle_body.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedIdleLegs ].loadFromFile("../res/sprites/enemies/gonraed/idle_legs.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedWalkBody ].loadFromFile("../res/sprites/enemies/gonraed/walk_body.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedWalkLegs ].loadFromFile("../res/sprites/enemies/gonraed/walk_legs.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GonraedPunchBody].loadFromFile("../res/sprites/enemies/gonraed/punch_body.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GondraeIdleBody ].loadFromFile("../res/sprites/enemies/gondrae/idle_body.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GondraeIdleLegs ].loadFromFile("../res/sprites/enemies/gondrae/idle_legs.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GondraeWalkBody ].loadFromFile("../res/sprites/enemies/gondrae/walk_body.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GondraeWalkLegs ].loadFromFile("../res/sprites/enemies/gondrae/walk_legs.png");
	m_Renderer.GetEnemyTextures()[Renderer::EnemyTextures::GondraePunchBody].loadFromFile("../res/sprites/enemies/gondrae/punch_body.png");

	m_Renderer.GetElevatorTextures().resize(3);
	m_Renderer.GetElevatorTextures()[Renderer::ElevatorTextures::PanelCloseUp       ].loadFromFile("../res/sprites/elevator/panel.png");
	m_Renderer.GetElevatorTextures()[Renderer::ElevatorTextures::PanelButtonLocked  ].loadFromFile("../res/sprites/elevator/button_blocked.png");
	m_Renderer.GetElevatorTextures()[Renderer::ElevatorTextures::PanelButtonSelected].loadFromFile("../res/sprites/elevator/button_selected.png");
	
	m_Renderer.GetParticleTextures().resize(3);
	m_Renderer.GetParticleTextures()[Renderer::ParticleTextures::Explosion].loadFromFile("../res/sprites/particles/explosion.png");
	m_Renderer.GetParticleTextures()[Renderer::ParticleTextures::WindHorizontal].loadFromFile("../res/sprites/particles/wind_horizontal.png");
	m_Renderer.GetParticleTextures()[Renderer::ParticleTextures::WindHorizontal].setRepeated(true);
	m_Renderer.GetParticleTextures()[Renderer::ParticleTextures::WindVertical].loadFromFile("../res/sprites/particles/wind_vertical.png");
	m_Renderer.GetParticleTextures()[Renderer::ParticleTextures::WindVertical].setRepeated(true);
	
	m_Renderer.GetOutroTextures().resize(3);
	m_Renderer.GetOutroTextures()[Renderer::OutroTextures::Building].loadFromFile("../res/sprites/outro/building.png");
	m_Renderer.GetOutroTextures()[Renderer::OutroTextures::Background].loadFromFile("../res/sprites/outro/background.png");
	m_Renderer.GetOutroTextures()[Renderer::OutroTextures::EndScreen].loadFromFile("../res/sprites/outro/end_screen.png");

	m_Renderer.GetMenuTextures().resize(9);
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::Main].loadFromFile("../res/sprites/menu/menu.png");
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonBack].loadFromFile("../res/sprites/menu/back.png");
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonContinue].loadFromFile("../res/sprites/menu/continue.png");
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonExit].loadFromFile("../res/sprites/menu/exit.png");
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonNewGame].loadFromFile("../res/sprites/menu/new_game.png");
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonOutline].loadFromFile("../res/sprites/menu/outline.png");
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonEasy].loadFromFile("../res/sprites/menu/easy.png");
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonMedium].loadFromFile("../res/sprites/menu/medium.png");
	m_Renderer.GetMenuTextures()[Renderer::MenuTextures::ButtonHard].loadFromFile("../res/sprites/menu/hard.png");

	SDL_Init(SDL_INIT_GAMEPAD);

	Controller controller;

	Input input;
	input.controller = &controller;
	input.keyboard = &m_Keyboard;
	input.mouse = &m_Mouse;

	sf::Clock cl;

	sf::Clock clock;

	State state(m_Renderer);
	state.Set(State::States::MainMenu);

	//state.Update(input);

	Time.UpdateDeltaTime();

	bool fullscreen = false;

	while (m_Window.isOpen()) {
		sf::Event event;

		m_Keyboard.EmptyInputList();
		m_Mouse.EmptyInputList();

		while (m_Window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				m_Window.close();

			m_Keyboard.Update(event);
			m_Mouse.Update(event);

			input.Update();
		}

		controller.EmptyInputList();

		controller.Update();

		if (input.keyboard->IsKeyPressed(sf::Keyboard::F11)) {
			fullscreen = !fullscreen;
			fullscreenWindow(m_Window, fullscreen);
		}

		if (m_Renderer.GetFadeStatus() != Renderer::Fade::Out) {
			state.Update(input);
		}

		g_ParticleSystem.Update();

		m_Renderer.GetRenderTarget().clear();

		state.Render();

		if (state.Get() == State::States::MainMenu || state.PauseMenuOpen()) {
			m_Window.setMouseCursorVisible(true);
		}
		else {
			m_Window.setMouseCursorVisible(false);
		}

		g_ParticleSystem.Render(m_Renderer);

		m_Renderer.RenderFade();

		m_Window.display();

		Time.UpdateDeltaTime();

		///////////////////////////////////////////////////////////////////
		if (cl.getElapsedTime().asMilliseconds() > 1000) {
			cl.restart();
			//printf("FPS: %f\n", 1.0f / Time.DeltaTime());
		}
	}
}