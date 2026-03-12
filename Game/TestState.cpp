#include "TestState.h"
#include <iostream>

#define DEBUG
void TestState::Enter(PlayerContext& _context)
{
#ifdef DEBUG
	std::cout << "Enter dans le testState" << std::endl;
#endif // DEBUG
	if (_context.playerSprite)
	{
#ifdef DEBUG
		std::cout << "Set anim dans le testState" << std::endl;
#endif // DEBUG

		_context.playerSprite->SetAnimationRule(SpriteAnimationRule(4, { 0, 0 }, { 50, 50 }));
	}
}

void TestState::Execute(PlayerContext& _context, float dt)
{
}

void TestState::Exit(PlayerContext& _context)
{

}