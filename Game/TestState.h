#pragma once
#include <iostream>
#include <ostream>

#include "PlayerContext.h"
#include "State.h"

class TestState : public FSM::State<PlayerContext>
{
	void Enter(PlayerContext& _context) override;
	void Execute(PlayerContext& _context, float dt) override;
	void Exit(PlayerContext& _context) override;
};