/*
 * @Author: chenjingyu
 * @Date: 2024-11-06 16:55:47
 * @Contact: 2458006466@qq.com
 * @Description: pybinder
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
  return i + j;
}

void BindPet(py::module &m);
void BindWidget(py::module &m);
void BindStl(py::module &m);

PYBIND11_MODULE(pybinder, m) {
  m.def(
    "add", 
    &add, 
    "A function thatadd two numbers", 
    py::arg("i"),
    py::arg("j") = 2
  );
  m.attr("__version__") = "0.1.0";

  BindPet(m);
  BindWidget(m);
  BindStl(m);
}