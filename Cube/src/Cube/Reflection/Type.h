#pragma once

namespace Cube {

    using TypeID = uint32_t;

    namespace Detail {
        inline TypeID nextArithmeticID; // ensure a unique copy
        inline TypeID nextClassID = 0x100;
    }

    namespace Type {
        template<typename T>
        constexpr bool isBarePtr() noexcept {
            using RawT = std::decay_t<T>;
            return std::is_pointer_v<RawT> && !std::is_member_pointer_v<RawT>;
        }

        template<typename T>
        struct PointerLevel {
            static constexpr uint8_t value = 0;
        };

        template<typename T>
        struct PointerLevel<T*> {
            static constexpr uint8_t value = 1 + PointerLevel<T>::value;
        };

        template<typename T>
        struct RemoveAllPointers {
            using Type = T;
        };

        template<typename T>
        struct RemoveAllPointers<T*> {
            using Type = typename RemoveAllPointers<T>::Type;
        };

        template<typename T>
        constexpr uint8_t getPtrLevel() noexcept {
            using RawT = std::decay_t<T>;
            static_assert(isBarePtr<RawT>());
            return PointerLevel<RawT>::value;
        }
    }

    template<typename T>
    TypeID getTypeID() {
        using RawT = std::decay_t<T>;
        if constexpr (Type::isBarePtr<RawT>()) {
            static_assert(Type::getPtrLevel<RawT>() < 8 && "Pointer depth is too deep!");
            return getTypeID<typename Type::RemoveAllPointers<RawT>::Type>() | (Type::getPtrLevel<RawT>() << 29);
        }else {
            if constexpr (std::is_arithmetic_v<RawT>) {
                static TypeID id = Detail::nextArithmeticID++;
                return id;
            } else {
                static TypeID id = Detail::nextClassID++;
                return id;
            }
        }
    }

    inline bool isPtr(TypeID typeID) {
        return typeID >> 29;
    }

    inline TypeID removeAllPtr(TypeID typeID) {
        return typeID & ~(0x7u << 29);
    }

    inline bool isArithmetic(TypeID typeID) {
        return !(typeID >> 8);
    }
}