#include "context.h"

using namespace godot;

namespace magnesium::fsm
{
	void context::_bind_methods()
	{
		// ClassDB::class_set_property
		ClassDB::bind_method(D_METHOD("get_value"), &context::get_value); 
		ClassDB::bind_method(D_METHOD("set_value", "value"), &context::set_value);
	}
} //namespace magnesium::fsm