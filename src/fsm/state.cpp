#include "state.h"
#include "context.h"

using namespace godot;

namespace magnesium::fsm
{
	void state::_bind_methods()
	{
		GDVIRTUAL_BIND(enter, "context");
		GDVIRTUAL_BIND(exit, "context");
		GDVIRTUAL_BIND(update, "context", "delta");
	}
} //namespace magnesium::fsm