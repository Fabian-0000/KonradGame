#include "Lightning.hpp"

void Lightning::Update(Input& input) {
	m_Animation.Update();
	textureRect.x = textureRect.w * m_Animation.GetIndex();
}