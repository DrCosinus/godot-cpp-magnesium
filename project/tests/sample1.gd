class_name Sample1

static func Factorial(n: int) -> int:
	var result := 1
	for i in range(2, n+1):
		result *= i
	return result
