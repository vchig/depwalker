#include "depwalker.hpp"
#include "peformat.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace fs = boost::filesystem;

DepWalker::DepWalker()
    : recursive_(false)
    , copy_(false)
{
}

void DepWalker::walk(const std::string &fname)
{
    fs::path full_path = fs::canonical(fname);
    if( !fs::exists(full_path) || !fs::is_regular_file(full_path) )
    {
        std::cerr << "File " << full_path << " not found.\n";
        return;
    }
    std::cout << "Load file: " << full_path.string() << std::endl;

    //! \brief Читает бинарный файл в формате PE и возвращает список импортируемых.
    auto find_deps = [this](const std::string& fname) -> std::list<std::string>
    {
        PEFormat fmt;
        fmt.load(fname);
        auto libs = fmt.importDlls();
        libs.remove_if( [this](const std::string& lib)
                        { return except_libs_.find(lib) != except_libs_.end(); }
                      );
        libs.sort();
        return libs;
    };

    std::function< std::list<std::string>(const std::string&) > find_all_deps;
    find_all_deps = [this, &find_deps, &find_all_deps](const std::string& pe_binary) -> std::list<std::string>
    {
        std::list<std::string> libs = find_deps(pe_binary);
        if( recursive_ )
        {
            std::list<std::string> deep_libs;
            for( const auto& l : libs )
            {
                std::string lib_path = findLibrary(l);
                if( lib_path.empty() )
                    continue;
                deep_libs.merge( find_all_deps(lib_path) );
                deep_libs.unique();
            }
            libs.merge(deep_libs);
            libs.unique();
        }
        return libs;
    };

    //! \brief Копирует библиотеку (попросту файл) в директорию с обрабатываемым файлом
    auto copy_lib = [](const std::string& from, const std::string& to)
    {
        boost::system::error_code ec;
        fs::path lib_name = fs::path(to).parent_path();
        lib_name += fs::path::preferred_separator;
        lib_name += fs::path(from).filename();
        std::cout << "    Copy library "
                  << std::setfill('.') << std::setw(40) << fs::path(from).filename().string()
                  << std::setfill('.') << std::setw(20);
        if( !exists(lib_name) )
        {
            copy_file(from, lib_name, fs::copy_option::fail_if_exists, ec);
            if( ec ) std::cout << ec.message() << std::endl;
            else std::cout << "copied" << std::endl;
        }
        else
        {
            std::cout << "exist" << std::endl;
        }
    };

    for( const auto& d : find_all_deps(full_path.string()) )
    {
        auto lib_path = findLibrary(d);
        std::cout << "Dependency: " << std::setfill('.') << std::setw(45) << d
                  << std::setfill('.') << std::setw(20) << (lib_path.empty() ? "not found" : "found") << std::endl;
        if( copy_ && !lib_path.empty() )
            copy_lib(lib_path, full_path.string() );
    }
}

void DepWalker::appendSearchPath(const std::string &path)
{
    if( fs::exists(path) && fs::is_directory(path) )
        find_paths_.insert(path);
}

void DepWalker::setConfigurationFile(const std::string &fconfig)
{
    using namespace boost::property_tree;
    if( !fs::exists(fconfig) )
        return;
    ptree conf;
    read_json( fconfig, conf );
    for( const auto& v : conf.get_child("system_libs") )
        except_libs_.insert(v.second.data());
    for( const auto& p : conf.get_child("search_paths") )
        appendSearchPath(p.second.data());
}

std::string DepWalker::findLibrary(const std::string &name)
{
    auto find_file = [](const std::string& f, const std::string& path) -> std::string
    {
        if( !fs::exists(path) || !fs::is_directory(path) )
            return "";
        for( auto& it : fs::directory_iterator(path) )
        {
            if( boost::iequals(f, it.path().filename().string()) )
                return it.path().string();
        }
        return "";
    };
    std::string result;
    for( const auto& dir : find_paths_ )
    {
        result = find_file(name, dir);
        if( !result.empty() )
            return result;
    }
    return result;
}
