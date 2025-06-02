#pragma once

#include "Level.hpp"
#include <memory>

class Elevator : public Level {
public:
	void Update(Input& input, State& state) override;

	void Render(Renderer& renderer) override;

	void LoadFromFile(const char* filename, Renderer& resources) override;

	void ChangeFloor();

	void SetSelectedFloor(uint8_t floor);

private:
	std::shared_ptr<Panel> m_Panel;

	std::shared_ptr<ElevatorDoor> m_ElevatorDoor;

	uint8_t m_SelectedFloor = 0;

	bool m_ChangingFloor = false;

	bool m_PanelAutoClosed = false;

	sf::Clock m_FloorChangingClock;

	sf::Clock m_ShakeClock;

	bool m_New = true;
};