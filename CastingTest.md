## Dynamic Cast Performance Testing

This file compares the performance of functions which use the dynamic_cast conversion on one of their parameters. Specifically, it looks to see how much performance gain there is when a function which takes a pointer as a parameter and performs a cast on that pointer is redesigned to expect a pointer which has already been cast before being called. 

This test is based on Graphitti's `AllSTDPSynapses::advanceEdge` function, which is called many times per timestep in a loop in `AllEdges::advanceEdges`. The `advanceEdge` function performs a dynamic_cast on one of its passed parameters, a pointer to a base neuron class. It was observed that some performance improvement could be made by casting the pointer once in `advanceEdges` and changing `advanceEdge` to accept the already cast pointer.

The test structures a hierarchy of base class and subclasses named after Graphitti's neuro vertex classes. Pointers are cast to the `allSpikingNeurons` class to mimic the actual casting done in `advanceEdge`. 



1. The pointer to the base class is passed to the function. The function then casts the pointer to a derived class and runs a class method of the derived class.
2. The pointer to the base class is cast to a derived class and passed to the function. The function then runs a class method of the derived class.

a function called in loop running many thousands of times has a parameter which is dynamically cast within the function (callee casting). The alternative is to cast the object once before the loop, change the function to accept the cast object, and pass the cast object (caller casting).

## Methodology
test methods do as little as possible so that the execution time is mostly just the time eaten by the cast.


## Results

The level of inheritance that the derived class started at made a significant impact. Casting from the lowest-level derived class is consistently slower than from a higher-level derived class. Callee casting times are over twice as slow, but caller casting are usually within a few milliseconds of each other.

Running with different levels of compiler optimization (-O0, -O1, etc) had no obvious consistent impact on running time. 

## Sample output

#### Running test1:
```
Casting 4 levels from base class:
  Callee casting time:  3333 ms
  Caller casting time:   473 ms
  Difference in time:   2860 ms
  Casting in the caller method is 7.04651 times faster than casting in the callee method

Casting 2 levels from base class:
  Callee casting time:  1380 ms
  Caller casting time:   473 ms
  Difference in time:    907 ms
  Casting in the caller method is 2.91755 times faster than casting in the callee method
  ```

  #### Running test2:
  ```
  Casting 4 levels from base class:
  Callee casting time:  3572 ms
  Caller casting time:   789 ms
  Difference in time:   2783 ms
  Casting in the caller method is 4.52725 times faster than casting in the callee method

Casting 2 levels from base class:
  Callee casting time:  1620 ms
  Caller casting time:   788 ms
  Difference in time:    832 ms
  Casting in the caller method is 2.05584 times faster than casting in the callee method
  ```