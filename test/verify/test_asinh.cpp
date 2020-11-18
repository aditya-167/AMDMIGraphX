
#include "verify_program.hpp"
#include <migraphx/program.hpp>
#include <migraphx/generate.hpp>
#include <migraphx/operators.hpp>

struct test_asinh : verify_program<test_asinh>
{
    migraphx::program create_program() const
    {
        migraphx::program p;
        auto* mm = p.get_main_module();
        migraphx::shape s{migraphx::shape::double_type, {16}};
        auto x = mm->add_parameter("x", s);
        mm->add_instruction(migraphx::op::asinh{}, x);
        return p;
    }
};