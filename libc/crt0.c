#include "../types.h"
#include "../user.h"

int main(int argc, char **argv);

void _start(int argc, char **argv) {
    main(argc, argv);
    exit();
}