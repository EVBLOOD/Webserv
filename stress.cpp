#include <stdlib.h>
#include <unistd.h>
#include <iostream>

int main(int ac, char** av) {
    assert(ac == 2);
    for (size_t x = 0; x < 4; ++x) {
        int pid = fork();
        if (pid == 0) {
            for (size_t i = 0; i != std::stoul(av[1]); ++i) {
                int first = system(
                    "curl http://localhost:8080 1>> debug_file 2>> debug_file");
                int second = system(
                    "curl -H \"Connection:keep-alive\" http://localhost:8081 "
                    "1>> debug_file 2>> debug_file");
                int third = system(
                    "curl -H \"Connection:keep-alive\" http://localhost:9090 "
                    "1>> debug_file 2>> debug_file");
                int forth = system(
                    "curl -H \"Connection:keep-alive\" http://localhost:9091 "
                    "1>> debug_file 2>> debug_file");
                if (first || second || third || forth) {
                    std::cerr << "test failed at " << i << " in the process "
                              << x << std::endl;
                    exit(1);
                }
            }
            exit(0);
        } else if (pid == -1) {
            std::cerr << "test failed because fork failed x == " << x
                      << std::endl;
            exit(1);
        }
    }
    int error = 0;
    for (size_t i = 0; i < 4; ++i) {
        wait(&error);
        if (error) {
            std::cerr << "test failed" << std::endl;
            exit(error);
        }
    }
    std::cerr << "test passed" << std::endl;
}
