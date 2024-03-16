#include "../include/cartridge.h"

char rom_title[16];
u8 type;
u8 rom_size;
u8 ram_size;
u8 lic_code;

// https://gbdev.io/pandocs/The_Cartridge_Header.html#0147--cartridge-type
static const char *ROM_TYPES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 None",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 None",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A None",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E None",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 None",
    "0x15 None",
    "0x16 None",
    "0x17 None",
    "0x18 None",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F None",
    "MBC6",
    "0x21 None",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

// https://gbdev.io/pandocs/The_Cartridge_Header.html#01440145--new-licensee-code
static const char *LICENSEE[0xA5] = {
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s'pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)"
};

// https://gbdev.io/pandocs/The_Cartridge_Header.html?highlight=licen#0149--ram-size
static const char *RAM_SIZES[] = {
    "No RAM",
    "Unused",
    "8 KiB (1 bank)",
    "32 KiB (4 banks of 8 KiB each)",
    "128 KiB (16 banks of 8 KiB each)",
    "64 KiB (8 banks of 8 KiB each)",
};

int load_cartridge(char *cartridge) {
    FILE *fp = fopen(cartridge, "rb");
    if (fp == NULL) {
        printf("Error: Unable to open file %s\n", cartridge);
        return 1;
    }


    // TODO checksum
    // https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum


    // read game company
    // https://gbdev.io/pandocs/The_Cartridge_Header.html?highlight=licen#01440145--new-licensee-code
    fseek(fp, 0x145, SEEK_SET);
    fread(&lic_code, sizeof(u16), 1, fp);


    // read game title
    // https://gbdev.io/pandocs/The_Cartridge_Header.html?highlight=title#0134-0143--title
    fseek(fp, 0x134, SEEK_SET);
    fread(rom_title, sizeof(char), 16, fp);
    rom_title[15] = '\0';


    // read cartridge type
    // https://gbdev.io/pandocs/The_Cartridge_Header.html?highlight=licen#0147--cartridge-type
    fseek(fp, 0x147, SEEK_SET);
    fread(&type, sizeof(u16), 1, fp);


    // read rom size
    // https://gbdev.io/pandocs/The_Cartridge_Header.html?highlight=licen#0148--rom-size
    fseek(fp, 0x148, SEEK_SET);
    fread(&rom_size, sizeof(u16), 1, fp);


    // read ram size
    // https://gbdev.io/pandocs/The_Cartridge_Header.html?highlight=licen#0149--ram-size
    fseek(fp, 0x149, SEEK_SET);
    fread(&ram_size, sizeof(u8), 1, fp);


    fclose(fp);
    printf("Company  : %s\n", LICENSEE[lic_code]);
    printf("Title    : %s\n", rom_title);
    printf("Type     : %s\n", ROM_TYPES[type]);
    printf("ROM Size : %d KiB\n", (1 << (rom_size + 1)));
    printf("RAM Size : %s\n", RAM_SIZES[ram_size]);

    return 0;
}




