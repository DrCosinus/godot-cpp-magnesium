#include "palettized_image_editor_plugin.hpp"

#include "../importers/palettized_image_importer.hpp"

using namespace godot;

namespace experimental
{
	void PalettizedImageEditorPlugin::_enter_tree()
	{
		print_line("PalettizedImageEditorPlugin entered tree");
		add_import_plugin(memnew(PalettizedImageImporter));
	}

	void PalettizedImageEditorPlugin::_exit_tree()
	{
		print_line("PalettizedImageEditorPlugin exited tree");
	}
} //namespace experimental