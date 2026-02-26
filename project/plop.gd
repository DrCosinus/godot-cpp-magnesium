class_name plop extends state


func enter(p_context: context) -> void:
	print("enter plop %d" % p_context.value);
	p_context.value = 42

func update(p_context: context, delta: float) -> void:
	p_context.value += delta
	print("update plop %f" % p_context.value)

class station extends context:
	pass
	
