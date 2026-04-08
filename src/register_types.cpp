#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "experimental/register_types.hpp"
#include "magnesium/register_types.hpp"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	print_line(vformat("Initializing GDExtension types at level: %d", p_level));
	// must wait until editor level to register editor-only types, and so LEVEL_SCENE ones should wait too, and also to ensure that
	switch (p_level)
	{
		case MODULE_INITIALIZATION_LEVEL_EDITOR:
			// Register types that will be used in runtime here.
			experimental::register_editor_types();
			break;
		case MODULE_INITIALIZATION_LEVEL_SCENE:
			// Register types that will be used in runtime here.
			magnesium::register_types();
			experimental::register_scene_types();
			break;
		default:
			break;
	}
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level)
{
	print_line(vformat("Uninitializing GDExtension types at level: %d", p_level));
	switch(p_level)
	{
		case MODULE_INITIALIZATION_LEVEL_EDITOR:
			// Unregister types that were registered for the editor here.
			experimental::unregister_editor_types();
			break;
		case MODULE_INITIALIZATION_LEVEL_SCENE:
			// Unregister types that were registered for the scene here.
			experimental::unregister_scene_types();
			magnesium::unregister_types();
			break;
		default:
			break;
	}
	print_line(vformat("GDExtension types uninitialized at level: %d.", p_level));
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT magnesium_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}