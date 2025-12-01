#pragma once
#include "Type.h"

#include <any>

namespace Cube {

    class Any final{
    public:
        Any() : data(nullptr), typeID(0), destructor(nullptr){}
        Any(void* data, TypeID typeID, const std::function<void(void*)>& destructor)
            : data(data), typeID(typeID), destructor(destructor) {}

        template<typename T>
        Any(const T& value) : typeID(getTypeID<T>()) {
            data = new T(value);
            destructor = [](void* ptr){
                delete static_cast<T*>(ptr);
            };
        }

        Any(const Any& other) = delete;
        Any& operator=(const Any& other) = delete;

        Any(Any&& other) noexcept : data(other.data), typeID(other.typeID), destructor(other.destructor){
            other.data = nullptr;
            other.typeID = 0;
            other.destructor = nullptr;
        }

        ~Any() {
            if(data && destructor) {
                destructor(data);
            }
        }

        Any& operator=(Any&& other) noexcept {
            if(this != &other) {
                data = other.data;
                typeID = other.typeID;
                destructor = other.destructor;
                other.data = nullptr;
                other.typeID = 0;
                other.destructor = nullptr;
            }
            return *this;
        }

        template<typename T>
        T& as() {
            if(typeID != getTypeID<T>()) {
                CB_CORE_ERROR("Reflection: Type mismatched in Any::as()");
                CB_ASSERT(0); // we assume this will never happen in release build
            }
            return *static_cast<T*>(data);
        }

        template<typename T>
        const T& as() const{
            if(typeID != getTypeID<T>()) {
                CB_CORE_ERROR("Reflection: Type mismatched in Any::as()");
                CB_ASSERT(0); // we assume this will never happen in release build
            }
            return *static_cast<const T*>(data);
        }

        TypeID getTypeID() const { return typeID; }
        void* getData(){ return data; }
        const void* getData() const { return data; }

    private:
        void* data;
        TypeID typeID;
        std::function<void(void*)> destructor;
    };

}