#include <iostream>
#include <vector>
#include <fstream>
#include <variant>
#include <type_traits>

namespace TestTaskIniti {

using Id = uint64_t;
using Buffer = std::vector<std::byte>;

enum class TypeId : Id {
  Uint,
  Float,
  String,
  Vector
};

class Any;

template <TypeId IdVal, typename ValType>
class ValueWrapper {
public:
  template <typename ConstructionVal, std::enable_if_t<std::is_convertible_v<ConstructionVal, ValType>, bool> = true>
  ValueWrapper(ConstructionVal&& ioValue) : _val(std::forward<ConstructionVal>(ioValue)) {}

  const ValType& getVal() const {
    return _val;
  }

  static constexpr TypeId id = IdVal;

protected:
  ValType _val;
};

class IntegerType : public ValueWrapper<TypeId::Uint, uint64_t> {
public:
  using ValWrapper = ValueWrapper<TypeId::Uint, uint64_t>;
  using ValWrapper::ValWrapper;
};

class FloatType : public ValueWrapper<TypeId::Float, double> {
public:
  using ValWrapper = ValueWrapper<TypeId::Float, double>;
  using ValWrapper::ValWrapper;
};

class StringType : public ValueWrapper<TypeId::String, std::string> {
public:
  using ValWrapper = ValueWrapper<TypeId::String, std::string>;
  using ValWrapper::ValWrapper;
};

template <typename T, typename Variant>
struct IsVariantType;

template <typename T, typename... Types>
struct IsVariantType<T, std::variant<Types...>> : std::disjunction<std::is_same<T, Types>...> {};

class VectorType : public ValueWrapper<TypeId::Vector, std::vector<Any>> {
public:
  using ValWrapper = ValueWrapper<TypeId::Vector, std::vector<Any>>;
  VectorType() : ValWrapper(std::vector<Any>{}){};
  template <typename... Args>
  VectorType(Args&&... ioArgs) : ValWrapper(std::vector<Any>{}) {
    (push_back(ioArgs), ...);
  }
  VectorType(const std::initializer_list<Any>& ioList);

  bool containsNestedVector() const;

  // Any already satisfies constraint requirements on template arguments
  template <typename T>
  void push_back(T&& iVal);
};

class Any {
public:
  using DataType = std::variant<IntegerType, FloatType, StringType, VectorType>;
  Any() = delete;
  template <typename ConstructionVal,
            std::enable_if_t<IsVariantType<std::decay_t<ConstructionVal>, DataType>::value, bool> = true>
  Any(ConstructionVal&& ioValue) : _data(std::forward<ConstructionVal>(ioValue)) {}

  TypeId getPayloadTypeId() const;

  template <typename Type>
  auto& getValue() const {
    return std::get<Type>(_data).getVal();
  }

  template <TypeId kId>
  auto& getValue() const {
    if constexpr (kId == TypeId::Uint) {
      return std::get<IntegerType>(_data).getVal();
    } else if constexpr (kId == TypeId::Float) {
      return std::get<FloatType>(_data).getVal();
    } else if constexpr (kId == TypeId::String) {
      return std::get<StringType>(_data).getVal();
    } else if constexpr (kId == TypeId::Vector) {
      return std::get<VectorType>(_data).getVal();
    } else {
      throw std::runtime_error("Unknown TypeId");
    }
  }

  bool operator==(const Any& _o) const;

private:
  const DataType _data;
};

// copying VectorType is forbiden to avoid nested recursion
template <typename T>
void VectorType::push_back(T&& iVal) {
  if constexpr (std::is_same_v<std::decay_t<T>, VectorType>) {
    if (!std::is_rvalue_reference_v<decltype(iVal)>)
      throw std::invalid_argument("Cannot push_back lvalue for VectorType - use std::move()");
    _val.push_back(std::move(iVal));
  } else {
    _val.push_back(std::forward<T>(iVal));
  }
}

}  // namespace TestTaskIniti