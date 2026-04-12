#pragma once
#include <iostream>
#include <utility>
#include <any>
#include <string>
#include "string.hpp"
#include "types.hpp"
namespace bedrock {

    template <typename... Args>
    void print(Args&&... objs) {
        (std::cout << ... << std::forward<Args>(objs));
    }

    template <typename... Args>
    void println(Args&&... objs) {
        print(std::forward<Args>(objs)..., '\n');
    }

    template <typename T>
    void input(T& o) {
        std::cin >> o;
    }

    string input(string &s) {
        std::string line;
        std::getline(std::cin >> std::ws, line);
        s += line.c_str();
        return s;
    }

    string input() {
        string s;
        std::string line;
        std::getline(std::cin, line);
        s = line.c_str();
        return s;
    }

}