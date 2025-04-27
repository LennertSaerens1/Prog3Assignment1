#pragma once

#include <memory>
#include <string>

namespace dae
{
    using sound_id = unsigned short;

    class ISoundSystem
    {
    public:
        virtual ~ISoundSystem() = default;

        virtual sound_id loadSound(const std::string& filepath) = 0;
        virtual void play(sound_id id, float volume, bool loop = false) = 0;
        virtual void shutdown() = 0;
    };

    class SoundSystem final : public ISoundSystem
    {
    public:
        SoundSystem();
        ~SoundSystem() override;

        sound_id loadSound(const std::string& filepath) override;
        void play(sound_id id, float volume, bool loop = false) override;
        void shutdown() override;

        SoundSystem(const SoundSystem&) = delete;
        SoundSystem& operator=(const SoundSystem&) = delete;
        SoundSystem(SoundSystem&&) = delete;
        SoundSystem& operator=(SoundSystem&&) = delete;

    private:
        class SoundImpl;
        std::unique_ptr<SoundImpl> m_SoundImpl;
    };

    class NullSoundSystem final : public ISoundSystem
    {
    public:
        NullSoundSystem() = default;
        ~NullSoundSystem() override = default;

        sound_id loadSound(const std::string&) override { return 0; }
        void play(sound_id , float , bool  = false) override {}
        void shutdown() override {}
    };
}
