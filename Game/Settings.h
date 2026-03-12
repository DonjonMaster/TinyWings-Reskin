#pragma once

// Types de paquets
namespace Settings {
	enum PacketTypes : int {
		NEW_CONNECTION = 1,
		// Ajouter les actions du joueurs à prendre en compte
		DISCONNECT = 2,
		STRING_MESSAGE = 3,
		PLAYER_DATA = 4,
		START_GAME = 5
	};
}