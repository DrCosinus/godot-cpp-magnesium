#include "register_types.hpp"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>
#include <godot_cpp/classes/resource_importer.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "helpers/logger.hpp"
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

	static Logger<true> Log{ "RegisterTypes" };

	void register_editor_types()
	{
		Log.print("--- Experimental editor types registered ---");

		GDREGISTER_CLASS(PalettizedImageImporter);
		GDREGISTER_CLASS(PalettizedImageEditorPlugin);

		Log.print("Registering PalettizedImageLoader and PalettizedImageSaver as resource format loader/saver...");

		EditorPlugins::add_by_type<PalettizedImageEditorPlugin>();

		Log.print("Instantiating PalettizedImageLoader and adding it to ResourceLoader...");

		palettized_image_loader.instantiate();
		ResourceLoader::get_singleton()->add_resource_format_loader(palettized_image_loader);

		Log.print("Instantiating PalettizedImageSaver and adding it to ResourceSaver...");

		palettized_image_saver.instantiate();
		ResourceSaver::get_singleton()->add_resource_format_saver(palettized_image_saver);

		Log.print("Experimental editor types registered successfully.");
	}

	void unregister_editor_types()
	{
		Log.print("--- Experimental editor types unregistered ---");

		ResourceSaver::get_singleton()->remove_resource_format_saver(palettized_image_saver);
		palettized_image_saver.unref();

		ResourceLoader::get_singleton()->remove_resource_format_loader(palettized_image_loader);
		palettized_image_loader.unref();

		EditorPlugins::remove_by_type<PalettizedImageEditorPlugin>();
	}

	void register_scene_types()
	{
		Log.print("--- Experimental scene types registered ---");

		GDREGISTER_CLASS(PalettizedMaterial);
		GDREGISTER_CLASS(PalettizedImage);
		GDREGISTER_CLASS(PalettizedImageLoader);
		GDREGISTER_CLASS(PalettizedImageSaver);
	}

	void unregister_scene_types()
	{
		Log.print("--- Experimental scene types unregistered ---");
		// No need to unregister individual classes, as Godot will handle that when the library is unloaded.
	}
} //namespace experimental
