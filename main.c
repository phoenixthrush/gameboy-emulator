#include "include/common.h"
#include "include/cartridge.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    printf("Welcome.\n");
    printf("ROM selected: %s\n\n", argv[1]);

    load_cartridge(argv[1]);

    return 0;
}
