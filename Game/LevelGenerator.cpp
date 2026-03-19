#include "LevelGenerator.h"
#include "GameObject.h"
#include "HillComponent.h"
#include <Scene.h>
#include <algorithm>

void LevelGenerator::Init(GameObject* playerObj) {
    player = playerObj;

    // Initialisation des listes d'assets
    hillAssets = { "Assets/Hchunk1.png", "Assets/Hchunk2.png", "Assets/Hchunk3.png" };
    cloudAssets = { "Assets/Cchunk1.png", "Assets/Cchunk2.png", "Assets/Cchunk3.png" };
    planetAssets = { "Assets/Pchunk1.png", "Assets/Pchunk2.png", "Assets/Pchunk.png" };

    // Initialisation du sol (5 chunks de départ)
    for (int i = 0; i < 5; ++i) SpawnNextHill();

    // Initialisation des nuages (3 colonnes de 2 étages = 6 nuages au total)
    for (int i = 0; i < 3; ++i) {
        SpawnCloudAt(nextCloudX, Y_CLOUD_LAYER_1);
        SpawnCloudAt(nextCloudX, Y_CLOUD_LAYER_2);
        nextCloudX += (CLOUD_WIDTH * CLOUD_SCALE);
    }

    // Initialisation planètes
    for (int i = 0; i < 2; ++i) SpawnPlanet();
}

void LevelGenerator::Update(float dt) {
    if (!owner || !player) return;
    float playerX = player->GetTransform().pos.x;

    // Sol : Succession infinie
    if (playerX + 5000.0f > nextHillX) {
        SpawnNextHill();
    }

    // Nuages : On génère les deux couches en même temps sur l'axe X
    if (playerX + 5000.0f > nextCloudX) {
        SpawnCloudAt(nextCloudX, Y_CLOUD_LAYER_1);
        SpawnCloudAt(nextCloudX, Y_CLOUD_LAYER_2);
        nextCloudX += (CLOUD_WIDTH * CLOUD_SCALE);
    }

    // Planètes : Succession infinie
    if (playerX + 6000.0f > nextPlanetX) {
        SpawnPlanet();
    }
}

void LevelGenerator::SpawnNextHill() {
    Scene* scene = owner->GetScene();
    GameObject* hill = nullptr;

    if (activeHills.size() < MAX_HILLS) {
        hill = scene->CreateGameObject({ nextHillX, Y_GROUND }, "HillChunk");
        hill->GetTransform().scale = { HILL_SCALE, HILL_SCALE };
        hill->SetZOrder(10);
        auto* comp = hill->AddComponent<HillComponent>();
        comp->isOneWay = false;
        activeHills.push_back(hill);
    }
    else {
        hill = activeHills.front();
        activeHills.erase(activeHills.begin());
        activeHills.push_back(hill);
        hill->GetTransform().pos = { nextHillX, Y_GROUND };
    }

    auto* comp = hill->GetComponent<HillComponent>();
    if (comp) comp->InitFromImage(GetRandomAsset(hillAssets), 10);

    nextHillX += (HILL_WIDTH * HILL_SCALE);
}

void LevelGenerator::SpawnCloudAt(float x, float targetY) {
    Scene* scene = owner->GetScene();
    GameObject* cloud = nullptr;

    float finalY = targetY + GetRandomFloat(-50.0f, 50.0f);

    if (activeClouds.size() < MAX_CLOUDS) {
        cloud = scene->CreateGameObject({ x, finalY }, "CloudChunk");
        cloud->GetTransform().scale = { CLOUD_SCALE, CLOUD_SCALE };
        cloud->SetZOrder(-15);
        auto* comp = cloud->AddComponent<HillComponent>();
        comp->isOneWay = true;
        activeClouds.push_back(cloud);
    }
    else {
        cloud = activeClouds.front();
        activeClouds.erase(activeClouds.begin());
        activeClouds.push_back(cloud);
        cloud->GetTransform().pos = { x, finalY };
    }

    auto* comp = cloud->GetComponent<HillComponent>();
    if (comp) comp->InitFromImage(GetRandomAsset(cloudAssets), 15);
}

void LevelGenerator::SpawnPlanet() {
    Scene* scene = owner->GetScene();
    GameObject* planet = nullptr;

    float currentY = Y_PLANET_BASE + GetRandomFloat(-100.f, 100.f);

    if (activePlanets.size() < MAX_PLANETS) {
        planet = scene->CreateGameObject({ nextPlanetX, currentY }, "PlanetChunk");
        planet->GetTransform().scale = { PLANET_SCALE, PLANET_SCALE };
        planet->SetZOrder(-20);
        auto* comp = planet->AddComponent<HillComponent>();
        comp->isOneWay = true;
        activePlanets.push_back(planet);
    }
    else {
        planet = activePlanets.front();
        activePlanets.erase(activePlanets.begin());
        activePlanets.push_back(planet);
        planet->GetTransform().pos = { nextPlanetX, currentY };
    }

    auto* comp = planet->GetComponent<HillComponent>();
    if (comp) comp->InitFromImage(GetRandomAsset(planetAssets), 20);

    nextPlanetX += (PLANET_WIDTH * PLANET_SCALE);
}

std::string LevelGenerator::GetRandomAsset(const std::vector<std::string>& assets) {
    if (assets.empty()) return "";
    return assets[rand() % assets.size()];
}

float LevelGenerator::GetRandomFloat(float min, float max) {
    if (min > max) std::swap(min, max);
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}