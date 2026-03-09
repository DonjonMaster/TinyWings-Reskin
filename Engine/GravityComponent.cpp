#include "GravityComponent.h"


void GravityComponent::Update(float dt)
{
	if (owner) {
		auto& transform = owner->GetTransform();

		transform.velocity += gravity * dt;
		transform.pos += transform.velocity * dt;

	}
}