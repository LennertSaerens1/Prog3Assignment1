#include "ServiceLocator.h"

dae::ISoundSystem& dae::ServiceLocator::GetSoundSystem()
{
    return *_ssInstance;
}

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss)
{
    _ssInstance = std::move(ss);
}
