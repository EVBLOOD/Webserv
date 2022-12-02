
// int
// access(const char *path, int mode);
int main() {
    {
        std::ifstream file("no_file");
        std::cerr << file.is_open() << '\n';
        std::cerr << file.fail() << '\n';
        std::cerr << file.rdstate() << '\n';
    }
    std::cout << "******************************\n";
    {
        std::ifstream file("per");
        std::cerr << file.is_open() << '\n';
        std::cerr << file.fail() << '\n';
        std::cerr << file.rdstate() << '\n';
    }
    std::cout << "******************************\n";
    {
        std::ifstream file("valid");
        std::cerr << file.is_open() << '\n';
        std::cerr << file.fail() << '\n';
        std::cerr << file.rdstate() << '\n';
    }
    std::cout << "******************************\n";
    errno = 0;
    {
        FILE* file = fopen("no_file", "r");
        if (file == NULL) {
            std::cout << "FILE  is NULL\n";
            perror("Error ");
        }
        perror("Error ");
    }
    errno = 0;
    std::cout << "******************************\n";
    {
        FILE* file = fopen("per", "r");
        if (file == NULL) {
            std::cout << "FILE  is NULL\n";
            perror("Error ");
        }
        perror("Error ");
    }
    std::cout << "******************************\n";
    errno = 0;
    {
        FILE* file = fopen("valid", "r");
        if (file == NULL) {
            std::cout << "FILE  is NULL\n";
            perror("Error ");
        }
        perror("Error ");
    }
    std::cout << "******************************\n";
    errno = 0;
    {
        if (access("no_file", F_OK) == -1) {
            std::cout << "access return is -1\n";
            perror("Error ");
        } else if (access("no_file", R_OK) == -1) {
            std::cout << "access return is -1\n";
            perror("Error ");
        } else {
            perror("Error ");
        }
    }
    errno = 0;
    std::cout << "******************************\n";
    {
        if (access("per", F_OK) == -1) {
            std::cout << "access return is -1\n";
            perror("Error ");
        } else if (access("per", R_OK) == -1) {
            std::cout << "access return is -1\n";
            perror("Error ");
        } else {
            perror("Error ");
        }
    }
    std::cout << "******************************\n";
    errno = 0;
    {
        if (access("valid", F_OK) == -1) {
            std::cout << "access return is -1\n";
            perror("Error ");
        } else if (access("valid", R_OK) == -1) {
            std::cout << "access return is -1\n";
            perror("Error ");
        } else {
            perror("Error ");
        }
    }
}
