extends Node


func _ready() -> void:
	var example := ExampleClass.new()
	example.print_type(example)
	pass
	var c := context.new()
	var s := plop.new()
	s.enter(c)
	s.update(c, 0.016)
	s.exit(c)
	
	var m := machine.new()
	m.travel_to(c,s)
	
	
