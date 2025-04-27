#pragma once

#include <memory>
#include "SoundSystem.h"

namespace dae
{
    class ServiceLocator final
    {
    public:
        static ISoundSystem& get_sound_system();

        static void register_sound_system(std::unique_ptr<ISoundSystem>&& ss);

    private:
        inline static std::unique_ptr<ISoundSystem> _ssInstance{ nullptr };
    };
}
