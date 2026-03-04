template<typename SceneType>
void SceneModule::RegisterScene(std::string name)
{

	// Pour éviter la duplication d'un constructeur de scène déjà enregistrée
	if (sceneConstructorsMap[name])
		return;

	sceneConstructorsMap[name] = []() {
		return new SceneType();
		};

}