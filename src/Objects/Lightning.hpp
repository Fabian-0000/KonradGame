#pragma once
#include "../Object.hpp"
#include "../Animation.hpp"

class Lightning : public Object {
public:
	void Update(Input& input) override;

private:
	Animation m_Animation = Animation(125, 4);
};