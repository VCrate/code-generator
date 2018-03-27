#include <vcrate/code-generator/helper/Dumper.hpp>

namespace vcrate { namespace code_gen {

std::ostream& Dumper::special(std::ostream& os, ui32 code) {
    return os << "\033[" << code << "m";
}

std::ostream& Dumper::colorize(std::ostream& os, Color color, bool foreground, bool bright) {
    return Dumper::special(os, Dumper::colorizer(color, foreground, bright).code);
}

std::ostream& Dumper::underline(std::ostream& os) {
    return Dumper::special(os, Dumper::underliner().code);
}

std::ostream& Dumper::italic(std::ostream& os) {
    return Dumper::special(os, Dumper::italicize().code);
}

std::ostream& Dumper::bold(std::ostream& os) {
    return Dumper::special(os, Dumper::bolder().code);
}

std::ostream& Dumper::clear(std::ostream& os) {
    return Dumper::special(os, Dumper::clearer().code);
}

std::ostream& operator << (std::ostream& os, Dumper::Specializer sp) {
    return Dumper::special(os, sp.code);
}

Dumper::Specializer Dumper::colorizer(Color color, bool foreground, bool bright) {
    auto id = static_cast<ui32>(color) + 30;
    if (!foreground) id += 10;
    if (bright) id += 60;
    return { id };
}

Dumper::Specializer Dumper::underliner() {
    return { 4 };
}

Dumper::Specializer Dumper::italicize() {
    return { 3 };
}

Dumper::Specializer Dumper::bolder() {
    return { 1 };
}

Dumper::Specializer Dumper::clearer() {
    return { 0 };
}

Dumper::Specializer Dumper::specializer(ui32 code) {
    return { code };
}


}}