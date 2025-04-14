#include <gtest/gtest.h>
#include "TestTaskInitiCoreValueWrapper.h"

class InitializationTests : public ::testing::Test {
public:
  InitializationTests()
      : _customIntVal(10),
        _customFloatVal(3.14),
        _customStringVal("Test String"),
        _customVecVal({TestTaskIniti::IntegerType(100500), TestTaskIniti::StringType("qwerty")}) {}

protected:
  std::vector<TestTaskIniti::IntegerType> _customIntegerVec;
  std::vector<uint64_t> _integerVec;

  std::vector<TestTaskIniti::FloatType> _customFloatVec;
  std::vector<double> _doubleVec;

  std::vector<TestTaskIniti::StringType> _customStringVec;
  std::vector<std::string> _stringVec;

  std::vector<TestTaskIniti::Any> _customAnyVec;
  TestTaskIniti::IntegerType _customIntVal;
  TestTaskIniti::FloatType _customFloatVal;
  TestTaskIniti::StringType _customStringVal;
  TestTaskIniti::VectorType _customVecVal;

  std::vector<TestTaskIniti::VectorType> _customVecVec;
  std::vector<std::vector<TestTaskIniti::Any>> _anyVecVec;

  void SetUp() override {
    intInitialization();
    doubleInitialization();
    stringInitialization();
    anyInitialization();
    vectorInitialization();
  }

private:
  void intInitialization();
  void doubleInitialization();
  void stringInitialization();
  void anyInitialization();
  void vectorInitialization();
};

void InitializationTests::doubleInitialization() {
  _doubleVec = {0.0,
                1.1,
                -2.2,
                1e-308,
                1e+308,
                std::numeric_limits<double>::infinity(),
                -std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::quiet_NaN()};

  _customFloatVec.push_back(TestTaskIniti::FloatType(0.0));
  _customFloatVec.push_back(TestTaskIniti::FloatType(1.1));
  _customFloatVec.push_back(TestTaskIniti::FloatType(-2.2));
  _customFloatVec.push_back(TestTaskIniti::FloatType(1e-308));
  _customFloatVec.push_back(TestTaskIniti::FloatType(1e+308));
  _customFloatVec.push_back(TestTaskIniti::FloatType(std::numeric_limits<double>::infinity()));
  _customFloatVec.push_back(TestTaskIniti::FloatType(-std::numeric_limits<double>::infinity()));
  _customFloatVec.push_back(std::numeric_limits<double>::quiet_NaN());
}

void InitializationTests::intInitialization() {
  _integerVec = {1, 2, 3, 4, 5, 6};

  _customIntegerVec.push_back(TestTaskIniti::IntegerType(1));

  uint64_t secondVal = 2;
  TestTaskIniti::IntegerType val(secondVal);
  _customIntegerVec.push_back(val);

  int intVal = 3;
  unsigned uintVal = 4;
  uint32_t u32 = 5;

  _customIntegerVec.push_back(TestTaskIniti::IntegerType(intVal));
  _customIntegerVec.push_back(TestTaskIniti::IntegerType(uintVal));
  _customIntegerVec.push_back(TestTaskIniti::IntegerType(u32));

  TestTaskIniti::IntegerType bracedInitialized{6};
  _customIntegerVec.push_back(bracedInitialized);
}

void InitializationTests::stringInitialization() {
  _stringVec = {
      "", "simple", "123456", "with spaces", "special_chars!@#$%^&*()", "unicode_✓_テスト_😊", std::string(1000, 'x')};

  _customStringVec.push_back(TestTaskIniti::StringType(""));
  _customStringVec.push_back(TestTaskIniti::StringType("simple"));
  _customStringVec.push_back(TestTaskIniti::StringType("123456"));
  _customStringVec.push_back(TestTaskIniti::StringType("with spaces"));
  _customStringVec.push_back(TestTaskIniti::StringType("special_chars!@#$%^&*()"));
  _customStringVec.push_back(TestTaskIniti::StringType("unicode_✓_テスト_😊"));
  _customStringVec.push_back(TestTaskIniti::StringType(std::string(1000, 'x')));
}

void InitializationTests::anyInitialization() {
  _customAnyVec.push_back(TestTaskIniti::IntegerType(10));
  _customAnyVec.push_back(TestTaskIniti::FloatType(3.14));
  _customAnyVec.push_back(TestTaskIniti::StringType("Test String"));
  _customAnyVec.push_back(
      TestTaskIniti::VectorType{TestTaskIniti::IntegerType(100500), TestTaskIniti::StringType("qwerty")});
}

void InitializationTests::vectorInitialization() {
  TestTaskIniti::VectorType customEmptyVec;
  std::vector<TestTaskIniti::Any> emptyVec;
  _anyVecVec.push_back(emptyVec);
  _customVecVec.push_back(customEmptyVec);

  TestTaskIniti::IntegerType intVal(42);
  TestTaskIniti::FloatType floatVal(3.14);
  TestTaskIniti::StringType strVal("hello");
  TestTaskIniti::VectorType customVecWithDiffType(intVal, floatVal, strVal);
  std::vector<TestTaskIniti::Any> anyVecWithDiffType{TestTaskIniti::Any(intVal), TestTaskIniti::Any(floatVal),
                                                     TestTaskIniti::Any(strVal)};
  _anyVecVec.push_back(anyVecWithDiffType);
  _customVecVec.push_back(customVecWithDiffType);

  TestTaskIniti::VectorType innerVec{intVal, floatVal, strVal};
  TestTaskIniti::VectorType vecWithInnerVec;
  std::vector<TestTaskIniti::Any> anyVecWithVec;
  anyVecWithVec.push_back(std::move(innerVec));
  vecWithInnerVec.push_back(TestTaskIniti::VectorType{intVal, floatVal, strVal});
  _anyVecVec.push_back(anyVecWithVec);
  _customVecVec.push_back(vecWithInnerVec);
}

TEST_F(InitializationTests, IntegerInitializationTest) {
  for (size_t i = 0; i < _integerVec.size(); ++i) {
    ASSERT_EQ(_customIntegerVec.at(i).getVal(), _integerVec.at(i));
  }
}

TEST_F(InitializationTests, DoubleInitializationTest) {
  for (size_t i = 0; i < _integerVec.size(); ++i) {
    ASSERT_EQ(_customFloatVec.at(i).getVal(), _doubleVec.at(i));
  }
}

TEST_F(InitializationTests, StringInitializationTest) {
  for (size_t i = 0; i < _stringVec.size(); ++i) {
    ASSERT_EQ(_customStringVec.at(i).getVal(), _stringVec.at(i));
  }
}

TEST_F(InitializationTests, AnyInitializationTest) {
  const auto intId = _customIntVal.id;
  ASSERT_EQ(intId, _customAnyVec.at(0).getPayloadTypeId());
  ASSERT_EQ(_customIntVal.getVal(), _customAnyVec.at(0).getValue<intId>());

  const auto floatId = _customFloatVal.id;
  ASSERT_EQ(floatId, _customAnyVec.at(1).getPayloadTypeId());
  ASSERT_DOUBLE_EQ(_customFloatVal.getVal(), _customAnyVec.at(1).getValue<floatId>());

  const auto stringId = _customStringVal.id;
  ASSERT_EQ(stringId, _customAnyVec.at(2).getPayloadTypeId());
  ASSERT_EQ(_customStringVal.getVal(), _customAnyVec.at(2).getValue<stringId>());

  const auto vectorId = _customVecVal.id;
  ASSERT_EQ(vectorId, _customAnyVec.at(3).getPayloadTypeId());
  const auto anyVectorSrc = _customAnyVec.at(3).getValue<vectorId>();
  const auto anyVectorTgt = _customVecVal.getVal();

  ASSERT_EQ(anyVectorTgt.at(0).getPayloadTypeId(), anyVectorSrc.at(0).getPayloadTypeId());
  ASSERT_EQ(anyVectorTgt.at(1).getPayloadTypeId(), anyVectorSrc.at(1).getPayloadTypeId());

  ASSERT_EQ(anyVectorTgt.at(0).getValue<TestTaskIniti::IntegerType>(),
            anyVectorSrc.at(0).getValue<TestTaskIniti::IntegerType>());
  ASSERT_EQ(anyVectorTgt.at(1).getValue<TestTaskIniti::StringType>(),
            anyVectorSrc.at(1).getValue<TestTaskIniti::StringType>());
}

TEST_F(InitializationTests, VectorInitializationTest) {
  for (size_t i = 0; i < _anyVecVec.size(); ++i) {
    ASSERT_EQ(_customVecVec.at(i).getVal(), _anyVecVec.at(i));
  }
}
