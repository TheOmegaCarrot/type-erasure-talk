#include <cstdio>
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
concept Has_Print = requires(T t) { t.print(); };

template <typename T>
concept Has_Draw = requires(T t) { t.draw(); };

template <typename T>
concept Shapelike = Has_Print<T> || Has_Draw<T>;

class Shape
{

    struct Shape_Concept
    {
        Shape_Concept()                                = default;
        Shape_Concept(const Shape_Concept&)            = delete;
        Shape_Concept(Shape_Concept&&)                 = delete;
        Shape_Concept& operator=(const Shape_Concept&) = delete;
        Shape_Concept& operator=(Shape_Concept&&)      = delete;
        virtual ~Shape_Concept()                       = default;

        virtual void print()           = 0;
        virtual Shape_Concept* clone() = 0;
    };

    template <typename T>
    struct Shape_Model final : Shape_Concept
    {
        Shape_Model()                              = default;
        Shape_Model(const Shape_Model&)            = delete;
        Shape_Model(Shape_Model&&)                 = delete;
        Shape_Model& operator=(const Shape_Model&) = delete;
        Shape_Model& operator=(Shape_Model&&)      = delete;
        virtual ~Shape_Model() final override      = default;

        Shape_Model(T wrapped)
                : m_wrapped {wrapped}
        {
        }

        T m_wrapped;

        virtual void print() final override
        {
            if constexpr ( Has_Print<T> )
                m_wrapped.print();
            else if constexpr ( Has_Draw<T> )
                m_wrapped.draw();
            else
                std::unreachable();
        }

        virtual Shape_Model* clone() final override
        {
            return new Shape_Model {m_wrapped};
        }
    };

    Shape_Concept* m_ptr;

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
            : m_ptr {new Shape_Model {arg}}
    {
    }

    Shape& operator=(Shapelike auto arg)
    {
        delete m_ptr;
        m_ptr = new Shape_Model {arg};
        return *this;
    }

    void print()
    {
        m_ptr->print();
    }
};

struct Line
{
    void draw()
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
}
