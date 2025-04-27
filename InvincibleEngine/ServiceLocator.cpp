#include "ServiceLocator.h"

dae::ISoundSystem& dae::ServiceLocator::get_sound_system()
{
    if (!_ssInstance)
    {
        _ssInstance = std::make_unique<NullSoundSystem>();
    }
    return *_ssInstance;
}

void dae::ServiceLocator::register_sound_system(std::unique_ptr<ISoundSystem>&& ss)
{
    _ssInstance = std::move(ss);
}
