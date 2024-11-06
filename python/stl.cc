/*
 * @Author: chenjingyu
 * @Date: 2024-11-06 18:18:37
 * @Contact: 2458006466@qq.com
 * @Description: stl
 */
#include <vector>
#include <iostream>

#include <pybind11/pybind11.h>
#include <Python.h>

void PrintVector(const std::vector<int> &vec) {
  for (const auto &v : vec) {
    std::cout << v << "\n";
  }
}

namespace py = pybind11;

void PrintList(py::list my_list) {
  for (const auto &item : my_list) {
    std::cout << item.cast<int>() << " ";
  }
  std::cout << "\n";
}

/**
 * @brief 当一个python str传参给一个接收std::string或char*类型参数的C++函数，
 * pybind11会将python的str编码成utf-8格式，所有python的str都可以被编码成utf-8格式
 * 所以这个操作不会失败
 * Python的bytes无需转换就可以传参给接收std::string或char*类型的C++函数，无论
 * C++接收参数是否使用const修饰符或引用
 */

/**
 * @brief 当C++函数返回一个std::string或char*类型参数给python调用时，pybind11
 * 会假设这个字符串是utf-8格式的，并使用和bytes解码相同的API(.decode("utf-8"))
 * 将其解码成python的str类型，若这个隐式的转换失败，pybind11会抛出UnicodeDecodeError
 * 
 * 如果一些C++代码构造一个非UTF-8编码格式的std::string，我们可以使用一个显式转换，返回
 * 一个py::str的对象，显式转换和隐式转换需要相同的开销
 * 
 * 若C++的std::string数据不表示文本，就应该作为bytes类型返回给Python，然后就可以
 * 返回py::bytes对象
 * 
 * 
 */

void BindStl(py::module &m) {
  m.def("PrintVector", &PrintVector);
  m.def("PrintList", &PrintList);
  // pybinder.utf8_test("🎂")
  m.def("utf8_test", [](const std::string &s) {
    std::cout << "utf-8 is icing on the cake.\n";
    std::cout << s << "\n";
  });

  // pybinder.utf8_charptr("🎂")
  m.def("utf8_charptr", [](const char *s) {
    std::cout << "my favorite food is\n";
    std::cout << s << "\n";
  });

  m.def("std_string_return", []() {
    return std::string("This string needs to be UTF-8 encoded");
  });

  m.def("str_output", []() {
    std::string s = "Send your r\xe9sum\ex9 to Alice in HR";
    py::handle py_s = PyUnicode_DecodeLatin1(s.data(), s.size(), nullptr);
    if (!py_s) {
      throw py::error_already_set();
    }
    return py::reinterpret_steal<py::str>(py_s);
  });

  m.def("return_bytes", []() {
    std::string s("\xba\xd0\xba\xd0");
    return py::bytes(s);
  });
}