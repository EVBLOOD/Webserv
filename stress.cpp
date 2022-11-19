#include <stdlib.h>
#include <iostream>

int main() {
    for (size_t i = 0; i != (size_t)-1; ++i) {
        std::cout << i << '\n';
        if (system("curl http://localhost:8080") != 0 ||
            system("curl http://localhost:8081") != 0) {
            std::cout << "test failed" << std::endl;
            exit(1);
        }
    }
    std::cout << "test passed" << std::endl;
}
