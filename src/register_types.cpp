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
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		// Register types that will be used in runtime here.
		magnesium::register_types();
		experimental::register_types();
	}
	else if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
		// Register types that need to be available in the editor but not at runtime here.
		experimental::register_editor_types();
	}
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		experimental::unregister_types();
		magnesium::unregister_types();
	}
	else if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
		experimental::unregister_editor_types();
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT magnesium_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

		return init_obj.init();
	}
}