#pragma once

#define INIT_EXECUTE(func)					\
namespace {									\
[[maybe_unused]] bool ret = (func, true);	\
}