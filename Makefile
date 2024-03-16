all:
	@clear
	mkdir -p build roms
	gcc -o build/main.o -c main.c -Wall -Wextra
	gcc -o build/cartridge.o -c lib/cartridge.c -Wall -Wextra
	gcc -o build/main build/main.o build/cartridge.o
	@build/main roms/Pokemon.gb