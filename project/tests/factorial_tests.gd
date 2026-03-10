class_name FactorialTest extends MgTest

static func Negative():
	EXPECT_EQ(1, Sample1.Factorial(-5))
	EXPECT_EQ(1, Sample1.Factorial(-1))
	EXPECT_LT(0, Sample1.Factorial(-10)) 

static func Zero():
	EXPECT_EQ(1, Sample1.Factorial(0))

static func Positive():
	EXPECT_EQ(1, Sample1.Factorial(1));
	EXPECT_EQ(2, Sample1.Factorial(2));
	EXPECT_EQ(6, Sample1.Factorial(3));
	EXPECT_EQ(40320, Sample1.Factorial(8));
