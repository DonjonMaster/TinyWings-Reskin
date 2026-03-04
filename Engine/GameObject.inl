template<typename ComponentType, typename... Param>
ComponentType* GameObject::AddComponent(Param... parameters)
{

	ComponentType* component = new ComponentType(parameters...);
	components.push_back(component);
	component->owner = this;
	component->Create();
	return component;

}

template<typename ComponentType>
ComponentType* GameObject::GetComponent()
{

	for (Component* component : components)
		if (ComponentType* componentType = dynamic_cast<ComponentType*>(component))
			return componentType;

	return nullptr;

}