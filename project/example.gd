extends Node2D

var ctx:= example_context.new()
var font := ThemeDB.fallback_font

func _ready() -> void:
	MgFsmMachine.changed.connect(on_state_changed)
	MgFsmMachine.travel_to(ctx, totolito)
	MgFsmMachine.update(ctx, 1.2)
	MgFsmMachine.travel_to(ctx, example_state_init)
	print("> %s" % (totolito as GDScript).source_code)

func _process(delta: float) -> void:
	MgFsmMachine.update(ctx, delta)
	queue_redraw()

func on_state_changed(from: Object, to: Object, _p_context: Object):
	print("changed signal from: %s to : %s" % [MgFsmMachine.get_state_name(from), MgFsmMachine.get_state_name(to)])

func _draw() -> void:
	draw_string(font, Vector2(4,20), "Frame count: %d" % ctx.counter)
	draw_string(font, Vector2(4,20+16), "Timer: %f" % ctx.time)
	draw_string(font, Vector2(4,20+32), "Timer: %4.2f" % [float(ctx.counter) / ctx.time])

class totolito:
	static func enter(_c):
		print("enter totolito")
	static func update(_c, dt):
		print("update totolito %f" % dt)
	static func exit(_c):
		print("exit totolito")
