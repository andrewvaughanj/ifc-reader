#include "Presenter.h"

#include "ifc/File.h"
#include "ifc/MSVCEnvironment.h"

#include <filesystem>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "expected: path to .ifc file\n";
        return EXIT_FAILURE;
    }

    std::string path_to_ifc = argv[1];
    if (!is_regular_file(std::filesystem::path(path_to_ifc)))
    {
        std::cerr << path_to_ifc << " is not regular file\n";
        return EXIT_FAILURE;
    }

    try
    {
        ifc::MSVCEnvironment env;
        ifc::File const & file = env.get_module_by_bmi_path(path_to_ifc);

        ifc::FileHeader const & header = file.header();
        std::cout << "IFC Version: " << header.major_version << "." << header.minor_version << "\n"
                  << "File contains " << raw_count(header.partition_count) << " partitions\n"
                  << "Global Scope Index: " << static_cast<int>(header.global_scope) << "\n"
            ;

        auto declarations = file.declarations();
        std::cout << "Total declarations count: " << declarations.size() << "\n";

        auto scopes = file.scope_descriptors();
        std::cout << "Scopes count: " << scopes.size() << "\n";

        size_t number_of_decls_from_all_scopes = 0;
        for (auto scope : scopes)
            number_of_decls_from_all_scopes += raw_count(scope.cardinality);

        std::cout << "Count of declarations from all scopes: " << number_of_decls_from_all_scopes << "\n";

        ifc::ScopeDescriptor global_scope = file.global_scope();
        const ifc::Index first_global_scope_declaration = global_scope.start;
        std::cout << "Global scope contains " << raw_count(global_scope.cardinality) << " declarations, "
                  << "first declaration index = " << static_cast<int>(first_global_scope_declaration) << "\n";

        Presenter presenter(file, std::cout);

        for (size_t i = 0; i != raw_count(global_scope.cardinality); ++i)
        {
            std::cout << "------------------- Declaration " << i << " -------------------\n";
            presenter.present(declarations[first_global_scope_declaration + i]);
        }

        return EXIT_SUCCESS;
    }
    catch (std::exception const & e)
    {
        std::cerr << e.what();
        return EXIT_FAILURE;
    }
}
