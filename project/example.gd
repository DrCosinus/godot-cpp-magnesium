extends Node2D

var ctx:= example_context.new()
#var state_init: example_state_init
var font := ThemeDB.fallback_font

func _ready() -> void:
	var example := ExampleClass.new()
	example.print_type(example_context)

	#ctx = example_context.new()
	#state_init = example_state_init.new()
	
	MgFsmMachine.changed.connect(on_state_changed)
	# nice to have: MgFsmMachine.get_current_state_name(ctx)

	test(example_context)

	print("before %s" % MgFsmMachine.get_current_state_name(ctx))
	MgFsmMachine.travel_to(ctx, example_state_init)
	#print( (example_state_init as Script).get_class())
	print("after %s" % MgFsmMachine.get_current_state_name(ctx))

func try_call_method_test():
	MgUtils.try_call_method(ctx, &"hello_world", "Sam")

func test(script: GDScript) -> void:
	print( "testouille %s" % script.get_global_name())

func _process(delta: float) -> void:
	MgFsmMachine.update(ctx, delta)
	queue_redraw()

func on_state_changed(_p_state: Object, _p_context: Object):
	print("changed signal: %s" % MgFsmMachine.get_state_name(_p_state))

func _draw() -> void:
	draw_string(font, Vector2(4,20), "Frame count: %d" % ctx.counter)
	draw_string(font, Vector2(4,20+16), "Timer: %f" % ctx.time)
	
