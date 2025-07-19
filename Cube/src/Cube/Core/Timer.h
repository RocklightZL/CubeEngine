#pragma once

#include <chrono>

namespace Cube {

	class Timer {
	public:
		Timer();
		~Timer() = default;

		float stop();
		void restart();

		float getDuration();
	private:
		std::chrono::time_point<std::chrono::steady_clock> startTime;
		std::chrono::time_point<std::chrono::steady_clock> lastTime;
	};

}