#include "state.h"
#include "context.h"

using namespace godot;

namespace magnesium::fsm
{
	void state::_bind_methods()
	{
		// GDVIRTUAL_BIND(enter, "context");
		ClassDB::add_virtual_method("state", MethodInfo("enter", PropertyInfo(Variant::OBJECT, "context")), { "context" });
		ClassDB::add_virtual_method("state", MethodInfo("exit", PropertyInfo(Variant::OBJECT, "context")), { "context" });
		ClassDB::add_virtual_method("state", MethodInfo("update", PropertyInfo(Variant::OBJECT, "context"), PropertyInfo(Variant::FLOAT, "delta")), { "context", "delta" });
		// ClassDB::bind_method(D_METHOD("enter", "context"), &state::enter);
		// ClassDB::bind_method(D_METHOD("exit", "context"), &state::exit);
		// ClassDB::bind_method(D_METHOD("update", "context", "delta"), &state::update);
	}

	// void state::enter(context* context)
	// {
	// }

	// void state::exit(context* context)
	// {
	// }

	// void state::update(context* context, float delta)
	// {
	// }
} //namespace magnesium::fsm