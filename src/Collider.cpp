#include "Collider.hpp"

Collider::Collider(const Collider& collider) {
	tag = collider.tag;
	isTrigger = collider.isTrigger;
	parent = collider.parent;

	if (collider.type == ColliderType::BoxCollider) {
		type = ColliderType::BoxCollider;
		box = collider.box;
	}
	if (collider.type == ColliderType::TriangleCollider) {
		type = ColliderType::TriangleCollider;
		triangle.point[0] = collider.triangle.point[0];
		triangle.point[1] = collider.triangle.point[1];
		triangle.point[2] = collider.triangle.point[2];
	}
	if (collider.type == ColliderType::CircleCollider) {
		type = ColliderType::CircleCollider;
		circle.center = collider.circle.center;
		circle.radius = collider.circle.radius;
	}
}