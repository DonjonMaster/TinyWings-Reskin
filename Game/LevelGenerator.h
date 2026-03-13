//#pragma once
//#include "Component.h"
//#include <string>
//#include <vector>
//
//class LevelGenerator : public Component {
//public:
//    void Init(GameObject* playerObj);
//    void Update(float dt) override;
//
//private:
//    void SpawnNextHill();
//    void SpawnCloud();
//    void SpawnPlanet();
//
//    GameObject* player = nullptr;
//
//    // --- Paramètres des Collines ---
//    float nextHillX = 0.0f;
//    const float HILL_WIDTH = 1212.0f;
//    const float HILL_SCALE = 8.0f;
//
//    // --- Suivi des distances ---
//    float lastCloudX = 0.0f;
//    float lastPlanetX = 0.0f;
//
//    // --- BANQUE D'ASSETS ---
//    std::vector<std::string> hillAssets;
//    std::vector<std::string> cloudAssets;
//    std::vector<std::string> planetAssets;
//
//    // Fonction utilitaire pour choisir un asset au hasard
//    std::string GetRandomAsset(const std::vector<std::string>& assets);
//};