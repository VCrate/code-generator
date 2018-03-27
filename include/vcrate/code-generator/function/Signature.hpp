#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Type.hpp>

#include <optional>
#include <vector>

namespace vcrate { namespace code_gen {

class Signature {
public:

    Signature(Type const& return_type);
    
    void set_return(Type const& type);
    void add_arg(Type const& type);
    void enable_variable_args(Type const& type);
    void disable_variable_args();

    Type get_return() const; 
    Type get_arg(ui32 pos) const;
    ui32 get_arg_count() const;
    std::optional<Type> get_variable_args() const;
     

private:

    Type ret;
    std::vector<Type> args;
    std::optional<Type> variable_args;

};

}}