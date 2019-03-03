#include <iostream>
#include <cstdlib>
#include <sstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "depwalker.hpp"

using namespace boost::program_options;

int main(int argc, char** argv)
try
{
    DepWalker dw;
    options_description opts("Options");
    opts.add_options()
        ("help,h", "print help message")
        ("version,v", "version")
        ("recursive,r","find dependencies recursive")
        ("copy,c", "copy dependency to binary folder")
        ("without-path", "find libraries without path")
        ("config", value<std::string>(), "configuration file")
        ("input-files", value< std::vector<std::string> >()->composing(), "Input files")
    ;

    positional_options_description pos_opts;
    pos_opts.add("input-files", -1);

    variables_map vm;
    store(command_line_parser(argc, argv).options(opts).positional(pos_opts).run(), vm);
    notify(vm);

    if( vm.count("help") )
    {
        std::cout << opts << std::endl;
        return 0;
    }

    if( vm.count("version") )
    {
        std::cout << "Version:" << std::endl;
        return 0;
    }

    if( vm.count("recursive") )
        dw.setRecursive(true);

    if( vm.count("copy") )
        dw.setCopy(true);

    if( !vm.count("input-files") )
    {
        std::cout << "Need input files\n" << opts << std::endl;
        return 0;
    }

    if( !vm.count("without-path") )
    {
        std::stringstream ss(std::getenv("PATH"));
        std::string path;
#ifdef __linux__
        while( std::getline(ss, path, ':') )
#elif __MINGW32__
        while( std::getline(ss, path, ';') )
#endif
        {
            dw.appendSearchPath(path);
        }
    }

    if( vm.count("config") )
    {
        dw.setConfigurationFile( vm["config"].as<std::string>() );
    }
    else
    {
        boost::filesystem::path config = boost::filesystem::path(argv[0]).parent_path();
        config += boost::filesystem::path::preferred_separator;
        config += "depwalker.conf";
        dw.setConfigurationFile(config.string());
    }

    std::vector<std::string> files = vm["input-files"].as<std::vector<std::string> >();
    for( const auto& f : files )
        dw.walk( f );
    return 0;
}
catch( const std::exception& e )
{
    std::cerr << "Failed with exception: " << e.what() << std::endl;
    return 1;
}
catch( ... )
{
    std::cerr << "Failed with unknown exception.\n";
    return 1;
}
