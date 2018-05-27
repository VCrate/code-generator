#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/core/Block.hpp>
#include <vcrate/vcx/Executable.hpp>

#include <optional>

namespace vcrate { namespace code_gen {

class Context;

class Function {
public:

    Block& get_block();
    Value get_parameter(ui32 pos) const;

    std::string name;

private:

    friend class Context;

    Function(Context& context);

    Context* context;
    Block* origin;

};

}}