#include "GravityComponent.h"
#include <iostream>


void GravityComponent::Update(float dt)
{
	// on recuperer le owner et on lui applique de la vitesse de tombé
	if (owner) {
		// pour modifier un seul et unique transform
		auto& transform = owner->GetTransform();

		transform.velocity += gravity * dt;
		transform.pos += transform.velocity * dt;

	}
}