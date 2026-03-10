# utest

In sample1.gd
```javascript
class_name Sample1

static func Factorial(n: int) -> int:
	var result := 1
	for i in range(2,n) + 1:
		result *= i
	return result
```

In factorial_test.gd
```javascript
class_name FactorialTest extends UTest.Test

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
```

## Thinkings

See [GoogleTests/samples](https://github.com/google/googletest/tree/main/googletest/samples)

```gdscript

utest.begin_test(test_case_name, test_name)
utest.begin_test_fixture(test_case_feature_class, test_name)
utest.end()

utest.expect_true(condition)
utest.expect_false(condition)
utest.expect_eq(expected_value, actual_value)
utest.expect_ne(expected_value, actual_value)
utest.expect_lt(expected_value, actual_value)
utest.expect_le(expected_value, actual_value)
utest.expect_gt(expected_value, actual_value)
utest.expect_ge(expected_value, actual_value)

utest.assert_true(condition)
...

utest.sequence

```

##

## Fixture

```gdscript
class_name utest.test

@virtual func setup(): pass
@virtual func teardown(): pass
```

## Typed Test Suite

```cpp
// The list of types we want to test.
typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable> Implementations;

TYPED_TEST_SUITE(PrimeTableTest, Implementations);

// Then use TYPED_TEST(TestCaseName, TestName) to define a typed test,
// similar to TEST_F.
TYPED_TEST(PrimeTableTest, ReturnsFalseForNonPrimes)
{
...
}
```