#pragma once

#include "../helpers/logger.hpp"
#include <godot_cpp/classes/resource_format_loader.hpp>

namespace experimental
{
	class PalettizedImageLoader : public godot::ResourceFormatLoader
	{
		GDCLASS(PalettizedImageLoader, godot::ResourceFormatLoader);

		inline static Logger<false> Log{ "PalImgLoader" };

	protected:
		static void _bind_methods() {}

	public:
		PalettizedImageLoader() = default;
		~PalettizedImageLoader() override = default;

		godot::PackedStringArray _get_recognized_extensions() const override;
		bool _handles_type(const godot::StringName& p_type) const override;
		godot::Variant _load(const godot::String& p_path, const godot::String& p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const override;
	};
} //namespace experimental