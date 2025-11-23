#include <iostream>
#include <string>
#include <utility> // for std::move
#include <vector>

struct Tracer {
    std::string name;
    int* resource; // simple pointer resource for demo purposes

    // Constructor
    Tracer(std::string n) : name(std::move(n)), resource(new int(42)) {
        std::cout << "  " << name << ": Constructor\n";
    }

    // Desctructor
    ~Tracer() {
        std::cout << "  " << name << ": Destructor\n";
        delete resource;
    }

    // Copy Constructor
    Tracer(const Tracer& other)
        : name(other.name), resource(new int(*other.resource))
    {
        std::cout << "  " << name << ": COPY Constructor from " << other.name << '\n';
    }

    // Copy Assignment Operator
    Tracer& operator=(const Tracer& other) {
        if (this == &other) 
            return *this;
        std::cout << "  " << name << ": COPY Assignment from " << other.name << '\n';
        int* new_res = new int(*other.resource);  // allocate first
        delete resource;
        resource = new_res;
        name = other.name;
        return *this;
    }

    // Move Constructor
    Tracer(Tracer&& other) noexcept
    {
        // capture the original name of the source object before moving its contents.
        std::string original_other_name = other.name;

        // move data
        this->name = std::move(other.name);
        this->resource = other.resource;

        // print
        std::cout << "  " << this->name << ": MOVE Constructor from " << original_other_name << '\n';
        
        // nullify the source object's resource pointer and modify its name for tracking its state.
        other.resource = nullptr;
        other.name += original_other_name + " [moved]"; 
    }

    // Move Assignment Operator
    Tracer& operator=(Tracer&& other) noexcept {
        if (this == &other) 
            return *this;
        std::string original_other_name = other.name;
        std::cout << "  " << name << ": MOVE Assignment from " << other.name << '\n';
        delete resource;
        resource = other.resource;
        name = other.name;
        other.resource = nullptr;
        other.name += original_other_name + " [moved]";
        return *this;
    }
};

// A helper function that creates and returns a Tracer object
Tracer createTracer(const std::string& name) {
    return Tracer(name);
}

Tracer createTracerNamed(const std::string& name) {
    Tracer temp(name);
    return temp;
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
        exercise_2_4();
        exercise_3_1();
        exercise_4_1();
        exercise_5_1();
        exercise_6_1();
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

    void exercise_2_2()
    {
        std::cout << "\n🚀 Exercise 2.2\n";
        Tracer t3 = createTracerNamed("temp");
        std::cout << "...end of scope\n";
    }

    // Question: What does std::move do to create t4?
    void exercise_2_3()
    {
        std::cout << "\n🚀 Exercise 2.3\n";
        Tracer t1("t1");
        Tracer t4 = std::move(t1);
        std::cout << "State of t1: name = '" << t1.name << "'\n";
    }

    // Question: Which assignment operator is called here? Copy or Move? Why?
    void exercise_2_4()
    {
        std::cout << "\n🚀 Exercise 2.4\n";
        Tracer t5("t5");
        t5 = createTracer("temp");
    }

    // Question: What happens at lines // A and // B? Which constructor is used in each push_back call and why?
    void exercise_3_1()
    {
        std::cout << "\n🚀 Exercise 3.1\n";
        std::vector<Tracer> vec;
        Tracer t1("t1");

        std::cout << "Pushing back t1 (lvalue)...\n";
        vec.push_back(t1); // A

        std::cout << "Pushing back temporary (rvalue)...\n";
        vec.push_back(createTracer("temp")); // B
    }

    // Question: A function returns a Tracer by value and we do Tracer t = createTracer("x");. 
    // With full optimization, how many calls do we see to the move constructor? Why?
    void exercise_4_1() {
        std::cout << "\n🚀 Exercise 4.1\n";
        Tracer t = createTracer("temp");
        std::cout << "...end\n";
    }

    // Question: We have two existing Tracer objects a and b. We write a = std::move(b);. 
    // Which of the five special member functions is invoked here, and what happens to the old resource owned by a?
    void exercise_5_1() {
        std::cout << "\n🚀 Exercise 5.1\n";
        Tracer a("Alice");
        Tracer b("Bob");
        std::cout << "Before move: a = " << a.name << ", b = " << b.name << "\n";
        a = std::move(b);
        std::cout << "After move:  a = " << a.name << ", b = " << b.name << "\n";
    }

    // Question: When a std::vector<Tracer> grows and reallocates its buffer, what happens to the already-stored elements? 
    // Which special member function is used to relocate them, and why does noexcept matter here?
    void exercise_6_1() {
        std::cout << "\n🚀 Exercise 6.1\n";
        std::vector<Tracer> vec;
        vec.reserve(4);

        std::cout << "Adding 4 elements...\n";
        vec.emplace_back("v0");
        vec.emplace_back("v1");
        vec.emplace_back("v2");
        vec.emplace_back("v3");

        std::cout << "Forcing reallocation...\n";
        vec.emplace_back("v4");

        std::cout << "Size of vector: " << vec.size() << ", capacity of vector: " << vec.capacity() << std::endl;
    }
};