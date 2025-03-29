#pragma once
namespace dae
{
	class Command {
	public:
		virtual ~Command() = default;
		virtual void Execute(float elapsedSec) = 0;
	};
}

