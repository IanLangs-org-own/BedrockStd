#include "types.hpp"
#include "io.hpp"
#include "ptrs.hpp"¡
#include "string.hpp"
#include "assert.hpp"
#include "args.hpp" //move, forward
int main() {
    bedrock::println("hola mundo");
    bedrock::unique_ptr<int> p(new int(1));
    bedrock::println(*p);
    bedrock::shared_ptr<char> p2(new char[11]{'h', 'o', 'l', 'a', ' ', 'm', 'u', 'n', 'd', 'o', '\0'}, bedrock::array_deleter_fn<char>);
    bedrock::println(p2);
    bedrock::print(1,2,3,4,p2,'\n');
    bedrock::string s;
    s = "Hola mundo";
    bedrock::println("const char (s.data)[", s.size()+1, "] = {");
    for (const char& c : s) {
        bedrock::println("    '", c, "',");
    }
    bedrock::println("    '", "\\0", "'\n}");
    bedrock::print("cual es tu nombre? ");
    bedrock::string name = bedrock::input();
    bedrock::println("hola ", name, '!');
    Vassert("cond" || false, "unreachable");
    if (false) unreachable;

    return 0;
}