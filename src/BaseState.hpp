#pragma once

class Input;
class State;
class Renderer;

class BaseState {
public:
	virtual void Update(Input& input, State& state) {}

	virtual void Render(Renderer& renderer) {}
};