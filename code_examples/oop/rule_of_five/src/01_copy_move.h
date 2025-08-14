#include <iostream>
#include <string>
#include <utility> // For std::move
#include <vector>

struct Tracer {
    std::string name;

    Tracer(std::string n) : name(std::move(n)) {
        std::cout << "  " << name << ": Constructor\n";
    }

    ~Tracer() {
        std::cout << "  " << name << ": Destructor\n";
    }

    // 1. Copy Constructor
    Tracer(const Tracer& other) : name(other.name) {
        std::cout << "  " << name << ": COPY CONSTRUCTOR from " << other.name << "\n";
    }

    // 2. Copy Assignment
    Tracer& operator=(const Tracer& other) {
        std::cout << "  " << name << ": COPY ASSIGNMENT from " << other.name << "\n";
        name = other.name;
        return *this;
    }

    // 3. Move Constructor
    Tracer(Tracer&& other) noexcept {
        std::cout << "  " << other.name << ": MOVE CONSTRUCTOR (into a new object)\n";
        name = std::move(other.name);
    }

    // 4. Move Assignment
    Tracer& operator=(Tracer&& other) noexcept {
        std::cout << "  " << this->name << ": MOVE ASSIGNMENT from " << other.name << "\n";
        name = std::move(other.name);
        return *this;
    }
};

// A helper function that creates and returns a Tracer object
Tracer createTracer(const std::string& name) {
    return Tracer(name);
}

class Program_01_CopyMove
{
public:
    void Run() 
    {
        exercise_1_1();
        exercise_1_2();
        exercise_2_1();
        exercise_2_2();
        exercise_2_3();
        exercise_3_1();
    }

private:

    // Question: What special member function is called to create t2? 
    void exercise_1_1()
    {
        std::cout << "\n🚀 Exercise 1.1\n";
        Tracer t1("t1");
        Tracer t2 = t1;
        std::cout << "...end of scope\n";
    }

    // Question: What special member function is called on the line t2 = t1;?
    void exercise_1_2()
    {
        std::cout << "\n🚀 Exercise 1.2\n";
        Tracer t1("t1");
        Tracer t2("t2");
        t2 = t1;
        std::cout << "...end of scope\n";
    }

    // Question: The createTracer function returns a temporary object (an rvalue). What special member function is called to initialize t3?
    void exercise_2_1()
    {
        std::cout << "\n🚀 Exercise 2.1\n";
        Tracer t3 = createTracer("temp");
        std::cout << "...end of scope\n";
    }

    // Question: What does std::move do to create t4?
    void exercise_2_2()
    {
        std::cout << "\n🚀 Exercise 2.2\n";
        Tracer t1("t1");
        Tracer t4 = std::move(t1);
        std::cout << "State of t1: name = '" << t1.name << "'\n";
    }

    // Question: Which assignment operator is called here? Copy or Move? Why?
    void exercise_2_3()
    {
        std::cout << "\n🚀 Exercise 2.3 ---\n";
        Tracer t5("t5");
        t5 = createTracer("temp");
    }

    // Question: What happens at lines // A and // B? Which constructor is used in each push_back call and why?
    void exercise_3_1()
    {
        std::cout << "\n🚀 Exercise 3.1 ---\n";
        std::vector<Tracer> vec;
        Tracer t1("t1");

        std::cout << "Pushing back t1 (lvalue)...\n";
        vec.push_back(t1); // A

        std::cout << "Pushing back temporary (rvalue)...\n";
        vec.push_back(createTracer("temp")); // B
    }
};
