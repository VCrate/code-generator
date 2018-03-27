#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/context/CompiledContext.hpp>
#include <vcrate/code-generator/function/Function.hpp>

#include <vector>
#include <memory>
#include <map>

namespace vcrate { namespace code_gen {

class Context {
public:

    Function& create_function(Signature const& signature);
    Function& create_function(std::string const& symbol, Signature const& signature);

    Constant& create_constant(Type const& type, Constant::values_t const& value);

    CompiledContext compile() const;
    
    void dump(std::ostream& os) const;

private:

    std::map<std::string, Function> functions;

    struct ConstantRef {
        Constant constant;
        ui32 referenced0;
    };

    std::vector<ConstantRef> constants;
    
};

}}