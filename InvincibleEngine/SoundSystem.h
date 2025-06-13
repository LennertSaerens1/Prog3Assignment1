#pragma once

#include <memory>
#include <string>

namespace dae
{
    using SoundId = unsigned short;

    class ISoundSystem
    {
    public:
        virtual ~ISoundSystem() = default;

        virtual SoundId LoadSound(const std::string& filepath) = 0;
        virtual void Play(SoundId id, float volume, bool loop = false) = 0;
        virtual void Shutdown() = 0;
		virtual void StopAllSounds() = 0;
		virtual void StopSound(SoundId id) = 0;
    };

    class SoundSystem final : public ISoundSystem
    {
    public:
        SoundSystem();
        ~SoundSystem() override;

        SoundId LoadSound(const std::string& filepath) override;
        void Play(SoundId id, float volume, bool loop = false) override;
        void Shutdown() override;
        void StopAllSounds() override;
        void StopSound(SoundId id) override;

        void SetMuted(bool muted);
        bool IsMuted() const;

        SoundSystem(const SoundSystem&) = delete;
        SoundSystem& operator=(const SoundSystem&) = delete;
        SoundSystem(SoundSystem&&) = delete;
        SoundSystem& operator=(SoundSystem&&) = delete;

    private:
        class SoundImpl;
        std::unique_ptr<SoundImpl> m_SoundImpl;
        bool m_isMuted{ false };
    };

    class NullSoundSystem final : public ISoundSystem
    {
    public:
        NullSoundSystem() = default;
        ~NullSoundSystem() override = default;

        SoundId LoadSound(const std::string&) override { return 0; }
        void Play(SoundId , float , bool  = false) override {}
        void Shutdown() override {}
		void StopAllSounds() override {}
		void StopSound(SoundId) override {}
    };
}
