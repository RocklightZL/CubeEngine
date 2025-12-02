#pragma once

namespace Cube {

    using TypeID = uint32_t;

    namespace Detail {
        inline TypeID nextArithmeticID; // ensure a unique copy
        inline TypeID nextClassID = 0x100;
    }

    template<typename T>
    TypeID getTypeID() {
        if constexpr (std::is_arithmetic_v<T>) {
            static TypeID id = Detail::nextArithmeticID++;
            return id;
        } else {
            static TypeID id = Detail::nextClassID++;
            return id;
        }
    }

    inline bool isArithmetic(TypeID typeID) {
        return !(typeID >> 8);
    }
}