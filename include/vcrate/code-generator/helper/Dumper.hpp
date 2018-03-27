#pragma once

#include <vcrate/Alias.hpp>

#include <iostream>

namespace vcrate { namespace code_gen {

enum class Color {
    Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
};

class Dumper {
public:

    struct Specializer {
        ui32 code;
    };

    static std::ostream& colorize(std::ostream& os, Color color, bool foreground = true, bool bright = false);
    static std::ostream& underline(std::ostream& os);
    static std::ostream& italic(std::ostream& os);
    static std::ostream& bold(std::ostream& os);
    static std::ostream& clear(std::ostream& os);

    static std::ostream& special(std::ostream& os, ui32 code);

    static Specializer colorizer(Color color, bool foreground = true, bool bright = false);
    static Specializer underliner();
    static Specializer italicize();
    static Specializer bolder();
    static Specializer clearer();

    static Specializer specializer(ui32 code);

};

std::ostream& operator << (std::ostream& os, Dumper::Specializer sp);

}}