# floatbench
String to Float Benchmark

```
               atof:  146ms
       lexical_cast:  980ms
 std::istringstream: 1034ms
          std::stod:  195ms
         crack_atof:   11ms
          fast_atof:    9ms <== do not use this one because conversion is not correct.
       boost_spirit:   52ms
```
