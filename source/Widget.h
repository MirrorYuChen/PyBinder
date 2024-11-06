/*
 * @Author: chenjingyu
 * @Date: 2024-11-06 17:44:14
 * @Contact: 2458006466@qq.com
 * @Description: Widget
 */
#pragma once

namespace mirror {
class Widget {
public:
  Widget() = default;
  ~Widget() = default;

  int foo(int x, float y);
  int foo(int x, float y) const;
};
} // namespace mirror