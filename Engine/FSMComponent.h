#pragma once
#include "Component.h"
#include "StateMachine.h"
//#include "../Game/PlayerContext.h"

template<typename ContextType>
class FSMComponent : public Component {
public:

    FSM::StateMachine<ContextType> fsm;
    ContextType context;

    void Update(float dt) override;
};

#include "FSMComponent.inl"