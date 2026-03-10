class_name MgTest

static var _success := 0
static var _failures : Array[String]

static func _expect(condition, message) -> void:
	if condition:
		_success += 1
	else:
		_failures.append(message)

static func EXPECT_TRUE(condition) -> void:
	_expect(condition, "Expected %d == true" % [ condition ])

static func EXPECT_FALSE(condition) -> void:
	_expect(not condition, "Expected %d == false" % [ condition ])

static func EXPECT_EQ(expected, actual) -> void:
	_expect(expected == actual, "Expected %d == %s" % [ expected, actual ])

static func EXPECT_LT(expected, actual) -> void:
	_expect(expected < actual, "Expected %d < %s" % [ expected, actual ])

static func run_test(script: Script) -> void:
	print("Running Test...")
	var parent : Script = script.get_base_script()
	var parent_method_infos := parent.get_script_method_list()
	var parent_method_names : Array[String]
	for d in parent_method_infos:
		parent_method_names.append(d["name"])
	
	var method_infos := script.get_script_method_list()
	print(method_infos.size())
	for i in range(method_infos.size()):
		var name : StringName = method_infos[i]["name"]
		if name in parent_method_names:
			continue
		print("Test Found: %s" % [ name ])
		# method_infos[i]["flags"] is 33 (METHOD_FLAG_NORMAL + METHOD_FLAG_STATIC)
