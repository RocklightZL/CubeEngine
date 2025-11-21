#pragma once

namespace Cube {

    using TypeID = uint32_t;
    static TypeID nextArithmeticID = 1;
    static TypeID nextClassID = 0x100;

    template<typename T>
    TypeID getTypeID() {
        if constexpr (std::is_arithmetic_v<T>) {
            static TypeID id = nextArithmeticID++;
            return id;
        } else {
            static TypeID id = nextClassID++;
            return id;
        }
    }

    inline bool isArithmetic(TypeID typeID) {
        return !(typeID >> 8);
    }
}