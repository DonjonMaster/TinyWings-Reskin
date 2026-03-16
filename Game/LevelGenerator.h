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

    // Fonction utilitaire pour choisir un asset au hasard
    std::string GetRandomAsset(const std::vector<std::string>& assets);

    // Fonction utilitaire pour générer un float aléatoire entre min et max
    float GetRandomFloat(float min, float max);

    GameObject* player = nullptr;


    // level generator settings
    
    // --- Paramètres des Collines ---
    float nextHillX = 0.0f;
    const float HILL_WIDTH = 1212.0f;
    const float HILL_SCALE = 8.0f;

   
    // --- Nuages ---
    float cloudMinY = -1000.0f;   // Hauteur la plus haute (plus c'est négatif, plus c'est haut)
    float cloudMaxY = -50.0f;    // Hauteur la plus basse
    float cloudMinGapX = 800.0f;  // Espace minimum entre 2 nuages
    float cloudMaxGapX = 1800.0f; // Espace maximum entre 2 nuages
    float cloudMinScale = 1.0f;   // Taille minimum
    float cloudMaxScale = 2.0f;   // Taille maximum

    // --- Planètes ---
    float planetMinY = -2500.0f;
    float planetMaxY = -500.0f;
    float planetMinGapX = 2000.0f;
    float planetMaxGapX = 6000.0f;
    float planetMinScale = 1.0f;
    float planetMaxScale = 3.0f;

    // --- Suivi des distances ---
    float lastCloudX = 0.0f;
    float lastPlanetX = 0.0f;

    // --- BANQUE D'ASSETS ---
    std::vector<std::string> hillAssets;
    std::vector<std::string> cloudAssets;
    std::vector<std::string> planetAssets;



};