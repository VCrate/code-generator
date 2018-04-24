#include <vcrate/code-generator/instruction/Application.hpp>

#include <algorithm>

namespace vcrate { namespace code_gen {

Operand::Operand(Value const& v, Access a) : value(v), access(a) {}

Application::Application(Operation const& ope, std::vector<Operand> const& operands)
    : ope(ope) {
        
    for(auto o : operands) {
        switch(o.access) {
            case Access::Write:
                write_values.push_back(o.value);
                break;
            case Access::Read:
                read_values.push_back(o.value);
                break;
            case Access::ReadWrite:
                write_values.push_back(o.value);
                read_values.push_back(o.value);
                break;
        }
    }
}

std::string Application::to_string() const {
    std::string s = ope.name + ": ";
    bool first  = true;
    if (write_values.size() > 0) {
        for(auto v : write_values) {
            if (!first)
                s += ", ";
            else
                first = false;

            s += std::to_string(v.get_id()) + "(" + v.get_type().name + ")"; 
        }
    } else {
        s += "*";
    }

    s += " -> ";

    if (read_values.size() > 0) {
        first  = true;
        for(auto v : read_values) {
            if (!first)
                s += ", ";
            else
                first = false;

            s += std::to_string(v.get_id()) + "(" + v.get_type().name + ")"; 
        }
    } else {
        s += "*";
    }

    return s;
}

std::function<Application(std::vector<Value> const&)> make_application(Operation const& ope, std::vector<Access> const& access) {
    return [ope, access] (std::vector<Value> const& values) {
        std::vector<Operand> operands;
        for(ui32 i = 0; i < access.size(); ++i)
            operands.emplace_back(values[i], access[i]);
        return Application(ope, operands);
    };
}


}}