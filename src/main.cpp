#include <array> // for array
#include <cstring>
#include <iostream> // for endl, cout, ostream
#include <memory>   // for shared_ptr, allocator, __shared_ptr_access
#include <string>   // for basic_string, allocator, string
#include <vector>   // for vector

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp" // for Renderer, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/elements.hpp" // for Element, operator|, text, center, border
#include "ftxui/dom/node.hpp"     // for Render
#include "ftxui/dom/table.hpp"    // for Table, TableSelection
#include "ftxui/screen/color.hpp" // for Color, Color::Blue, Color::Cyan, Color::White, ftxui
#include "ftxui/screen/screen.hpp" // for Screen

#include "compiler.h"

using namespace std;
using namespace ftxui;

unsigned int zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7,
    s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra, pc, hi, lo;
char str[8];
std::string str1;
std::string ToHexString(unsigned int num) {
  sprintf(str, "%08x", num);
  std::string str1 = str;
  return str1;
}

int main() {
  int i = 0, num = 0;

  get_code();
  Assembly_to_Machine(mem);
  write_code();

  for (int j = 0; mem[j] != ""; ++j) {
    num++;
  }

  bool states[num];

  auto text_segment = Container::Vertical({});
  for (int i = 0; i < num; ++i) {
    states[i] = false;
    text_segment->Add(
        Checkbox("0x00000000 | " + Hex[i] + " | " + mem[i], &states[i]));
  }

  auto top = Renderer(text_segment, [&] {
    return vbox({
               hbox({
                   text("   |Address    | Code       | Source "),
               }),
               separator(),
               text_segment->Render() | vscroll_indicator | frame |
                   size(HEIGHT, LESS_THAN, 20),
           }) |
           border;
  });

  auto table = Table({
      {"Registers Name", "Number", "value"},
      {"$zero", "0", "0x" + ToHexString(zero)},
      {"$at", "1", "0x" + ToHexString(at)},
      {"$v0", "2", "0x" + ToHexString(v1)},
      {"$v1", "3", "0x" + ToHexString(v1)},
      {"$a0", "4", "0x" + ToHexString(a0)},
      {"$a1", "5", "0x" + ToHexString(a1)},
      {"$a2", "6", "0x" + ToHexString(a2)},
      {"$a3", "7", "0x" + ToHexString(a3)},
      {"$t0", "8", "0x" + ToHexString(t0)},
      {"$t1", "9", "0x" + ToHexString(t1)},
      {"$t2", "10", "0x" + ToHexString(t2)},
      {"$t3", "11", "0x" + ToHexString(t3)},
      {"$t4", "12", "0x" + ToHexString(t4)},
      {"$t5", "13", "0x" + ToHexString(t6)},
      {"$t6", "14", "0x" + ToHexString(t6)},
      {"$t7", "15", "0x" + ToHexString(t7)},
      {"$s0", "16", "0x" + ToHexString(s0)},
      {"$s1", "17", "0x" + ToHexString(s1)},
      {"$s2", "18", "0x" + ToHexString(s2)},
      {"$s3", "19", "0x" + ToHexString(s3)},
      {"$s4", "20", "0x" + ToHexString(s4)},
      {"$s5", "21", "0x" + ToHexString(s5)},
      {"$s6", "22", "0x" + ToHexString(s6)},
      {"$s7", "23", "0x" + ToHexString(s7)},
      {"$t8", "24", "0x" + ToHexString(t8)},
      {"$t9", "25", "0x" + ToHexString(t9)},
      {"$k1", "26", "0x" + ToHexString(k1)},
      {"$gp", "27", "0x" + ToHexString(gp)},
      {"$sp", "28", "0x" + ToHexString(sp)},
      {"$fp", "29", "0x" + ToHexString(fp)},
      {"$ra", "30", "0x" + ToHexString(ra)},
      {"$pc", "31", "0x" + ToHexString(pc)},
      {"$hi", "", "0x" + ToHexString(hi)},
      {"$lo", "", "0x" + ToHexString(lo)},
  });

  table.SelectAll().Border(LIGHT);

  // Add border around the first column.
  table.SelectColumn(0).Border(LIGHT);

  // Make first row bold with a double border.
  table.SelectRow(0).Decorate(bold);
  table.SelectRow(0).SeparatorVertical(LIGHT);
  table.SelectRow(0).Border(DOUBLE);

  auto document = table.Render();

  auto middle = Renderer([] { return text("middle") | center; });
  auto right = Renderer([&] { return document; });
  auto bottom = Renderer([] { return text("bottom") | center; });

  int top_size = 20;
  int right_size = 35;
  int bottom_size = 7;

  auto container = middle;
  container = ResizableSplitTop(top, container, &top_size);
  container = ResizableSplitBottom(bottom, container, &bottom_size);
  container = ResizableSplitRight(right, container, &right_size);

  auto renderer =
      Renderer(container, [&] { return container->Render() | border; });
  /* auto screen = ScreenInteractive::FitComponent(); */
  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(renderer);
}
