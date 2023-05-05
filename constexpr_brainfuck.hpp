// A constexpr BrainFuck interpreter
// Created by Jamie Pond on 04/05/2023.
#pragma once

using LoopLut = std::vector<std::pair<size_t, size_t>>;
constexpr static auto generateLoopLookupTable(const std::string_view &sourceCode) {
    LoopLut loopLut{};
    std::vector<size_t> bracketStack{};
    for (int index = 0; const auto &character: sourceCode) {
        if (character == '[') {
            bracketStack.emplace_back(index);
        } else if (character == ']') {
            const auto indexOfOpeningBracket = bracketStack.back();
            bracketStack.pop_back();
            loopLut.push_back({indexOfOpeningBracket, index});
        }
        index++;
    }
    assert(bracketStack.empty());
    return loopLut;
}

constexpr static size_t findMatchingBracket(const LoopLut &loopLut, const size_t indexOfClosingBracket) {
    const auto found = std::find_if(loopLut.begin(), loopLut.end(), [&](const auto &loop) {
        return loop.first == indexOfClosingBracket || loop.second == indexOfClosingBracket;
    });
    assert(found != loopLut.end());
    return (found->first == indexOfClosingBracket) ? found->second : found->first;
}

constexpr static size_t defaultMemorySize = 256;
using Memory = std::array<char, defaultMemorySize>;
using Instruction = void (*)(size_t &, Memory &, std::string &, size_t &, const LoopLut &);

constexpr static std::initializer_list<std::pair<char, Instruction>> instructionList = {
        {'<', [](auto &dataPointer, auto &, auto &, auto &, auto &) { dataPointer--; }},
        {'>', [](auto &dataPointer, auto &, auto &, auto &, auto &) { dataPointer++; }},
        {'+', [](auto &dataPointer, auto &memory, auto &, auto &, auto &) { memory[dataPointer] += 1; }},
        {'-', [](auto &dataPointer, auto &memory, auto &, auto &, auto &) { memory[dataPointer] -= 1; }},
        {'.', [](auto &dataPointer, auto &memory, auto &output, auto &, auto &) { output += memory[dataPointer]; }},
        {'[', [](auto &dataPointer, auto &memory, auto &, auto &instructionPointer, auto &loopLookupTable) {
             if (memory[dataPointer] == 0) {
                 instructionPointer = findMatchingBracket(loopLookupTable, instructionPointer);
             }
         }},
        {']', [](auto &dataPointer, auto &memory, auto &, auto &insPtr, auto &loopLut) {
             insPtr = findMatchingBracket(loopLut, insPtr) - 1;
         }},
};

constexpr static auto interpretBrainfuckCore(const auto &sourceCode, auto &memory, auto &dataPointer, auto &output) {
    const auto loopLookupTable = generateLoopLookupTable(sourceCode);
    for (size_t instructionPointer = 0; instructionPointer < sourceCode.size(); ++instructionPointer) {
        const auto character = sourceCode[instructionPointer];
        for (auto &ins: instructionList) {
            if (ins.first == character) {
                ins.second(dataPointer, memory, output, instructionPointer, loopLookupTable);
                break;
            }
        }
    }
}

constexpr static auto interpretBrainfuck(const std::string_view &sourceCode) {
    Memory memory{};
    size_t dataPointer = 0;
    std::string output = "";
    interpretBrainfuckCore(sourceCode, memory, dataPointer, output);
    return output;
}

#include "tests.hpp"