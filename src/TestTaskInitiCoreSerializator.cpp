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

/*static*/ Any Serializator::deserealizeNumber(BufferIterator& it, const TypeId typeId) {
  if (typeId != TypeId::Uint && typeId != TypeId::Float)
    throw std::invalid_argument("TypeId must be a number type!");

  if (typeId == TypeId::Uint) {
    uint64_t intVal = ReadPrimitive<uint64_t>(it);
    return IntegerType(intVal);
  } else {
    double doubleVal = ReadPrimitive<double>(it);
    return FloatType(doubleVal);
  }
}

/*static*/ Any Serializator::deserealizeString(BufferIterator& it) {
  uint64_t stringSize = ReadPrimitive<uint64_t>(it);
  std::string str(reinterpret_cast<const char*>(&*it), stringSize);
  it += stringSize;
  return StringType(str);
}

/*static*/ Any Serializator::deserializeVector(BufferIterator& it, BufferIterator end) {
  uint64_t vectorSize = ReadPrimitive<uint64_t>(it);
  std::vector<Any> result;
  result.reserve(vectorSize);

  for (uint64_t i = 0; i < vectorSize; ++i) {
    TypeId typeId = static_cast<TypeId>(ReadPrimitive<uint64_t>(it));
    switch (typeId) {
      case TypeId::Uint:
      case TypeId::Float:
        result.push_back(deserealizeNumber(it, typeId));
        break;
      case TypeId::String:
        result.push_back(deserealizeString(it));
        break;
      case TypeId::Vector:
        result.push_back(std::move(deserializeVector(it, end)));
        break;
      default:
        throw std::runtime_error("Unknown TypeId in nested vector");
    }
  }

  return VectorType(std::move(result));
}

/*static*/ std::vector<Any> Serializator::deserialize(const Buffer& buffer) {
  BufferIterator it = buffer.begin();
  BufferIterator end = buffer.end();

  uint64_t vectorSize = ReadPrimitive<uint64_t>(it);
  std::vector<Any> result;
  result.reserve(vectorSize);

  for (uint64_t i = 0; i < vectorSize; ++i) {
    TypeId typeId = static_cast<TypeId>(ReadPrimitive<uint64_t>(it));
    switch (typeId) {
      case TypeId::Uint:
      case TypeId::Float:
        result.push_back(deserealizeNumber(it, typeId));
        break;
      case TypeId::String:
        result.push_back(deserealizeString(it));
        break;
      case TypeId::Vector:
        result.push_back(deserializeVector(it, end));
        break;
      default:
        throw std::runtime_error("Unknown TypeId");
    }
  }

  return result;
}

}  // namespace TestTaskIniti