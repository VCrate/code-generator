#include <vcrate/code-generator/instruction/Operation.hpp>

namespace vcrate { namespace code_gen {

Operation::Operation(std::string const& name, std::string const& format, ui32 args_count) 
        : name(name), format(format), args_count(args_count) {}

namespace operation {

Operation retrn("return", "return $0", 1);

}

}}