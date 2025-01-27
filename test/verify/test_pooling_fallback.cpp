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

#include "verify_program.hpp"
#include <migraphx/program.hpp>
#include <migraphx/generate.hpp>
#include <migraphx/op/pooling.hpp>

struct test_pooling_valid : verify_program<test_pooling_valid>
{
    migraphx::program create_program() const
    {
        migraphx::program p;
        auto* mm = p.get_main_module();

        // Valid pooling operation
        migraphx::shape s0{migraphx::shape::float_type, {1, 3, 32, 32}};
        auto l0 = mm->add_parameter("x", s0);
        migraphx::op::pooling valid_op{migraphx::op::pooling_mode::max};
        valid_op.lengths = {3, 3};
        valid_op.stride  = {1, 1};
        valid_op.padding = {0, 0};
        mm->add_instruction(valid_op, l0);

        return p;
    }
};

struct test_pooling_large_kernel : verify_program<test_pooling_large_kernel>
{
    migraphx::program create_program() const
    {
        migraphx::program p;
        auto* mm = p.get_main_module();

        // Pooling operation with a large kernel size should fallback
        migraphx::shape s0{migraphx::shape::float_type, {1, 3, 32, 32}};
        auto l0 = mm->add_parameter("y", s0);
        migraphx::op::pooling large_kernel_op{migraphx::op::pooling_mode::max};
        large_kernel_op.lengths = {15, 15};
        large_kernel_op.stride  = {1, 1};
        large_kernel_op.padding = {0, 0};
        mm->add_instruction(large_kernel_op, l0);

        return p;
    }
};
