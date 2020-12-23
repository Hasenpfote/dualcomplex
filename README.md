

# dualcomplex

---------------------

## About

- Class template for dual complex numbers.

- Header-only.

  

## Compatibility

Supports C++ 11 or higher.

| Compiler | Version           | Remarks |
| -------- | ----------------- | ------- |
| gcc      | 5.5.0 or higher.  |         |
| clang    | 7.0.0 or higher.  |         |
| msvc     | 16.5.4 or higher. |         |

**Older versions of compilers might work as well but they are not tested.**



## usage

```c++
#include <dualcomplex/dualcomplex.h>

int main()
{
    using value_type = double;
    using DC = dcn::DualComplex<value_type>;

    return 0;
}
```



## References

- [Eigen](http://eigen.tuxfamily.org)



## License

This software is released under the CC0 License, see LICENSE.