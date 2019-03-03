#pragma once
#include <cstddef>
#include <cstdint>

// Constants from winnt.h
#define IMAGE_DOS_SIGNATURE         0x5A4D
#define IMAGE_OS2_SIGNATURE         0x454E
#define IMAGE_OS2_SIGNATURE_LE      0x454C
#define IMAGE_VXD_SIGNATURE         0x454C
#define IMAGE_NT_SIGNATURE          0x00004550

#define IMAGE_FILE_MACHINE_I386     0x014c
#define IMAGE_FILE_MACHINE_IA64     0x0200
#define IMAGE_FILE_MACHINE_AMD64    0x8664

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 0x10

// Directory Entries
#define IMAGE_DIRECTORY_ENTRY_EXPORT            0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT            1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE          2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION         3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY          4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC         5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG             6   // Debug Directory
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE      7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR         8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS               9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG       10  // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT      11  // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT               12  // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT      13  // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR    14  // COM Runtime descriptor

#define IMAGE_SIZEOF_SHORT_NAME     8

#pragma pack(push, 1)

struct DosHeader
{
    std::uint16_t   magic;
    std::uint8_t    __unused[58];
    std::uint32_t   lfnew;
};

struct ImageFileHeader
{
    std::uint16_t   machine;
    std::uint16_t   numberOfSection;
    std::uint32_t   timeDateStamp;
    std::uint32_t   pointerToSymbolTable;
    std::uint32_t   numberOfSymbols;
    std::uint16_t   sizeOfOptionalHeader;
    std::uint16_t   charasteristics;
};

struct ImageDataDirectory
{
    std::uint32_t   virtualAddress;         //! RVA to section table.
    std::uint32_t   size;                   //! Table size.
};

struct ImageOptionalHeader32
{
    std::uint16_t       magic;
    std::uint8_t        majorLinkerVersion;
    std::uint8_t        minorLinkerVersion;
    std::uint32_t       sizeOfCode;
    std::uint32_t       sizeOfInitializedData;
    std::uint32_t       sizeOfUninitializedData;
    std::uint32_t       addressOfEntryPoint;
    std::uint32_t       baseOfCode;
    std::uint32_t       baseOfData;
    std::uint32_t       imageBase;
    std::uint32_t       sectionAlignment;
    std::uint32_t       fileAlignment;
    std::uint16_t       majorOperatingSystemVersion;
    std::uint16_t       minorOperatingSystemVersion;
    std::uint16_t       majorImageVersion;
    std::uint16_t       minorImageVersion;
    std::uint16_t       majorSubsystemVersion;
    std::uint16_t       minorSubsystemVersion;
    std::uint32_t       win32VersionValue;
    std::uint32_t       sizeOfImage;
    std::uint32_t       sizeOfHeaders;
    std::uint32_t       checkSum;
    std::uint16_t       subsystem;
    std::uint16_t       dllCharacteristics;
    std::uint32_t       sizeOfStackReserve;
    std::uint32_t       sizeOfStackCommit;
    std::uint32_t       sizeOfHeapReserve;
    std::uint32_t       sizeOfHeapCommit;
    std::uint32_t       loaderFlags;
    std::uint32_t       numberOfRvaAndSizes;
    ImageDataDirectory  dataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
};

struct ImageOptionalHeader64 {
    std::uint16_t       magic;
    std::uint8_t        majorLinkerVersion;
    std::uint8_t        minorLinkerVersion;
    std::uint32_t       sizeOfCode;
    std::uint32_t       sizeOfInitializedData;
    std::uint32_t       sizeOfUninitializedData;
    std::uint32_t       addressOfEntryPoint;
    std::uint32_t       baseOfCode;
    std::uint64_t       imageBase;
    std::uint32_t       sectionAlignment;
    std::uint32_t       fileAlignment;
    std::uint16_t       majorOperatingSystemVersion;
    std::uint16_t       minorOperatingSystemVersion;
    std::uint16_t       majorImageVersion;
    std::uint16_t       minorImageVersion;
    std::uint16_t       majorSubsystemVersion;
    std::uint16_t       minorSubsystemVersion;
    std::uint32_t       win32VersionValue;
    std::uint32_t       sizeOfImage;
    std::uint32_t       sizeOfHeaders;
    std::uint32_t       checkSum;
    std::uint16_t       subsystem;
    std::uint16_t       dllCharacteristics;
    std::uint64_t       sizeOfStackReserve;
    std::uint64_t       sizeOfStackCommit;
    std::uint64_t       sizeOfHeapReserve;
    std::uint64_t       sizeOfHeapCommit;
    std::uint32_t       loaderFlags;
    std::uint32_t       numberOfRvaAndSizes;
    ImageDataDirectory  dataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
};

struct NTHeader
{
    std::uint32_t   signature;
    ImageFileHeader fileHeader;
    union
    {
        ImageOptionalHeader32 opt32;
        ImageOptionalHeader64 opt64;
    };
};

struct ImageSectionHeader
{
    std::uint8_t    name[IMAGE_SIZEOF_SHORT_NAME];
    union
    {
        std::uint32_t   physicalAddress;
        std::uint32_t   virtualSize;
    } misc;
    std::uint32_t   virtualAddress;
    std::uint32_t   sizeOfRawData;
    std::uint32_t   pointerToRawData;
    std::uint32_t   pointerToRelocations;
    std::uint32_t   pointerToLinenumbers;
    std::uint16_t   numberOfRelocations;
    std::uint16_t   numberOfLinenumbers;
    std::uint32_t   vharacteristics;
};

struct ImageExportDirectory
{
    std::uint32_t characteristics;
    std::uint32_t timeDateStamp;
    std::uint16_t majorVersion;
    std::uint16_t minorVersion;
    std::uint32_t name;
    std::uint32_t base;
    std::uint32_t numberOfFunctions;
    std::uint32_t numberOfNames;
    std::uint32_t addressOfFunctions;
    std::uint32_t addressOfNames;
    std::uint32_t addressOfNameOrdinals;
};

struct ImageImportDescriptor
{
    union
    {
        std::uint32_t characteristics;
        std::uint32_t originalFirstThunk;
    };
    std::uint32_t timeDateStamp;
    std::uint32_t forwarderChain;
    std::uint32_t name;
    std::uint32_t firstThunk;
};

struct ImageThunkData32
{
    union
    {
        std::uint32_t forwarderString;
        std::uint32_t function;
        std::uint32_t ordinal;
        std::uint32_t addressOfData;
    } u1;
};

struct ImageImportByName
{
    std::uint16_t   hint;
    std::uint8_t    name[1];
};

#pragma pack(pop)
