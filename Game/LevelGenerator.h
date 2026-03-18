#pragma once
#include "Component.h"
#include <string>
#include <vector>

class LevelGenerator : public Component {
public:
    void Init(GameObject* playerObj);
    void Update(float dt) override;

private:
    void SpawnNextHill();
    void SpawnCloud();
    void SpawnPlanet();

    std::string GetRandomAsset(const std::vector<std::string>& assets);
    float GetRandomFloat(float min, float max);

    GameObject* player = nullptr;

    // --- PARAMÈTRES ÉTAGE 1 : LE SOL ---
    float nextHillX = 0.0f;
    const float Y_GROUND = -20.0f;
    const float HILL_WIDTH = 1212.0f;
    const float HILL_SCALE = 8.0f;

    // --- PARAMÈTRES ÉTAGE 2 : LES NUAGES (2 COUCHES) ---
    float nextCloudX = 0.0f;
    const float Y_CLOUD_LAYER_1 = -1000.0f;   // Altitude du premier étage de nuages
    const float Y_CLOUD_LAYER_2 = -1500.0f;  // Altitude du deuxième étage de nuages
    const float CLOUD_WIDTH = 1500.0f;
    const float CLOUD_SCALE = 3.0f;
    bool alternateCloudHeight = false;       // Alterne entre Layer 1 et Layer 2

    // --- PARAMÈTRES ÉTAGE 3 : LES PLANÈTES ---
    float nextPlanetX = 0.0f;
    const float Y_PLANET_BASE = -2500.0f;
    const float PLANET_WIDTH = 1212.0f;
    const float PLANET_SCALE = 4.0f;

    // --- BANQUES D'ASSETS ---
    std::vector<std::string> hillAssets;
    std::vector<std::string> cloudAssets;
    std::vector<std::string> planetAssets;

    // --- POOLING (RECYCLAGE D'OBJETS) ---
    std::vector<GameObject*> activeHills;
    std::vector<GameObject*> activeClouds;
    std::vector<GameObject*> activePlanets;

    // Limites d'objets en mémoire
    const size_t MAX_HILLS = 8;
    const size_t MAX_CLOUDS = 14;
    const size_t MAX_PLANETS = 6;
};