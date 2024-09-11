/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <migraphx/instruction.hpp>
#include <migraphx/literal.hpp>
#include <migraphx/make_op.hpp>
#include <migraphx/program.hpp>
#include <migraphx/register_target.hpp>
#include <migraphx/verify.hpp>

#include <test.hpp>

TEST_CASE(bitwise_and_test)
{
    migraphx::program p;
    auto* mm = p.get_main_module();
    migraphx::shape s{migraphx::shape::int32_type, {4}};
    std::vector<int32_t> data1{2, 5, 255, -20};
    std::vector<int32_t> data2{3, 14, 16, 38};
    auto l1 = mm->add_literal(migraphx::literal{s, data1});
    auto l2 = mm->add_literal(migraphx::literal{s, data2});
    mm->add_instruction(migraphx::make_op("bitwise_and"), l1, l2);
    p.compile(migraphx::make_target("ref"));
    auto result = p.eval({}).back();
    std::vector<int32_t> results_vector;
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    std::vector<int32_t> gold(data2.size());
    std::transform(data1.begin(),
                   data1.end(),
                   data2.begin(),
                   gold.begin(),
                   [](int32_t n1, int32_t n2) -> int32_t {
                       // NOLINTNEXTLINE(hicpp-signed-bitwise)
                       return n1 & n2;
                   });

    EXPECT(migraphx::verify::verify_rms_range(results_vector, gold));
}

TEST_CASE(bitwise_and_dyn_test)
{
    migraphx::program p;
    auto* mm = p.get_main_module();
    std::vector<migraphx::shape::dynamic_dimension> dd{{2, 6, {4}}};
    migraphx::shape s{migraphx::shape::uint8_type, dd};
    auto left  = mm->add_parameter("l", s);
    auto right = mm->add_parameter("r", s);
    mm->add_instruction(migraphx::make_op("bitwise_and"), left, right);
    p.compile(migraphx::make_target("ref"));

    std::vector<uint8_t> left_data{28, 0, 12, 55};
    std::vector<uint8_t> right_data{4, 1, 128, 62};
    migraphx::parameter_map params0;
    migraphx::shape input_fixed_shape0{migraphx::shape::uint8_type, {4}};
    params0["l"] = migraphx::argument(input_fixed_shape0, left_data.data());
    params0["r"] = migraphx::argument(input_fixed_shape0, right_data.data());
    auto result  = p.eval(params0).back();
    std::vector<char> results_vector;
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    std::vector<uint8_t> gold(left_data.size());
    std::transform(left_data.begin(),
                   left_data.end(),
                   right_data.begin(),
                   gold.begin(),
                   [](uint8_t n1, uint8_t n2) -> uint8_t { return n1 & n2; });
    EXPECT(migraphx::verify::verify_rms_range(results_vector, gold));
}