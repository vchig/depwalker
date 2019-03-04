#include "peformat.hpp"
#include <fstream>
#include <iterator>

#include <iostream>

static uint32_t align_value(uint32_t value, uint32_t base)
{
    std::uint32_t mask = ~(~base + 1);
    if( mask & value )
        value = (~mask & value) + (mask + 1);
    return value;
}

PEFormat::PEFormat()
    : sections(nullptr)
{
}

PEFormat::~PEFormat()
{
}

void PEFormat::load(const std::string &fname)
{
    pe_file.open( fname, std::ios_base::in | std::ios_base::binary );
    if( !pe_file.is_open() )
        throw std::runtime_error("Failed open: '" + fname + "' file.");
    DosHeader dos;
    pe_file.read( reinterpret_cast<char*>(&dos), sizeof(dos));
    if( dos.magic != IMAGE_DOS_SIGNATURE )   // Must be equal 'MZ'
        throw std::runtime_error("File '" + fname + "' is not pe-executable.");
    pe_file.seekg(dos.lfnew, std::ios_base::beg);

    pe_file.read( reinterpret_cast<char*>(&nt), sizeof(nt) );
    if( nt.signature != IMAGE_NT_SIGNATURE )  // Must be equal 'PE\0\0'
        throw std::runtime_error("File '" + fname + "' is not contain PE header.");

    switch (nt.fileHeader.machine)
    {
    case IMAGE_FILE_MACHINE_I386:
    {
        //! Need move back in file.
        int diff_size = sizeof(ImageOptionalHeader64) - sizeof(ImageOptionalHeader32);
        pe_file.seekg( -diff_size, std::ios_base::cur );
        type_ = PE_TYPE_I386;
        break;
    }
    case IMAGE_FILE_MACHINE_AMD64:
        type_ = PE_TYPE_AMD64;
        break;
    default:
        throw std::runtime_error("Unknown architecture of PE file: '" + fname + "' ");
    }

    sections.reset( new ImageSectionHeader[nt.fileHeader.numberOfSection] );
    pe_file.read( reinterpret_cast<char*>(sections.get()), sizeof(ImageSectionHeader) * nt.fileHeader.numberOfSection);
}

std::list<std::string> PEFormat::importDlls()
{
    std::list<std::string> result;
    std::string dll_name;
    std::uint32_t import_table_offset = convertAddress(importVirtualAddr(), sectionAligment());

    std::ifstream::pos_type import_desc_pos = import_table_offset;
    ImageImportDescriptor import_desc;
    do
    {
        pe_file.seekg( import_desc_pos, std::ios_base::beg );
        pe_file.read( reinterpret_cast<char*>(&import_desc), sizeof(import_desc) );
        import_desc_pos = pe_file.tellg();
        std::uint32_t dll_ptr = convertAddress(import_desc.name, sectionAligment());
        if( dll_ptr != 0 )
        {
            dll_name.clear();
            pe_file.seekg( dll_ptr, std::ios_base::beg );
            std::istream_iterator<char> iname(pe_file);
            while( *iname != '\0')
                dll_name.push_back(*iname++);
            result.push_back(dll_name);
        }
    } while( import_desc.name != 0 );
    return result;
}

uint32_t PEFormat::convertAddress(uint32_t addr, uint32_t align) const
{
    int index = -1;
    for( std::size_t i = 0; i < nt.fileHeader.numberOfSection; ++i )
    {
        std::uint32_t start = sections[i].virtualAddress;
        std::uint32_t end = start + align_value(sections[i].misc.virtualSize, align);
        if( addr >= start && addr < end )
            index = i;
    }
    if( index != -1 )
        return addr - sections[index].virtualAddress + sections[index].pointerToRawData;
    return 0;
}

uint32_t PEFormat::importVirtualAddr() const
{
    switch( nt.fileHeader.machine )
    {
    case IMAGE_FILE_MACHINE_I386:
        return nt.opt32.dataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].virtualAddress;
    case IMAGE_FILE_MACHINE_AMD64:
        return nt.opt64.dataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].virtualAddress;
    }
    return 0;
}

uint32_t PEFormat::sectionAligment() const
{
    switch( nt.fileHeader.machine )
    {
    case IMAGE_FILE_MACHINE_I386:
        return nt.opt32.sectionAlignment;
    case IMAGE_FILE_MACHINE_AMD64:
        return nt.opt64.sectionAlignment;
    }
    return 0;
}
