#include "register_types.hpp"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>
#include <godot_cpp/classes/resource_importer.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "indexed_image_editor_plugin.hpp"
#include "indexed_image_importer.hpp"
#include "indexed_material2d.hpp"

using namespace godot;

namespace experimental
{
	void register_types()
	{
		print_line("--- IndexedMaterial2D registered (experimental) ---");
		GDREGISTER_CLASS(IndexedMaterial2D);
	}

	void register_editor_types()
	{
		print_line("--- IndexedImage Importer initialized (experimental) ---");
		GDREGISTER_CLASS(IndexedImageImporter);
		GDREGISTER_CLASS(IndexedImageEditorPlugin);
		EditorPlugins::add_by_type<IndexedImageEditorPlugin>();
	}

	void unregister_editor_types()
	{
	}

	void unregister_types()
	{
	}
} //namespace experimental
