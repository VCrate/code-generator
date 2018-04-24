#pragma once

#include <vector>
#include <functional>

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <vcrate/code-generator/instruction/Operation.hpp>


namespace vcrate { namespace code_gen {

enum class Access {
    Write, Read, ReadWrite
};

struct Operand {
    Operand(Value const& v, Access a);
    Value value;
    Access access;
};

class Application {
public:

    Application(Operation const& ope, std::vector<Operand> const& operands);

    std::string to_string() const;

    Operation ope;
    std::vector<Value> read_values;
    std::vector<Value> write_values;

};

std::function<Application(std::vector<Value> const&)> make_application
    (Operation const& ope, std::vector<Access> const& access);

auto const app_return = make_application(operation::retrn, { Access::Read });

}}