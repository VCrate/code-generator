#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/core/Block.hpp>
#include <vcrate/vcx/Executable.hpp>

#include <optional>

namespace vcrate { namespace code_gen {

class Context;

class Function {
public:

    Function(Context& context);

    Block& get_block();
    Value get_parameter(ui32 pos) const;

private:

    friend class Context;

    Context* context;
    Block origin;

};

}}