#pragma once

#include "TestTaskInitiCoreDefinitions.h"
#include "TestTaskInitiCoreValueWrapper.h"

namespace TestTaskIniti {

class Serializator {
public:
  Serializator() : _storage(std::vector<Any>{}) {}
  Serializator(std::vector<Any>&& iStorage) : _storage(std::move(iStorage)) {}

  template <typename Arg>
  void push(Arg&& _val);

  Buffer serialize() const;

  static std::vector<Any> deserialize(const Buffer& _val);

  const std::vector<Any>& getStorage() const;

private:
  static Any numberDeserealization(const Buffer& iBuffer, size_t& ioOffset, const TypeId iTypeId);

  std::vector<Any> _storage;
};

}  // namespace TestTaskIniti
