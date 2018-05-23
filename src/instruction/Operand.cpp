#include <vcrate/code-generator/instruction/Operand.hpp>

#include <vcrate/bytecode/v1.hpp>

namespace vcrate { namespace code_gen {

namespace btc = ::vcrate::bytecode::v1;

Operand Operand::Register(ui32 id) {
    Operand op;
    op.b12.first = btc::arg_12_type.encode(btc::arg_type_register, op.b12.first);
    op.b12.first = btc::arg_12_register.encode(id, op.b12.first);
    op.b12.second = std::nullopt;

    op.b24.first = btc::arg_24_type.encode(btc::arg_type_register, op.b24.first);
    op.b24.first = btc::arg_24_register.encode(id, op.b24.first);
    op.b24.second = std::nullopt;

    return op;
}

Operand Operand::Deferred(ui32 id) {
    Operand op;
    op.b12.first = btc::arg_12_type.encode(btc::arg_type_defer_register, op.b12.first);
    op.b12.first = btc::arg_12_register.encode(id, op.b12.first);
    op.b12.second = std::nullopt;

    op.b24.first = btc::arg_24_type.encode(btc::arg_type_defer_register, op.b24.first);
    op.b24.first = btc::arg_24_register.encode(id, op.b24.first);
    op.b24.second = std::nullopt;

    return op;
}

Operand Operand::Displacement(ui32 id, i32 disp) {
    Operand op;
    {
        ui32 abs_value = disp < 0 ? -static_cast<ui32>(disp) : static_cast<ui32>(disp);
        if (btc::arg_12_unsigned_disp.max_value() < abs_value) {
            op.b12.first = btc::arg_12_type.encode(btc::arg_type_defer_register_disp_next, 0);
            op.b12.first = btc::arg_12_register.encode(id, op.b12.first);
            op.b12.second = disp;
        } else {
            op.b12.first = btc::arg_12_type.encode(btc::arg_type_defer_register_disp, 0);
            op.b12.first = btc::arg_12_register.encode(id, op.b12.first);
            op.b12.first = bytecode::encode_signed_value(btc::arg_12_unsigned_disp, btc::arg_12_sign_disp,
                disp, op.b12.first);
            op.b12.second = std::nullopt;
        }
    }

    {
        ui32 abs_value = disp < 0 ? -static_cast<ui32>(disp) : static_cast<ui32>(disp);
        if (btc::arg_24_unsigned_disp.max_value() < abs_value) {
            op.b24.first = btc::arg_24_type.encode(btc::arg_type_defer_register_disp_next, 0);
            op.b24.first = btc::arg_24_register.encode(id, op.b24.first);
            op.b24.second = disp;
        } else {
            op.b24.first = btc::arg_24_type.encode(btc::arg_type_defer_register_disp, 0);
            op.b24.first = btc::arg_24_register.encode(id, op.b24.first);
            op.b24.first = bytecode::encode_signed_value(btc::arg_24_unsigned_disp, btc::arg_24_sign_disp,
                disp, op.b24.first);
            op.b24.second = std::nullopt;
        }
    }

    return op;
}

Operand Operand::Value(i32 value) {
    Operand op;
    {
        ui32 abs_value = value < 0 ? -static_cast<ui32>(value) : static_cast<ui32>(value);
        if (btc::arg_12_unsigned_value.max_value() < abs_value) {
            op.b12.first = btc::arg_12_type.encode(btc::arg_type_value_next, 0);
            op.b12.second = value;
        } else {
            op.b12.first = btc::arg_12_type.encode(btc::arg_type_value, 0);
            op.b12.first = bytecode::encode_signed_value(btc::arg_12_unsigned_value, btc::arg_12_sign_value,
                value, op.b12.first);
            op.b12.second = std::nullopt;
        }
    }

    {
        ui32 abs_value = value < 0 ? -static_cast<ui32>(value) : static_cast<ui32>(value);
        if (btc::arg_24_unsigned_value.max_value() < abs_value) {
            op.b24.first = btc::arg_24_type.encode(btc::arg_type_value_next, 0);
            op.b24.second = value;
        } else {
            op.b24.first = btc::arg_24_type.encode(btc::arg_type_value, 0);
            op.b24.first = bytecode::encode_signed_value(btc::arg_24_unsigned_value, btc::arg_24_sign_value,
                value, op.b24.first);
            op.b24.second = std::nullopt;
        }
    }

    return op;
}

Operand Operand::Address(ui32 addr) {
    Operand op;
    {
        if (btc::arg_12_signed_value.max_value() < addr) {
            op.b12.first = btc::arg_12_type.encode(btc::arg_type_address_next, 0);
            op.b12.second = addr;
        } else {
            op.b12.first = btc::arg_12_type.encode(btc::arg_type_address, 0);
            op.b12.first = btc::arg_12_signed_value.encode(addr, op.b12.first);
            op.b12.second = std::nullopt;
        }
    }

    {
        if (btc::arg_24_signed_value.max_value() < addr) {
            op.b24.first = btc::arg_24_type.encode(btc::arg_type_address_next, 0);
            op.b24.second = addr;
        } else {
            op.b24.first = btc::arg_24_type.encode(btc::arg_type_address, 0);
            op.b24.first = btc::arg_24_signed_value.encode(addr, op.b24.first);
            op.b24.second = std::nullopt;
        }
    }

    return op;
}


}}