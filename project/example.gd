extends Node

var mac: example_machine
var ctx: example_context
var state_init: example_state_init

func _ready() -> void:
	var example := ExampleClass.new()
	example.print_type(example)

	ctx = example_context.new()
	state_init = example_state_init.new()
	
	mac = example_machine.new()
	mac.changed.connect(on_state_changed)
	mac.travel_to(ctx,state_init)
	
func _process(delta: float) -> void:
	mac.update(ctx, delta)

func on_state_changed(_p_state: state, _p_context: context):
	print("changed signal: %s" % _p_state.get_class())
