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

    // --- Paramètres des Collines (Sol) ---
    float nextHillX = 0.0f;
    const float HILL_WIDTH = 1212.0f;
    const float HILL_SCALE = 8.0f;

    // --- Nuages (Chunks en hauteur) ---
    float lastCloudX = 2000.0f;
    const float CLOUD_WIDTH = 1212.0f; // Ajuste selon la taille réelle de l'image nuage
    const float CLOUD_SCALE = 2.0f;
    const float cloudMinY = -1200.0f;
    const float cloudMaxY = -600.0f;
    const float cloudMinGapX = 1500.0f;
    const float cloudMaxGapX = 3000.0f;

    // --- Planètes (Chunks très hauts) ---
    float lastPlanetX = 5000.0f;
    const float PLANET_WIDTH = 1212.0f; // Ajuste selon la taille réelle de l'image planète
    const float PLANET_SCALE = 4.0f;
    const float planetMinY = -3500.0f;
    const float planetMaxY = -2000.0f;
    const float planetMinGapX = 4000.0f;
    const float planetMaxGapX = 8000.0f;

    // --- Banque d'Assets ---
    std::vector<std::string> hillAssets;
    std::vector<std::string> cloudAssets;
    std::vector<std::string> planetAssets;

    // --- Object Pooling ---
    std::vector<GameObject*> activeHills;
    std::vector<GameObject*> activeClouds;
    std::vector<GameObject*> activePlanets;

    const size_t MAX_HILLS = 8;
    const size_t MAX_CLOUDS = 6;
    const size_t MAX_PLANETS = 4;
};