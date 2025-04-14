#include <gtest/gtest.h>
#include "TestTaskInitiCoreSerializator.h"

class SerializationTests : public ::testing::Test {
protected:
  std::vector<TestTaskIniti::Any> _customIntegerVec;
  TestTaskIniti::Buffer _integerSerializedBuf;

  std::vector<TestTaskIniti::Any> _customStringVec;
  TestTaskIniti::Buffer _stringSerializedBuf;

  std::vector<TestTaskIniti::Any> _customVectorVec;
  TestTaskIniti::Buffer _vectorSerializedBuf;

  void SetUp() override {
    numberInitialization();
    stringInitialization();
    vectorInitialization();
  }

private:
  void numberInitialization();
  void stringInitialization();
  void vectorInitialization();
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

void SerializationTests::stringInitialization() {
  uint64_t elemSize = 2;
  const std::byte* pToByteSize = reinterpret_cast<std::byte*>(&elemSize);
  _stringSerializedBuf.insert(_stringSerializedBuf.end(), pToByteSize, pToByteSize + sizeof(elemSize));

  TestTaskIniti::TypeId stringTypeId = TestTaskIniti::TypeId::String;
  const std::byte* pToStrTypeId = reinterpret_cast<const std::byte*>(&stringTypeId);

  std::string str1 = "hello";
  std::string str2 = "serialization";

  uint64_t len1 = str1.size();
  uint64_t len2 = str2.size();
  const std::byte* pToLen1 = reinterpret_cast<const std::byte*>(&len1);
  const std::byte* pToLen2 = reinterpret_cast<const std::byte*>(&len2);

  _stringSerializedBuf.insert(_stringSerializedBuf.end(), pToStrTypeId, pToStrTypeId + sizeof(uint64_t));
  _stringSerializedBuf.insert(_stringSerializedBuf.end(), pToLen1, pToLen1 + sizeof(len1));
  _stringSerializedBuf.insert(_stringSerializedBuf.end(), reinterpret_cast<const std::byte*>(str1.data()),
                              reinterpret_cast<const std::byte*>(str1.data()) + str1.size());

  _stringSerializedBuf.insert(_stringSerializedBuf.end(), pToStrTypeId, pToStrTypeId + sizeof(uint64_t));
  _stringSerializedBuf.insert(_stringSerializedBuf.end(), pToLen2, pToLen2 + sizeof(len2));
  _stringSerializedBuf.insert(_stringSerializedBuf.end(), reinterpret_cast<const std::byte*>(str2.data()),
                              reinterpret_cast<const std::byte*>(str2.data()) + str2.size());

  _customStringVec.push_back(TestTaskIniti::StringType(str1));
  _customStringVec.push_back(TestTaskIniti::StringType(str2));
}

void SerializationTests::vectorInitialization() {
  using namespace TestTaskIniti;

  uint64_t outerSize = 1;
  const std::byte* pOuterSize = reinterpret_cast<const std::byte*>(&outerSize);
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pOuterSize, pOuterSize + sizeof(outerSize));

  TypeId vectorTypeId = TypeId::Vector;
  const std::byte* pVecTypeId = reinterpret_cast<const std::byte*>(&vectorTypeId);
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pVecTypeId, pVecTypeId + sizeof(uint64_t));

  uint64_t outerVecSize = 1;
  const std::byte* pOuterVecSize = reinterpret_cast<const std::byte*>(&outerVecSize);
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pOuterVecSize, pOuterVecSize + sizeof(outerVecSize));

  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pVecTypeId, pVecTypeId + sizeof(uint64_t));

  uint64_t nestedSize = 2;
  const std::byte* pNestedSize = reinterpret_cast<const std::byte*>(&nestedSize);
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pNestedSize, pNestedSize + sizeof(nestedSize));

  TypeId uintType = TypeId::Uint;
  uint64_t uintVal = 42;
  const std::byte* pUintType = reinterpret_cast<const std::byte*>(&uintType);
  const std::byte* pUintVal = reinterpret_cast<const std::byte*>(&uintVal);
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pUintType, pUintType + sizeof(uint64_t));
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pUintVal, pUintVal + sizeof(uint64_t));

  TypeId strType = TypeId::String;
  std::string nestedStr = "nested";
  uint64_t strLen = nestedStr.size();
  const std::byte* pStrType = reinterpret_cast<const std::byte*>(&strType);
  const std::byte* pStrLen = reinterpret_cast<const std::byte*>(&strLen);
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pStrType, pStrType + sizeof(uint64_t));
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), pStrLen, pStrLen + sizeof(uint64_t));
  _vectorSerializedBuf.insert(_vectorSerializedBuf.end(), reinterpret_cast<const std::byte*>(nestedStr.data()),
                              reinterpret_cast<const std::byte*>(nestedStr.data()) + nestedStr.size());

  TestTaskIniti::VectorType nestedVec;
  nestedVec.push_back(TestTaskIniti::IntegerType(42));
  nestedVec.push_back(TestTaskIniti::StringType("nested"));

  TestTaskIniti::VectorType outerVec;
  outerVec.push_back(std::move(nestedVec));
  _customVectorVec.push_back(std::move(outerVec));
}

TEST_F(SerializationTests, NumberSerializationTests) {
  auto customIntegerVec = _customIntegerVec;
  TestTaskIniti::Serializator serilazator(std::move(customIntegerVec));
  TestTaskIniti::Buffer resultBuf = serilazator.serialize();
  std::vector<TestTaskIniti::Any> resultVec = TestTaskIniti::Serializator::deserialize(_integerSerializedBuf);

  ASSERT_EQ(resultBuf, _integerSerializedBuf);
  ASSERT_EQ(resultVec, _customIntegerVec);
}

TEST_F(SerializationTests, StringSerializationTests) {
  auto customStringVec = _customStringVec;
  TestTaskIniti::Serializator serilazator(std::move(customStringVec));
  TestTaskIniti::Buffer resultBuf = serilazator.serialize();
  std::vector<TestTaskIniti::Any> resultVec = TestTaskIniti::Serializator::deserialize(_stringSerializedBuf);

  ASSERT_EQ(resultBuf, _stringSerializedBuf);
  ASSERT_EQ(resultVec, _customStringVec);
}

TEST_F(SerializationTests, VectorSerializationTests) {
  auto customVectorVec = _customVectorVec;
  TestTaskIniti::Serializator serializator(std::move(customVectorVec));
  TestTaskIniti::Buffer resultBuf = serializator.serialize();
  std::vector<TestTaskIniti::Any> resultVec = TestTaskIniti::Serializator::deserialize(_vectorSerializedBuf);

  ASSERT_EQ(resultVec, _customVectorVec);
  ASSERT_EQ(resultBuf, _vectorSerializedBuf);
}