#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <stdio.h>

using namespace std;

typedef chrono::high_resolution_clock Clock;

int totalEdgeCount_ = 100000000;
int arrayLength = 2;
int* r_array;

// base class
class IAllVertices {
public:
    virtual ~IAllVertices() {}

    virtual int test1() = 0;
    virtual int test2() = 0;
};

// derived class
class AllVertices : public IAllVertices {
public:
    virtual ~AllVertices() {};
};

// derived class. the class that gets cast to
class AllSpikingNeurons : public AllVertices {
public:
    virtual ~AllSpikingNeurons() {};

    // trivial test method
    virtual int test1() override {
        return 1;
    };

    // less trivial test method
    virtual int test2() override {
        int count = 0;
        for (int i = 0; i < arrayLength; i++) {
            count += r_array[i];
        }
        return count;
    }
};

// lower level derived class
class AllIFNeurons : public AllSpikingNeurons {
public:
    virtual ~AllIFNeurons() {};
};

// lowest level derived class
class AllLIFNeurons : public AllIFNeurons {
public:
    virtual ~AllLIFNeurons() {};
};



// caller casting
void advanceEdge(AllSpikingNeurons* neurons) {
    int result = neurons->test2();
}
// casting is done before passing in
void advanceEdges(AllSpikingNeurons* vertices) {
    for (int i = 0; i < totalEdgeCount_; i++) { advanceEdge(vertices); }
}

// callee casting
void advanceEdge(IAllVertices* neurons) {
    AllSpikingNeurons* spNeurons = dynamic_cast<AllSpikingNeurons*>(neurons);
    int result = spNeurons->test2();
}
// casting is done each time function is called
void advanceEdges(IAllVertices* vertices) {
    for (int i = 0; i < totalEdgeCount_; i++) { advanceEdge(vertices); }
}

// create, populate, and return an array of random numbers
int* makeRandomArray() {
    // fill random array
    random_device rd;  // seeds the rng
    mt19937 gen(rd()); // mersenne twister seeded with rd()
    uniform_int_distribution<> distrib(0, (arrayLength - 1));  // range of output
    int* randomArray = new int[arrayLength];
    for (unsigned int i = 0; i < arrayLength; ++i)
        randomArray[i] = distrib(gen);
    return randomArray;
}


int main() {
    // instantiate lower level class assigned to base class pointer
    IAllVertices* parentObject = new AllLIFNeurons();   // 4 levels from base class
    // IAllVertices* parentObject = new AllSpikingNeurons();  // 2 levels from base class
    // dynamic cast to class with the defined methods
    AllSpikingNeurons* castObject = dynamic_cast<AllSpikingNeurons*>(parentObject);

    // populate random array to be used by test methods
    r_array = makeRandomArray();

    // time the method's execution when callee function casts each time it's called
    auto clockStart = Clock::now();
    advanceEdges(parentObject);
    auto clockEnd = Clock::now();
    auto calleeTime = chrono::duration_cast<chrono::milliseconds>(clockEnd - clockStart).count();
    cout << "Callee casting took " << setw(5) << calleeTime << " ms" << endl;

    // time the method's execution when caller function casts once and passes the cast object
    clockStart = Clock::now();
    advanceEdges(castObject);
    clockEnd = Clock::now();
    auto callerTime = chrono::duration_cast<chrono::milliseconds>(clockEnd - clockStart).count();
    cout << "Caller casting took " << setw(5) << callerTime << " ms" << endl;

    cout << "Difference in time: " << setw(5) << calleeTime - callerTime << " ms" << endl;
    cout << "Casting in the caller method is " << (float)calleeTime / callerTime
        << " times faster than casting in the callee method" << endl;

    return 0;
}
