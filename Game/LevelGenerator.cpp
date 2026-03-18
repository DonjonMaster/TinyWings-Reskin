#include "LevelGenerator.h"
#include "GameObject.h"
#include "HillComponent.h"
#include <Scene.h>
#include <algorithm>

void LevelGenerator::Init(GameObject* playerObj) {
    player = playerObj;

    // Assets
    hillAssets = { "Assets/Hchunk1.png" };
    cloudAssets = { "Assets/Cchunk1.png", "Assets/Cchunk2.png","Assets/Cchunk3.png", }; // Doit être une image avec un profil de collision
    planetAssets = { "Assets/Pchunk.png" };

    // Initialisation du sol
    for (int i = 0; i < 5; ++i) {
        SpawnNextHill();
    }
}

void LevelGenerator::Update(float dt) {
    if (!owner || !player) return;
    float playerX = player->GetTransform().pos.x;

    // Sol
    if (playerX + 4000.0f > nextHillX) {
        SpawnNextHill();
    }

    // Nuages (Chunks physiques)
    if (playerX + 3000.0f > lastCloudX) {
        SpawnCloud();
        lastCloudX += GetRandomFloat(cloudMinGapX, cloudMaxGapX);
    }

    // Planètes (Chunks physiques)
    if (playerX + 4000.0f > lastPlanetX) {
        SpawnPlanet();
        lastPlanetX += GetRandomFloat(planetMinGapX, planetMaxGapX);
    }
}

void LevelGenerator::SpawnNextHill() {
    Scene* scene = owner->GetScene();
    GameObject* hill = nullptr;

    if (activeHills.size() < MAX_HILLS) {
        hill = scene->CreateGameObject({ nextHillX, -20.0f }, "HillChunk");
        hill->GetTransform().scale = { HILL_SCALE, HILL_SCALE };
        hill->SetZOrder(0);
        auto* comp = hill->AddComponent<HillComponent>();
        comp->isOneWay = false;
        activeHills.push_back(hill);
    }
    else {
        hill = activeHills.front();
        activeHills.erase(activeHills.begin());
        activeHills.push_back(hill);
        hill->GetTransform().pos = { nextHillX, -20.0f };
    }

    auto* comp = hill->GetComponent<HillComponent>();
    if (comp) comp->InitFromImage(GetRandomAsset(hillAssets), 10);

    nextHillX += (HILL_WIDTH * HILL_SCALE);
}

void LevelGenerator::SpawnCloud() {
    Scene* scene = owner->GetScene();
    GameObject* cloud = nullptr;
    float randomY = GetRandomFloat(cloudMinY, cloudMaxY);

    if (activeClouds.size() < MAX_CLOUDS) {
        cloud = scene->CreateGameObject({ lastCloudX, randomY }, "CloudChunk");
        cloud->GetTransform().scale = { CLOUD_SCALE, CLOUD_SCALE };
        cloud->SetZOrder(-5);
        auto* comp = cloud->AddComponent<HillComponent>();
        comp->isOneWay = true; // Permet de passer à travers par le bas
        activeClouds.push_back(cloud);
    }
    else {
        cloud = activeClouds.front();
        activeClouds.erase(activeClouds.begin());
        activeClouds.push_back(cloud);
        cloud->GetTransform().pos = { lastCloudX, randomY };
    }

    auto* comp = cloud->GetComponent<HillComponent>();
    if (comp) comp->InitFromImage(GetRandomAsset(cloudAssets), 10);
}

void LevelGenerator::SpawnPlanet() {
    Scene* scene = owner->GetScene();
    GameObject* planet = nullptr;
    float randomY = GetRandomFloat(planetMinY, planetMaxY);

    if (activePlanets.size() < MAX_PLANETS) {
        planet = scene->CreateGameObject({ lastPlanetX, randomY }, "PlanetChunk");
        planet->GetTransform().scale = { PLANET_SCALE, PLANET_SCALE };
        planet->SetZOrder(-10);
        auto* comp = planet->AddComponent<HillComponent>();
        comp->isOneWay = true;
        activePlanets.push_back(planet);
    }
    else {
        planet = activePlanets.front();
        activePlanets.erase(activePlanets.begin());
        activePlanets.push_back(planet);
        planet->GetTransform().pos = { lastPlanetX, randomY };
    }

    auto* comp = planet->GetComponent<HillComponent>();
    if (comp) comp->InitFromImage(GetRandomAsset(planetAssets), 10);
}

// Utilitaires
std::string LevelGenerator::GetRandomAsset(const std::vector<std::string>& assets) {
    if (assets.empty()) return "";
    return assets[rand() % assets.size()];
}

float LevelGenerator::GetRandomFloat(float min, float max) {
    if (min > max) std::swap(min, max);
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}