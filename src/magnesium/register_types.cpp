#include "register_types.hpp"

#include "fsm/register_types.hpp"
#include "utils.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"

#include "instance_provider.hpp"

using namespace godot;

namespace magnesium
{
	void register_types()
	{
		print_line("--- Magnesium types registered ---");
		fsm::register_types();
		{
			GDREGISTER_CLASS(utils);
			InstanceProvider<utils>::create_instance();
			godot::Engine::get_singleton()->register_singleton("MgUtils", InstanceProvider<utils>::instance());
		}
	}
	void unregister_types()
	{
		print_line("--- Magnesium types unregistered ---");
		{
			godot::Engine::get_singleton()->unregister_singleton("MgUtils");
			InstanceProvider<utils>::release_instance();
		}
		fsm::unregister_types();
	}
} //namespace magnesium