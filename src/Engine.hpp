#pragma once

#include "Object.hpp"
#include "Input.hpp"

#include <memory>

class Engine {
public:
	Engine();

	~Engine();

	void AddObject(std::shared_ptr<Object> object);

	void Update(Input& input);

	std::vector<std::shared_ptr<Object>> m_Objects;
};