#include <vcrate/code-generator/core/Context.hpp>

#include <vcrate/code-generator/core/Function.hpp>
#include <vcrate/code-generator/core/Block.hpp>

#include <vcrate/code-generator/instruction/Operand.hpp>
#include <vcrate/code-generator/instruction/Encoder.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <vcrate/code-generator/value/Type.hpp>

#include <stack>

namespace vcrate { namespace code_gen {

void Context::blocks_info_t::dump(std::ostream& os) {
    os << "Starting from " << Dumper::bolder() << origin->name << Dumper::clearer() << '\n';
    os << "Blocks: \n";
    for (auto b : blocks) {
        os << '\t' << Dumper::bolder() << b->name << Dumper::clearer();
        if (end.count(b) > 0)
            os << Dumper::italicize() << " [ENDING]" << Dumper::clearer();
        if (!previous[b].empty()) {
            os << Dumper::colorizer(Color::Yellow) << "\n\t  Parents {";
            for(auto p : previous[b])
                os << ' ' << p->name;
            os << " }" << Dumper::clearer();
        }

        if (!path_to[b].empty()) {
            os << Dumper::colorizer(Color::Green) << "\n\t  Link to {";
            for(auto p : path_to[b])
                os << ' ' << p->name;
            os << " }" << Dumper::clearer();
        }

        os << '\n';
    }
}

Function& Context::create_function(std::string const& name) {
    auto& function = functions.emplace_back(new Function(*this));
    function->name = name;
    return *function;
}

Block& Context::create_block(std::string const& name) {
    auto& block = blocks.emplace_back(new Block(*this));
    block->name = name;
    return *block;
}

Value& Context::create_value(Type const& type, std::string const& name) {
    auto& value = values.emplace_back(new Value(*this, type));
    value->name = name;
    return *value;
}

Context::blocks_info_t Context::get_blocks_info(const Block* first) {
    Context::blocks_info_t infos;
    infos.origin = first;
    {
        std::stack<const Block*> to_visit;
        to_visit.push(first);

        while(!to_visit.empty()) {
            const Block* cur = to_visit.top();
            to_visit.pop();

            if (infos.blocks.count(cur))
                continue;

            infos.blocks.insert(cur);

            std::visit(overloaded {
                [&] (Block::EndValueJump const& value) { 
                    to_visit.push(value.next);
                    infos.previous[value.next].push_back(cur);
                    infos.path_to[cur].insert(value.next);
                    infos.path_to[cur].insert(infos.path_to[value.next].begin(), infos.path_to[value.next].end());
                },
                [&] (Block::EndValueBranch const& value) {
                    to_visit.push(value.then);
                    infos.previous[value.then].push_back(cur);
                    to_visit.push(value.otherwise);
                    infos.previous[value.otherwise].push_back(cur);
                },
                [&] (auto) { 
                    infos.end.insert(cur);
                }
            }, cur->end_value);
        }
    }

    for(auto b : infos.blocks) {
        Context::blocks_vec_t to_visit;

        std::visit(overloaded {
            [&] (Block::EndValueJump const& value)   { to_visit.push_back(value.next); },
            [&] (Block::EndValueBranch const& value) { to_visit.push_back(value.then); 
                                                       to_visit.push_back(value.otherwise); },
            [ ] (auto) {}
        }, b->end_value);

        while(!to_visit.empty()) {
            const Block* cur = to_visit.back();
            to_visit.pop_back();

            infos.path_to[b].insert(cur);

            if (infos.path_to[cur].empty()) {
                std::visit(overloaded {
                    [&] (Block::EndValueJump const& value) { 
                        if (value.next != b && infos.path_to[b].count(value.next) == 0)
                            to_visit.push_back(value.next); 
                    },
                    [&] (Block::EndValueBranch const& value) {
                        if (value.then != b && infos.path_to[b].count(value.then) == 0)
                            to_visit.push_back(value.then); 
                        if (value.otherwise != b && infos.path_to[b].count(value.otherwise) == 0)
                            to_visit.push_back(value.otherwise);
                    },
                    [ ] (auto) {}
                }, cur->end_value);
            } else {
                infos.path_to[b].insert(infos.path_to[cur].begin(), infos.path_to[cur].end());
            }
        }
    }

    return infos;
}

Context::blocks_order_t Context::find_best_order(blocks_vec_t const& current, blocks_vec_t const& all) {
    ui32 i = 0;

    Context::blocks_order_t best;

    if (all.empty())
        return {current, 0};
    
    for(auto it = std::begin(all); it != std::end(all); ++it, ++i) {
        blocks_vec_t remaining(std::begin(all), std::end(all));
        remaining.erase(std::begin(remaining) + i);

        auto cur = current;
        if (cur.empty()) {
            cur.push_back(*it);
            Context::blocks_order_t cur_best = find_best_order(cur, remaining);
            if (i == 0 || best.second < cur_best.second)
                best = cur_best;
        } else {
            auto last = cur.back();

            cur.push_back(*it);

            Context::blocks_order_t cur_best = find_best_order(cur, remaining);

            bool is_short_branch = std::visit(overloaded {
                [&] (Block::EndValueJump const& value) { 
                    return value.next == *it; 
                },
                [&] (Block::EndValueBranch const& value) {
                    return value.then == *it && value.otherwise == *it; 
                },
                [] (auto) { 
                    return false; 
                }
            }, last->end_value);

            if (is_short_branch)
                cur_best.second++;

            if (i == 0 || best.second < cur_best.second)
                best = cur_best;
        }
    }

    return best;
}

std::map<const Block*, ui32> Context::compute_position(Context::blocks_info_t const& info) {
    std::map<const Block*, ui32> positions;

    auto best_order = find_best_order({}, {std::begin(info.blocks), std::end(info.blocks)});

    for(auto const& b : best_order.first)
        std::cout << b->name << ", ";
    std::cout << '\n';

    ui32 pos = 0;
    blocks_vec_t blocks(std::rbegin(best_order.first), std::rend(best_order.first));
    while(!blocks.empty()) {
        auto b = blocks.back();
        blocks.pop_back();
        ui32 size = predict_size(*b);
        positions[b] = pos;
        pos += size;
    }

    return positions;
} 

ui32 Context::predict_size(Block const& b) {
    ui32 size = 0;
    for(auto const& insn : b.insn) {
        switch(insn.op) {
            case Block::InsnOp::Copy:
                size += 1;
                break;
            case Block::InsnOp::Load:
                size += (insn.raw.size() + 3) / 4;
                break;
            case Block::InsnOp::Debug:
                size += 1;
                break;
            case Block::InsnOp::Compare:
                size += 1;
                break;
            default:
                std::cout << "Unknown Instruction Operation\n";
                break;
        }
    }

    return size + std::visit(overloaded {
        [] (Block::EndValueJump const&  ) { return 1; }, // JMP
        [] (Block::EndValueBranch const&) { return 2; }, // JMPX, JMP
        [] (Block::EndValueHalt const&  ) { return 2; }, // MOV, HLT
        [] (Block::EndValueThrow const& ) { return 2; }, // LVE, RET
        [] (Block::EndValueReturn const&) { return 2; }  // MOV, HLT
    }, b.end_value);
}

Context::values_info_t Context::get_values_info(Context::blocks_info_t const& blocks) {
    Context::values_info_t info;

    for(auto* block : blocks.blocks)
        for(auto const& insn : block->insn)
            for(auto value : insn.values) {
                info.values.insert(value);
                info.used_in[value].insert(block);
            }
    
    return info;
}

Context::scope_t Context::find_scope(Value const& value, Context::blocks_info_t& blocks, 
    Context::values_info_t values) {

    Context::blocks_set_t& blocks_using_value = values.used_in[&value];
    Context::scope_t scope;

    for(auto block : blocks_using_value) {
        scope.insert(block);
        for(auto child : blocks.path_to[block]) {
            if (blocks_using_value.count(child) > 0)
                scope.insert(child);
            else {
                for(auto using_value : blocks_using_value) {
                    if (blocks.path_to[child].count(using_value) > 0) {
                        scope.insert(child);
                        break;
                    }
                }
            }
        }
    }

    return scope;
}

std::optional<vcx::Executable> Context::compile() const {
    Function const& function = *functions[0];

    Context::blocks_info_t blocks_info = get_blocks_info(function.origin);
    Context::values_info_t values_info = get_values_info(blocks_info);
    blocks_info.dump();

    for(auto& value : values) {
        std::cout << "Value " << value->name << " is used in \n";
        for(auto b : find_scope(*value, blocks_info, values_info))
            std::cout << "> " << b->name << '\n';
    }

    auto block_pos = compute_position(blocks_info);
    std::multimap<ui32, const Block*> ordered_blocks;
    for(auto[b, p] : block_pos)
        ordered_blocks.insert({p, b});

    auto push_insn = [] (auto& vec, auto const& insn) {
        vec.push_back(insn.instruction);
        if (insn.extra_0)
            vec.push_back(*insn.extra_0);
        if (insn.extra_1)
            vec.push_back(*insn.extra_1);
    };

    vcx::Executable exe;
    exe.entry_point = block_pos[function.origin]*4;

    auto get_offset_to = [&block_pos, &exe] (auto b) {
        return block_pos[b] * 4 - exe.code.size() * 4;
    };

    for(auto[pos, block] : ordered_blocks) {
        (void)pos;

        std::cout << block->name << '\n';

        for(auto const& insn : block->insn) {
            switch(insn.op) {
                case Block::InsnOp::Copy:
                    //push_insn(exe.code, make_mov(Operand::Register(insn.values[0].get_id()), Operand::Register(insn.values[1].get_id())));
                    break;
                case Block::InsnOp::Load: {
                    ui32 raw_size = (insn.raw.size() + 3) / 4;
                    auto get_raw = [&raw = insn.raw] (ui32 idx) {
                        return idx < raw.size() ? raw[idx] : 0;
                    };
                    for(ui32 i = 0; i < raw_size; ++i) {
                        ui32 value = get_raw(i) | get_raw(i+1)<<8 | get_raw(i+2)<<16 | get_raw(i+3)<<24;
                        //push_insn(exe.code, make_mov(Operand::Register(insn.values[0].get_id()), Operand::Value(value)));
                    }
                    break;
                }
                case Block::InsnOp::Debug:
                    //push_insn(exe.code, make_dbg(Operand::Register(insn.values[0].get_id())));
                    break;
                case Block::InsnOp::Compare:
                    //push_insn(exe.code, make_cmp(Operand::Register(insn.values[0].get_id()), Operand::Register(insn.values[1].get_id())));
                    break;
            }
        }
        std::visit(overloaded {
            [&] (Block::EndValueJump const& value) {
                //push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_jmp(Operand::Value(get_offset_to(value.next))));
            },
            [&] (Block::EndValueBranch const& value) {
                //push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_jmpe(Operand::Value(get_offset_to(value.then))));
                push_insn(exe.code, make_jmp(Operand::Value(get_offset_to(value.otherwise))));
            },
            [&] (Block::EndValueHalt const& value) {
                //push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                //push_insn(exe.code, make_mov(Operand::Register(0), Operand::Register(value.halt_code.get_id())));
                push_insn(exe.code, make_hlt());
            },
            [&] (Block::EndValueThrow const& ) {
                //push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_mov(Operand::Register(0), Operand::Value(1)));
                push_insn(exe.code, make_hlt());
            },
            [&] (Block::EndValueReturn const& ) {
                //push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_lve());
                push_insn(exe.code, make_ret());
            }
        }, block->end_value);
    }

    return exe;
}

}}