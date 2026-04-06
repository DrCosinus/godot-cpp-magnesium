#include "register_types.hpp"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>
#include <godot_cpp/classes/resource_importer.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>

#include "indexed_material2d.hpp"
#include "plugins/palettized_image_editor_plugin.hpp"
#include "importers/palettized_image_importer.hpp"
#include "resources/palettized_image.hpp"
#include "resources/palettized_image_loader.hpp"
#include "resources/palettized_image_saver.hpp"

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
		print_line("--- Palettized Image Importer initialized (experimental) ---");
		GDREGISTER_CLASS(PalettizedImageImporter);
		GDREGISTER_CLASS(PalettizedImageEditorPlugin);
		GDREGISTER_CLASS(PalettizedImage);
		GDREGISTER_CLASS(PalettizedImageLoader);
		GDREGISTER_CLASS(PalettizedImageSaver);
		EditorPlugins::add_by_type<PalettizedImageEditorPlugin>();
		ResourceLoader::get_singleton()->add_resource_format_loader(memnew(PalettizedImageLoader));
		ResourceSaver::get_singleton()->add_resource_format_saver(memnew(PalettizedImageSaver));
	}

	void unregister_editor_types()
	{
	}

	void unregister_types()
	{
	}
} //namespace experimental
