#include <stdafx.hpp>

namespace systems {

	void view::update()
	{
		this->m_matrix = g::memory.read<math::matrix4x4>(g::offsets.view_matrix);

		// dwViewRender = 0x232DBB8 (Build 14157, April 2026)
		// Try pattern first, then fall back to known offset
		static std::uintptr_t view_render_ptr = 0;
		static bool view_render_searched = false;

		if (!view_render_searched)
		{
			// Pattern for CViewRender instance pointer
			const auto pat = g::memory.find_pattern(g::modules.client, "48 8B 05 ? ? ? ? 48 8B 80 ? ? ? ? 48 85 C0 74 ? F3 0F 10 88");
			if (pat)
			{
				view_render_ptr = g::memory.resolve_rip(pat);
			}
			else
			{
				// fallback: find via vtable
				view_render_ptr = g::memory.find_vtable_instance(g::modules.client, "CViewRender");
			}
			view_render_searched = true;
		}

		if (!view_render_ptr)
		{
			this->m_origin = { k_invalid, k_invalid, k_invalid };
			this->m_angles = { k_invalid, k_invalid, k_invalid };
			this->m_fov = k_invalid;
			return;
		}

		const auto view_render = g::memory.read<std::uintptr_t>(view_render_ptr);
		if (!view_render)
		{
			this->m_origin = { k_invalid, k_invalid, k_invalid };
			this->m_angles = { k_invalid, k_invalid, k_invalid };
			this->m_fov = k_invalid;
			return;
		}

		// Camera state sits at offset 0x10 into the view_render data block
		const auto view = view_render + 0x10;

		this->m_origin = g::memory.read<math::vector3>(view + 0x0);
		this->m_angles = g::memory.read<math::vector3>(view + 0xc);
		this->m_fov = g::memory.read<float>(view + 0x18);

		// Sanity check
		if (!std::isfinite(this->m_origin.x) || !std::isfinite(this->m_angles.x))
		{
			this->m_origin = { k_invalid, k_invalid, k_invalid };
			this->m_angles = { k_invalid, k_invalid, k_invalid };
			this->m_fov = k_invalid;
		}
	}

	math::vector2 view::project(const math::vector3& world_pos)
	{
		const auto& m = this->m_matrix;

		if (m[3][3] == 0.0f)
		{
			return { this->k_invalid, this->k_invalid };
		}

		const auto w = m[3][0] * world_pos.x + m[3][1] * world_pos.y + m[3][2] * world_pos.z + m[3][3];

		if (w < 0.01f)
		{
			return { this->k_invalid, this->k_invalid };
		}

		const auto x = m[0][0] * world_pos.x + m[0][1] * world_pos.y + m[0][2] * world_pos.z + m[0][3];
		const auto y = m[1][0] * world_pos.x + m[1][1] * world_pos.y + m[1][2] * world_pos.z + m[1][3];

		const auto display = zdraw::get_display_size();
		const auto inv_w = 1.0f / w;

		return
		{
			display.first * 0.5f * (1.0f + x * inv_w),
			display.second * 0.5f * (1.0f - y * inv_w)
		};
	}

} // namespace systems