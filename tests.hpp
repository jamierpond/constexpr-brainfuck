//
// Created by Jamie Pond on 04/05/2023.
//

#pragma once

constexpr static size_t indexOfPointer = 0;
constexpr static size_t indexOfMemoryBlock = 1;
constexpr static auto interpretBrainfuckWithDebug(const std::string_view &sourceCode) {
    Memory memory{};
    size_t dataPointer = 0;
    std::string output = "";
    interpretBrainfuckCore(sourceCode, memory, dataPointer, output);
    return std::tuple<size_t, Memory>(dataPointer, memory);
}

constexpr static auto getFinalPointerPositionForProgram(const std::string_view &sourceCode) {
    return std::get<indexOfPointer>(interpretBrainfuckWithDebug(sourceCode));
}

constexpr static auto getFinalMemoryBlockStateForProgram(const std::string_view &sourceCode) {
    return std::get<indexOfMemoryBlock>(interpretBrainfuckWithDebug(sourceCode));
}

static_assert(generateLoopLookupTable("[]") == LoopLut{{0, 1}});
static_assert(generateLoopLookupTable("[[]]") == LoopLut{{1, 2}, {0, 3}});
static_assert(generateLoopLookupTable("[[][]]") == LoopLut{{1, 2}, {3, 4}, {0, 5}});

static_assert(findMatchingBracket(LoopLut{{0, 1}}, 0) == 1);
static_assert(findMatchingBracket(LoopLut{{0, 1}}, 1) == 0);

static_assert(getFinalPointerPositionForProgram(("")) == 0);                                  // default case
static_assert(getFinalPointerPositionForProgram(("<")) == std::numeric_limits<size_t>::max());// rolls around
static_assert(getFinalPointerPositionForProgram((">")) == 1);
static_assert(getFinalPointerPositionForProgram((">>")) == 2);
static_assert(getFinalPointerPositionForProgram((">><")) == 1);

static_assert(getFinalMemoryBlockStateForProgram((">>>+"))[3] == 1);// the memory has the correct data.
static_assert(getFinalMemoryBlockStateForProgram(("++"))[0] == 2);
static_assert(getFinalMemoryBlockStateForProgram((">++"))[1] == 2);
static_assert(getFinalMemoryBlockStateForProgram((">>++"))[2] == 2);
static_assert(getFinalMemoryBlockStateForProgram((">>>++-"))[3] == 1);

static_assert(interpretBrainfuck("++++.")[0] == 4);
static_assert(interpretBrainfuck("++++.>++++++.")[0] == 4);
static_assert(interpretBrainfuck("++++.>++++++.")[1] == 6);

static_assert(interpretBrainfuck(("+[----->+++<]>+.")) == "h");
constexpr static std::string_view helloWorld = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
static_assert(interpretBrainfuck(helloWorld) == "Hello World!\n");

constexpr static std::string_view fib = "+++++++++++\n"
                                        ">+>>>>++++++++++++++++++++++++++++++++++++++++++++\n"
                                        ">++++++++++++++++++++++++++++++++<<<<<<[>[>>>>>>+>\n"
                                        "+<<<<<<<-]>>>>>>>[<<<<<<<+>>>>>>>-]<[>++++++++++[-\n"
                                        "<-[>>+>+<<<-]>>>[<<<+>>>-]+<[>[-]<[-]]>[<<[>>>+<<<\n"
                                        "-]>>[-]]<<]>>>[>>+>+<<<-]>>>[<<<+>>>-]+<[>[-]<[-]]\n"
                                        ">[<<+>>[-]]<<<<<<<]>>>>>[+++++++++++++++++++++++++\n"
                                        "+++++++++++++++++++++++.[-]]++++++++++<[->-<]>++++\n"
                                        "++++++++++++++++++++++++++++++++++++++++++++.[-]<<\n"
                                        "<<<<<<<<<<[>>>+>+<<<<-]>>>>[<<<<+>>>>-]<-[>>.>.<<<\n"
                                        "[-]]<<[>>+>+<<<-]>>>[<<<+>>>-]<<[<+>-]>[<+>-]<<<-]";

// this is correct, but there's too much recursion and the compiler doesn't like it!
// static_assert(interpretBrainfuck(fib) == "1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89");

