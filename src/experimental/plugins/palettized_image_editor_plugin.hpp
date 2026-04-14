#pragma once

#include "../helpers/logger.hpp"
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace experimental
{
	class PalettizedImageImporter;

	class PalettizedImageEditorPlugin : public godot::EditorPlugin
	{
		GDCLASS(PalettizedImageEditorPlugin, EditorPlugin);

		inline static Logger<false> Log{ "PalImgEditorPlugin" };

	protected:
		static void _bind_methods()
		{
		}

	public:
		godot::String _get_plugin_name() const override
		{
			return "Palettized Image Importer";
		}
		godot::String get_plugin_version() const
		{
			return "1.0";
		}
		void _enter_tree() override;
		void _exit_tree() override;

	private:
		godot::Ref<PalettizedImageImporter> importer;
	};
} //namespace experimental