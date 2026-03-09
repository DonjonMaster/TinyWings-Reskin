#pragma once

namespace Settings {
	enum PacketTypes : int {
		NEW_CONNECTION = 1,
		// Ajouter les actions du joueurs à prendre en compte
		DISCONNECT = 2
	};
}