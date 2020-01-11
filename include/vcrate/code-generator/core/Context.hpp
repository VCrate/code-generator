#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/vcx/Executable.hpp>
#include <vcrate/code-generator/instruction/Operand.hpp>

#include <unordered_set>
#include <unordered_map>
#include <memory>

namespace vcrate { namespace code_gen {

class Block;
class Function;
class Value;
class Type;
class Context;

class Context {
public:

    Function& create_function(std::string const& name = "");
    Block& create_block(std::string const& name = "");
    Value& create_value(Type const& type, std::string const& name = "");

    std::optional<vcx::Executable> compile() const;

private:

    std::vector<std::unique_ptr<Function>> functions;
    std::vector<std::unique_ptr<Block>> blocks;
    std::vector<std::unique_ptr<Value>> values;

private:

    using blocks_vec_t = std::vector<const Block*>;
    using blocks_set_t = std::unordered_set<const Block*>;
    using blocks_order_t = std::pair<blocks_vec_t, ui32>; 
    struct blocks_info_t {
        const Block* origin;
        blocks_set_t blocks;
        blocks_set_t end;
        std::unordered_map<const Block*, blocks_vec_t> previous;
        std::unordered_map<const Block*, blocks_set_t> path_to;

        void dump(std::ostream& os = std::cout);
    };

    using values_vec_t = std::vector<const Value*>;
    using values_set_t = std::unordered_set<const Value*>;
    using scope_t = blocks_set_t;
    using values_scope_t = std::map<const Value*, scope_t>;
    using operands_usage_t = std::map<std::pair<const Block*, const Value*>, Operand>;
    struct values_info_t {
        values_set_t values;
        std::unordered_map<const Value*, blocks_set_t> used_in;
    };

    static values_info_t get_values_info(blocks_info_t const& blocks);
    static scope_t find_scope(Value const& value, blocks_info_t const& blocks, Context::values_info_t const& values);
    static values_scope_t get_values_scope(Context::blocks_info_t const& blocks, Context::values_info_t const& values);
    static operands_usage_t compute_operands_usage(blocks_info_t const& blocks_info, values_scope_t const& scope);

    static blocks_info_t get_blocks_info(const Block* first);
    static blocks_order_t find_best_order(blocks_vec_t const& current, blocks_vec_t const& all);
    static std::map<const Block*, ui32> compute_position(blocks_info_t const& info);
    static ui32 predict_size(const Block& block);

};

}}