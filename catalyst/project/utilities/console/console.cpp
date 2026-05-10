#include <stdafx.hpp>
#include <Windows.h>
#include <string>
#include <format>
#include <cstdio>

namespace detail {

	struct rgb
	{
		int r, g, b;

		static rgb lerp(const rgb& a, const rgb& b, float t)
		{
			return {
				a.r + static_cast<int>((b.r - a.r) * t),
				a.g + static_cast<int>((b.g - a.g) * t),
				a.b + static_cast<int>((b.b - a.b) * t)
			};
		}
	};

	static constexpr auto k_reset{ "\033[0m" };

	// 💗 EXTREME BUBBLEGUM PINK
	static constexpr rgb k_accent_light{ 255, 120, 200 };
	static constexpr rgb k_accent_dark{ 255, 20, 147 };

	static constexpr rgb k_text{ 255, 200, 230 };
	static constexpr rgb k_dim{ 180, 60, 140 };

	static constexpr rgb k_error_light{ 255, 80, 140 };
	static constexpr rgb k_error_dark{ 200, 0, 90 };

	static constexpr rgb k_warn_light{ 255, 120, 180 };
	static constexpr rgb k_warn_dark{ 210, 40, 120 };

	static constexpr rgb k_success_light{ 255, 150, 210 };
	static constexpr rgb k_success_dark{ 255, 60, 160 };

	static std::string colorize(char c, const rgb& col)
	{
		return std::format("\033[38;2;{};{};{}m{}", col.r, col.g, col.b, c);
	}

	static std::string gradient_text(std::string_view text, const rgb& from, const rgb& to)
	{
		std::string result;
		result.reserve(text.size() * 20);

		const auto len = text.size();
		for (std::size_t i = 0; i < len; ++i)
		{
			float t = len > 1 ? (float)i / (len - 1) : 0.f;
			result += colorize(text[i], rgb::lerp(from, to, t));
		}
		return result;
	}

	static std::string col(const rgb& c)
	{
		return std::format("\033[38;2;{};{};{}m", c.r, c.g, c.b);
	}

} // namespace detail


bool console::initialize(std::string_view title)
{
	// ✅ FIX UTF-8 (keine kaputten Zeichen mehr)
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	m_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	m_input_handle = ::GetStdHandle(STD_INPUT_HANDLE);

	DWORD mode{};
	if (::GetConsoleMode(m_handle, &mode))
		::SetConsoleMode(m_handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	// Font
	CONSOLE_FONT_INFOEX cfi{ .cbSize = sizeof(CONSOLE_FONT_INFOEX) };
	if (::GetCurrentConsoleFontEx(m_handle, FALSE, &cfi))
	{
		wcscpy_s(cfi.FaceName, L"Consolas");
		cfi.dwFontSize = { 0, 16 };
		::SetCurrentConsoleFontEx(m_handle, FALSE, &cfi);
	}

	// 💗 Pink Background + Text
	CONSOLE_SCREEN_BUFFER_INFOEX csbi{ .cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) };
	if (::GetConsoleScreenBufferInfoEx(m_handle, &csbi))
	{
		csbi.ColorTable[0] = RGB(25, 0, 20);     // background
		csbi.ColorTable[7] = RGB(255, 180, 220); // foreground
		::SetConsoleScreenBufferInfoEx(m_handle, &csbi);
	}

	// Window
	if (const auto hwnd = ::GetConsoleWindow())
	{
		auto style = ::GetWindowLongW(hwnd, GWL_STYLE);
		style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
		::SetWindowLongW(hwnd, GWL_STYLE, style);

		::SetWindowLongW(hwnd, GWL_EXSTYLE,
			::GetWindowLongW(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		::SetLayeredWindowAttributes(hwnd, 0, 245, LWA_ALPHA);

		SMALL_RECT size{ 0, 0, 90, 30 };
		::SetConsoleWindowInfo(m_handle, TRUE, &size);

		COORD buffer{ 91, 2000 };
		::SetConsoleScreenBufferSize(m_handle, buffer);
	}

	// Cursor off
	CONSOLE_CURSOR_INFO cursor{ .dwSize = 1, .bVisible = FALSE };
	::SetConsoleCursorInfo(m_handle, &cursor);

	// Input
	DWORD input_mode{};
	if (::GetConsoleMode(m_input_handle, &input_mode))
		::SetConsoleMode(m_input_handle,
			input_mode | ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

	::FlushConsoleInputBuffer(m_input_handle);

	// 😈 Title mit Smiley
	std::string new_title = std::string(title) + " megamind v.5.4";
	SetConsoleTitleA(new_title.c_str());

	std::printf("\n");
	this->emit(level::success, "initialized");

	return m_handle != nullptr;
}


void console::emit(level lvl, const std::string& message) const
{
	using namespace detail;

	rgb from, to, bracket, text;
	const char* icon = "";

	switch (lvl)
	{
	case level::error:
		from = k_error_light; to = k_error_dark;
		bracket = k_error_dark; text = k_error_light;
		icon = "[-]";
		break;

	case level::warn:
		from = k_warn_light; to = k_warn_dark;
		bracket = k_warn_dark; text = k_warn_light;
		icon = "[!]";
		break;

	case level::success:
		from = k_success_light; to = k_success_dark;
		bracket = k_success_dark; text = k_success_light;
		icon = "[+]";
		break;

	default:
		from = k_accent_light; to = k_accent_dark;
		bracket = k_dim; text = k_text;
		icon = "[*]";
		break;
	}

	const auto label = gradient_text("megamind", from, to);

	const auto col_bracket = col(bracket);
	const auto col_text = col(text);
	const auto col_dim = col(k_dim);

	std::printf("  %s[%s%s%s]%s %s %s%s%s\n",
		col_dim.c_str(),
		col_bracket.c_str(),
		label.c_str(),
		k_reset,
		k_reset,
		icon,
		col_text.c_str(),
		message.c_str(),
		k_reset
	);
}


void console::wait_and_exit() const
{
	using namespace detail;

	const auto col_err = col(k_error_light);

	std::printf("\n  %sdo not close this...%s", col_err.c_str(), k_reset);
	std::getchar();
	std::exit(1);
}