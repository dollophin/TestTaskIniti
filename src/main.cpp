#include "TestTaskInitiCoreDefinitions.h"
#include "TestTaskInitiCoreSerializator.h"
using namespace TestTaskIniti;

int main() {
  std::ifstream raw;
  raw.open("raw.bin", std::ios_base::in | std::ios_base::binary);
  if (!raw.is_open())
    return 1;
  raw.seekg(0, std::ios_base::end);
  std::streamsize size = raw.tellg();
  raw.seekg(0, std::ios_base::beg);

  Buffer buff(size);
  raw.read(reinterpret_cast<char*>(buff.data()), size);

  auto res = Serializator::deserialize(buff);

  Serializator s;
  for (auto&& i : res)
    s.push(i);

  std::cout << (buff == s.serialize()) << '\n';

  return 0;
}