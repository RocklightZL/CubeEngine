#pragma once
#include "Class.h"
#include "ClassRegistry.h"

namespace Cube {

	template<typename T>
	class ClassBuilder final{
	public:
		ClassBuilder(const std::string& name) {
		    classInfo = ClassRegistry::get().registerClass<T>(name);
		}

		template<typename PropertyType>
		ClassBuilder& property(const std::string& name, PropertyType T::*memberPtr) {
		    size_t offset = reinterpret_cast<size_t>(&(static_cast<T>(nullptr)->*memberPtr));
			classInfo->addProperty(name, getTypeID<PropertyType>(), offset, sizeof(PropertyType), [memberPtr](void* obj) {
			    T* instance = static_cast<T*>(obj);
				return Any(instance->*memberPtr);
			}, [memberPtr](void* obj, const Any& value) {
			    T* instance = static_cast<T>(obj);
				instance->*memberPtr = value.as<PropertyType>();
			});
			return *this;
		}

		template<typename PropertyType>
		ClassBuilder& property(const std::string& name, PropertyType T::*memberPtr, PropertyType(T::*getterPtr)(), void(T::*setterPtr)(PropertyType)) {
			size_t offset = reinterpret_cast<size_t>(&(static_cast<T>(nullptr)->*memberPtr));
			classInfo->addProperty(name, getTypeID<PropertyType>(), offset, sizeof(PropertyType), [getterPtr](void* obj) {
				T* instance = static_cast<T*>(obj);
				return Any(instance->*getterPtr());
			}, [setterPtr](void* obj, const Any& value) {
				T* instance = static_cast<T>(obj);
				instance->*setterPtr(value.as<PropertyType>());
			});
			return *this;
		}

		template<typename ReturnType, typename... Args>
		ClassBuilder& method(const std::string& name, ReturnType(T::*methodPtr)(Args...)) {
		    classInfo->addMethod(name, getTypeID<ReturnType>(), {getTypeID<Args>()...}, [methodPtr](void* obj, const std::vector<Any>& args) {
		        T* instance = static_cast<T*>(obj);
				if(args.size() != sizeof...(Args)) {
					CB_CORE_ERROR("Reflection: Mismatched number of function parameter");
				    CB_ASSERT(0);
					return Any();
				}
				return invokeImpl(instance, methodPtr, args, std::make_index_sequence<sizeof...(Args)>{});
		    });
			return *this;
		}

		template<typename BaseType>
		ClassBuilder& base() {
		    classInfo->setBase(getTypeID<BaseType>());
			return *this;
		}

		Class* GetClass() const {
		    return classInfo;
		}

	private:
		Class* classInfo;

		template<typename ReturnType, typename... Args, size_t... Idx>
		Any invokeImpl(T* instance, ReturnType(T::*methodPtr)(Args...), const std::vector<Any>& args, std::index_sequence<Idx...>) {
		    if constexpr (std::is_void_v<ReturnType>) {
		        (instance->*methodPtr)(args[Idx].template as<std::tuple_element_t<Idx, std::tuple<Args...>>>()...);
				return Any();
		    }else {
				return Any((instance->*methodPtr)(args[Idx].template as<std::tuple_element_t<Idx, std::tuple<Args...>>>()...));
		    }
		}
	};

}