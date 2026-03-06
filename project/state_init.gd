class_name state_init

static func enter(c: context) -> void:
	c.check_sequence_index(6, "enter idle")
	c.counter  = 0
	c.time = 0.0

static func update(c: context, delta: float) -> void:
	if c.counter == 0:
		c.check_sequence_index(8, "first update idle")
	c.counter += 1
	c.time += delta

static func draw(ctx: context, drawer: Node2D, font: Font) -> void:
	drawer.draw_string(font, Vector2(4,20), "Frame count: %d" % ctx.counter)
	drawer.draw_string(font, Vector2(4,20+16), "Timer: %.1f" % ctx.time)
	drawer.draw_string(font, Vector2(4,20+32), "FPS: %4.2f" % [float(ctx.counter) / ctx.time])
