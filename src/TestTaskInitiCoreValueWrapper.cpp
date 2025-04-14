#include "TestTaskInitiCoreSerializator.h"
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

void Any::serializeVectorType(Buffer& ioBuffer) const {
  serializeTypeId(ioBuffer, TypeId::Vector);
  const auto& vec = getValue<TypeId::Vector>();
  uint64_t vecSize = static_cast<uint64_t>(vec.size());
  const std::byte* pSizeBytes = reinterpret_cast<const std::byte*>(&vecSize);
  ioBuffer.insert(ioBuffer.end(), pSizeBytes, pSizeBytes + sizeof(vecSize));
  for (const Any& elem : vec) {
    elem.serialize(ioBuffer);
  }
}

void Any::serializeStringType(Buffer& ioBuffer) const {
  serializeTypeId(ioBuffer, TypeId::String);
  std::string stringVal = getValue<TypeId::String>();
  uint64_t stringSize = static_cast<uint64_t>(stringVal.size());
  const std::byte* pToStringSize = reinterpret_cast<std::byte*>(&stringSize);
  const std::byte* pToStringBeg = reinterpret_cast<std::byte*>(stringVal.data());
  ioBuffer.insert(ioBuffer.end(), pToStringSize, pToStringSize + sizeof(stringSize));
  ioBuffer.insert(ioBuffer.end(), pToStringBeg, pToStringBeg + stringSize * sizeof(char));
}

void Any::serialize(Buffer& ioBuffer) const {
  std::visit(
      [this, &ioBuffer](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if (T::id == TypeId::Uint) {
          serializeIntType(ioBuffer);
        } else if (T::id == TypeId::Float) {
          serializeFloatType(ioBuffer);
        } else if (T::id == TypeId::String) {
          serializeStringType(ioBuffer);
        } else if (T::id == TypeId::Vector) {
          serializeVectorType(ioBuffer);
        }
      },
      _data);
}

BufferIterator Any::deserialize(BufferIterator it, BufferIterator end) {
  TypeId typeId = static_cast<TypeId>(Serializator::ReadPrimitive<uint64_t>(it));
  switch (typeId) {
    case TypeId::Uint: {
      IntegerType val = IntegerType(Serializator::deserealizeNumber(it, typeId).getValue<TypeId::Uint>());
      _data.emplace<IntegerType>(val.getVal());
      break;
    }
    case TypeId::Float: {
      FloatType val = FloatType(Serializator::deserealizeNumber(it, typeId).getValue<TypeId::Float>());
      _data.emplace<FloatType>(val.getVal());
      break;
    }
    case TypeId::String: {
      std::string str = Serializator::deserealizeString(it).getValue<TypeId::String>();
      _data.emplace<StringType>(std::move(str));
      break;
    }
    default:
      throw std::runtime_error("Unsupported TypeId in Any::deserialize");
  }
  return it;
}

}  // namespace TestTaskIniti