#include <cstdio>
#include <stdexcept>
#include <utility>

struct Octogon
{
    void print()
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

struct Square
{
    void print()
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

template <typename T>
concept Shapelike = requires(T t) { t.print(); };

class Shape
{

    struct Abstract_Shape
    {
        Abstract_Shape()                                 = default;
        Abstract_Shape(const Abstract_Shape&)            = delete;
        Abstract_Shape(Abstract_Shape&&)                 = delete;
        Abstract_Shape& operator=(const Abstract_Shape&) = delete;
        Abstract_Shape& operator=(Abstract_Shape&&)      = delete;
        virtual ~Abstract_Shape()                        = default;

        virtual void print()            = 0;
        virtual Abstract_Shape* clone() = 0;
    };

    template <Shapelike T>
    struct Concrete_Shape final : Abstract_Shape
    {
        Concrete_Shape()                                 = default;
        Concrete_Shape(const Concrete_Shape&)            = delete;
        Concrete_Shape(Concrete_Shape&&)                 = delete;
        Concrete_Shape& operator=(const Concrete_Shape&) = delete;
        Concrete_Shape& operator=(Concrete_Shape&&)      = delete;
        virtual ~Concrete_Shape() final override         = default;

        Concrete_Shape(T wrapped)
                : m_wrapped {wrapped}
        {
        }

        T m_wrapped;

        virtual void print() final override
        {
            m_wrapped.print();
        }

        virtual Concrete_Shape* clone() final override
        {
            return new Concrete_Shape {m_wrapped};
        }
    };

    Abstract_Shape* m_ptr;

public:

    Shape() = default;

    Shape(const Shape& src)
            : m_ptr {src.m_ptr->clone()}
    {
    }

    Shape(Shape&& src)
            : m_ptr {src.m_ptr}
    {
        src.m_ptr = nullptr;
    }

    Shape& operator=(const Shape& src)
    {
        if ( &src == this )
            return *this;

        delete m_ptr;
        m_ptr = src.m_ptr->clone();
        return *this;
    }

    Shape& operator=(Shape&& src)
    {
        if ( &src == this )
            return *this;

        delete m_ptr;
        m_ptr = nullptr;
        std::swap(m_ptr, src.m_ptr);
        return *this;
    }

    ~Shape()
    {
        delete m_ptr;
    }

    Shape(Shapelike auto arg)
            : m_ptr {new Concrete_Shape {arg}}
    {
    }

    Shape& operator=(Shapelike auto arg)
    {
        delete m_ptr;
        m_ptr = new Concrete_Shape {arg};
        return *this;
    }

    void print()
    {
        if ( ! m_ptr )
            throw std::runtime_error {"Null access"};

        m_ptr->print();
    }

    bool has_value()
    {
        return m_ptr != nullptr;
    }
};

struct Line
{
    void print()
    {
        std::puts(R"(
_______________________________
)");
    }
};

static_assert(Shapelike<Line>);

int main()
{
    Shape shape = Octogon {};
    shape.print();

    shape = Line {};
    shape.print();

    Shape shape2 = Square {};
    shape2.print();

    shape2 = shape;
    shape2.print();

    Shape i_am_null {};
    if ( not i_am_null.has_value() )
        std::puts("I am null");
    else
        std::puts("This should not appear");
}
