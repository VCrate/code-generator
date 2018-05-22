#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/vcx/Executable.hpp>

#include <unordered_set>
#include <unordered_map>

namespace vcrate { namespace code_gen {

class Block;
class Function;
class Value;
class Type;

class Context {
public:

    Function& create_function();
    Block create_block();
    Value create_value(Type const& type);

    std::optional<vcx::Executable> compile() const;

private:

    ui32 uid_value = 0;
    std::vector<Function> functions;

private:

    using blocks_vec_t = std::vector<const Block*>;
    using blocks_set_t = std::unordered_set<const Block*>;
    using blocks_order_t = std::pair<blocks_vec_t, ui32>; 
    struct blocks_info_t {
        blocks_set_t blocks;
        blocks_set_t end;
        std::unordered_map<const Block*, blocks_vec_t> previous;
    };

    static blocks_info_t get_blocks_info(const Block* first);
    static blocks_order_t find_best_order(blocks_vec_t const& current, blocks_vec_t const& all);
    static std::map<const Block*, ui32> compute_position(blocks_info_t const& info);
    static ui32 predict_size(const Block& block);

};

}}