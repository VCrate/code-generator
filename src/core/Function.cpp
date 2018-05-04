#include <vcrate/code-generator/core/Function.hpp>

#include <vcrate/code-generator/helper/Dumper.hpp>
#include <vcrate/code-generator/instruction/Encoder.hpp>
#include <vcrate/bytecode/v1.hpp>

#include <stack>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace vcrate { namespace code_gen {

Block& Function::get_block() {
    return origin;
}

Value Function::get_parameter(ui32 pos) const {
    return Value{ 0, type::none };
}

ui32 predict_size(Block const& b) {
    switch(b.end_method) {
        case Block::EndMethod::Jump:
            return 1; // JMP

        case Block::EndMethod::Branch:
            return 2; // JMPEQ, JMP
        
        case Block::EndMethod::Halt:
            return 1; // HLT

        case Block::EndMethod::Return:
            return 2; // LVE, RET

        case Block::EndMethod::Throw:
            return 2; // OUT, HLT

        default:
            return 0;
    }
}

struct blocks_info_t {
    std::unordered_set<const Block*> blocks;
    std::unordered_set<const Block*> end;
    std::unordered_map<const Block*, std::vector<const Block*>> previous;
};

blocks_info_t get_blocks_info(const Block* start) {
    blocks_info_t infos;
    std::stack<const Block*> to_visit;
    to_visit.push(start);
    while(!to_visit.empty()) {
        const Block* cur = to_visit.top();
        to_visit.pop();

        if (infos.blocks.count(cur))
            continue;

        infos.blocks.insert(cur);

        switch(cur->end_method) {
            case Block::EndMethod::Jump: {
                to_visit.push(cur->end_value.next);
                infos.previous[cur->end_value.next].push_back(cur);
                break;
            }

            case Block::EndMethod::Branch: {
                to_visit.push(cur->end_value.then);
                infos.previous[cur->end_value.then].push_back(cur);
                to_visit.push(cur->end_value.otherwise);
                infos.previous[cur->end_value.otherwise].push_back(cur);
                break;
            }

            default:
                infos.end.insert(cur);
                continue;
        }
    }
    return infos;
}

bool check_termination(blocks_info_t& blocks) {
    std::unordered_set<const Block*> end(std::begin(blocks.end), std::end(blocks.end));
    std::stack<const Block*> to_visit;

    for(auto t : end)
        to_visit.push(t);

    while(!to_visit.empty()) {
        const Block* cur = to_visit.top();
        to_visit.pop();

        for(auto p : blocks.previous[cur]) {
            if (end.count(p))
                continue;
            end.insert(p);
            to_visit.push(p);
        }
    }

    return end.size() == blocks.blocks.size();
}

using best_order_t = std::pair<std::vector<const Block*>, ui32>;

best_order_t find_best_order(std::vector<const Block*> const& current, std::vector<const Block*> const& all) {
    ui32 i = 0;

    std::vector<const Block*> best;
    ui32 best_value = 0;

    if (all.empty())
        return {current, 0};
    
    for(auto it = std::begin(all); it != std::end(all); ++it, ++i) {
        std::vector<const Block*> remaining(std::begin(all), std::end(all));
        remaining.erase(std::begin(remaining) + i);

        auto cur = current;
        if (cur.empty()) {
            cur.push_back(*it);
            best_order_t cur_best = find_best_order(cur, remaining);
            if (i == 0 || best_value < cur_best.second) {
                best = cur_best.first;
                best_value = cur_best.second;
            }
        } else {
            auto last = cur.back();

            cur.push_back(*it);

            best_order_t cur_best = find_best_order(cur, remaining);

            bool is_short_branch = false;
            switch(last->end_method) {
                case Block::EndMethod::Jump:
                    is_short_branch = last->end_value.next == *it;
                    break;
                case Block::EndMethod::Branch:
                    is_short_branch = last->end_value.then == *it && last->end_value.otherwise == *it;
                    break;
                default:
                    is_short_branch = false;
                    break;
            }

            if (is_short_branch)
                cur_best.second++;

            if (i == 0 || best_value < cur_best.second) {
                best = cur_best.first;
                best_value = cur_best.second;
            }
        }
    }

    return { best, best_value };
}

std::map<const Block*, ui32> compute_position(blocks_info_t const& info) {
    std::map<const Block*, ui32> positions;

    auto best_order = find_best_order({}, {std::begin(info.blocks), std::end(info.blocks)});

    for(auto const& b : best_order.first)
        std::cout << b->name << ", ";
    std::cout << '\n';

    ui32 pos = 0;
    std::vector<const Block*> blocks(std::rbegin(best_order.first), std::rend(best_order.first));
    while(!blocks.empty()) {
        auto b = blocks.back();
        blocks.pop_back();
        ui32 size = predict_size(*b)+1;
        positions[b] = pos;
        pos += size;
    }

    return positions;
} 

std::optional<vcx::Executable> Function::compile() const {
    blocks_info_t blocks = get_blocks_info(&origin);

    if (!check_termination(blocks))
        return std::nullopt;

    auto block_pos = compute_position(blocks);
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
    exe.entry_point = block_pos[&origin]*4;

    auto get_offset_to = [&block_pos, &exe] (auto b) {
        return block_pos[b] * 4 - exe.code.size() * 4;
    };

    for(auto[pos, block] : ordered_blocks) {
        (void)pos;
        switch(block->end_method) {
            case Block::EndMethod::Jump:
                push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_jmp(Operand::Value(get_offset_to(block->end_value.next))));
                continue;

            case Block::EndMethod::Branch:
                push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_jmpe(Operand::Value(get_offset_to(block->end_value.then))));
                push_insn(exe.code, make_jmp(Operand::Value(get_offset_to(block->end_value.otherwise))));
                continue;
            
            case Block::EndMethod::Halt:
                push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_hlt());
                continue;

            case Block::EndMethod::Return:
                push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_lve());
                push_insn(exe.code, make_ret());
                continue;

            case Block::EndMethod::Throw:
                push_insn(exe.code, make_out(Operand::Value((ui32)block->name.back())));
                push_insn(exe.code, make_out(Operand::Value(42)));
                push_insn(exe.code, make_hlt());
                continue;
        }
    }

    return exe;
}

}}