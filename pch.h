#define DEBUG 1

#if DEBUG==1
#include <iostream>
#define DEBUG_PRINT(x) std::cout << x << std::endl
#else
#define DEBUG_PRINT()
#endif
