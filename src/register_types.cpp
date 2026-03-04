#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

// magnesium includes
#include "example_class.h"
#include "fsm/context.h"
#include "fsm/machine.h"
#include "fsm/state.h"
#include "magnesium/instance_provider.hpp"
#include "magnesium/utils.h"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
	GDREGISTER_CLASS(ExampleClass);

	{
		using namespace magnesium;
		{
			using namespace fsm;
			GDREGISTER_CLASS(machine);
			InstanceProvider<machine>::create_instance();
			Engine::get_singleton()->register_singleton("MgFsmMachine", InstanceProvider<machine>::instance());
		}
		{
			GDREGISTER_CLASS(utils);
			InstanceProvider<utils>::create_instance();
			Engine::get_singleton()->register_singleton("MgUtils", InstanceProvider<utils>::instance());
		}
	}
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
	{
		using namespace magnesium;
		{
			Engine::get_singleton()->unregister_singleton("MgUtils");
			InstanceProvider<utils>::release_instance();
		}
		{
			using namespace fsm;
			Engine::get_singleton()->unregister_singleton("MgFsmMachine");
			InstanceProvider<machine>::release_instance();
		}
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
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}