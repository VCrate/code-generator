#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/core/Block.hpp>
#include <vcrate/vcx/Executable.hpp>

#include <optional>

namespace vcrate { namespace code_gen {

class Function {
public:

    Block& get_block();

    Value get_parameter(ui32 pos) const;

    std::optional<vcx::Executable> compile() const;

private:

    Block origin;

};

}}