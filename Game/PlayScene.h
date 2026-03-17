#pragma once
#include "BaseScene.h"
#include "TestState.h"
#include "PlayerContext.h"
#include <FSMComponent.h>


class PlayScene : public BaseScene {
public:

    void Create() override;
//    void Update(float dt) override;
//
//private:
//    float currentScore = 0.f;
//    float lastScore = 0.f;
//    const float SCORE_MULTIPLIER = 1.5f;
};
