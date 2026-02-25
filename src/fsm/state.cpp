#include "state.h"
#include "context.h"

using namespace godot;

namespace magnesium::fsm
{
	void state::_bind_methods()
	{
		ClassDB::add_virtual_method("state", MethodInfo("enter", PropertyInfo(Variant::OBJECT, "context")));
		ClassDB::bind_method(D_METHOD("exit", "context"), &state::exit);
		ClassDB::bind_method(D_METHOD("update", "context", "delta"), &state::update);
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