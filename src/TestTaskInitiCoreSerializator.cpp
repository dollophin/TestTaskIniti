#include "TestTaskInitiCoreSerializator.h"

namespace TestTaskIniti {
Buffer Serializator::serialize() const {
  Buffer buff;
  uint64_t elementsSize = _storage.size();
  const std::byte* pToSize = reinterpret_cast<std::byte*>(&elementsSize);
  buff.insert(buff.end(), pToSize, pToSize + sizeof(elementsSize));

  for (const auto& element : _storage) {
    element.serialize(buff);
  }

  return buff;
}

/*static*/ Any Serializator::numberDeserealization(const Buffer& iBuffer, size_t& iOffset, const TypeId iTypeId) {
  if (iTypeId != TypeId::Uint && iTypeId != TypeId::Float)
    throw std::invalid_argument("TypeId must be a number type!");

  if (iTypeId == TypeId::Uint) {
    uint64_t intVal;
    std::memcpy(&intVal, &iBuffer.at(iOffset), sizeof(uint64_t));
    iOffset += sizeof(uint64_t);
    return IntegerType(intVal);
  } else if (iTypeId == TypeId::Float) {
    double doubleVal;
    std::memcpy(&doubleVal, &iBuffer.at(iOffset), sizeof(double));
    iOffset += sizeof(double);
    return FloatType(doubleVal);
  }
}

/*static*/ std::vector<Any> Serializator::deserialize(const Buffer& iBuffer) {
  size_t offset = 0, vectorSize = 0;
  std::vector<Any> result;
  std::memcpy(&vectorSize, &iBuffer.at(offset), sizeof(uint64_t));
  offset += sizeof(uint64_t);
  result.reserve(vectorSize);
  uint64_t idType;
  for (size_t i = 0; i < vectorSize; ++i) {
    std::memcpy(&idType, &iBuffer.at(offset), sizeof(uint64_t));
    offset += sizeof(uint64_t);
    switch (TypeId(idType)) {
      case TypeId::Uint:
        result.push_back(numberDeserealization(iBuffer, offset, TypeId::Uint));
        break;
      case TypeId::Float:
        result.push_back(numberDeserealization(iBuffer, offset, TypeId::Float));
        break;
    }
  }

  return result;
}

}  // namespace TestTaskIniti