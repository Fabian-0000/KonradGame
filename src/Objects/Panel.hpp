#pragma once
#include "../Object.hpp"

class Panel : public Object {
public:
	Panel();

	bool IsOpened();

	void Close();

	bool IsSelectionUpdated();

	void SetSelectedFloor(uint8_t panel);

	uint8_t GetSelectedFloor();

	void Update(Input& input) override;

	void Render(Renderer& renderer) override;

	void OnCollision(const Collider& self, const Collider& other) override;

private:
	bool m_Opened = false;

	bool m_ButtonLocked[4] = { true, true, true, true };

	int8_t m_SelectedButton = 0;

	bool m_SelectionUpdate = false;

	bool m_SelectionConfirmed = false;
};