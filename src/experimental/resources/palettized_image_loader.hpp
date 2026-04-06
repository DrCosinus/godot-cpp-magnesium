#pragma once

#include <godot_cpp/classes/resource_format_loader.hpp>

namespace experimental
{
	class PalettizedImageLoader : public godot::ResourceFormatLoader
	{
		GDCLASS(PalettizedImageLoader, godot::ResourceFormatLoader);
	protected:
		static void _bind_methods() {}

	public:
		PalettizedImageLoader() = default;
		~PalettizedImageLoader() override = default;

		godot::PackedStringArray _get_recognized_extensions() const override;
		// virtual bool _recognize_path(const String &p_path, const StringName &p_type) const;
		bool _handles_type(const godot::StringName& p_type) const override;
		// virtual String _get_resource_type(const String &p_path) const;
		// virtual String _get_resource_script_class(const String &p_path) const;
		// virtual int64_t _get_resource_uid(const String &p_path) const;
		// virtual PackedStringArray _get_dependencies(const String &p_path, bool p_add_types) const;
		// virtual Error _rename_dependencies(const String &p_path, const Dictionary &p_renames) const;
		// virtual bool _exists(const String &p_path) const;
		// virtual PackedStringArray _get_classes_used(const String &p_path) const;
		godot::Variant _load(const godot::String& p_path, const godot::String& p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const override;
	};
} //namespace experimental