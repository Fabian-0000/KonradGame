#include "Darkness.hpp"

#include "../Player.hpp"

static bool g_TextureCreated = false;
static sf::Texture g_EmptyTexture;

const static char* vertex_shader = 
"#version 120\n\
\n\
void main()\n\
{\n\
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n\
}";

const static char* fragment_shader =
"#version 120\n\
\n\
#define MAX_LIGHTS 10\n\
\n\
uniform vec2 u_lightPos[MAX_LIGHTS];   // Array of light positions\n\
uniform float u_strength[MAX_LIGHTS];   // Array of strengths\n\
uniform float u_brightness[MAX_LIGHTS];   // Array of brightness (0..1)\n\
uniform int u_lightCount;              // Number of active lights\n\
\n\
uniform vec2 u_objPos;\n\
uniform vec2 u_objSize;\n\
\n\
float smoothFalloff(float distance, float strength) {\n\
	return smoothstep(strength, strength / 2.0f, distance); // Adjust '20.0' for smoothness\n\
}\n\
\n\
void main()\n\
{\n\
	float transparency = 1.0;\n\
\n\
	// Iterate over each light source\n\
	for (int i = 0; i < u_lightCount; ++i)\n\
	{\n\
		// Calculate the distance between the fragment and the light\n\
		float dist = distance(u_objPos + gl_TexCoord[0].xy * u_objSize, u_lightPos[i]);\n\
\n\
		// Apply smooth falloff using smoothstep for a softer edge\n\
		float alpha = min(1.0, smoothFalloff(dist, u_strength[i])) * u_brightness[i];\n\
\n\
		// Take the max transparency from all lights\n\
		transparency *= 1 - alpha;\n\
	}\n\
\n\
	// Output the final color with alpha transparency\n\
	gl_FragColor = vec4(0.0, 0.0, 0.0, transparency);\n\
}";

Darkness::Darkness() {
	if (!g_TextureCreated) { 
		g_EmptyTexture.create(200, 200); 
		g_TextureCreated = true;
	}

	m_Shader.loadFromMemory(vertex_shader, fragment_shader);
}

void Darkness::Update(Input& input) {
	m_ShotLastFrame = m_Shot;
	m_LightSourcesIndex = 0;
	m_Shot = false;
	m_FullLight = false;
}

void Darkness::OnCollision(const Collider& self, const Collider& other) {
	if (other.tag == Collider::Tags::Light) {
		if (other.type == ColliderType::CircleCollider) {
			if (m_LightSourcesIndex < MAX_LIGHTS) {
				LightSource source;
				source.pos = other.parent->GetCircleColliderCircle(other).center;
				source.radius = other.parent->GetCircleColliderCircle(other).radius;
				source.brightness = 1.0f;
				m_LightSources[m_LightSourcesIndex++] = source;
			}
		}
	}
	else if (other.tag == Collider::Tags::PlayerShot) {
		if (m_LightSourcesIndex < MAX_LIGHTS) {
			m_Shot = true;

			if (!m_ShotLastFrame) {
				m_ShotClock.restart();
			}

			FloatRect r = other.parent->GetBoxColliderRect(other);

			LightSource source;
			source.pos = r.position + r.size / 2.0f;
			source.radius = 4.5f;
			source.brightness = 1.0f;

			uint32_t time = m_ShotClock.getElapsedTime().asMilliseconds();

			if (time > 650) {
				source.brightness = 0.0f;
				m_Shot = false;
			}
			else if (time > 500) {
				source.brightness = (650 - time) / 200.0f;
			}

			m_LightSources[m_LightSourcesIndex++] = source;
		}
	}
	else if (other.tag == Collider::Tags::Player) {
		if (m_LightSourcesIndex < MAX_LIGHTS) {
			FloatRect r = other.parent->transform.rect;

			LightSource source;
			source.pos = r.position + r.size / 2.0f;
			source.radius = 2.0f;
			source.brightness = 0.2f;

			m_LightSources[m_LightSourcesIndex++] = source;
		}
	}
	else if (other.tag == Collider::Tags::PowerOn) {
		m_FullLight = true;
	}
}

void Darkness::Render(Renderer& renderer) {
	if (m_FullLight) return;

	static sf::Glsl::Vec2 lightPosArray[MAX_LIGHTS];
	static float strengthArray[MAX_LIGHTS];
	static float brightnessArray[MAX_LIGHTS];

	for (std::size_t i = 0; i < MAX_LIGHTS; ++i)
	{
		if (i < m_LightSourcesIndex)
		{
			lightPosArray[i] = sf::Glsl::Vec2(m_LightSources[i].pos.x, m_LightSources[i].pos.y);
			strengthArray[i] = m_LightSources[i].radius;
			brightnessArray[i] = m_LightSources[i].brightness;
		}
		else
		{
			lightPosArray[i] = sf::Vector2f(0, 0);
			strengthArray[i] = 0.0f;
			brightnessArray[i] = 0.0f;
		}
	}

	m_Shader.setUniformArray("u_lightPos", lightPosArray, MAX_LIGHTS);
	m_Shader.setUniformArray("u_strength", strengthArray, MAX_LIGHTS);
	m_Shader.setUniformArray("u_brightness", brightnessArray, MAX_LIGHTS);
	m_Shader.setUniform("u_lightCount", static_cast<int>(m_LightSourcesIndex));

	m_Shader.setUniform("u_objPos", sf::Glsl::Vec2(transform.position.x, transform.position.y));
	m_Shader.setUniform("u_objSize", sf::Glsl::Vec2(transform.size.x, transform.size.y));

	sf::RectangleShape object;

	object.setTexture(&g_EmptyTexture);
	object.setPosition(ToSFMLVec2(transform.position));
	object.setSize(ToSFMLVec2(transform.size));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object, &m_Shader);
}