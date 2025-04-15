#pragma once

#include "TestTaskInitiCoreDefinitions.h"
#include "TestTaskInitiCoreValueWrapper.h"

namespace TestTaskIniti {

class Serializator {
public:
  Serializator() : _storage(std::vector<Any>{}) {}
  Serializator(std::vector<Any>&& iStorage) : _storage(std::move(iStorage)) {}

  template <typename Arg>
  void push(Arg&& ioVal) {
    if constexpr (std::is_same_v<std::decay_t<Arg>, Any>) {
      _storage.push_back(std::forward<Arg>(ioVal));
    } else {
      _storage.push_back(Any(std::forward<Arg>(ioVal)));
    }
  }

  Buffer serialize() const;

  static std::vector<Any> deserialize(const Buffer& _val);
  static Any deserealizeNumber(BufferIterator& it, const TypeId typeId);
  static Any deserealizeString(BufferIterator& it);
  static Any deserializeVector(BufferIterator& it, BufferIterator end);

  template <typename T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0>
  static T ReadPrimitive(BufferIterator& it) {
    T value;
    std::memcpy(&value, &*it, sizeof(T));
    it += sizeof(T);
    return value;
  }

  const std::vector<Any>& getStorage() const;

private:
  std::vector<Any> _storage;
};

}  // namespace TestTaskIniti
