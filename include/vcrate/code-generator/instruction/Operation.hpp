#pragma once

#include <vcrate/Alias.hpp>

#include <string>

namespace vcrate { namespace code_gen {

class Operation {
public:

    Operation(std::string const& name, std::string const& format, ui32 args_count);

    std::string name, format;
    ui32 args_count;

};

namespace operation {

extern Operation retrn;

}

}}