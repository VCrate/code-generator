#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/core/Function.hpp>

namespace vcrate { namespace code_gen {

class CompiledFunction {
public:

    static CompiledFunction from_function(Function const& f);

private:


};

}}