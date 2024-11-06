/*
 * @Author: chenjingyu
 * @Date: 2024-11-06 17:06:40
 * @Contact: 2458006466@qq.com
 * @Description: Pet
 */
#include "Pet.h"

namespace mirror {
Pet::Pet(const std::string &name, Kind kind) : name_(name), kind_(kind) { 

}

void Pet::setName(const std::string &name) {
  name_ = name;
}

const std::string &Pet::getName() const {
  return name_;
}

Dog::Dog(const std::string &name) : Pet(name, Pet::Dog) {

}

std::string Dog::bark() const {
  return "Woof!";
}
} // namespace mirror
