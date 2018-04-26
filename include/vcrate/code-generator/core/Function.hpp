#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/core/Block.hpp>

namespace vcrate { namespace code_gen {

class Function {
public:

    Block& get_block();

    Value get_parameter(ui32 pos) const;

    bool check_terminate() const;

private:

    friend class CompiledFunction;

    Block origin;

};

}}