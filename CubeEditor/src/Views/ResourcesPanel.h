#pragma once
#include "View.h"
#include "Cube/Renderer/Texture.h"
#include "Cube/Resource/Resource.h"

#include <memory>

namespace Cube {
    struct Node;

    class ResourcesPanel : public View {
	public:
		ResourcesPanel();
		~ResourcesPanel() override;

		void render(float deltaTime) override;

        static void importResources();

    private:
		Resource<Texture2D>* dirIcon;
		Resource<Texture2D>* fileIcon;
	};

}