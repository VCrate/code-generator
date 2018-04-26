#include <vcrate/code-generator/core/CompiledFunction.hpp>

#include <vcrate/code-generator/instruction/Encoder.hpp>
#include <vcrate/vcx/Executable.hpp>
#include <vcrate/bytecode/v1.hpp>

#include <unordered_map>
#include <unordered_set>
#include <stack>

#include <fstream>

namespace vcrate { namespace code_gen {

CompiledFunction CompiledFunction::from_function(Function const& f) {
    std::unordered_map<const Block*, ui32> position;
    {
        ui32 current_position = 0;
        std::stack<const Block*> to_visit;
        to_visit.push(&f.origin);
        while(!to_visit.empty()) {
            const Block* cur = to_visit.top();
            to_visit.pop();

            auto it = position.find(cur);
            if (it == position.end()) {
                position[cur] = current_position;
                switch(cur->end_method) {
                    case Block::EndMethod::Halt: {
                        // compile to `Ope::Halt`
                        current_position += 4;
                        break;
                    }
                    case Block::EndMethod::Throw: {
                        // compile to `Ope::Halt`
                        current_position += 4;
                        break;
                    }
                    case Block::EndMethod::Return: {
                        // compile to `Ope::Lve, Ope::Ret`
                        current_position += 8;
                        break;
                    }
                    case Block::EndMethod::Jump: {
                        // compile to `Ope::Jmp`
                        current_position += 4;
                        break;
                    }
                    case Block::EndMethod::Branch: {
                        // compile to `Ope::Jmpeq, Ope::Jmp`
                        current_position += 8;
                        break;
                    }
                }
            }
        }
    }

    vcx::Executable exe;
    exe.entry_point = 0;
    auto push_instruction = [] (std::vector<ui32>& to, instruction_t const& isn) {
        to.push_back(isn.instruction);
        if (isn.extra_0) {
            to.push_back(*isn.extra_0);
            if (isn.extra_1)
                to.push_back(*isn.extra_1);
        }
    };

    push_instruction(exe.code, make_etr());

    {
        std::unordered_set<const Block*> blocks;
        std::stack<const Block*> to_visit;
        to_visit.push(&f.origin);

        while(!to_visit.empty()) {
            const Block* cur = to_visit.top();
            to_visit.pop();
            
            if (blocks.count(cur))
                continue;
            std::cout << cur->name << '\n';
            blocks.insert(cur);

            switch(cur->end_method) {
                case Block::EndMethod::Throw:
                case Block::EndMethod::Halt:
                    push_instruction(exe.code, make_hlt());
                    break;
                case Block::EndMethod::Return: 
                    push_instruction(exe.code, make_lve());
                    push_instruction(exe.code, make_ret());
                    break;
                case Block::EndMethod::Jump:
                    push_instruction(exe.code, make_jmp(Operand::Value(position[cur->end_value.next])));
                    to_visit.push(cur->end_value.next);
                    break;
                case Block::EndMethod::Branch:
                    push_instruction(exe.code, make_cmp(Operand::Value(position[cur->end_value.next]), Operand::Value(0)));
                    push_instruction(exe.code, make_jmpe(Operand::Value(position[cur->end_value.otherwise])));
                    push_instruction(exe.code, make_jmp(Operand::Value(position[cur->end_value.then])));
                    to_visit.push(cur->end_value.otherwise);
                    to_visit.push(cur->end_value.then);
                    break;
            }
        }
    }

    exe.dump();
    std::ofstream os("temp/test_program2.vcx");
    os << exe;

    return {};
}

}}