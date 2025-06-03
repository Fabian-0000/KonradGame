#include "Object.hpp"
#include "Collision.hpp"
#include "Player.hpp"

struct Rectangle {
	vec2 points[4];
};

Rectangle RectFromPoints(FloatRect rect) {
	Rectangle res;

	res.points[0] = rect.xy;
	res.points[1] = rect.xy; res.points[1].x += rect.w;
	res.points[2] = rect.xy + rect.wh;
	res.points[3] = rect.xy; res.points[3].y += rect.h;

	return res;
}

void Object::Render(Renderer& renderer) {
	if (!hasTexture && color.a == 0.0f) return;

	sf::RectangleShape object;

	if (hasTexture) {
		object.setTexture(&renderer.GetTextures()[textureIndex]);
		object.setTextureRect(ToSFMLIntRect(textureRect));
	}
	else {
		object.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	}
	object.setPosition(ToSFMLVec2(transform.position));
	object.setSize(ToSFMLVec2(transform.size));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object);
}

FloatRect Object::GetBoxColliderRect(const Collider& collider) {
	if (collider.type == ColliderType::BoxCollider)
		return FloatRect(transform.position + transform.size * collider.box.xy, transform.size * collider.box.wh);
	return FloatRect();
}

Circle Object::GetCircleColliderCircle(const Collider& collider) {
	if (collider.type == ColliderType::CircleCollider) {
		return { transform.position + transform.size * collider.circle.center, transform.size.w * collider.circle.radius };
	}
	return {};
}

FloatRect Object::GetColliderRect(FloatRect rect) {
	return FloatRect((rect.xy - transform.position) / transform.size, rect.wh / transform.size);
}

void Object::ColliderUpdate(std::vector<std::shared_ptr<Object>>& objects) {
	Player* player = nullptr;
	for (std::shared_ptr<Object> obj : objects) {
		for (Collider& col : obj->colliders) {
			if (col.tag == Collider::Tags::Player) {
				player = (Player*)obj.get();
			}
		}
	}
	if (!player) return;

	for (Collider& col : colliders) {
		if (col.tag == Collider::Tags::Ground) {
			FloatRect rect = col.parent->GetBoxColliderRect(col);

			if (player->transform.position.y + player->transform.size.h - rect.y > 0.1f) {
				col.enabled = false;
			}
			else {
				col.enabled = true;
			}
		}

		if (col.tag == Collider::Climb) {
			col.isTrigger = true;
		}
	}
}

void Object::Collide(Object& other) {
	if (other.id == Object::Id::Default && id == Object::Id::Default) return;

	for (Collider& col1 : other.colliders) {
		for (Collider& col2 : colliders) {
			if (!col1.enabled || !col2.enabled) continue;
			if (!other.exists || !exists) return;
			bool collision = false;

			float response1 = 0.0f, response2 = 1.0f;
			vec2  movement1 = 0.0f, movement2 = 0.0f;

			if (col1.isTrigger || col2.isTrigger) {
				response1 = 0.0f;
				response2 = 0.0f;
			}

			else if (type == ObjectType::Dynamic && other.type == ObjectType::Dynamic) {
				response1 = 0.0f;
				response2 = 0.0f;
			}

			else if (type == ObjectType::HalfDynamic && other.type == ObjectType::HalfDynamic) {
				response1 = 0.5f;
				response2 = 0.5f;
			}

			if (type == ObjectType::Static) {
				response1 = 0.0f;
				response2 = 0.0f;
			}

			switch (col1.type) {
			case ColliderType::BoxCollider:

				switch (col2.type) {
				case ColliderType::BoxCollider:
				{
					collision = RectCollide(col2.box.xy, col2.box.wh, response2, &movement2, &transform.position, &transform.size, col1.box.xy, col1.box.wh, response1, &movement1, &other.transform.position, &other.transform.size);
					goto end;
				}
				case ColliderType::TriangleCollider:
				{
					Rectangle r = RectFromPoints(col1.box);
					collision = SAT::Collide(col2.triangle.point, 3, response2, &movement2, &transform.position, &transform.size, r.points, 4, response1, &movement1, &other.transform.position, &other.transform.size);
					goto end;
				}
				case ColliderType::CircleCollider:
				{
					Rectangle r = RectFromPoints(col1.box);
					collision = SAT::CircleCollide(r.points, 4, response1, &movement1, &other.transform.position, &other.transform.size, col2.circle.center, col2.circle.radius, response2, &movement2, &transform.position, &transform.size);
					goto end;
				}
				}

			case ColliderType::TriangleCollider:

				switch (col2.type) {
				case ColliderType::BoxCollider:
				{
					Rectangle r = RectFromPoints(col2.box);
					collision = SAT::Collide(col1.triangle.point, 3, response1, &movement1, &other.transform.position, &other.transform.size, r.points, 4, response2, &movement2, &transform.position, &transform.size);
					goto end;
				}
				case ColliderType::TriangleCollider:
				{
					collision = SAT::Collide(col2.triangle.point, 3, response2, &movement2, &transform.position, &transform.size, col1.triangle.point, 3, response1, &movement1, &other.transform.position, &other.transform.size);
					goto end;
				}
				case ColliderType::CircleCollider:
				{
					collision = SAT::CircleCollide(col1.triangle.point, 3, response1, &movement1, &other.transform.position, &other.transform.size, col2.circle.center, col2.circle.radius, response2, &movement2, &transform.position, &transform.size);
					goto end;
				}
				}


			case ColliderType::CircleCollider:

				switch (col2.type) {
				case ColliderType::BoxCollider:
				{
					Rectangle r = RectFromPoints(col2.box);
					collision = SAT::CircleCollide(r.points, 4, response2, &movement2, &transform.position, &transform.size, col1.circle.center, col1.circle.radius, response1, &movement1, &other.transform.position, &other.transform.size);
					goto end;
				}
				case ColliderType::TriangleCollider:
				{
					collision = SAT::CircleCollide(col2.triangle.point, 3, response2, &movement2, &transform.position, &transform.size, col1.circle.center, col1.circle.radius, response1, &movement1, &other.transform.position, &other.transform.size);
					goto end;
				}
				case ColliderType::CircleCollider:
				{
					collision = CircCollide(col2.circle.center, col2.circle.radius, response2, &movement2, &transform.position, &transform.size, col1.circle.center, col1.circle.radius, response1, &movement1, &other.transform.position, &other.transform.size);
					goto end;
				}
				}
			}
		end:

			transform.position += movement2;
			other.transform.position += movement1;

			if (collision) {
				OnCollision(col2, col1);
			}
		}
	}
}

void Object::Delete() {
	exists = false;
}