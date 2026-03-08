# utest

See [GoogleTests/samples](https://github.com/google/googletest/tree/main/googletest/samples)

```gdscript

utest.begin_test(test_case_name, test_name)
utest.begin_test_fixture(test_case_feature_class, test_name)
utest.end()

utest.expect_true(condition)
utest.expect_false(condition)
utest.expect_eq(expected_value, tested_value)
utest.expect_ne(expected_value, tested_value)
utest.expect_lt(expected_value, tested_value)
utest.expect_le(expected_value, tested_value)
utest.expect_gt(expected_value, tested_value)
utest.expect_ge(expected_value, tested_value)

utest.assert_true(condition)
...

utest.sequence

```

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