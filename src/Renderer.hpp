#pragma once

#include <SFML/Graphics.hpp>
#include "Rect.hpp"
#include "View.hpp"
#include "Camera.hpp"

inline sf::Vector2f ToSFMLVec2(vec2 v) {
	return sf::Vector2f(v.x, v.y);
}

inline sf::FloatRect ToSFMLFloatRect(FloatRect v) {
	return sf::FloatRect(v.x, v.y, v.w, v.h);
}

inline sf::IntRect ToSFMLIntRect(IntRect v) {
	return sf::IntRect(v.x, v.y, v.w, v.h);
}

class Renderer {
public:
	Renderer(sf::RenderTarget& rt);

	enum class Fade {
		In,
		InComplete,
		Out,
		OutComplete,
	};

	void FadeOut(sf::Time time);

	void FadeIn(sf::Time time);

	void RenderFade();

	Fade GetFadeStatus();

	sf::RenderTarget& GetRenderTarget();

	sf::RenderTarget& GetOverlayRenderTarget();

	std::vector<sf::Texture>& GetTextures();

	enum PlayerTextures {
		IdleBody, IdleLegs, WalkBody, WalkLegs, SprintBody, SprintLegs, JumpBody, JumpLegs, DashBody, DashLegs, Sneak, Shoot, Shot, ShotDown, Climb, DoubleJump, Overlay
	};

	std::vector<sf::Texture>& GetPlayerTextures();

	enum EnemyTextures {
		GonraedIdleBody, GonraedIdleLegs, GonraedWalkBody, GonraedWalkLegs, GonraedPunchBody, 
		GondraeIdleBody, GondraeIdleLegs, GondraeWalkBody, GondraeWalkLegs, GondraePunchBody
	};

	std::vector<sf::Texture>& GetEnemyTextures();

	enum ElevatorTextures {
		PanelCloseUp, PanelButtonLocked, PanelButtonSelected
	};

	std::vector<sf::Texture>& GetElevatorTextures();

	enum ParticleTextures {
		Explosion,
		WindHorizontal,
		WindVertical
	};

	std::vector<sf::Texture>& GetParticleTextures();

	enum OutroTextures {
		Building,
		Background,
		EndScreen
	};

	std::vector<sf::Texture>& GetOutroTextures();

	enum MenuTextures {
		Main,
		ButtonBack,
		ButtonContinue,
		ButtonExit,
		ButtonNewGame,
		ButtonEasy,
		ButtonMedium,
		ButtonHard,
		ButtonOutline
	};

	std::vector<sf::Texture>& GetMenuTextures();
	
	View& GetView();

	Camera& GetCamera();

private:
	sf::RenderTarget& m_RenderTarget;
	
	std::vector<sf::Texture> m_Textures;

	std::vector<sf::Texture> m_PlayerTextures;

	std::vector<sf::Texture> m_EnemyTextures;

	std::vector<sf::Texture> m_ElevatorTextures;

	std::vector<sf::Texture> m_ParticleTextures;

	std::vector<sf::Texture> m_OutroTextures;

	std::vector<sf::Texture> m_MenuTextures;

	sf::Clock m_FadeTimer;

	sf::Time m_FadeTime;

	Fade m_FadeStatus = Fade::InComplete;

	Camera m_Camera;

	sf::View m_OverlayView;
};