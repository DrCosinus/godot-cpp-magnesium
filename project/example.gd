extends Node2D

var ctx:= context.new()
var font := ThemeDB.fallback_font
static var cookie

# 0 - Warmup enter
# 1 - stated_changed
# 2 - warmup first orchestration
# 3 - warmup first update
# 4 - warmup last orchestration
# 5 - Warmup exit
# 6 - enter idle
# 7 - stated_changed

func _ready() -> void:
	MgFsmMachine.changed.connect(on_state_changed)
	MgFsmMachine.travel_to(ctx, warmup)
	MgFsmMachine.update(ctx, 1.2)
	
	print("Init Done.")
	MgTest.run_test(FactorialTest)
	#MgTest.run_test(MgTest)

func _process(delta: float) -> void:
	MgFsmMachine.update(ctx, delta)
	queue_redraw()

func on_state_changed(_from: Script, _to: Script, obj: Object):
	var c := obj as context
	c.check_sequence_index([1, 7], "state changed signal")

func _draw() -> void:
	MgFsmMachine.call_on_state(ctx, &"draw", ctx, self, font)

class warmup:
	const state_name := &"WarmUp"
	static func enter(c: context):
		c.check_sequence_index(0, "enter warmup")
		c.time = 5.0
		c.counter = 0
	static func orchestrate(c: context):
		if c.counter == 0:
			c.check_sequence_index(2, "warmup first orchestration")
			c.counter += 1
		if c.time<=0:
			c.check_sequence_index(4, "warmup last orchestration")
			MgFsmMachine.travel_to(c, state_init)
	static func update(c:context, dt):
		if c.counter == 1:
			c.check_sequence_index(3, "warmup first update")
			c.counter += 1
		c.time-=dt
	static func exit(c: context):
		c.check_sequence_index(5, "exit warmup")
	static func draw(ctx: context, drawer: Node2D, font: Font) -> void:
		drawer.draw_string(font, Vector2(4,20), "Countdown: %.0f" % ctx.time)

class ploum:
	const state_name := &"ploum"
	static func test(): return ploum
