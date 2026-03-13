//#include "LevelGenerator.h"
//#include "GameObject.h"
//#include "HillComponent.h"
//#include "SpriteRendererComponent.h"
//#include <Scene.h>
//#include <random>
//
//void LevelGenerator::Init(GameObject* playerObj) {
//    player = playerObj;
//
//    hillAssets = { "Assets/chunk1.png" };
//    cloudAssets = { "Assets/chunk1.png" };
//    planetAssets = { "Assets/chunk1.png", "Assets/chunk1.png" };
//
//    for (int i = 0; i < 5; ++i) {
//        SpawnNextHill();
//    }
//}
//
//std::string LevelGenerator::GetRandomAsset(const std::vector<std::string>& assets) {
//    if (assets.empty()) return "";
//    return assets[rand() % assets.size()];
//}
//
//void LevelGenerator::Update(float dt) {
//    if (!player) return;
//    float playerX = player->GetTransform().pos.x;
//
//    if (playerX + 4000.0f > nextHillX) {
//        SpawnNextHill();
//    }
//
//    if (playerX + 2500.0f > lastCloudX) {
//        SpawnCloud();
//        lastCloudX = playerX + 2500.0f + (rand() % 1000 + 800);
//    }
//
//    if (playerX + 3000.0f > lastPlanetX) {
//        SpawnPlanet();
//        lastPlanetX = playerX + 3000.0f + (rand() % 4000 + 2000);
//    }
//}
//
//void LevelGenerator::SpawnNextHill() {
//    Scene* scene = owner->GetScene();
//    GameObject* hill = scene->CreateGameObject({ nextHillX, 20.0f }, "HillChunk");
//    hill->GetTransform().scale = { HILL_SCALE, HILL_SCALE };
//    hill->SetZOrder(0);
//
//    auto* comp = hill->AddComponent<HillComponent>();
//    comp->InitFromImage(GetRandomAsset(hillAssets), 10);
//
//    // Le sol n'est PAS traversable
//    comp->isOneWay = false;
//
//    nextHillX += (HILL_WIDTH * HILL_SCALE);
//}
//
//void LevelGenerator::SpawnCloud() {
//    Scene* scene = owner->GetScene();
//    float randomY = -static_cast<float>(rand() % 800 + 400);
//    float spawnX = player->GetTransform().pos.x + 2500.0f;
//
//    GameObject* cloud = scene->CreateGameObject({ spawnX, randomY }, "Cloud");
//    cloud->SetZOrder(-5);
//
//    auto* comp = cloud->AddComponent<HillComponent>();
//    comp->InitFromImage(GetRandomAsset(cloudAssets), 10);
//
//    // Les nuages SONT traversables par le bas
//    comp->isOneWay = true;
//
//    float s = static_cast<float>(rand() % 10 + 10) / 10.0f;
//    cloud->GetTransform().scale = { s, s };
//}
//
//void LevelGenerator::SpawnPlanet() {
//    Scene* scene = owner->GetScene();
//    float randomY = -static_cast<float>(rand() % 1500 + 1000);
//    float spawnX = player->GetTransform().pos.x + 3000.0f;
//
//    GameObject* planet = scene->CreateGameObject({ spawnX, randomY }, "Planet");
//    planet->SetZOrder(-15);
//
//    auto* comp = planet->AddComponent<HillComponent>();
//    comp->InitFromImage(GetRandomAsset(planetAssets), 10);
//
//    comp->isOneWay = false;
//
//    float s = static_cast<float>(rand() % 20 + 10) / 10.0f;
//    planet->GetTransform().scale = { s, s };
//}