#include "state.h"
#include "context.h"
#include "godot_cpp/classes/script.hpp"

using namespace godot;

namespace magnesium::fsm
{
	StringName state::get_class_name() const
	{
		Script* script = static_cast<Script*>(static_cast<Object*>(get_script()));
		return script ? script->get_global_name() : StringName{ get_class() };
		// print_line(vformat("Class name: %s", obj->get_class()));
	}

	void state::_bind_methods()
	{
		ClassDB::bind_method(D_METHOD("get_class_name"), &state::get_class_name);
		GDVIRTUAL_BIND(enter, "context");
		GDVIRTUAL_BIND(exit, "context");
		GDVIRTUAL_BIND(update, "context", "delta");
	}
} //namespace magnesium::fsm