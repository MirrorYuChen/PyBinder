/*
 * @Author: chenjingyu
 * @Date: 2024-11-06 17:06:36
 * @Contact: 2458006466@qq.com
 * @Description: Pet
 */
#pragma once

#include <string>

namespace mirror {
class Pet {
public:
  enum Kind {
    Dog = 0,
    Cat
  };

  explicit Pet(const std::string &name, Kind kind);
  // 基类最少要有一个虚函数，多态继承接口才能被识别到
  virtual ~Pet() = default;

  void setName(const std::string &name);
  const std::string &getName() const;

public:
  int age_;
  Kind kind_;

protected:
  std::string name_;
};

class Dog : public Pet {
public:
  explicit Dog(const std::string &name);
  std::string bark() const;
};
} // namespace mirror

