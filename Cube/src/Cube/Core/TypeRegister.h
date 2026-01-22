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
                  const ResPtr<T>& resPtr = obj.as<ResPtr<T>>();
                  nlohmann::json j;
                  if(resPtr) {
                      j = nlohmann::json(resPtr->getIdentifier());
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
                    return Any(ResPtr<T>(data.get<std::string>()));
                }
            }
        });
    }
}  // namespace Cube