class_name example_state_init extends state

func enter(p_context: Object) -> void:
	p_context = p_context as example_context
	print("enter state_init");
	p_context.counter  = 0
	p_context.time = 0.0

func update(p_context: Object, delta: float) -> void:
	p_context = p_context as example_context
	p_context.counter += 1
	p_context.time += delta
