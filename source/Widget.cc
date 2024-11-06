/*
 * @Author: chenjingyu
 * @Date: 2024-11-06 17:44:20
 * @Contact: 2458006466@qq.com
 * @Description: Widget
 */
#include "Widget.h"

namespace mirror {
int Widget::foo(int x, float y) {
  return x + y;
}

int Widget::foo(int x, float y) const {
  return x + y;
}

} // namespace mirror
