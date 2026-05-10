#pragma once

class menu
{
public:
	void draw();
	[[nodiscard]] bool is_open() const noexcept { return this->m_open; }

private:
	enum class tab : int { combat = 0, esp, misc, config, credits };

	void draw_header(float width, float height);
	void draw_sidebar(float width, float height);
	void draw_content(float width, float height);
	void draw_accent_lines(const zui::rect& bounds, float fade_ratio = 0.12f);

	void draw_combat();
	void draw_esp();
	void draw_misc();
	void draw_config();
	void draw_credits();

	tab  m_tab{ tab::combat };
	bool m_open{ false };

	float m_x{ 200.0f };
	float m_y{ 150.0f };
	float m_w{ 680.0f };
	float m_h{ 855.0f };

	// -1 = all weapons, 0-5 = specific group
	static constexpr const char* k_weapon_groups[]{ "pistol","smg","rifle","shotgun","sniper","lmg" };
	int m_weapon_group{ -1 };

	// config
	std::vector<std::string> m_config_list{};
	int                      m_config_selected{ -1 };
	std::string              m_config_name{};
	std::string              m_config_msg{};
	bool                     m_config_list_dirty{ true };
};