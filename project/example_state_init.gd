class_name example_state_init

static func enter(p_context: example_context) -> void:
	#p_context = p_context as example_context
	print("enter state_init");
	p_context.counter  = 0
	p_context.time = 0.0

static func update(p_context: example_context, delta: float) -> void:
	#p_context = p_context as example_context
	p_context.counter += 1
	p_context.time += delta
