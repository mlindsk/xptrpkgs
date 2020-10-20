Testing External Pointers
================

<!-- README.md is generated from README.Rmd. Please edit that file -->
This package serves as an extension to the question asked here:

<https://stackoverflow.com/questions/64389299/assigning-rcppxptr-on-the-r-side?noredirect=1#comment113930746_64389299>

The package comes with two functions, `make` and `release`, which are C++ functions. `make(i, j)` returns an *external pointer* to a `arma::Mat<short>` object with `i` rows and `j` columns and `release` frees the memory of an external pointer. For example:

``` r
x <- make(1e3, 1e6)
```

which takes up approximately 1.9Gb on my machine (using `free -m` on linux). We can then free the memory of `x` by

``` r
release(x)
```

We can also register a finalizer on the `R` side:

``` r
f <- function(x) release(x)
reg.finalizer(x, f)
#> NULL
```

This will ensure, that `x` is garbage collected when `gc()` is run. However, `R` does not "see" the actual memory footprint of `x`:

``` r
format(object.size(x), units = "b", standard = "auto", digits = 1L)
#> [1] "64 bytes"

# cleaning
rm(x)
gc()
#>          used (Mb) gc trigger (Mb) max used (Mb)
#> Ncells 494290 26.4    1091657 58.4   641200 34.3
#> Vcells 920185  7.1    8388608 64.0  1752832 13.4
```

Thus, creating external pointers does not set aside enough memory if we hope these to be garbage collected at some point.

However, for very large objects, there is a huge benefit staying on the `C++` side using external pointers, since similar objects in `R` takes up *much more* memory. The equvivalent to `x` takes:

``` r
y <- matrix(1L, nrow = 1000L, ncol = 1e6)
format(object.size(y), units = "Gb", standard = "auto", digits = 1L)
#> [1] "3.7 Gb"

# cleaning
rm(y)
gc()
#>          used (Mb) gc trigger   (Mb)  max used   (Mb)
#> Ncells 494320 26.4    1091657   58.4    641200   34.3
#> Vcells 920039  7.1  482557802 3681.7 500950983 3822.0
```

Of course this is also a result of representing the matrix with `short` integers on C++; but that is the exact benefit we hope to exploit.

Is there, by any means, a way to achive the above? That is, can we trick `R` into calling `gc()` and treat external pointers similar to ordinary `R` objects? That is, can we design us out of the problem. If so, I have no idea where to start and I welcome any advise.
