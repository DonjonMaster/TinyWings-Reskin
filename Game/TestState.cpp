#include "TestState.h"
#include <iostream>
void TestState::Enter(PlayerContext& _context)
{
	std::cout << "Enter dans le idle" << std::endl;
	if (_context.playerSprite)
	{
		std::cout << "Set anim dans le idle" << std::endl;
		_context.playerSprite->SetAnimationRule(SpriteAnimationRule(4, { 0, 0 }, { 50, 50 }));
	}
}

void TestState::Execute(PlayerContext& _context, float dt)
{
}

void TestState::Exit(PlayerContext& _context)
{

}