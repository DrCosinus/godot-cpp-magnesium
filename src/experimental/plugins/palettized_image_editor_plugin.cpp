#include "palettized_image_editor_plugin.hpp"

#include "../importers/palettized_image_importer.hpp"

using namespace godot;

namespace experimental
{
	void PalettizedImageEditorPlugin::_enter_tree()
	{
		Log.print("PalettizedImageEditorPlugin entered tree");
		add_import_plugin(memnew(PalettizedImageImporter));
	}

	void PalettizedImageEditorPlugin::_exit_tree()
	{
		Log.print("PalettizedImageEditorPlugin exited tree");
	}
} //namespace experimental