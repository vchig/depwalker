#pragma once
#include <fstream>
#include <string>
#include <list>
#include <memory>
#include "winheaders.hpp"

class PEFormat
{
public:
    explicit PEFormat();
    ~PEFormat();

    void load(const std::string& fname);

    std::list<std::string> importDlls();

private:
    std::uint32_t convertAddress(std::uint32_t addr, std::uint32_t align) const;

    uint32_t importVirtualAddr() const;
    uint32_t sectionAligment() const;

    NTHeader nt;
    std::ifstream pe_file;

    std::unique_ptr<ImageSectionHeader[]> sections;
};
