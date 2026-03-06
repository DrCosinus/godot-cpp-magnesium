# Magnesium FSM

## State

States are currently pointer to godot::Script.
States are "stateless" :wink:, so all methods but be `static` ones and no inheritance is needed.

Methods are called thanks to duck typing, so they are optional.
- can_enter
- enter
- orchestrate
- update
- exit

### can_enter() -> bool

an inactive state can declare itself non enterable by returning `false`,
`travel_to` method of `MgFsmMachine` singleton will check for it and will return `false` if `to_state`'s `can_enter` method exist and returns false.

### enter(context: Object) -> void

> [!IMPORTANT] Question: merge can_enter into enter ???

### orchestrate(context: Object) -> Script

### update(context: Object, delta: float) -> void

### exit(context:Object) -> void

## Current state storage

## Machine

MgFsmMachine is a singleton of the class machine.

Machine holds a map of machine instances' current states regarding the pointer to the context object.

Machine instances are defined by their context. So different machines can not share their context.

The machine keeps track of differents 'instances' of the machine with a map (key is the context, value is the current_state Script*)




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