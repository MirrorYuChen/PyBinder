/*
 * @Author: chenjingyu
 * @Date: 2024-11-06 17:49:12
 * @Contact: 2458006466@qq.com
 * @Description: widget
 */
#include "Widget.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

using namespace mirror;

void BindWidget(py::module &m) {
  py::class_<Widget>widget(m, "Widget");
  widget.def(py::init<>());
  widget.def("foo_mutable", py::overload_cast<int, float>(&Widget::foo));
  widget.def("foo_const", py::overload_cast<int, float>(&Widget::foo, py::const_));
}
