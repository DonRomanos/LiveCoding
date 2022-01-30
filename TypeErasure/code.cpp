// References
// https://quuxplusone.github.io/blog/2019/03/18/what-is-type-erasure/
// https://www.youtube.com/watch?v=jKt6A3wnDyI&t=1668s


// From the Video:

// Type erasure is not:
// a void ptr
// pointer to a base class 
// a std::variant


// When we need "type erasure"
// We want to invoke a behavior on unrelated types.
// Behaviors or affordances can be something like render or serialize

#include <vector>
#include <iostream>
#include <variant>
#include <type_traits>
#include <memory>
#include <algorithm>

struct Spot
{
    int x;
    int y;
    int radius;
};

struct SpotCandidate
{
    std::vector<Spot> neighbors;
};

// Behaviors we are currently interested in: Rendering
// Pseudocode:
// renderable = {Spot{...}, SpotCandidate{...}, ...}
// for all things in renderable:
//   renderable.render();

// Attempt 1: 
struct Renderable
{
    virtual void render() = 0;
};

struct ValueType : public Renderable
{
    // ...
};

// Dont ever do this please!
// We lose value semantics
// It does not scale, what if we want another behavior or implementation?
// We run into dependency issues really fast OpenGLCircle, MetalCircle does not sound great


// Attempt 2:
// std::variant<Spot, SpotCandidate> renderable_thing;
// template<class> inline constexpr bool always_false_v = false;
// std::visit([](auto&& arg) {
//     using T = std::decay_t<decltype(arg)>;
//     if constexpr (std::is_same_v<T, Spot>)
//         std::cout << "int with value " << arg << '\n';
//     else if constexpr (std::is_same_v<T, SpotCandidate>)
//         std::cout << "long with value " << arg << '\n';
//     else 
//         static_assert(always_false_v<T>, "non-exhaustive visitor!");
// }, renderable_thing);

// You have to change the variant whenever you need to add new types!
// -> Breaking abi change!
// If you use visit in different places you have to touch all of them when you add a new type 


// What is type erasure:
// templated constructor
// completely non virtual interface

// lets do this:
void render(const Spot& x)
{
    std::cout << "(" << x.x << "," << x.y << ")" << std::endl;
};

void render(const SpotCandidate& y)
{
    for (const auto& spot : y.neighbors)
    {
        render(spot);
    }
};


class OpenGlRenderer
{
public:
    template<typename T>
    void add(const T& renderable)
    {
        m_renderables.push_back(std::make_unique<Renderable<T>>(renderable));
    }

    void render()
    {
        for (const auto& renderable : m_renderables)
        {
            renderable->render();
        }
    }

private:
    struct RenderableI
    {
        virtual ~RenderableI() = default;
        virtual void render() = 0;
    };

    template<typename T>
    struct Renderable final : public RenderableI
    {
        Renderable(const T& x) : m_concrete_renderable(x) {}
        void render() override
        {
            ::render(m_concrete_renderable);
        }

        T m_concrete_renderable;
    };

    std::vector<std::unique_ptr<RenderableI> > m_renderables;
};

int main()
{
    OpenGlRenderer renderer;

    renderer.add(Spot{ 3,4 });
    renderer.add(SpotCandidate{ {Spot{7,9},Spot{5,6}} });

    renderer.render();
};

// Who calls me a liar?

// Where is the templated constructor???
// -> add method

// if you want a collection of object satisfying multiple things
// then youd use a wrapper with a templated constructor, e.g.

class Shape
{
    Shape(const auto& shape) : pimpl(std::make_unique<Affordances>(shape)) {}

    friend void doX(const Shape& shape)
    {
        shape.pimpl->doX();
    }

    // Make copyable using prototype pattern...

private:
    struct AffordancesI
    {
        virtual ~AffordancesI() = default;
        virtual void doX() = 0;
    };

    template<typename T>
    struct Affordances : public AffordancesI
    {
        void doX() override {};

        T actual;
    };

    std::unique_ptr<AffordancesI> pimpl;
};

//std::vector<Shape> shapes{Circle,Square,...}
// for(const auto& shape : shapes)
// {
//   doX(shape);   
// }