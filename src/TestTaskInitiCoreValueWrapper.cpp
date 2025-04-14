#include "TestTaskInitiCoreValueWrapper.h"

namespace TestTaskIniti {

VectorType::VectorType(const std::initializer_list<Any>& ioList) : ValWrapper(std::vector<Any>{}) {
  for (auto&& elem : ioList) {
    _val.push_back(elem);
  }
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

void Any::serializeTypeId(Buffer& ioBuffer, const TypeId iTypeId) const {
  uint64_t id = static_cast<uint64_t>(iTypeId);
  const std::byte* pIdByte = reinterpret_cast<std::byte*>(&id);
  ioBuffer.insert(ioBuffer.end(), pIdByte, pIdByte + sizeof(id));
}

void Any::serializeNumberType(Buffer& ioBuffer, const TypeId iTypeId) const {
  if (iTypeId != TypeId::Uint && iTypeId != TypeId::Float)
    throw std::invalid_argument("TypeId must be a number type!");

  std::byte* pValByte = nullptr;

  if (iTypeId == TypeId::Uint) {
    auto val = getValue<TypeId::Uint>();
    pValByte = reinterpret_cast<std::byte*>(&val);
    ioBuffer.insert(ioBuffer.end(), pValByte, pValByte + sizeof(val));
  } else if (iTypeId == TypeId::Float) {
    auto val = getValue<TypeId::Float>();
    pValByte = reinterpret_cast<std::byte*>(&val);
    ioBuffer.insert(ioBuffer.end(), pValByte, pValByte + sizeof(val));
  }
}

void Any::serializeIntType(Buffer& ioBuffer) const {
  serializeTypeId(ioBuffer, TypeId::Uint);
  serializeNumberType(ioBuffer, TypeId::Uint);
}

void Any::serializeFloatType(Buffer& ioBuffer) const {
  serializeTypeId(ioBuffer, TypeId::Float);
  serializeNumberType(ioBuffer, TypeId::Float);
}

void Any::serializeStringType(Buffer& ioBuffer) const {}

void Any::serializeVectorType(Buffer& ioBuffer) const {}

void Any::serialize(Buffer& ioBuffer) const {
  std::visit(
      [this, &ioBuffer](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if (T::id == TypeId::Uint) {
          serializeIntType(ioBuffer);
        } else if (T::id == TypeId::Float) {
          serializeFloatType(ioBuffer);
        }
      },
      _data);
}

}  // namespace TestTaskIniti