#pragma once

#include <memory>
#include "SoundSystem.h"

namespace dae
{
    class ServiceLocator final
    {
    public:
        static ISoundSystem& GetSoundSystem();

        static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss);

    private:
        inline static std::unique_ptr<ISoundSystem> _ssInstance{ nullptr };
    };
}
