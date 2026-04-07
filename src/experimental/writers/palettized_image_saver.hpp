#pragma once

#include <godot_cpp/classes/resource_format_saver.hpp>

namespace experimental
{
	class PalettizedImageSaver : public godot::ResourceFormatSaver
	{
		GDCLASS(PalettizedImageSaver, godot::ResourceFormatSaver);
	protected:
		static void _bind_methods() {}

	public:
		PalettizedImageSaver() = default;
		~PalettizedImageSaver() override = default;

	bool _recognize(const godot::Ref<godot::Resource> &p_resource) const override;
	godot::Error _save(const godot::Ref<godot::Resource> &p_resource, const godot::String &p_path, uint32_t p_flags) override;
	// virtual Error _set_uid(const String &p_path, int64_t p_uid);
	godot::PackedStringArray _get_recognized_extensions(const godot::Ref<godot::Resource> &p_resource) const override;
	// virtual bool _recognize_path(const Ref<Resource> &p_resource, const String &p_path) const;
	// godot::Error _save(const godot::Ref<godot::Resource>& p_resource, const godot::String& p_path, int32_t p_flags) const override;
	};
} //namespace experimental