#include "LevelGenerator.h"
#include "GameObject.h"
#include "HillComponent.h"
#include <Scene.h>
#include <random>
#include <algorithm> // Pour std::swap

void LevelGenerator::Init(GameObject* playerObj) {
    player = playerObj;

    // N'hésite pas à ajouter plus d'images dans ces listes !
    hillAssets = { "Assets/chunk1.png" };
    cloudAssets = { "Assets/cloud1.png" }; // Pense à mettre le chemin de ton nuage
    planetAssets = { "Assets/planet1.png", "Assets/planet2.png" };

    // On génère les 5 premiers morceaux de sol au démarrage
    for (int i = 0; i < 5; ++i) {
        SpawnNextHill();
    }
}

std::string LevelGenerator::GetRandomAsset(const std::vector<std::string>& assets) {
    if (assets.empty()) return "";
    return assets[rand() % assets.size()];
}

float LevelGenerator::GetRandomFloat(float min, float max) {
    if (min > max) std::swap(min, max);
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void LevelGenerator::Update(float dt) {
    if (!owner || !player) return;
    float playerX = player->GetTransform().pos.x;

    // Génération du sol
    if (playerX + 4000.0f > nextHillX) {
        SpawnNextHill();
    }

    // Génération des nuages
    if (playerX + 2500.0f > lastCloudX) {
        SpawnCloud();
        lastCloudX = playerX + 2500.0f + GetRandomFloat(cloudMinGapX, cloudMaxGapX);
    }

    // Génération des planètes
    if (playerX + 3000.0f > lastPlanetX) {
        SpawnPlanet();
        lastPlanetX = playerX + 3000.0f + GetRandomFloat(planetMinGapX, planetMaxGapX);
    }
}

void LevelGenerator::SpawnNextHill() {
    Scene* scene = owner->GetScene();

    GameObject* hill = scene->CreateGameObject({ nextHillX, -20.0f }, "AutoHill");
    hill->GetTransform().scale = { HILL_SCALE, HILL_SCALE };
    hill->SetZOrder(0);

    auto* comp = hill->AddComponent<HillComponent>();
    comp->InitFromImage(GetRandomAsset(hillAssets), 10);
    comp->isOneWay = false;

    nextHillX += (HILL_WIDTH * HILL_SCALE);
}

void LevelGenerator::SpawnCloud() {
    Scene* scene = owner->GetScene();

    float randomY = GetRandomFloat(cloudMinY, cloudMaxY);
    float spawnX = player->GetTransform().pos.x + 2500.0f;

    GameObject* cloud = scene->CreateGameObject({ spawnX, randomY }, "Cloud");
    cloud->SetZOrder(-5);

    float s = GetRandomFloat(cloudMinScale, cloudMaxScale);
    cloud->GetTransform().scale = { s, s };

    auto* comp = cloud->AddComponent<HillComponent>();
    comp->InitFromImage(GetRandomAsset(cloudAssets), 10);
    comp->isOneWay = true;
}

void LevelGenerator::SpawnPlanet() {
    Scene* scene = owner->GetScene();

    float randomY = GetRandomFloat(planetMinY, planetMaxY);
    float spawnX = player->GetTransform().pos.x + 3000.0f;

    GameObject* planet = scene->CreateGameObject({ spawnX, randomY }, "Planet");
    planet->SetZOrder(-15);

    float s = GetRandomFloat(planetMinScale, planetMaxScale);
    planet->GetTransform().scale = { s, s };

    auto* comp = planet->AddComponent<HillComponent>();
    comp->InitFromImage(GetRandomAsset(planetAssets), 10);
    comp->isOneWay = true;
}