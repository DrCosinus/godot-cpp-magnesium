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
	// static IndexedImageEditorPlugin *editor_plugin{ nullptr };

	void register_types()
	{
		GDREGISTER_CLASS(IndexedMaterial2D);
		print_line_rich("[color=grey]--- IndexedMaterial2D registered (experimental) --- [/color]");
	}

	void register_editor_types()
	{
		GDREGISTER_CLASS(IndexedImageImporter);
		GDREGISTER_CLASS(IndexedImageEditorPlugin);
		EditorPlugins::add_by_type<IndexedImageEditorPlugin>();
		print_line_rich("[color=grey]--- IndexedImage Importer initialized (experimental) --- [/color]");
	}

	void unregister_editor_types()
	{
	}

	void unregister_types()
	{
	}
} //namespace experimental
