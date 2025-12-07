#pragma once
#include "Cube/Reflection/Serializer.h"
#include "Cube/Resource/ResPtr.h"

namespace Cube {

    void registerTypes();

    template <typename T>
    void registerResPtrSerializer() {
        static_assert(std::is_base_of_v<ResourceBase, T>);
        Serializer::get().registerConverter(getTypeID<ResPtr<T>>(), {
            // toJson
            [](const Any& obj) {
                  const T* resPtr = obj.as<T*>();
                  nlohmann::json j;
                  if(resPtr) {
                      j = nlohmann::json(resPtr->getPath());
                  } else {
                      j = nullptr;
                  }
                  return j;
            },
            // fromJson
            [](const nlohmann::json& data) {
                if(data.is_null()) {
                    return Any(ResPtr<T>(nullptr));
                } else {
                    std::string path = data.get<std::string>();
                    return Any(ResPtr<T>(path));
                }
            }
        });
    }
}  // namespace Cube