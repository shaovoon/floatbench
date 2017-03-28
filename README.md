# C++ String to Float Benchmark

Looping 1 million times

```
               atof:  180ms
       lexical_cast: 1309ms
 std::istringstream: 1395ms <== Probably unfair comparison since istringstream instaniate a string
          std::stod:  255ms
        std::strtod:  186ms
         crack_atof:   12ms <== Cannot convert float string in scientific notation.
          fast_atof:   10ms <== do not use this one because conversion is not correct.
       boost_spirit:   52ms <== reported to be inaccurate in some case
       google_dconv:  121ms
```
