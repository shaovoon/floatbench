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

Benchmark Version 1.1 uses crack_atof contributed by Tian Bo which fixes scientific notation conversion problem and and improves performance by 10%. Now crack_atof is as fast as fast_atof.

```
               atof:  230ms
       lexical_cast: 1245ms
 std::istringstream: 1335ms
          std::stod:  260ms
        std::strtod:  235ms
         crack_atof:   11ms
          fast_atof:   11ms
       boost_spirit:   41ms
       google_dconv:   89ms
```