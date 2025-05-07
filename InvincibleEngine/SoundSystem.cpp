#include "SoundSystem.h"

#include <thread>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <condition_variable>
#include <string>
#include <atomic>
#include <SDL_mixer.h>
#include <SDL_log.h>

#include <filesystem>
#include <iostream>

namespace dae
{
    class SoundSystem::SoundImpl
    {
    public:
        SoundImpl()
            : m_Running(true)
        {
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
            }

            m_WorkerThread = std::thread(&SoundImpl::ProcessQueue, this);
        }

        ~SoundImpl()
        {
            Shutdown();
        }

        SoundId LoadSound(const std::string& filepath)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());
            if (!chunk)
            {
                SDL_Log("Failed to load sound: %s, SDL_mixer Error: %s", filepath.c_str(), Mix_GetError());
                return 0;
            }

            SoundId newId = ++m_NextSoundId;
            m_Sounds[newId] = chunk;
            return newId;
        }

        void Play(const SoundId id, float volume, bool loop)
        {
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                m_Requests.push({ id, volume, loop });
            }
            m_Condition.notify_one();
        }

        void StopAllSounds()
        {
            // Stop all sounds on all channels
            Mix_HaltChannel(-1);
        }

        void StopSound(SoundId id)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);

            // Find the sound by ID
            auto it = m_Sounds.find(id);
            if (it != m_Sounds.end())
            {
                // Stop all channels playing this specific sound
                for (int channel = 0; channel < Mix_AllocateChannels(-1); ++channel)
                {
                    if (Mix_GetChunk(channel) == it->second)
                    {
                        Mix_HaltChannel(channel);
                    }
                }
            }
        }

        void Shutdown()
        {
            if (!m_Running) return;

            m_Running = false;
            m_Condition.notify_one();
            if (m_WorkerThread.joinable())
                m_WorkerThread.join();

            for (auto& [id, chunk] : m_Sounds)
            {
                Mix_FreeChunk(chunk);
            }
            m_Sounds.clear();

            Mix_CloseAudio();
        }

    private:
        struct SoundRequest
        {
            SoundId id;
            float volume;
            bool loop; 
        };

        void ProcessQueue()
        {
            while (m_Running)
            {
                SoundRequest request;

                {
                    std::unique_lock<std::mutex> lock(m_Mutex); 
                    m_Condition.wait(lock, [this] { return !m_Requests.empty() || !m_Running; });

                    if (!m_Running && m_Requests.empty())
                        break;

                    request = m_Requests.front();
                    m_Requests.pop();
					//lock.unlock(); //This makes the threading work again because the lock only needs to lock shared recources, so after the pop it can be unlocked
				}//Or do it in curly braces, since the lock goes out of scope here and also unlocks the mutex

                auto it = m_Sounds.find(request.id);
                if (it != m_Sounds.end())
                {
                    Mix_VolumeChunk(it->second, static_cast<int>(MIX_MAX_VOLUME * request.volume));
                    int loops = request.loop ? -1 : 0; 
                    Mix_PlayChannel(-1, it->second, loops);
                }
            }
        }

        std::atomic<bool> m_Running;
        std::thread m_WorkerThread;
        std::mutex m_Mutex;
        std::condition_variable m_Condition;
        std::queue<SoundRequest> m_Requests;
        std::unordered_map<SoundId, Mix_Chunk*> m_Sounds;
        SoundId m_NextSoundId{ 0 };
    };


    SoundSystem::SoundSystem()
        : m_SoundImpl(std::make_unique<SoundImpl>())
    {
    }

    SoundSystem::~SoundSystem() = default;

    SoundId SoundSystem::LoadSound(const std::string& filepath)
    {
        namespace fs = std::filesystem;

        fs::path currentPath = fs::current_path();
        std::string pathStr = currentPath.string();

        std::string projectName = "MsPacman";
        size_t pos = pathStr.find(projectName);

        if (pos != std::string::npos)
        {
            pathStr = pathStr.substr(0, pos);
        }

        fs::path fullPath = fs::path(pathStr) / "Data" / filepath;
        std::string finalPath = fullPath.string();


        return m_SoundImpl->LoadSound(finalPath);
    }

    void SoundSystem::Play(const SoundId id, float volume, bool loop)
    {
        m_SoundImpl->Play(id, volume, loop);
    }

    void SoundSystem::StopAllSounds()
    {
        m_SoundImpl->StopAllSounds();
    }

    void SoundSystem::StopSound(SoundId id)
    {
        m_SoundImpl->StopSound(id);
    }

    void SoundSystem::Shutdown()
    {
        m_SoundImpl->Shutdown();
    }
}
