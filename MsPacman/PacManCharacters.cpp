#include "PacManCharacters.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
namespace dae
{
	// Initialize static state members
	std::unique_ptr<DownState> PacManState::m_downState = nullptr;
	std::unique_ptr<UpState> PacManState::m_upState = nullptr;
	std::unique_ptr<LeftState> PacManState::m_leftState = nullptr;
	std::unique_ptr<RightState> PacManState::m_rightState = nullptr;


	dae::PacManCharacter::PacManCharacter(GameObject& gameObject, bool isMale)
		: RenderComponent("SpriteSheet.png", gameObject)
	{
		m_lives = 5;
		m_score = 0;
		m_movementSpeed = 100;
		m_spriteTimer = 0;
		m_spriteInterval = 0.1f;
		if (!isMale)
		{
			SetTexture("MsPacMan.png");
		}

		SetSrcRect(utils::Rect{ 488, 0, 16, 16 });
		SetDestRect(utils::Rect{ 0,0, 16 * 3, 16 * 3 });
		
		// Initialize static states
		PacManState::m_downState = std::make_unique<DownState>(*this);
		PacManState::m_upState = std::make_unique<UpState>(*this);
		PacManState::m_leftState = std::make_unique<LeftState>(*this);
		PacManState::m_rightState = std::make_unique<RightState>(*this);

		// Set initial state
		m_pacManState = PacManState::m_downState.get();
		m_pacManState->OnEnter(*this);

	}

	dae::PacManCharacter::~PacManCharacter()
	{
		SaveHighScore(m_score);
	}

	void dae::PacManCharacter::DecreaseLives()
	{
		m_lives--;
		auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
		auto id = soundSystem.LoadSound("Sounds/ms_death.wav");
		soundSystem.StopSound(m_loopingId);
		//soundSystem.StopAllSounds();
		soundSystem.Play(id, 1.0f);
		GetOwner()->NotifyObservers(GameEvent::PlayerDied);

	}

	void dae::PacManCharacter::AddScore(int score)
	{
		m_score += score;
		GetOwner()->NotifyObservers(GameEvent::PlayerScored);
	}

	int dae::PacManCharacter::GetLives() const
	{
		return m_lives;
	}

	int dae::PacManCharacter::GetScore() const
	{
		return m_score;
	}

	void PacManCharacter::MoveLeft(float deltaTime)
	{
		GetOwner()->AddWorldOffset(glm::vec3( -m_movementSpeed * deltaTime,0,0));
		m_spriteTimer += deltaTime;
		if (m_spriteTimer > m_spriteInterval)
		{
			++m_spriteIndex;
			m_spriteTimer = 0;
			if (m_spriteIndex > 3)
			{
				m_spriteIndex = 0;
			}

			switch (m_spriteIndex)
			{
			case 0:
				SetSrcRect(utils::Rect{ 456, 17, 15, 15 });
				break;
			case 1:
				SetSrcRect(utils::Rect{ 472, 17, 15, 15 });
				break;
			case 2:
				SetSrcRect(utils::Rect{ 488, 17, 15, 15 });
				break;
			case 3:
				SetSrcRect(utils::Rect{ 472, 17, 15, 15 });
				break;
			default:
				std::cerr << "Invalid sprite index" << std::endl;
				break;
			}
		}

	}

	void PacManCharacter::MoveRight(float deltaTime)
	{
		GetOwner()->AddWorldOffset(glm::vec3(m_movementSpeed * deltaTime, 0, 0));
		m_spriteTimer += deltaTime;
		if (m_spriteTimer > m_spriteInterval)
		{
			++m_spriteIndex;
			m_spriteTimer = 0;
			if (m_spriteIndex > 3)
			{
				m_spriteIndex = 0;
			}

			switch (m_spriteIndex)
			{
			case 0:
				SetSrcRect(utils::Rect{ 456, 1, 15, 15 });
				break;
			case 1:
				SetSrcRect(utils::Rect{ 472, 1, 15, 15 });
				break;
			case 2:
				SetSrcRect(utils::Rect{ 488, 1, 15, 15 });
				break;
			case 3:
				SetSrcRect(utils::Rect{ 472, 1, 15, 15 });
				break;
			default:
				std::cerr << "Invalid sprite index" << std::endl;
				break;
			}
		}
	}

	void PacManCharacter::MoveUp(float deltaTime)
	{
		GetOwner()->AddWorldOffset(glm::vec3(0,-m_movementSpeed * deltaTime, 0));
		m_spriteTimer += deltaTime;
		if (m_spriteTimer > m_spriteInterval)
		{
			++m_spriteIndex;
			m_spriteTimer = 0;
			if (m_spriteIndex > 3)
			{
				m_spriteIndex = 0;
			}

			switch (m_spriteIndex)
			{
			case 0:
				SetSrcRect(utils::Rect{ 456, 33, 15, 15 });
				break;
			case 1:
				SetSrcRect(utils::Rect{ 472, 33, 15, 15 });
				break;
			case 2:
				SetSrcRect(utils::Rect{ 488, 33, 15, 15 });
				break;
			case 3:
				SetSrcRect(utils::Rect{ 472, 33, 15, 15 });
				break;
			default:
				std::cerr << "Invalid sprite index" << std::endl;
				break;
			}
		}
	}

	void PacManCharacter::MoveDown(float deltaTime)
	{
		GetOwner()->AddWorldOffset(glm::vec3(0, m_movementSpeed * deltaTime, 0));
		m_spriteTimer += deltaTime;
		if (m_spriteTimer > m_spriteInterval)
		{
			++m_spriteIndex;
			m_spriteTimer = 0;
			if (m_spriteIndex > 3)
			{
				m_spriteIndex = 0;
			}

			switch (m_spriteIndex)
			{
			case 0:
				SetSrcRect(utils::Rect{ 456, 49, 15, 15 });
				break;
			case 1:
				SetSrcRect(utils::Rect{ 472, 49, 15, 15 });
				break;
			case 2:
				SetSrcRect(utils::Rect{ 488, 49, 15, 15 });
				break;
			case 3:
				SetSrcRect(utils::Rect{ 472, 49, 15, 15 });
				break;
			default:
				std::cerr << "Invalid sprite index" << std::endl;
				break;
			}
		}
	}

	void dae::PacManCharacter::SetState(PacManState* newState)
	{
		if (m_pacManState)
		{
			m_pacManState->OnExit(*this);
		}
		m_pacManState = newState;
		if (m_pacManState)
		{
			m_pacManState->OnEnter(*this);
		}
	}

	void dae::PacManCharacter::Update(float deltaTime)
	{
		RenderComponent::Update(deltaTime);

		if (m_pacManState)
		{
			m_pacManState->Update(*this, deltaTime);
		}
	}

	// DownState Implementation
	void DownState::Update(PacManCharacter& player, float deltaTime)
	{
		player.MoveDown(deltaTime);
	}

	void DownState::OnEnter(PacManCharacter& player)
	{
		player.MoveDown(0);
	}

	void DownState::OnExit(PacManCharacter& )
	{
		// Logic when exiting DownState
	}

	// LeftState Implementation
	void LeftState::Update(PacManCharacter& player, float deltaTime)
	{
		player.MoveLeft(deltaTime);
	}

	void LeftState::OnEnter(PacManCharacter& player)
	{
		player.MoveLeft(0);
	}

	void LeftState::OnExit(PacManCharacter& )
	{
		// Logic when exiting LeftState
	}

	// RightState Implementation
	void RightState::Update(PacManCharacter& player, float deltaTime)
	{
		player.MoveRight(deltaTime);
	}

	void RightState::OnEnter(PacManCharacter& player)
	{
		player.MoveRight(0);
	}

	void RightState::OnExit(PacManCharacter& )
	{
		// Logic when exiting RightState
	}

	// UpState Implementation
	void UpState::Update(PacManCharacter& player, float deltaTime)
	{
		player.MoveUp(deltaTime);
	}

	void UpState::OnEnter(PacManCharacter& player)
	{
		player.MoveUp(0);
	}

	void UpState::OnExit(PacManCharacter& )
	{
		// Logic when exiting UpState
	}
}
