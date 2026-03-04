
# possible current state storage

## Current State in context without inheritance (duck typing)

The context object could have a field of type `state` named `current_state` (checked at some point by the machine).

## Global map of machines' current states

The machine could keep track of differents instances of the machine with a map (key is the context, value is the current_state)


# misc
```cpp
		Variant var = p_context->get("current_state");
		if (var.get_type() == Variant::OBJECT)
		{
			Object* obj = static_cast<Object*>(var);
			if (obj->is_class("state"))
			{
				print_line(vformat("Machine update, current state: '%s'", get_state_name(obj)));
			}
			else
			{
				print_line(vformat("Warning: current state '%s' is not of type 'state'", get_state_name(obj)));
			}
		}
		else
		{
			print_line("Machine update, current state: is not an object or does not exist");
		}
```