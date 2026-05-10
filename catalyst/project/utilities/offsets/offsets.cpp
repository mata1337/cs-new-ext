#include <stdafx.hpp>

bool offsets::initialize()
{
	// csgo_input
	// Pattern targets dwCSGOInput = 0x233DC80 (Build 14157, April 2026)
	{
		const auto initial = g::memory.find_pattern(g::modules.client, "48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 48 8B 0D ? ? ? ? 48 85 C9");
		if (!initial)
		{
			// fallback pattern
			const auto fallback = g::memory.find_pattern(g::modules.client, "48 89 05 ? ? ? ? 0F 57 C0 0F 11 05");
			if (!fallback)
			{
				g::console.warn("csgo_input pattern failed");
				return false;
			}
			csgo_input = g::memory.resolve_rip(fallback);
		}
		else
		{
			csgo_input = g::memory.resolve_rip(initial);
		}

		if (!csgo_input)
		{
			g::console.warn("csgo_input resolve failed");
			return false;
		}
	}

	// entity_list
	// Pattern targets dwEntityList = 0x24CEC60 (Build 14157, April 2026)
	{
		const auto initial = g::memory.find_pattern(g::modules.client, "4C 8B 0D ? ? ? ? 4D 85 C9 74 ? 4C 8B C3");
		if (!initial)
		{
			// fallback
			const auto fallback = g::memory.find_pattern(g::modules.client, "48 89 35 ? ? ? ? 48 85 F6");
			if (!fallback)
			{
				g::console.warn("entity_list pattern failed");
				return false;
			}
			entity_list = g::memory.resolve_rip(fallback);
		}
		else
		{
			entity_list = g::memory.resolve_rip(initial);
		}

		if (!entity_list)
		{
			g::console.warn("entity_list resolve failed");
			return false;
		}
	}

	// local_player_controller
	// Pattern targets dwLocalPlayerController = 0x23084C0 (Build 14157, April 2026)
	{
		const auto initial = g::memory.find_pattern(g::modules.client, "48 8B 05 ? ? ? ? 48 85 C0 74 ? 80 B8 ? ? ? ? ? 74 ? 48 8D");
		if (!initial)
		{
			// fallback
			const auto fallback = g::memory.find_pattern(g::modules.client, "48 8B 05 ? ? ? ? 41 89 BE");
			if (!fallback)
			{
				g::console.warn("local_player_controller pattern failed");
				return false;
			}
			local_player_controller = g::memory.resolve_rip(fallback);
		}
		else
		{
			local_player_controller = g::memory.resolve_rip(initial);
		}

		if (!local_player_controller)
		{
			g::console.warn("local_player_controller resolve failed");
			return false;
		}
	}

	// global_vars
	// Pattern targets dwGlobalVars = 0x20495A0 (Build 14157, April 2026)
	{
		const auto initial = g::memory.find_pattern(g::modules.client, "48 89 0D ? ? ? ? E8 ? ? ? ? 48 8B 05 ? ? ? ? 48 8B 40");
		if (!initial)
		{
			// fallback
			const auto fallback = g::memory.find_pattern(g::modules.client, "48 89 15 ? ? ? ? 48 89 42");
			if (!fallback)
			{
				g::console.warn("global_vars pattern failed");
				return false;
			}
			global_vars = g::memory.resolve_rip(fallback);
		}
		else
		{
			global_vars = g::memory.resolve_rip(initial);
		}

		if (!global_vars)
		{
			g::console.warn("global_vars resolve failed");
			return false;
		}
	}

	// view_matrix
	// Pattern targets dwViewMatrix = 0x232E9C0 (Build 14157, April 2026)
	{
		const auto initial = g::memory.find_pattern(g::modules.client, "48 8D 0D ? ? ? ? 48 C1 E0 06 48 03 C1");
		if (!initial)
		{
			// fallback pattern
			const auto fallback = g::memory.find_pattern(g::modules.client, "C6 86 F0 12 00 00 01 48 8D 0D ? ? ? ?");
			if (!fallback)
			{
				g::console.warn("view_matrix pattern failed");
				return false;
			}
			view_matrix = g::memory.resolve_rip(fallback + 0x7);
		}
		else
		{
			view_matrix = g::memory.resolve_rip(initial);
		}

		if (!view_matrix)
		{
			g::console.warn("view_matrix resolve failed");
			return false;
		}
	}

	g::console.print("offsets initialized.");

	return true;
}