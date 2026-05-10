#include <stdafx.hpp>
#include <fstream>
#include <filesystem>

namespace zhelper
{
	static inline void rect_rounded(float x, float y, float w, float h, float r, zdraw::rgba col, int segs = 6)
	{
		r = std::min(r, std::min(w, h) * 0.5f);
		if (r < 1.0f) { zdraw::rect_filled(x, y, w, h, col); return; }
		zdraw::rect_filled(x, y + r, w, h - r * 2, col);
		zdraw::rect_filled(x + r, y, w - r * 2, r, col);
		zdraw::rect_filled(x + r, y + h - r, w - r * 2, r, col);
		zdraw::circle_filled(x + r, y + r, r, col, segs * 4);
		zdraw::circle_filled(x + w - r, y + r, r, col, segs * 4);
		zdraw::circle_filled(x + r, y + h - r, r, col, segs * 4);
		zdraw::circle_filled(x + w - r, y + h - r, r, col, segs * 4);
	}
}

namespace config_io
{
	static const std::string k_dir = "configs/";
	static void ensure_dir() { std::filesystem::create_directories(k_dir); }
	static std::string path(const std::string& name) { return k_dir + name + ".txt"; }

	static std::vector<std::string> list()
	{
		ensure_dir();
		std::vector<std::string> out{};
		for (const auto& e : std::filesystem::directory_iterator(k_dir))
			if (e.path().extension() == ".txt")
				out.push_back(e.path().stem().string());
		return out;
	}

	static void write_rgba(std::ofstream& f, const zdraw::rgba& c)
	{
		f << (int)c.r << " " << (int)c.g << " " << (int)c.b << " " << (int)c.a << "\n";
	}

	static zdraw::rgba read_rgba(std::ifstream& f)
	{
		int r, g, b, a; f >> r >> g >> b >> a;
		return zdraw::rgba(r, g, b, a);
	}

	static void save(const std::string& name)
	{
		ensure_dir();
		std::ofstream f(path(name));
		if (!f.is_open()) return;

		for (auto& g : settings::g_combat.groups)
		{
			f << g.aimbot.enabled << " " << g.aimbot.key << " " << g.aimbot.fov << " "
				<< g.aimbot.smoothing << " " << g.aimbot.head_only << " " << g.aimbot.visible_only << " "
				<< g.aimbot.autowall << " " << g.aimbot.min_damage << " " << g.aimbot.draw_fov << " "
				<< g.aimbot.predictive << "\n";
			write_rgba(f, g.aimbot.fov_color);
			f << g.triggerbot.enabled << " " << g.triggerbot.key << " " << g.triggerbot.hitchance << " "
				<< g.triggerbot.delay << " " << g.triggerbot.autowall << " " << g.triggerbot.min_damage << " "
				<< g.triggerbot.autostop << " " << g.triggerbot.early_autostop << " " << g.triggerbot.predictive << "\n";
		}

		auto& p = settings::g_esp.m_player;
		f << p.m_box.enabled << " " << (int)p.m_box.style << " " << p.m_box.fill << " "
			<< p.m_box.outline << " " << p.m_box.corner_length << "\n";
		write_rgba(f, p.m_box.visible_color); write_rgba(f, p.m_box.occluded_color);
		f << p.m_skeleton.enabled << " " << p.m_skeleton.thickness << "\n";
		write_rgba(f, p.m_skeleton.visible_color); write_rgba(f, p.m_skeleton.occluded_color);
		f << p.m_health_bar.enabled << " " << p.m_health_bar.outline << " "
			<< p.m_health_bar.gradient << " " << p.m_health_bar.show_value << "\n";
		write_rgba(f, p.m_health_bar.full_color); write_rgba(f, p.m_health_bar.low_color);
		f << p.m_ammo_bar.enabled << " " << p.m_ammo_bar.outline << " "
			<< p.m_ammo_bar.gradient << " " << p.m_ammo_bar.show_value << "\n";
		write_rgba(f, p.m_ammo_bar.full_color); write_rgba(f, p.m_ammo_bar.low_color);
		f << p.m_name.enabled << "\n"; write_rgba(f, p.m_name.color);
		f << p.m_weapon.enabled << " " << (int)p.m_weapon.display << "\n";
		write_rgba(f, p.m_weapon.text_color); write_rgba(f, p.m_weapon.icon_color);

		auto& it = settings::g_esp.m_item;
		f << it.enabled << " " << it.max_distance << " " << it.m_icon.enabled << " "
			<< it.m_name.enabled << " " << it.m_ammo.enabled << "\n";
		write_rgba(f, it.m_icon.color); write_rgba(f, it.m_name.color);
		write_rgba(f, it.m_ammo.color); write_rgba(f, it.m_ammo.empty_color);
		f << it.m_filters.rifles << " " << it.m_filters.smgs << " " << it.m_filters.shotguns << " "
			<< it.m_filters.snipers << " " << it.m_filters.pistols << " " << it.m_filters.heavy << " "
			<< it.m_filters.grenades << " " << it.m_filters.utility << "\n";

		auto& pr = settings::g_esp.m_projectile;
		f << pr.enabled << " " << pr.show_icon << " " << pr.show_name << " "
			<< pr.show_timer_bar << " " << pr.show_inferno_bounds << "\n";
		write_rgba(f, pr.color_he); write_rgba(f, pr.color_flash);
		write_rgba(f, pr.color_smoke); write_rgba(f, pr.color_molotov); write_rgba(f, pr.color_decoy);

		auto& gr = settings::g_misc.m_grenades;
		f << gr.enabled << " " << gr.local_only << " " << gr.line_thickness << " "
			<< gr.line_gradient << " " << gr.show_bounces << " " << gr.bounce_size << " "
			<< gr.detonate_size << " " << gr.per_type_colors << " " << gr.fade_duration << "\n";
		write_rgba(f, gr.line_color); write_rgba(f, gr.bounce_color); write_rgba(f, gr.detonate_color);
		write_rgba(f, gr.color_he); write_rgba(f, gr.color_flash);
		write_rgba(f, gr.color_smoke); write_rgba(f, gr.color_molotov); write_rgba(f, gr.color_decoy);

		f << settings::g_misc.m_overlay.enabled << "\n";
		write_rgba(f, settings::g_misc.m_overlay.color);
		write_rgba(f, settings::g_misc.m_accent_color);
	}

	static bool load(const std::string& name)
	{
		std::ifstream f(path(name));
		if (!f.is_open()) return false;

		for (auto& g : settings::g_combat.groups)
		{
			f >> g.aimbot.enabled >> g.aimbot.key >> g.aimbot.fov >> g.aimbot.smoothing
				>> g.aimbot.head_only >> g.aimbot.visible_only >> g.aimbot.autowall
				>> g.aimbot.min_damage >> g.aimbot.draw_fov >> g.aimbot.predictive;
			g.aimbot.fov_color = read_rgba(f);
			f >> g.triggerbot.enabled >> g.triggerbot.key >> g.triggerbot.hitchance
				>> g.triggerbot.delay >> g.triggerbot.autowall >> g.triggerbot.min_damage
				>> g.triggerbot.autostop >> g.triggerbot.early_autostop >> g.triggerbot.predictive;
		}

		auto& p = settings::g_esp.m_player;
		int bs; f >> p.m_box.enabled >> bs >> p.m_box.fill >> p.m_box.outline >> p.m_box.corner_length;
		p.m_box.style = static_cast<settings::esp::player::box::style0>(bs);
		p.m_box.visible_color = read_rgba(f); p.m_box.occluded_color = read_rgba(f);
		f >> p.m_skeleton.enabled >> p.m_skeleton.thickness;
		p.m_skeleton.visible_color = read_rgba(f); p.m_skeleton.occluded_color = read_rgba(f);
		f >> p.m_health_bar.enabled >> p.m_health_bar.outline >> p.m_health_bar.gradient >> p.m_health_bar.show_value;
		p.m_health_bar.full_color = read_rgba(f); p.m_health_bar.low_color = read_rgba(f);
		f >> p.m_ammo_bar.enabled >> p.m_ammo_bar.outline >> p.m_ammo_bar.gradient >> p.m_ammo_bar.show_value;
		p.m_ammo_bar.full_color = read_rgba(f); p.m_ammo_bar.low_color = read_rgba(f);
		f >> p.m_name.enabled; p.m_name.color = read_rgba(f);
		int wd; f >> p.m_weapon.enabled >> wd;
		p.m_weapon.display = static_cast<settings::esp::player::weapon::display_type>(wd);
		p.m_weapon.text_color = read_rgba(f); p.m_weapon.icon_color = read_rgba(f);

		auto& it = settings::g_esp.m_item;
		f >> it.enabled >> it.max_distance >> it.m_icon.enabled >> it.m_name.enabled >> it.m_ammo.enabled;
		it.m_icon.color = read_rgba(f); it.m_name.color = read_rgba(f);
		it.m_ammo.color = read_rgba(f); it.m_ammo.empty_color = read_rgba(f);
		f >> it.m_filters.rifles >> it.m_filters.smgs >> it.m_filters.shotguns
			>> it.m_filters.snipers >> it.m_filters.pistols >> it.m_filters.heavy
			>> it.m_filters.grenades >> it.m_filters.utility;

		auto& pr = settings::g_esp.m_projectile;
		f >> pr.enabled >> pr.show_icon >> pr.show_name >> pr.show_timer_bar >> pr.show_inferno_bounds;
		pr.color_he = read_rgba(f); pr.color_flash = read_rgba(f);
		pr.color_smoke = read_rgba(f); pr.color_molotov = read_rgba(f); pr.color_decoy = read_rgba(f);

		auto& gr = settings::g_misc.m_grenades;
		f >> gr.enabled >> gr.local_only >> gr.line_thickness >> gr.line_gradient
			>> gr.show_bounces >> gr.bounce_size >> gr.detonate_size >> gr.per_type_colors >> gr.fade_duration;
		gr.line_color = read_rgba(f); gr.bounce_color = read_rgba(f); gr.detonate_color = read_rgba(f);
		gr.color_he = read_rgba(f); gr.color_flash = read_rgba(f);
		gr.color_smoke = read_rgba(f); gr.color_molotov = read_rgba(f); gr.color_decoy = read_rgba(f);

		if (f.good())
		{
			f >> settings::g_misc.m_overlay.enabled;
			settings::g_misc.m_overlay.color = read_rgba(f);
			settings::g_misc.m_accent_color = read_rgba(f);
		}

		return true;
	}
}

static void draw_overlay_dim()
{
	if (!settings::g_misc.m_overlay.enabled) return;
	const auto [sw_i, sh_i] = zdraw::get_display_size();
	zdraw::rect_filled(0, 0, static_cast<float>(sw_i), static_cast<float>(sh_i), settings::g_misc.m_overlay.color);
}

void menu::draw()
{
	static zdraw::rgba s_last_accent(0, 200, 255, 255);

	auto apply_theme = [&](const zdraw::rgba& ac)
		{
			auto& s = zui::get_style();
			s.accent = ac;
			s.window_bg = zdraw::rgba(4, 7, 14, 253);
			s.window_border = zdraw::rgba(ac.r, ac.g, ac.b, 45);
			s.nested_bg = zdraw::rgba(7, 11, 20, 250);
			s.nested_border = zdraw::rgba(ac.r, ac.g, ac.b, 28);
			s.group_box_bg = zdraw::rgba(6, 9, 17, 242);
			s.group_box_border = zdraw::rgba(ac.r, ac.g, ac.b, 50);
			s.group_box_title_text = ac;
			s.text = zdraw::rgba(180, 210, 230, 255);
			s.item_spacing_y = 5.0f;
			s.checkbox_bg = zdraw::rgba(8, 13, 22, 255);
			s.checkbox_border = zdraw::rgba(ac.r, ac.g, ac.b, 110);
			s.checkbox_check = ac;
			s.slider_bg = zdraw::rgba(8, 13, 22, 255);
			s.slider_border = zdraw::rgba(ac.r, ac.g, ac.b, 70);
			s.slider_fill = zdraw::rgba(ac.r, ac.g, ac.b, 180);
			s.slider_grab = ac;
			s.slider_grab_active = zdraw::rgba(std::min(255, (int)ac.r + 40), std::min(255, (int)ac.g + 40), std::min(255, (int)ac.b + 40), 255);
			s.button_bg = zdraw::rgba(8, 13, 22, 255);
			s.button_border = zdraw::rgba(ac.r, ac.g, ac.b, 90);
			s.button_hovered = zdraw::rgba(ac.r, ac.g, ac.b, 35);
			s.button_active = zdraw::rgba(ac.r, ac.g, ac.b, 70);
			s.keybind_bg = zdraw::rgba(8, 13, 22, 255);
			s.keybind_border = zdraw::rgba(ac.r, ac.g, ac.b, 90);
			s.keybind_waiting = ac;
			s.combo_bg = zdraw::rgba(8, 13, 22, 255);
			s.combo_border = zdraw::rgba(ac.r, ac.g, ac.b, 90);
			s.combo_arrow = zdraw::rgba(ac.r, ac.g, ac.b, 180);
			s.combo_hovered = zdraw::rgba(ac.r, ac.g, ac.b, 25);
			s.combo_popup_bg = zdraw::rgba(5, 8, 18, 252);
			s.combo_popup_border = zdraw::rgba(ac.r, ac.g, ac.b, 60);
			s.combo_item_hovered = zdraw::rgba(ac.r, ac.g, ac.b, 35);
			s.combo_item_selected = zdraw::rgba(ac.r, ac.g, ac.b, 60);
			s.color_picker_bg = zdraw::rgba(8, 13, 22, 255);
			s.color_picker_border = zdraw::rgba(ac.r, ac.g, ac.b, 100);
			s.text_input_bg = zdraw::rgba(6, 10, 18, 255);
			s.text_input_border = zdraw::rgba(ac.r, ac.g, ac.b, 90);
		};

	static bool theme_applied = false;
	if (!theme_applied)
	{
		settings::g_misc.m_accent_color = zdraw::rgba(0, 200, 255, 255);
		apply_theme(settings::g_misc.m_accent_color);
		s_last_accent = settings::g_misc.m_accent_color;
		theme_applied = true;
	}
	else if (settings::g_misc.m_accent_color.val != s_last_accent.val)
	{
		apply_theme(settings::g_misc.m_accent_color);
		s_last_accent = settings::g_misc.m_accent_color;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		this->m_open = !this->m_open;

	draw_overlay_dim();

	if (!this->m_open) return;

	zui::begin();

	if (zui::begin_window("megamind##main", this->m_x, this->m_y, this->m_w, this->m_h, true, 580.0f, 440.0f))
	{
		const auto [avail_w, avail_h] = zui::get_content_region_avail();

		if (zui::begin_nested_window("##inner", avail_w, avail_h))
		{
			constexpr auto header_h = 38.0f;
			constexpr auto sidebar_w = 115.0f;
			constexpr auto padding = 9.0f;

			zui::set_cursor_pos(0.0f, 0.0f);
			this->draw_header(avail_w, header_h);

			zui::set_cursor_pos(0.0f, header_h + padding);
			this->draw_sidebar(sidebar_w, avail_h - header_h - padding * 2.0f);

			zui::set_cursor_pos(sidebar_w + padding, header_h + padding);
			this->draw_content(avail_w - sidebar_w - padding * 2.0f, avail_h - header_h - padding * 2.0f);

			if (const auto win = zui::detail::get_current_window())
				this->draw_accent_lines(win->bounds);

			zui::end_nested_window();
		}

		zui::end_window();
	}

	zui::end();
}

void menu::draw_header(float width, float height)
{
	if (!zui::begin_nested_window("##header", width, height)) return;

	const auto w = zui::detail::get_current_window();
	if (!w) { zui::end_nested_window(); return; }

	const auto bx = w->bounds.x, by = w->bounds.y, bw = w->bounds.w, bh = w->bounds.h;
	constexpr float rounding = 6.0f;

	zhelper::rect_rounded(bx, by, bw, bh, rounding, zdraw::rgba(4, 7, 14, 255));
	zhelper::rect_rounded(bx, by + 6, 3.0f, bh - 12, 2.0f, zdraw::rgba(0, 200, 255, 220));
	zdraw::rect_filled_multi_color(bx + 3.0f, by, 22.0f, bh,
		zdraw::rgba(0, 200, 255, 30), zdraw::rgba(0, 200, 255, 0),
		zdraw::rgba(0, 200, 255, 0), zdraw::rgba(0, 200, 255, 30));

	{ auto [tw, th] = zdraw::measure_text("megamind"); zdraw::text(bx + 32, by + (bh * .34f) - th * .5f, "megamind", zdraw::rgba(0, 200, 255, 255)); }
	{ auto [sw, sh] = zdraw::measure_text("made by luta"); zdraw::text(bx + 32, by + (bh * .70f) - sh * .5f, "made by luta", zdraw::rgba(30, 70, 110, 140)); }
	{ auto [vw, vh] = zdraw::measure_text("v2.3"); zdraw::text(bx + bw - vw - 14, by + (bh - vh) * .5f, "v2.3", zdraw::rgba(0, 140, 190, 110)); }

	const auto ac = zui::get_accent_color();
	const auto tr = zdraw::rgba(ac.r, ac.g, ac.b, 0);
	const auto fw = bw * 0.1f;
	zdraw::rect_filled_multi_color(bx, by + bh - 1, fw, 1, tr, ac, ac, tr);
	zdraw::rect_filled(bx + fw, by + bh - 1, bw - fw * 2, 1, ac);
	zdraw::rect_filled_multi_color(bx + bw - fw, by + bh - 1, fw, 1, ac, tr, tr, ac);

	zui::end_nested_window();
}

void menu::draw_sidebar(float width, float height)
{
	if (!zui::begin_nested_window("##sidebar", width, height)) return;

	const auto w = zui::detail::get_current_window();
	if (!w) { zui::end_nested_window(); return; }

	const auto dt = zdraw::get_delta_time();
	const auto bx = w->bounds.x, by = w->bounds.y, bw = w->bounds.w, bh = w->bounds.h;

	zdraw::rect_filled(bx, by, bw, bh, zdraw::rgba(5, 8, 16, 250));
	zdraw::rect_filled(bx + bw - 1, by, 1, bh, zdraw::rgba(0, 200, 255, 20));

	static constexpr std::pair<const char*, tab> tabs[]
	{
		{ "combat",  tab::combat  },
		{ "esp",     tab::esp     },
		{ "misc",    tab::misc    },
		{ "config",  tab::config  },
		{ "credits", tab::credits },
	};

	constexpr auto tab_count = static_cast<int>(std::size(tabs));
	constexpr auto tab_h = 34.0f;
	constexpr auto tab_gap = 5.0f;
	constexpr auto rounding = 5.0f;
	constexpr auto h_pad = 5.0f;

	struct anim_t { float v{ 0.0f }; };
	static std::array<anim_t, tab_count> anims{};

	auto cy = by + 10.0f;

	for (int i = 0; i < tab_count; ++i)
	{
		const auto& t = tabs[i];
		const auto is_sel = (this->m_tab == t.second);
		const auto tr_rect = zui::rect{ bx + h_pad, cy, bw - h_pad * 2 - 1, tab_h };
		const auto hov = zui::detail::mouse_hovered(tr_rect) && !zui::detail::overlay_blocking_input();

		if (hov && zui::detail::mouse_clicked())
			this->m_tab = t.second;

		auto& a = anims[i];
		a.v += ((is_sel ? 1.f : 0.f) - a.v) * std::min(14.f * dt, 1.f);

		if (a.v > 0.01f || hov)
		{
			const std::uint8_t fill_a = (std::uint8_t)(45 * (is_sel ? a.v : (hov ? 0.55f : 0.f)));
			if (fill_a > 0)
				zhelper::rect_rounded(bx + h_pad, cy, bw - h_pad * 2 - 1, tab_h, rounding, zdraw::rgba(0, 200, 255, (int)fill_a));
		}

		if (a.v > 0.01f)
		{
			const auto bh_s = tab_h * 0.55f * a.v;
			const auto by2 = cy + (tab_h - bh_s) * 0.5f;
			zhelper::rect_rounded(bx + h_pad, by2, 3, bh_s, 2.0f, zdraw::rgba(0, 200, 255, (int)(255 * a.v)));
			zdraw::rect_filled_multi_color(bx + h_pad + 3, by2, 10, bh_s,
				zdraw::rgba(0, 200, 255, (int)(35 * a.v)), zdraw::rgba(0, 200, 255, 0),
				zdraw::rgba(0, 200, 255, 0), zdraw::rgba(0, 200, 255, (int)(35 * a.v)));
		}

		auto [tw, th] = zdraw::measure_text(t.first);
		const auto col = is_sel ? zdraw::rgba(0, 200, 255, 255)
			: (hov ? zdraw::rgba(120, 190, 220, 220) : zdraw::rgba(55, 90, 130, 175));
		zdraw::text(bx + 22, cy + (tab_h - th) * .5f, t.first, col);
		cy += tab_h + tab_gap;
	}

	zui::end_nested_window();
}

void menu::draw_content(float width, float height)
{
	zui::push_style_var(zui::style_var::window_padding_x, 10.0f);
	zui::push_style_var(zui::style_var::window_padding_y, 10.0f);

	if (!zui::begin_nested_window("##content", width, height))
	{
		zui::pop_style_var(2); return;
	}

	if (const auto win = zui::detail::get_current_window())
		this->draw_accent_lines(win->bounds);

	switch (this->m_tab)
	{
	case tab::combat:  this->draw_combat();  break;
	case tab::esp:     this->draw_esp();     break;
	case tab::misc:    this->draw_misc();    break;
	case tab::config:  this->draw_config();  break;
	case tab::credits: this->draw_credits(); break;
	default: break;
	}

	zui::pop_style_var(2);
	zui::end_nested_window();
}

void menu::draw_accent_lines(const zui::rect& bounds, float fade_ratio)
{
	const auto ix = bounds.x + 1, iw = bounds.w - 2;
	const auto ty = bounds.y + 1, by = bounds.y + bounds.h - 2;
	const auto ac = zui::get_accent_color();
	const auto tr = zdraw::rgba(ac.r, ac.g, ac.b, 0);
	const auto fw = iw * fade_ratio, sw = iw - fw * 2;

	for (const auto ly : { ty, by })
	{
		zdraw::rect_filled_multi_color(ix, ly, fw, 1, tr, ac, ac, tr);
		zdraw::rect_filled(ix + fw, ly, sw, 1, ac);
		zdraw::rect_filled_multi_color(ix + fw + sw, ly, fw, 1, ac, tr, tr, ac);
	}
}

void menu::draw_combat()
{
	const auto [avail_w, avail_h] = zui::get_content_region_avail();
	const auto col_w = (avail_w - 10.0f) * 0.5f;
	const auto& style = zui::get_style();

	{
		const auto win = zui::detail::get_current_window();
		if (win)
		{
			constexpr auto bar_h = 26.0f;
			constexpr auto gap = 10.0f;
			auto gx = win->bounds.x + style.window_padding_x;
			const auto gy = win->bounds.y + win->cursor_y;

			zdraw::rect_filled(win->bounds.x, gy, avail_w, bar_h, zdraw::rgba(3, 6, 14, 220));
			zdraw::rect_filled(win->bounds.x, gy, avail_w, 1, zdraw::rgba(0, 200, 255, 18));
			zdraw::rect_filled(win->bounds.x, gy + bar_h - 1, avail_w, 1, zdraw::rgba(0, 200, 255, 18));

			for (int i = 0; i < 6; ++i)
			{
				auto [tw, th] = zdraw::measure_text(k_weapon_groups[i]);
				const auto item_w = tw + 12.0f;
				const auto hov = zui::detail::mouse_hovered(zui::rect{ gx - 4, gy, item_w, bar_h })
					&& !zui::detail::overlay_blocking_input();
				if (hov && zui::detail::mouse_clicked()) this->m_weapon_group = i;

				const auto sel = (this->m_weapon_group == i);
				if (sel)
				{
					zdraw::rect_filled_multi_color(gx - 4, gy + bar_h - 2, item_w * 0.2f, 2,
						zdraw::rgba(0, 200, 255, 0), zdraw::rgba(0, 200, 255, 220), zdraw::rgba(0, 200, 255, 220), zdraw::rgba(0, 200, 255, 0));
					zdraw::rect_filled(gx - 4 + item_w * 0.2f, gy + bar_h - 2, item_w * 0.6f, 2, zdraw::rgba(0, 200, 255, 220));
					zdraw::rect_filled_multi_color(gx - 4 + item_w * 0.8f, gy + bar_h - 2, item_w * 0.2f, 2,
						zdraw::rgba(0, 200, 255, 220), zdraw::rgba(0, 200, 255, 0), zdraw::rgba(0, 200, 255, 0), zdraw::rgba(0, 200, 255, 220));
				}

				const auto col = sel ? zdraw::rgba(0, 200, 255, 255)
					: (hov ? zdraw::rgba(100, 175, 210, 220) : zdraw::rgba(50, 85, 120, 160));
				zdraw::text(gx, gy + (bar_h - th) * .5f, k_weapon_groups[i], col);
				gx += tw + gap + 10;
			}

			win->cursor_y += bar_h + style.item_spacing_y + 4;
			win->line_height = 0;
		}
	}

	auto& cfg = settings::g_combat.groups[this->m_weapon_group];

	if (zui::begin_group_box("aimbot", col_w))
	{
		zui::checkbox("enabled##ab", cfg.aimbot.enabled);
		zui::keybind("key##ab", cfg.aimbot.key);
		zui::separator();
		zui::slider_int("fov##ab", cfg.aimbot.fov, 1, 45);
		zui::slider_int("smoothing##ab", cfg.aimbot.smoothing, 0, 50);
		zui::separator();
		zui::checkbox("visible only##ab", cfg.aimbot.visible_only);
		zui::checkbox("head only##ab", cfg.aimbot.head_only);
		zui::checkbox("predictive##ab", cfg.aimbot.predictive);
		zui::separator();
		zui::checkbox("autowall##ab", cfg.aimbot.autowall);
		if (cfg.aimbot.autowall)
			zui::slider_float("min dmg##ab", cfg.aimbot.min_damage, 1, 100, "%.0f");
		zui::separator();
		zui::checkbox("draw fov##ab", cfg.aimbot.draw_fov);
		if (cfg.aimbot.draw_fov)
			zui::color_picker("fov color##ab", cfg.aimbot.fov_color);
		zui::end_group_box();
	}

	zui::same_line();

	if (zui::begin_group_box("triggerbot", col_w))
	{
		zui::checkbox("enabled##tb", cfg.triggerbot.enabled);
		zui::keybind("key##tb", cfg.triggerbot.key);
		zui::separator();
		zui::slider_float("hitchance##tb", cfg.triggerbot.hitchance, 0, 100, "%.0f%%");
		zui::slider_int("delay ms##tb", cfg.triggerbot.delay, 0, 500);
		zui::separator();
		zui::checkbox("predictive##tb", cfg.triggerbot.predictive);
		zui::checkbox("autostop##tb", cfg.triggerbot.autostop);
		if (cfg.triggerbot.autostop)
			zui::checkbox("early autostop##tb", cfg.triggerbot.early_autostop);
		zui::separator();
		zui::checkbox("autowall##tb", cfg.triggerbot.autowall);
		if (cfg.triggerbot.autowall)
			zui::slider_float("min dmg##tb", cfg.triggerbot.min_damage, 1, 100, "%.0f");
		zui::end_group_box();
	}
}

void menu::draw_esp()
{
	const auto [avail_w, avail_h] = zui::get_content_region_avail();
	const auto col_w = (avail_w - 10.0f) * 0.5f;
	auto& p = settings::g_esp.m_player;

	if (zui::begin_group_box("player", col_w))
	{
		zui::checkbox("box##bx", p.m_box.enabled);
		constexpr const char* bs[]{ "full","cornered" };
		auto bsi = (int)p.m_box.style;
		if (zui::combo("style##bx", bsi, bs, 2))
			p.m_box.style = (settings::esp::player::box::style0)bsi;
		if (p.m_box.style == settings::esp::player::box::style0::cornered)
			zui::slider_float("corner len##bx", p.m_box.corner_length, 4, 30, "%.0f");
		zui::checkbox("fill##bx", p.m_box.fill);
		zui::checkbox("outline##bx", p.m_box.outline);
		zui::color_picker("visible##bx", p.m_box.visible_color);
		zui::color_picker("occluded##bx", p.m_box.occluded_color);
		zui::separator();
		zui::checkbox("skeleton##sk", p.m_skeleton.enabled);
		zui::slider_float("thickness##sk", p.m_skeleton.thickness, 0.5f, 4, "%.1f");
		zui::color_picker("skel visible##sk", p.m_skeleton.visible_color);
		zui::color_picker("skel occluded##sk", p.m_skeleton.occluded_color);
		zui::separator();
		zui::checkbox("name##nm", p.m_name.enabled);
		zui::color_picker("name color##nm", p.m_name.color);
		zui::end_group_box();
	}

	if (zui::begin_group_box("bars", col_w))
	{
		zui::checkbox("health bar##hb", p.m_health_bar.enabled);
		zui::checkbox("hb gradient##hb", p.m_health_bar.gradient);
		zui::checkbox("hb value##hb", p.m_health_bar.show_value);
		zui::color_picker("hb full##hb", p.m_health_bar.full_color);
		zui::color_picker("hb low##hb", p.m_health_bar.low_color);
		zui::separator();
		zui::checkbox("ammo bar##amb", p.m_ammo_bar.enabled);
		zui::checkbox("ammo gradient##amb", p.m_ammo_bar.gradient);
		zui::checkbox("ammo value##amb", p.m_ammo_bar.show_value);
		zui::color_picker("ammo full##amb", p.m_ammo_bar.full_color);
		zui::color_picker("ammo low##amb", p.m_ammo_bar.low_color);
		zui::end_group_box();
	}

	zui::set_cursor_pos(col_w + 10.0f + zui::get_style().window_padding_x, zui::get_style().window_padding_y);

	if (zui::begin_group_box("weapon", col_w))
	{
		zui::checkbox("enabled##wp", p.m_weapon.enabled);
		constexpr const char* dt[]{ "text","icon","text + icon" };
		auto dti = (int)p.m_weapon.display;
		if (zui::combo("display##wp", dti, dt, 3))
			p.m_weapon.display = (settings::esp::player::weapon::display_type)dti;
		zui::color_picker("text color##wp", p.m_weapon.text_color);
		zui::color_picker("icon color##wp", p.m_weapon.icon_color);
		zui::end_group_box();
	}

	if (zui::begin_group_box("items", col_w))
	{
		auto& it = settings::g_esp.m_item;
		zui::checkbox("enabled##it", it.enabled);
		zui::slider_float("max dist##it", it.max_distance, 5, 150, "%.0fm");
		zui::separator();
		zui::checkbox("icon##it", it.m_icon.enabled);
		zui::color_picker("icon color##it", it.m_icon.color);
		zui::checkbox("name##it", it.m_name.enabled);
		zui::color_picker("name color##it", it.m_name.color);
		zui::checkbox("ammo##it", it.m_ammo.enabled);
		zui::color_picker("ammo color##it", it.m_ammo.color);
		zui::color_picker("empty color##it", it.m_ammo.empty_color);
		zui::end_group_box();
	}

	if (zui::begin_group_box("filters", col_w))
	{
		auto& f = settings::g_esp.m_item.m_filters;
		zui::checkbox("rifles##f", f.rifles);   zui::checkbox("smgs##f", f.smgs);
		zui::checkbox("shotguns##f", f.shotguns); zui::checkbox("snipers##f", f.snipers);
		zui::checkbox("pistols##f", f.pistols);  zui::checkbox("heavy##f", f.heavy);
		zui::checkbox("grenades##f", f.grenades); zui::checkbox("utility##f", f.utility);
		zui::end_group_box();
	}

	if (zui::begin_group_box("projectiles", col_w))
	{
		auto& pr = settings::g_esp.m_projectile;
		zui::checkbox("enabled##pr", pr.enabled);
		zui::checkbox("show icon##pr", pr.show_icon);
		zui::checkbox("show name##pr", pr.show_name);
		zui::checkbox("timer bar##pr", pr.show_timer_bar);
		zui::checkbox("inferno##pr", pr.show_inferno_bounds);
		zui::separator();
		zui::color_picker("he##pr", pr.color_he);
		zui::color_picker("flash##pr", pr.color_flash);
		zui::color_picker("smoke##pr", pr.color_smoke);
		zui::color_picker("molotov##pr", pr.color_molotov);
		zui::color_picker("decoy##pr", pr.color_decoy);
		zui::end_group_box();
	}
}

void menu::draw_misc()
{
	const auto [avail_w, avail_h] = zui::get_content_region_avail();
	const auto col_w = (avail_w - 10.0f) * 0.5f;
	auto& gr = settings::g_misc.m_grenades;

	if (zui::begin_group_box("grenade prediction", col_w))
	{
		zui::checkbox("enabled##gr", gr.enabled);
		zui::checkbox("local only##gr", gr.local_only);
		zui::separator();
		zui::slider_float("line thickness##gr", gr.line_thickness, 0.5f, 5, "%.1f");
		zui::checkbox("gradient line##gr", gr.line_gradient);
		zui::color_picker("line color##gr", gr.line_color);
		zui::separator();
		zui::checkbox("show bounces##gr", gr.show_bounces);
		zui::slider_float("bounce size##gr", gr.bounce_size, 1, 8, "%.1f");
		zui::color_picker("bounce color##gr", gr.bounce_color);
		zui::separator();
		zui::slider_float("detonate size##gr", gr.detonate_size, 1, 10, "%.1f");
		zui::slider_float("fade duration##gr", gr.fade_duration, 0, 2, "%.2f");
		zui::color_picker("detonate color##gr", gr.detonate_color);
		zui::separator();
		zui::checkbox("per type colors##gr", gr.per_type_colors);
		if (gr.per_type_colors)
		{
			zui::color_picker("he##gr", gr.color_he);
			zui::color_picker("flash##gr", gr.color_flash);
			zui::color_picker("smoke##gr", gr.color_smoke);
			zui::color_picker("molotov##gr", gr.color_molotov);
			zui::color_picker("decoy##gr", gr.color_decoy);
		}
		zui::end_group_box();
	}

	zui::same_line();

	if (zui::begin_group_box("visuals", col_w))
	{
		auto& ov = settings::g_misc.m_overlay;
		zui::checkbox("screen overlay##ov", ov.enabled);
		if (ov.enabled)
		{
			zui::color_picker("overlay color##ov", ov.color);
			zui::text_colored("lower alpha = subtle dim", zdraw::rgba(40, 80, 120, 130));
		}
		zui::separator();
		zui::text_colored("accent color", zdraw::rgba(120, 180, 210, 200));
		zui::color_picker("##accent_color", settings::g_misc.m_accent_color);
		{
			const auto win = zui::detail::get_current_window();
			if (win)
			{
				const float sw_h = 4.0f, sw_w = col_w - 24.0f;
				const float sx = win->bounds.x + win->cursor_x;
				const float sy = win->bounds.y + win->cursor_y;
				const auto& ac = settings::g_misc.m_accent_color;
				const auto tr = zdraw::rgba(ac.r, ac.g, ac.b, 0);
				zdraw::rect_filled_multi_color(sx, sy, sw_w * 0.15f, sw_h, tr, ac, ac, tr);
				zdraw::rect_filled(sx + sw_w * 0.15f, sy, sw_w * 0.70f, sw_h, ac);
				zdraw::rect_filled_multi_color(sx + sw_w * 0.85f, sy, sw_w * 0.15f, sw_h, ac, tr, tr, ac);
				win->cursor_y += sw_h + 4;
				win->line_height = 0;
			}
		}
		zui::end_group_box();
	}
}

void menu::draw_config()
{
	const auto [avail_w, avail_h] = zui::get_content_region_avail();
	const float left_w = avail_w * 0.42f;
	const float right_w = avail_w - left_w - 20.0f;

	if (this->m_config_list_dirty)
	{
		this->m_config_list = config_io::list();
		this->m_config_list_dirty = false;
		if (this->m_config_selected >= (int)this->m_config_list.size())
			this->m_config_selected = -1;
	}

	if (zui::begin_group_box("configs", left_w))
	{
		if (zui::begin_nested_window("##cfglist", left_w - 20.0f, 260.0f))
		{
			if (this->m_config_list.empty())
			{
				zui::text_colored("no configs found.", zdraw::rgba(80, 110, 95, 180));
			}
			else
			{
				const auto win = zui::detail::get_current_window();
				constexpr float item_h = 28.0f;
				constexpr float rnd = 5.0f;

				for (int i = 0; i < (int)this->m_config_list.size(); ++i)
				{
					const auto& name = this->m_config_list[i];
					const bool selected = (this->m_config_selected == i);

					float ax = win->bounds.x + win->cursor_x;
					float ay = win->bounds.y + win->cursor_y - win->scroll_y;

					zui::rect ir{ ax, ay, left_w - 24.0f, item_h };
					bool hov = zui::detail::mouse_hovered(ir);
					bool clicked = hov && zui::detail::mouse_clicked();

					if (clicked)
					{
						this->m_config_selected = i;
						this->m_config_name = name;
					}

					if (selected || hov)
						zhelper::rect_rounded(ax, ay, ir.w, item_h, rnd,
							selected ? zdraw::rgba(0, 200, 255, 25) : zdraw::rgba(0, 200, 255, 8));

					if (selected)
						zdraw::rect_filled(ax, ay + item_h - 2, ir.w, 2, zdraw::rgba(0, 200, 255, 180));

					auto [tw, th] = zdraw::measure_text(name);
					zdraw::text(ax + 10, ay + (item_h - th) * 0.5f, name,
						selected ? zdraw::rgba(0, 200, 255, 255) : zdraw::rgba(160, 195, 215, 200));

					win->cursor_y += item_h + 3;
				}
			}

			zui::end_nested_window();
		}

		zui::end_group_box();
	}

	zui::set_cursor_pos(left_w + 10.0f + zui::get_style().window_padding_x, zui::get_style().window_padding_y);

	if (zui::begin_group_box("manager", right_w))
	{
		const float btn_w = (right_w - 30.0f) * 0.5f;

		if (zui::button("create##cfg", btn_w, 26.0f))
		{
			int idx = 1;
			std::string new_name;
			do {
				new_name = "config_" + std::to_string(idx++);
			} while (std::find(this->m_config_list.begin(), this->m_config_list.end(), new_name) != this->m_config_list.end());
			config_io::save(new_name);
			this->m_config_list_dirty = true;
			this->m_config_msg = "created: " + new_name;
		}

		zui::same_line();

		if (zui::button("save##cfg", btn_w, 26.0f))
		{
			if (this->m_config_selected >= 0)
			{
				const auto& name = this->m_config_list[this->m_config_selected];
				config_io::save(name);
				this->m_config_msg = "saved: " + name;
			}
		}

		zui::spacing(6.0f);

		if (zui::button("load selected##cfg", right_w - 20.0f, 26.0f))
		{
			if (this->m_config_selected >= 0)
			{
				const auto& name = this->m_config_list[this->m_config_selected];
				this->m_config_msg = config_io::load(name) ? "loaded: " + name : "failed loading config";
			}
		}

		if (zui::button("delete selected##cfg", right_w - 20.0f, 26.0f))
		{
			if (this->m_config_selected >= 0)
			{
				const auto& name = this->m_config_list[this->m_config_selected];
				std::filesystem::remove(config_io::path(name));
				this->m_config_list_dirty = true;
				this->m_config_selected = -1;
				this->m_config_msg = "deleted: " + name;
			}
		}

		if (!this->m_config_msg.empty())
		{
			zui::spacing(4.0f);
			zui::text_colored(this->m_config_msg, zdraw::rgba(0, 190, 240, 180));
		}

		zui::end_group_box();
	}
}

void menu::draw_credits()
{
	const auto [avail_w, avail_h] = zui::get_content_region_avail();
	const auto& style = zui::get_style();
	const auto col_w = avail_w * 0.6f;
	const auto win = zui::detail::get_current_window();
	if (!win) return;

	{
		auto [tw, th] = zdraw::measure_text("megamind v2.3");
		zdraw::text(win->bounds.x + (avail_w - tw) * .5f, win->bounds.y + win->cursor_y,
			"megamind v2.3", zdraw::rgba(0, 200, 255, 255));
		win->cursor_y += th + 6;
	}
	{
		auto [sw, sh] = zdraw::measure_text("made by luta");
		zdraw::text(win->bounds.x + (avail_w - sw) * .5f, win->bounds.y + win->cursor_y,
			"made by luta", zdraw::rgba(80, 130, 100, 175));
		win->cursor_y += sh + 20;
		win->line_height = 0;
	}

	if (zui::begin_group_box("links", col_w))
	{
		const auto btn_w = col_w - style.window_padding_x * 2.0f;
		if (zui::button("join discord##cr", btn_w, 26.0f))
			ShellExecuteA(nullptr, "open", "https://discord.gg/7juxjTcA", nullptr, nullptr, SW_SHOWNORMAL);
		zui::separator();
		zui::text_colored("join dc if ur cool.", zdraw::rgba(60, 80, 70, 120));
		zui::end_group_box();
	}
}