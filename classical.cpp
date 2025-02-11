#include <cstdio>

struct Shape
{
    Shape()                        = default;
    Shape(const Shape&)            = delete;
    Shape(Shape&&)                 = delete;
    Shape& operator=(const Shape&) = delete;
    Shape& operator=(Shape&&)      = delete;
    virtual ~Shape()               = default;
    virtual void print()           = 0;
};

struct Octogon : Shape
{
    Octogon()                          = default;
    Octogon(const Octogon&)            = delete;
    Octogon(Octogon&&)                 = delete;
    Octogon& operator=(const Octogon&) = delete;
    Octogon& operator=(Octogon&&)      = delete;
    virtual ~Octogon()                 = default;

    virtual void print() override
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
};

struct Square : Shape
{
    Square()                         = default;
    Square(const Square&)            = delete;
    Square(Square&&)                 = delete;
    Square& operator=(const Square&) = delete;
    Square& operator=(Square&&)      = delete;
    virtual ~Square()                = default;

    virtual void print() override
    {
        std::puts(R"(
______
|     |
|     |
|     |
______
)");
    }
};

// Doesn't "know about" Shape, and so cannot polymorphize as a Shape
// despite implementing the same interface
// (you could make a wrapper class that *does* inherit from Shape and forwards calls to a contained Line)
struct Line
{

    Line()                       = default;
    Line(const Line&)            = default;
    Line(Line&&)                 = default;
    Line& operator=(const Line&) = default;
    Line& operator=(Line&&)      = default;
    ~Line()                      = default;

    void print()
    {
        std::puts(R"(
_______________________________
)");
    }
};
