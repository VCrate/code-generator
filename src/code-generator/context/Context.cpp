#include <vcrate/code-generator/context/Context.hpp>

namespace vcrate { namespace code_gen {

Function& Context::create_function(std::string const& symbol, Signature const& signature) {
    return functions.insert(std::make_pair(symbol, Function(*this, symbol, signature))).first->second;
}

Constant& Context::create_constant(Type const& type, Constant::values_t const& value) {
    constants.push_back(ConstantRef {{type, value}, 1});
    return constants.back().constant;
}

CompiledContext Context::compile() const {
    return {};
}

void Context::dump(std::ostream& os) const {
    for(auto const& pair : functions) {
        pair.second.dump(os);
    }
}

}}