#pragma once
#include <string>
#include <functional>
#include <chrono>

namespace dae
{


	class InvincibleEngine
	{
	public:
		explicit InvincibleEngine(const std::string& dataPath);
		~InvincibleEngine();
		void Run(const std::function<void()>& load);
		void SetFrameRate(int FrameRate);

		InvincibleEngine(const InvincibleEngine& other) = delete;
		InvincibleEngine(InvincibleEngine&& other) = delete;
		InvincibleEngine& operator=(const InvincibleEngine& other) = delete;
		InvincibleEngine& operator=(InvincibleEngine&& other) = delete;
	private:
		float fixed_time_step = 0.08f;
		std::chrono::milliseconds ms_per_frame{ 16 };
	};
}