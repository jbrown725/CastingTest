#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <random>
#include <valarray>
#include <stdio.h>
#include <memory>
#include <cstring>


using namespace std;

typedef chrono::high_resolution_clock Clock;

int totalEdgeCount_ = 100000000;
int arrayLength = 10;

// highest level class
class IAllVertices {
public:
    virtual ~IAllVertices() {}

    // trivial test method
    int test() {
        //    cout << "IAllVertices" << endl;
        return 5;
    };

};

// middle class level
class AllVertices : public IAllVertices {
public:
    AllVertices();
    virtual ~AllVertices() {};
};

// lowest class level
class AllSpikingNeurons : public AllVertices {
public:
    AllSpikingNeurons();
    virtual ~AllSpikingNeurons() {};

    // trivial test method
    int test() {
        // cout << "AllSpikingNeurons" << endl;
        return 5;
    };

};


// caller function casts
void advanceEdge(AllSpikingNeurons* neurons) {
    int result = neurons->test();
}

void advanceEdgesCasted(AllSpikingNeurons* vertices) {
    for (int i = 0; i < totalEdgeCount_; i++) {
        advanceEdge(vertices);
    }
}

// callee function casts
void advanceEdge(IAllVertices* neurons) {
    AllSpikingNeurons* spNeurons = dynamic_cast<AllSpikingNeurons*>(neurons);
    int result = spNeurons->test();
}

void advanceEdges(IAllVertices* vertices) {
    for (int i = 0; i < totalEdgeCount_; i++) {
        advanceEdge(vertices);
    }
}

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
    IAllVertices* parentObject = new IAllVertices();
    AllSpikingNeurons* castedObject = dynamic_cast<AllSpikingNeurons*>(parentObject);

    int* randomArray = makeRandomArray();
    // parentObject->randomArray = randomArray;
    // castedObject->setRandomArray(randomArray);
    cout << "Running each test " << totalEdgeCount_ << " times" << endl;

    auto clockStart = Clock::now();
    advanceEdges(parentObject);
    auto clockEnd = Clock::now();
    auto calleeTime = chrono::duration_cast<chrono::milliseconds>(clockEnd - clockStart).count();
    cout << "Callee casting took " << setw(5) << calleeTime << " ms" << endl;

    clockStart = Clock::now();
    advanceEdgesCasted(castedObject);
    clockEnd = Clock::now();
    auto callerTime = chrono::duration_cast<chrono::milliseconds>(clockEnd - clockStart).count();
    cout << "Caller casting took " << setw(5) << callerTime << " ms" << endl;

    cout << "Difference in time: " << setw(5) << calleeTime - callerTime << " ms" << endl;
    cout << "Ratio : "<< (float)calleeTime / callerTime << endl;


    return 0;
}
