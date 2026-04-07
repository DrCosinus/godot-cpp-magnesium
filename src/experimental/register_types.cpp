#include "register_types.hpp"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>
#include <godot_cpp/classes/resource_importer.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "importers/palettized_image_importer.hpp"
#include "plugins/palettized_image_editor_plugin.hpp"
#include "readers/palettized_image_loader.hpp"
#include "resources/palettized_image.hpp"
#include "resources/palettized_material.hpp"
#include "writers/palettized_image_saver.hpp"

using namespace godot;

namespace experimental
{
	static Ref<PalettizedImageLoader> palettized_image_loader;
	static Ref<PalettizedImageSaver> palettized_image_saver;
	static Ref<PalettizedImageImporter> palettized_image_importer;

	void register_types()
	{
		print_line("--- Experimental types registered ---");
		GDREGISTER_CLASS(PalettizedMaterial);
		GDREGISTER_CLASS(PalettizedImageImporter);
		GDREGISTER_CLASS(PalettizedImage);

		GDREGISTER_CLASS(PalettizedImageEditorPlugin);
		EditorPlugins::add_by_type<PalettizedImageEditorPlugin>();

		GDREGISTER_CLASS(PalettizedImageLoader);
		palettized_image_loader = memnew(PalettizedImageLoader);
		ResourceLoader::get_singleton()->add_resource_format_loader(palettized_image_loader);

		GDREGISTER_CLASS(PalettizedImageSaver);
		palettized_image_saver = memnew(PalettizedImageSaver);
		ResourceSaver::get_singleton()->add_resource_format_saver(palettized_image_saver);
	}

	void unregister_types()
	{
		print_line("--- Experimental types unregistered ---");
		auto& resource_saver = *ResourceSaver::get_singleton();
		resource_saver.remove_resource_format_saver(palettized_image_saver);

		auto& resource_loader = *ResourceLoader::get_singleton();
		resource_loader.remove_resource_format_loader(palettized_image_loader);

		EditorPlugins::remove_by_type<PalettizedImageEditorPlugin>();
	}
} //namespace experimental
