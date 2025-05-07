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


	dae::PacManCharacter::PacManCharacter(GameObject& gameObject, GridComponent* pGridComponent, bool isMale)
		: RenderComponent("SpriteSheet.png", gameObject)
	{
		m_lives = 5;
		m_score = 0;
		m_spriteIndex = 0;
		m_movementSpeed = 100;
		m_spriteTimer = 0;
		m_spriteInterval = 0.1f;
		m_pGridComponent = pGridComponent;
		if (!isMale)
		{
			SetTexture("MsPacMan.png");
		}

		SetSrcRect(utils::Rect{ 456, 17, 15, 15 });
		SetDestRect(utils::Rect{ 0,0, 15 * 3, 15 * 3 });
		
		// Initialize static states
		PacManState::m_downState = std::make_unique<DownState>(*this);
		PacManState::m_upState = std::make_unique<UpState>(*this);
		PacManState::m_leftState = std::make_unique<LeftState>(*this);
		PacManState::m_rightState = std::make_unique<RightState>(*this);

		// Set initial state
		//m_pacManState = PacManState::m_downState.get();
		//m_pacManState->OnEnter(*this);

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

	bool PacManCharacter::CanMoveUp() const
	{
		int cellSize = m_pGridComponent->GetCellSize();
		utils::Vector2f middlePos = GetMiddlePosition();
		return m_pGridComponent->GetUpCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize/2 +1, middlePos.y + cellSize / 2)).isWalkable && m_pGridComponent->GetUpCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2 -1, middlePos.y + cellSize / 2)).isWalkable;
	}

	bool PacManCharacter::CanMoveDown() const
	{
		int cellSize = m_pGridComponent->GetCellSize();
		utils::Vector2f middlePos = GetMiddlePosition();
		return m_pGridComponent->GetDownCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize/2 +1, middlePos.y - cellSize / 2)).isWalkable && m_pGridComponent->GetDownCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2 -1, middlePos.y - cellSize / 2)).isWalkable;
	}

	bool PacManCharacter::CanMoveLeft() const
	{
		int cellSize = m_pGridComponent->GetCellSize();
		utils::Vector2f middlePos = GetMiddlePosition();
		return m_pGridComponent->GetLeftCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2, middlePos.y - cellSize/2 +1)).isWalkable && m_pGridComponent->GetLeftCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2, middlePos.y + cellSize / 2-1)).isWalkable;
	}

	bool PacManCharacter::CanMoveRight() const
	{
		int cellSize = m_pGridComponent->GetCellSize();
		utils::Vector2f middlePos = GetMiddlePosition();
		return m_pGridComponent->GetRightCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize / 2, middlePos.y - cellSize/2 +1)).isWalkable && m_pGridComponent->GetRightCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize / 2, middlePos.y + cellSize / 2 -1)).isWalkable;
	}

	void PacManCharacter::MoveLeft(float deltaTime)
	{
		
		if (CanMoveLeft()) {
			GetOwner()->AddWorldOffset(glm::vec3(-m_movementSpeed * deltaTime, 0, 0));

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

	}

	void PacManCharacter::MoveRight(float deltaTime)
	{

		if (CanMoveRight()) 
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
	}

	void PacManCharacter::MoveUp(float deltaTime)
	{
		if (CanMoveUp()) {
			GetOwner()->AddWorldOffset(glm::vec3(0, -m_movementSpeed * deltaTime, 0));

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
	}

	void PacManCharacter::MoveDown(float deltaTime)
	{
		if (CanMoveDown()) 
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
