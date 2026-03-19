#pragma once

namespace Settings {
	enum PacketTypes : int {
		NEW_CONNECTION = 1,
		DISCONNECT = 2,
		STRING_MESSAGE = 3,
		PLAYER_DATA = 4,
		START_GAME = 5
	};
}
