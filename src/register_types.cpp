#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

// magnesium includes
#include "example_class.h"
#include "fsm/context.h"
#include "fsm/machine.h"
#include "fsm/state.h"
#include "utils/utils.h"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
	GDREGISTER_CLASS(ExampleClass);
	GDREGISTER_CLASS(magnesium::fsm::state);
	GDREGISTER_CLASS(magnesium::fsm::context);
	GDREGISTER_CLASS(magnesium::fsm::machine);
	GDREGISTER_CLASS(magnesium::utils::utils);
	using namespace magnesium::utils;
	InstanceProvider<utils>::create_instance();
	Engine::get_singleton()->register_singleton("MgUtils", InstanceProvider<utils>::instance());
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
	using namespace magnesium::utils;
	Engine::get_singleton()->unregister_singleton("MgUtils");
	InstanceProvider<utils>::destroy_instance();
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