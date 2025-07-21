#pragma once

namespace Cube {

	class FrameBuffer {
	public:
		FrameBuffer();
		~FrameBuffer();

		void bind();
		static void bindDefaultFrameBuffer();
		void bindAttachment(unsigned int width, unsigned int height);
		void resize(unsigned int width, unsigned int height);

		uint32_t getTexture() const;
	private:
		uint32_t id = 0;
		uint32_t texture = 0;
		uint32_t rbo = 0;
	};

}