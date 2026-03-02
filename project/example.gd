extends Node2D

var mac: example_machine
var ctx: example_context
var state_init: example_state_init
var font := ThemeDB.fallback_font

func _ready() -> void:
	var example := ExampleClass.new()
	example.print_type(example)

	ctx = example_context.new()
	state_init = example_state_init.new()
	
	mac = example_machine.new()
	mac.changed.connect(on_state_changed)
	print("before %s" % [ mac.current_state.get_class_name() if mac.current_state else &"None" ])
	print("try_call_method// null object ", MgUtils.try_call_method(null, &"hello_world"))
	print("try_call_method// ok ", MgUtils.try_call_method(ctx, &"hello_world"))
	print("try_call_method// invalid method name ", MgUtils.try_call_method(ctx, &"hello_world2"))
	mac.travel_to(ctx,state_init)
	print("after %s" % mac.current_state.get_class_name())

func _process(delta: float) -> void:
	mac.update(ctx, delta)
	queue_redraw()

func on_state_changed(_p_state: state, _p_context: Object):
	print("changed signal: %s" % _p_state.get_class())

func _draw() -> void:
	draw_string(font, Vector2(4,20), "Frame count: %d" % ctx.counter)
	draw_string(font, Vector2(4,20+16), "Timer: %f" % ctx.time)
	
