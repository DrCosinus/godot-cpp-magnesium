#include "indexed_image_editor_plugin.hpp"
#include "indexed_image_importer.hpp"

using namespace godot;

void IndexedImageEditorPlugin::_enter_tree()
{
	print_line("IndexedImageEditorPlugin entered tree");
	add_import_plugin(memnew(IndexedImageImporter));
}

void IndexedImageEditorPlugin::_exit_tree()
{
	print_line("IndexedImageEditorPlugin exited tree");
}