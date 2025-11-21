#pragma once
#include "Type.h"

namespace Cube {

    class Any final{
    public:
        Any() : data(nullptr), typeID(0), size(0){}

        template<typename T>
        Any(const T& value) : typeID(getTypeID<T>()), size(sizeof(T)) {
            data = operator new(size);
            new (data) T(value);
        }

        Any(const Any& other) : typeID(other.typeID), size(other.size){
            if(other.data) {
                data = operator new(size);
                std::memcpy(data, other.data, size);
            } else {
                data = nullptr;
            }
        }

        Any(Any&& other) noexcept : data(other.data), typeID(other.typeID), size(other.size){
            other.data = nullptr;
            other.typeID = 0;
            other.size = 0;
        }

        ~Any() {
            if(data) {
                operator delete(data);
            }
        }

        Any& operator=(const Any& other) {
            if(this != &other) {
                if(data) {
                    operator delete(data);
                }
                typeID = other.typeID;
                size = other.size;
                if(other.data) {
                    data = operator new(size);
                    std::memcpy(data, other.data, size);
                } else {
                    data = nullptr;
                }
            }
            return *this;
        }

        Any& operator=(Any&& other) noexcept {
            if(this != &other) {
                if(data) {
                    operator delete(data);
                }
                data = other.data;
                typeID = other.typeID;
                size = other.size;
                other.data = nullptr;
                other.typeID = 0;
                other.size = 0;
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
        size_t getSize() const { return size; }
        void* getData(){ return data; }
        const void* getData() const { return data; }

    private:
        void* data;
        TypeID typeID;
        size_t size;
    };

}