#pragma once
#include <string>
#include <functional>
#include <chrono>

namespace dae
{


	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void SetFrameRate(int FrameRate);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	private:
		float fixed_time_step = 0.08f;
		std::chrono::milliseconds ms_per_frame{ 16 };
	};
}