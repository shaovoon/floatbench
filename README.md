# C++ String to Float Benchmark

Looping 1 million times

```
               atof:  142ms
       lexical_cast:  968ms
 std::istringstream: 1013ms <== Probably unfair comparison since istringstream instaniate a string
          std::stod:  183ms
         crack_atof:   11ms <== Cannot convert float string in scientific notation.
          fast_atof:    9ms <== do not use this one because conversion is not correct.
       boost_spirit:   41ms
```
