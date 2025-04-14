#include "TestTaskInitiCoreValueWrapper.h"

namespace TestTaskIniti {

VectorType::VectorType(const std::initializer_list<Any>& ioList) : ValWrapper(std::vector<Any>{}) {
  for (auto&& elem : ioList) {
    _val.push_back(elem);
  }
}

bool VectorType::containsNestedVector() const {
  for (const auto& elem : _val) {
    if (elem.getPayloadTypeId() == TypeId::Vector) {
      return true;
    }
  }
  return false;
}

TypeId Any::getPayloadTypeId() const {
  switch (_data.index()) {
    case 0:
      return std::get<IntegerType>(_data).id;
      break;
    case 1:
      return std::get<FloatType>(_data).id;
      break;
    case 2:
      return std::get<StringType>(_data).id;
      break;
    case 3:
      return std::get<VectorType>(_data).id;
      break;
    default:
      throw std::runtime_error("Unknown TypeId");
  }
}

bool Any::operator==(const Any& _o) const {
  bool isEqVal = false;
  const bool isEqType = (getPayloadTypeId() == _o.getPayloadTypeId());
  switch (getPayloadTypeId()) {
    case TypeId::Uint:
      isEqVal = (getValue<IntegerType>() == _o.getValue<IntegerType>());
      break;
    case TypeId::Float:
      isEqVal = (getValue<FloatType>() == _o.getValue<FloatType>());
      break;
    case TypeId::String:
      isEqVal = (getValue<StringType>() == _o.getValue<StringType>());
      break;
    case TypeId::Vector:
      isEqVal = (getValue<VectorType>() == _o.getValue<VectorType>());
      break;
  }

  return isEqVal && isEqType;
}

}  // namespace TestTaskIniti