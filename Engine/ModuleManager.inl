template<typename ModuleType>
ModuleType* ModuleManager::CreateModule()
{

	ModuleType* module = new ModuleType();
	modules.push_back(module);
	return module;

}

template<typename ModuleType>
ModuleType* ModuleManager::GetModule()
{

	for (Module* module : modules)
		if (ModuleType* moduleType = dynamic_cast<ModuleType*>(module))
			return moduleType;

	return nullptr;

}