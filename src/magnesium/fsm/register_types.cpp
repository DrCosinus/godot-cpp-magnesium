#include "register_types.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"

#include "machine.h"
#include "magnesium/instance_provider.hpp"

namespace magnesium::fsm
{
	void register_types()
	{
		GDREGISTER_CLASS(machine);
		InstanceProvider<machine>::create_instance();
		godot::Engine::get_singleton()->register_singleton("MgFsmMachine", InstanceProvider<machine>::instance());
	}

	void unregister_types()
	{
		godot::Engine::get_singleton()->unregister_singleton("MgFsmMachine");
		InstanceProvider<machine>::release_instance();
	}
} // namespace magnesium::fsm
