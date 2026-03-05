class_name state_init

static func enter(p_context: context) -> void:
	print("enter state_init");
	p_context.counter  = 0
	p_context.time = 0.0

static func update(p_context: context, delta: float) -> void:
	p_context.counter += 1
	p_context.time += delta

static func draw(ctx: context, drawer: Node2D, font: Font) -> void:
	drawer.draw_string(font, Vector2(4,20), "Frame count: %d" % ctx.counter)
	drawer.draw_string(font, Vector2(4,20+16), "Timer: %.1f" % ctx.time)
	drawer.draw_string(font, Vector2(4,20+32), "FPS: %4.2f" % [float(ctx.counter) / ctx.time])
