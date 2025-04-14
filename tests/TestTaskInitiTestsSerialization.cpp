#include <gtest/gtest.h>
#include "TestTaskInitiCoreSerializator.h"

class SerializationTests : public ::testing::Test {
protected:
  std::vector<TestTaskIniti::Any> _customIntegerVec;
  TestTaskIniti::Buffer _integerSerializedBuf;

  void SetUp() override {
    numberInitialization();
  }

private:
  void numberInitialization();
};

void SerializationTests::numberInitialization() {
  uint64_t elemSize = 4;
  const std::byte* pToByteSize = reinterpret_cast<std::byte*>(&elemSize);
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToByteSize, pToByteSize + sizeof(elemSize));

  uint64_t intVal1 = 123456789;
  uint64_t intVal2 = 987654321;

  const std::byte* pToIntVal1 = reinterpret_cast<std::byte*>(&intVal1);
  const std::byte* pToIntVal2 = reinterpret_cast<std::byte*>(&intVal2);
  TestTaskIniti::TypeId intTypeId = TestTaskIniti::TypeId::Uint;
  const std::byte* pToTypeId = reinterpret_cast<const std::byte*>(&intTypeId);
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToTypeId, pToTypeId + sizeof(uint64_t));
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToIntVal1, pToIntVal1 + sizeof(intVal1));
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToTypeId, pToTypeId + sizeof(uint64_t));
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToIntVal2, pToIntVal2 + sizeof(intVal2));
  _customIntegerVec.push_back(TestTaskIniti::IntegerType(intVal1));
  _customIntegerVec.push_back(TestTaskIniti::IntegerType(intVal2));

  double doubleVal1 = 3.14;
  double doubleVal2 = 432142.32254;

  const std::byte* pToDoubleVal1 = reinterpret_cast<std::byte*>(&doubleVal1);
  const std::byte* pToDoubleVal2 = reinterpret_cast<std::byte*>(&doubleVal2);
  TestTaskIniti::TypeId floatTypeId = TestTaskIniti::TypeId::Float;
  const std::byte* pToFTypeId = reinterpret_cast<const std::byte*>(&floatTypeId);
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToFTypeId, pToFTypeId + sizeof(double));
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToDoubleVal1, pToDoubleVal1 + sizeof(doubleVal1));
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToFTypeId, pToFTypeId + sizeof(double));
  _integerSerializedBuf.insert(_integerSerializedBuf.end(), pToDoubleVal2, pToDoubleVal2 + sizeof(doubleVal2));
  _customIntegerVec.push_back(TestTaskIniti::FloatType(doubleVal1));
  _customIntegerVec.push_back(TestTaskIniti::FloatType(doubleVal2));
}

TEST_F(SerializationTests, NumberSerializationTests) {
  auto customIntegerVec = _customIntegerVec;
  TestTaskIniti::Serializator serilazator(std::move(customIntegerVec));
  TestTaskIniti::Buffer resultBuf = serilazator.serialize();
  std::vector<TestTaskIniti::Any> resultVec = TestTaskIniti::Serializator::deserialize(_integerSerializedBuf);

  ASSERT_EQ(resultBuf, _integerSerializedBuf);
  ASSERT_EQ(resultVec, _customIntegerVec);
}