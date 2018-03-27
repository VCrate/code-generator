#pragma once

#include <vcrate/Alias.hpp>

#include <string>

namespace vcrate { namespace code_gen {

struct Type {

    constexpr Type(const char* name, ui32 size) : name(name), size(size) {}
    constexpr Type(Type const& type) : Type(type.name, type.size) {}
    constexpr Type(Type&& type) : Type(std::move(type.name), std::move(type.size)) {}
    constexpr Type& operator =(Type const& type) { name = type.name; size = type.size; return *this; } 
    constexpr Type& operator =(Type&& type) { name = std::move(type.name); size = std::move(type.size); return *this; } 

    const char* name;
    ui32 size;
    
};

namespace type {

constexpr Type void_ptr { "void*", 4 };
constexpr Type func_ptr { "function*", 4 };
constexpr Type char_ptr { "char*", 4 };

constexpr Type int32 { "int32", 4 };
constexpr Type uint32 { "uint32", 4 };

constexpr Type int8 { "int8", 1 };
constexpr Type uint8 { "uint8", 1 };

}

}}