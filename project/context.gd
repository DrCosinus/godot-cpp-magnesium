class_name context

var counter : int
var time : float

var next_sequence_index:= 0

func check_sequence_index(idx: Variant, msg: String) -> void:
	if idx is int:
		assert(next_sequence_index == idx, msg+ " " + str(idx) + " <-> " + str(next_sequence_index))
	elif idx is Array:
		assert(next_sequence_index in idx, msg+ " " + str(idx) + " <-> " + str(next_sequence_index))
	else:
		assert(false, "Bad idx")
		
	next_sequence_index += 1

func hello_world(name: String):
	print("Hello World, %s" % name)
	return 509
