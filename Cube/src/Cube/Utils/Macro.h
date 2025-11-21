#pragma once

#define INIT_EXECUTE(func)  \
namespace {                 \
bool ret = [] {             \
    (func)                    \
    return true;            \
}                           \
}