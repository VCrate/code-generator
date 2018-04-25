#include <vcrate/code-generator/core/Function.hpp>

#include <vcrate/code-generator/helper/Dumper.hpp>

#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <thread>

namespace vcrate { namespace code_gen {

Block& Function::get_block() {
    return origin;
}

Value Function::get_parameter(ui32 pos) const {
    return Value{ 0, type::none };
}

bool Function::check_terminate() const {
    std::unordered_set<const Block*> terminate_block;
    std::unordered_set<const Block*> blocks;
    std::unordered_map<const Block*, std::vector<const Block*>> previous_block;

    {
        std::stack<const Block*> to_visit;
        to_visit.push(&origin);
        while(!to_visit.empty()) {
            const Block* cur = to_visit.top();
            to_visit.pop();

            if (blocks.count(cur))
                continue;

            blocks.insert(cur);

            switch(cur->end_method) {
                case Block::EndMethod::Jump: {
                    to_visit.push(cur->end_value.next);
                    previous_block[cur->end_value.next].push_back(cur);
                    break;
                }

                case Block::EndMethod::Branch: {
                    to_visit.push(cur->end_value.then);
                    previous_block[cur->end_value.then].push_back(cur);
                    to_visit.push(cur->end_value.otherwise);
                    previous_block[cur->end_value.otherwise].push_back(cur);
                    break;
                }

                default:
                    terminate_block.insert(cur);
                    continue;
            }
        }
    }

    {
        std::stack<const Block*> to_visit;
        for(auto t : terminate_block)
            to_visit.push(t);

        while(!to_visit.empty()) {
            const Block* cur = to_visit.top();
            to_visit.pop();

            for(auto p : previous_block[cur]) {
                if (terminate_block.count(p))
                    continue;
                terminate_block.insert(p);
                to_visit.push(p);
            }
        }
    }

    std::cout << "Blocks: \n";
    for(auto b : blocks) {
        std::cout << b->name << '\n';
        std::cout << "\tFROM: ";
        for(auto p : previous_block[b])
            std::cout << p->name << ", ";
        std::cout << '\n';
    }

    std::cout << "Terminate: \n";
    for(auto b : terminate_block) {
        std::cout << b->name << '\n';
    }
    return terminate_block.size() == blocks.size();
}


}}