extends Node2D

var ctx:= context.new()
var font := ThemeDB.fallback_font

func _ready() -> void:
	MgFsmMachine.changed.connect(on_state_changed)
	MgFsmMachine.travel_to(ctx, warmup)
	MgFsmMachine.update(ctx, 1.2)

func _process(delta: float) -> void:
	MgFsmMachine.update(ctx, delta)
	queue_redraw()

func on_state_changed(from: Object, to: Object, _p_context: Object):
	print("changed signal from: %s to: %s" % [MgFsmMachine.get_state_name(from), MgFsmMachine.get_state_name(to)])

func _draw() -> void:
	MgFsmMachine.call_on_state(ctx, &"draw", ctx, self, font)

class warmup:
	const state_name := &"WarmUp"
	static func enter(c: context):
		print("enter warmup")
		c.time = 3.0
		c.counter = 1
	static func orchestrate(c: context):
		if c.time<=0:
			MgFsmMachine.travel_to(c, state_init)
	static func update(c:context, dt):
		c.time-=dt
	static func exit(_c):
		print("exit warmup")
	static func draw(ctx: context, drawer: Node2D, font: Font) -> void:
		drawer.draw_string(font, Vector2(4,20), "Countdown: %.0f" % ctx.time)
