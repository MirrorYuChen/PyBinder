/*
 * @Author: chenjingyu
 * @Date: 2024-11-06 17:13:40
 * @Contact: 2458006466@qq.com
 * @Description: pet
 */
#include "Pet.h"

#include <pybind11/pybind11.h>
#include <memory>

namespace py = pybind11;
using namespace mirror;

void BindPet(py::module &m) {
  py::class_<Pet> pet (m, "Pet");
  pet.def(py::init<const std::string&, Pet::Kind>());
  pet.def("getName", &Pet::getName);
  pet.def("setName", &Pet::setName);
  pet.def("__repr__", 
    [](const Pet &p) {
      return "<Pet named '" + p.getName() + "'>";
    }
  );
  pet.def_property("name", &Pet::getName, &Pet::setName);
  pet.def_readwrite("age", &Pet::age_);
  pet.def_readwrite("kind", &Pet::kind_);

  py::enum_<Pet::Kind>kind(pet, "Kind");
  kind.value("Dog", Pet::Kind::Dog);
  kind.value("Cat", Pet::Kind::Cat);
  // 导出枚举值到父模块：Pet.Dog或Pet.Cat
  kind.export_values();

  py::class_<Dog> dog(m, "Dog", pet);
  dog.def(py::init<const std::string&>());
  dog.def("bark", &Dog::bark);

  m.def("pet_store", []() {
    return std::unique_ptr<Pet>(new Dog("Molly"));
  });
}