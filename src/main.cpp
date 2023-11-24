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

#include "MIPS_Simulator.h"
#include "compiler.h"

using namespace std;
using namespace ftxui;

unsigned int hi, lo;
char str[8];
std::string str1;
std::string ToHexString(unsigned int num) {
  sprintf(str, "%08x", num);
  std::string str1 = str;
  return str1;
}

int main() {
  /* TOP */
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
        Checkbox("0x" + ToHexString(i * 4) + " | " + Hex[i] + " | " + mem[i],
                 &states[i]));
  }

  auto top = Renderer(text_segment, [&] {
    return vbox({
               hbox({
                   text("   |Address    |Code        |Source  "),
               }),
               separator(),
               text_segment->Render() | vscroll_indicator | frame |
                   size(HEIGHT, LESS_THAN, 20),
           }) |
           border;
  });

  /* MIDDLE */
  std::vector<unsigned int> registers;
  std::vector<unsigned int> memories;
  MIPS_Simulator simulator;
  simulator.loadInstructions("..\\IO\\out.bin");
  registers = simulator.getRegisters();
  memories = simulator.getMemories();

  int page = 0;

  auto buttons0 = Container::Horizontal({
      Button(
          "step",
          [&] {
            simulator.executeNextInstruction();
            registers = simulator.getRegisters();
            memories = simulator.getMemories();
          },
          ButtonOption::Animated()),
      Button(
          "excute",
          [&] {
            while (!simulator.isExecutionFinished()) {
              simulator.executeNextInstruction();
            }
            registers = simulator.getRegisters();
            memories = simulator.getMemories();
          },
          ButtonOption::Animated()),
      Button(
          "<---",
          [&] {
            if (page >= 64)
              page -= 64;
          },
          ButtonOption::Animated()),
      Button(
          "--->",
          [&] {
            if (page <= 896)
              page += 64;
          },
          ButtonOption::Animated()),
  });

  auto middle = Renderer(buttons0, [&] {
    return vbox({
        vbox({
            text("   |  Address  | Value(+0)  | Value(+4)  | Value(+8)  | "
                 "Value(+C)  | Value(+10) | Value(+14) | Value(+18) "
                 "| Value(+1C) |"),
            text(
                "   "
                "-------------------------------------------------------------"
                "--------------------------------------------------------    "),
            text("   |0x" + ToHexString(page) + " | 0x" +
                 ToHexString(memories[page]) + " | 0x" +
                 ToHexString(memories[page + 1]) + " | 0x" +
                 ToHexString(memories[page + 2]) + " | 0x" +
                 ToHexString(memories[page + 3]) + " | 0x" +
                 ToHexString(memories[page + 4]) + " | 0x" +
                 ToHexString(memories[page + 5]) + " | 0x" +
                 ToHexString(memories[page + 6]) + " | 0x" +
                 ToHexString(memories[page + 7]) + " |"),
            text("   |0x" + ToHexString(page + 8) + " | 0x" +
                 ToHexString(memories[page + 8]) + " | 0x" +
                 ToHexString(memories[page + 9]) + " | 0x" +
                 ToHexString(memories[page + 10]) + " | 0x" +
                 ToHexString(memories[page + 11]) + " | 0x" +
                 ToHexString(memories[page + 12]) + " | 0x" +
                 ToHexString(memories[page + 13]) + " | 0x" +
                 ToHexString(memories[page + 14]) + " | 0x" +
                 ToHexString(memories[page + 15]) + " |"),
            text("   |0x" + ToHexString(page + 16) + " | 0x" +
                 ToHexString(memories[page + 16]) + " | 0x" +
                 ToHexString(memories[page + 17]) + " | 0x" +
                 ToHexString(memories[page + 18]) + " | 0x" +
                 ToHexString(memories[page + 19]) + " | 0x" +
                 ToHexString(memories[page + 20]) + " | 0x" +
                 ToHexString(memories[page + 21]) + " | 0x" +
                 ToHexString(memories[page + 22]) + " | 0x" +
                 ToHexString(memories[page + 23]) + " |"),
            text("   |0x" + ToHexString(page + 24) + " | 0x" +
                 ToHexString(memories[page + 24]) + " | 0x" +
                 ToHexString(memories[page + 25]) + " | 0x" +
                 ToHexString(memories[page + 26]) + " | 0x" +
                 ToHexString(memories[page + 27]) + " | 0x" +
                 ToHexString(memories[page + 28]) + " | 0x" +
                 ToHexString(memories[page + 29]) + " | 0x" +
                 ToHexString(memories[page + 30]) + " | 0x" +
                 ToHexString(memories[page + 31]) + " |"),
            text("   |0x" + ToHexString(page + 32) + " | 0x" +
                 ToHexString(memories[page + 32]) + " | 0x" +
                 ToHexString(memories[page + 33]) + " | 0x" +
                 ToHexString(memories[page + 34]) + " | 0x" +
                 ToHexString(memories[page + 35]) + " | 0x" +
                 ToHexString(memories[page + 36]) + " | 0x" +
                 ToHexString(memories[page + 37]) + " | 0x" +
                 ToHexString(memories[page + 38]) + " | 0x" +
                 ToHexString(memories[page + 39]) + " |"),
            text("   |0x" + ToHexString(page + 40) + " | 0x" +
                 ToHexString(memories[page + 40]) + " | 0x" +
                 ToHexString(memories[page + 41]) + " | 0x" +
                 ToHexString(memories[page + 42]) + " | 0x" +
                 ToHexString(memories[page + 43]) + " | 0x" +
                 ToHexString(memories[page + 44]) + " | 0x" +
                 ToHexString(memories[page + 45]) + " | 0x" +
                 ToHexString(memories[page + 46]) + " | 0x" +
                 ToHexString(memories[page + 47]) + " |"),
            text("   |0x" + ToHexString(page + 48) + " | 0x" +
                 ToHexString(memories[page + 48]) + " | 0x" +
                 ToHexString(memories[page + 49]) + " | 0x" +
                 ToHexString(memories[page + 50]) + " | 0x" +
                 ToHexString(memories[page + 51]) + " | 0x" +
                 ToHexString(memories[page + 52]) + " | 0x" +
                 ToHexString(memories[page + 53]) + " | 0x" +
                 ToHexString(memories[page + 54]) + " | 0x" +
                 ToHexString(memories[page + 55]) + " |"),
            text("   |0x" + ToHexString(page + 56) + " | 0x" +
                 ToHexString(memories[page + 56]) + " | 0x" +
                 ToHexString(memories[page + 57]) + " | 0x" +
                 ToHexString(memories[page + 58]) + " | 0x" +
                 ToHexString(memories[page + 59]) + " | 0x" +
                 ToHexString(memories[page + 60]) + " | 0x" +
                 ToHexString(memories[page + 61]) + " | 0x" +
                 ToHexString(memories[page + 62]) + " | 0x" +
                 ToHexString(memories[page + 63]) + " |"),
        }) | border,
        buttons0->Render(),
    });
  });

  /* RIGHT */

  auto buttons = Container::Horizontal({
      Button(
          "step",
          [&] {
            simulator.executeNextInstruction();
            registers = simulator.getRegisters();
            memories = simulator.getMemories();
          },
          ButtonOption::Animated()),
      Button(
          "excute",
          [&] {
            while (!simulator.isExecutionFinished()) {
              simulator.executeNextInstruction();
            }
            registers = simulator.getRegisters();
            memories = simulator.getMemories();
          },
          ButtonOption::Animated()),
  });

  // Modify the way to render them on screen:
  auto right = Renderer(buttons, [&] {
    return vbox({
        vbox({
            text("Registers Name | Number | Value"),
            text("--------------------------------------"),
            text("$zero          | 0      | 0x" + ToHexString(registers[0])),
            text("$at            | 1      | 0x" + ToHexString(registers[1])),
            text("$v0            | 2      | 0x" + ToHexString(registers[2])),
            text("$v1            | 3      | 0x" + ToHexString(registers[3])),
            text("$a0            | 4      | 0x" + ToHexString(registers[4])),
            text("$a1            | 5      | 0x" + ToHexString(registers[5])),
            text("$a2            | 6      | 0x" + ToHexString(registers[6])),
            text("$a3            | 7      | 0x" + ToHexString(registers[7])),
            text("$t0            | 8      | 0x" + ToHexString(registers[8])),
            text("$t1            | 9      | 0x" + ToHexString(registers[9])),
            text("$t2            | 10     | 0x" + ToHexString(registers[10])),
            text("$t3            | 11     | 0x" + ToHexString(registers[11])),
            text("$t4            | 12     | 0x" + ToHexString(registers[12])),
            text("$t5            | 13     | 0x" + ToHexString(registers[13])),
            text("$t6            | 14     | 0x" + ToHexString(registers[14])),
            text("$t7            | 15     | 0x" + ToHexString(registers[15])),
            text("$s0            | 16     | 0x" + ToHexString(registers[16])),
            text("$s1            | 17     | 0x" + ToHexString(registers[17])),
            text("$s2            | 18     | 0x" + ToHexString(registers[18])),
            text("$s3            | 19     | 0x" + ToHexString(registers[19])),
            text("$s4            | 20     | 0x" + ToHexString(registers[20])),
            text("$s5            | 21     | 0x" + ToHexString(registers[21])),
            text("$s6            | 22     | 0x" + ToHexString(registers[22])),
            text("$s7            | 23     | 0x" + ToHexString(registers[23])),
            text("$t8            | 24     | 0x" + ToHexString(registers[24])),
            text("$t9            | 25     | 0x" + ToHexString(registers[25])),
            text("$k0            | 26     | 0x" + ToHexString(registers[26])),
            text("$k1            | 27     | 0x" + ToHexString(registers[27])),
            text("$gp            | 28     | 0x" + ToHexString(registers[28])),
            text("$sp            | 29     | 0x" + ToHexString(registers[29])),
            text("$fp            | 30     | 0x" + ToHexString(registers[30])),
            text("$ra            | 31     | 0x" + ToHexString(registers[31])),
        }) | border,
        buttons->Render(),
    });
  });

  // BOTTOM
  //  The data:
  std::string num1;
  std::string num2;
  std::string ope;
  std::string phoneNumber;

  // The basic input components:
  Component input_num1 = Input(&num1, "number1");
  Component input_num2 = Input(&num2, "number2");

  Component input_ope = Input(&ope, "operator(+ or -)");

  // The phone number input component:
  // We are using `CatchEvent` to filter out non-digit characters.
  Component input_phone_number = Input(&phoneNumber, "phone number");
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });

  // The component tree:
  auto component = Container::Vertical({
      input_num1,
      input_num2,
      input_ope,
  });

  // Tweak how the component tree is rendered:
  auto bottom = Renderer(component, [&] {
    return hbox({
               vbox({
                   hbox(text(" number1  : "), input_num1->Render()),
                   hbox(text(" number2  : "), input_num2->Render()),
                   hbox(text(" operator : "), input_ope->Render()),

               }),
               separator(),
               vbox({
                   text("Number1") | center,
                   text("Decimal(exact) :" + num1),
                   text("Binary         :" + num1),
                   text("Hexadecimal    :" + num1),
               }),
               separator(),
               vbox({
                   text("Number2") | center,
                   text("Decimal(exact) :" + num2),
                   text("Binary         :" + num2),
                   text("Hexadecimal    :" + num2),
               }),
               separator(),
               vbox({
                   text("Result") | center,
                   text("Decimal(exact) :" + ope),
                   text("Binary         :" + ope),
                   text("Hexadecimal    :" + ope),
               }),
               separator(),
           }) |
           border;
  });
  /* auto bottom = Renderer([] { return text("bottom") | center; }); */

  int top_size = 23;
  int right_size = 40;
  int bottom_size = 10;

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
