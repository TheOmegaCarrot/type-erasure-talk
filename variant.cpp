#include <cstdio>
#include <variant>

struct Octogon
{
};

void print(Octogon)
{
    std::puts(R"(
    ---
   /   \
  /     \
 |       |
 |       |
  \     /
   \   /
    ---
)");
}

struct Square
{
};

void print(Square)
{
    std::puts(R"(
______
|     |
|     |
|     |
______
)");
}

struct Line
{
};

void print(Line)
{
    std::puts(R"(
_______________________________
)");
}

using Shape = std::variant<std::monostate, Octogon, Square, Line>;

void print(std::monostate)
{
    std::puts("");
}

struct
{
    static void operator()(auto arg)
    {
        print(arg);
    }
} do_print;

int main()
{
    Shape shape = Octogon {};
    std::visit(
      [](auto arg)
      {
          print(arg);
      },
      shape
    );

    Shape shape2 = Square {};
    std::visit(do_print, shape2);

    shape = shape2;
    std::visit(do_print, shape);
}
