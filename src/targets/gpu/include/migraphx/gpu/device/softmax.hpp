#ifndef MIGRAPHX_GUARD_RTGLIB_DEVICE_SOFTMAX_HPP
#define MIGRAPHX_GUARD_RTGLIB_DEVICE_SOFTMAX_HPP

#include <migraphx/argument.hpp>
#include <migraphx/config.hpp>
#include <hip/hip_runtime_api.h>

namespace migraphx {
inline namespace MIGRAPHX_INLINE_NS {
namespace gpu {
namespace device {

argument softmax(hipStream_t stream, argument result, argument arg, int axis);

} // namespace device
} // namespace gpu
} // namespace MIGRAPHX_INLINE_NS
} // namespace migraphx

#endif
