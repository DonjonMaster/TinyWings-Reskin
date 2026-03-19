#include "LevelGenerator.h"
#include "GameObject.h"
#include "HillComponent.h"
#include <Scene.h>
#include <algorithm>

void LevelGenerator::Init(GameObject* playerObj) {
    player = playerObj;

    hillAssets = { "Assets/Hchunk1.png" };
    cloudAssets = { "Assets/Cchunk1.png", "Assets/Cchunk2.png", "Assets/Cchunk3.png" };
    planetAssets = { "Assets/Pchunk.png" };

    // Initialisation du sol
    for (int i = 0; i < 5; ++i) SpawnNextHill();
    // Initialisation des premiers nuages et plan�tes
    for (int i = 0; i < 3; ++i) SpawnCloud();
}

void LevelGenerator::Update(float dt) {
    if (!owner || !player) return;
    float playerX = player->GetTransform().pos.x;

    // Sol : Succession infinie
    if (playerX + 5000.0f > nextHillX) {
        SpawnNextHill();
    }

    // Nuages : Succession sur deux �tages
    if (playerX + 5000.0f > nextCloudX) {
        SpawnCloud();
    }

    // Plan�tes : Succession infinie
    if (playerX + 6000.0f > nextPlanetX) {
        SpawnPlanet();
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

    nextHillX += (HILL_WIDTH * HILL_SCALE); // Succession exacte
}

void LevelGenerator::SpawnCloud() {
    Scene* scene = owner->GetScene();
    GameObject* cloud = nullptr;

    // Utilisation des variables CloudLayer1 et CloudLayer2
    float targetY = alternateCloudHeight ? Y_CLOUD_LAYER_2 : Y_CLOUD_LAYER_1;
    alternateCloudHeight = !alternateCloudHeight;

    // Petite variation al�atoire pour casser la ligne droite parfaite
    targetY += GetRandomFloat(-50.0f, 50.0f);

    if (activeClouds.size() < MAX_CLOUDS) {
        cloud = scene->CreateGameObject({ nextCloudX, targetY }, "CloudChunk");
        cloud->GetTransform().scale = { CLOUD_SCALE, CLOUD_SCALE };
        cloud->SetZOrder(-5);
        auto* comp = cloud->AddComponent<HillComponent>();
        comp->isOneWay = true;
        activeClouds.push_back(cloud);
    }
    else {
        cloud = activeClouds.front();
        activeClouds.erase(activeClouds.begin());
        activeClouds.push_back(cloud);
        cloud->GetTransform().pos = { nextCloudX, targetY };
    }

    auto* comp = cloud->GetComponent<HillComponent>();
    if (comp) comp->InitFromImage(GetRandomAsset(cloudAssets), 15);

    nextCloudX += (CLOUD_WIDTH * CLOUD_SCALE); // Succession exacte
}

void LevelGenerator::SpawnPlanet() {
    Scene* scene = owner->GetScene();
    GameObject* planet = nullptr;

    // Utilisation de la variable Y_PLANET_BASE
    float currentY = Y_PLANET_BASE + GetRandomFloat(-100.f, 100.f);

    if (activePlanets.size() < MAX_PLANETS) {
        planet = scene->CreateGameObject({ nextPlanetX, currentY }, "PlanetChunk");
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
        planet->GetTransform().pos = { nextPlanetX, currentY };
    }

    auto* comp = planet->GetComponent<HillComponent>();
    if (comp) comp->InitFromImage(GetRandomAsset(planetAssets), 20);

    nextPlanetX += (PLANET_WIDTH * PLANET_SCALE); // Succession exacte
}

std::string LevelGenerator::GetRandomAsset(const std::vector<std::string>& assets) {
    if (assets.empty()) return "";
    return assets[rand() % assets.size()];
}

float LevelGenerator::GetRandomFloat(float min, float max) {
    if (min > max) std::swap(min, max);
    return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}