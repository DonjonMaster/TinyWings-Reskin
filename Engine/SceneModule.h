#pragma once
#include <functional>
#include "Module.h"
#include "Scene.h"
#include "WindowModule.h"

class SceneModule : public Module
{
	using SceneConstructor = std::function<Scene* ()>;

public:
	void Create() override;
	void Update(float dt) override;
	void Render() override;

	// Applique une scène (et supprime ceux présent) dans la scene stack
	void SetScene(std::string name);

	// Push une scène dans la scene stack
	void PushScene(std::string name);

	// Enregistre une scène avec un nom
	template<typename SceneType>
	void RegisterScene(std::string name);

private:
	WindowModule* windowModule;
	std::unordered_map<std::string, SceneConstructor> sceneConstructorsMap;
	std::vector<Scene*> sceneStack;
};

#include "SceneModule.inl"