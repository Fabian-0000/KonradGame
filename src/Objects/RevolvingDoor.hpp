#pragma once

#include "../Object.hpp"
#include "../Animation.hpp"

class RevolvingDoor : public Object {
public:
	void Update(Input& input);

private:
	Animation m_Animation = Animation(200, 4);
};