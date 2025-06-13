#include "PacManCharacters.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "Utils.h"

int g_globalScore = 0;

namespace dae
{
	// Initialize static state members
	std::unique_ptr<MovingState> PacManState::m_movingState = nullptr;
	std::unique_ptr<IdleState> PacManState::m_idleState = nullptr;
	std::unique_ptr<MovingState> PacManState::m_mrMovingState = nullptr;
	std::unique_ptr<IdleState> PacManState::m_mrIdleState = nullptr;


	dae::PacManCharacter::PacManCharacter(GameObject& gameObject, GridComponent* pGridComponent, std::function<void()> onDeath, bool isMale)
		: RenderComponent("SpriteSheet.png", gameObject)
		, m_onDeath(onDeath)
	{
		m_isMale = isMale;
		m_lives = 3;
		m_score = 0;
		m_spriteIndex = 0;
		m_movementSpeed = 150;
		m_spriteTimer = 0;
		m_spriteInterval = 0.1f;
		m_pGridComponent = pGridComponent;

		m_movementDirection = glm::vec3(0, 0, 0);

		SetSrcRect(utils::Rect{ 456, 17, 15, 15 });
		SetDestRect(utils::Rect{ 0,0, 15 * 3, 15 * 3 });

		if (m_isMale) SetSrcRect(utils::Rect{456,144,13,13});
		
		

		// Set initial state
		// MAKE THIS AN IDLE STATE INSTEAD OF NO STATE!!!!!!
		if (!m_isMale)
		{
			// Initialize static states
			PacManState::m_movingState = std::make_unique<MovingState>(*this);
			PacManState::m_idleState = std::make_unique<IdleState>(*this);

			SetState(PacManState::m_idleState.get());

			auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
			auto id = soundSystem.LoadSound("Sounds/ms_start.wav");
			soundSystem.Play(id, 1.0f);
		}
		else
		{
			// Initialize static states
			PacManState::m_mrMovingState = std::make_unique<MovingState>(*this);
			PacManState::m_mrIdleState = std::make_unique<IdleState>(*this);
			SetState(PacManState::m_mrIdleState.get());
		}

		
	}

	dae::PacManCharacter::~PacManCharacter()
	{
		
	}

	void dae::PacManCharacter::DecreaseLives()
	{
		m_lives--;
		auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
		auto id = soundSystem.LoadSound("Sounds/ms_death.wav");
		soundSystem.StopAllSounds();
		soundSystem.Play(id, 1.0f);

		if (!m_isMale)
		{
			auto pacManWorld = m_pGridComponent->GetWorldCoordinatesMiddle(13, 23);

			SetMiddlePosition(pacManWorld.x, pacManWorld.y);
		}
		else
		{
			auto pacManWorld = m_pGridComponent->GetWorldCoordinatesMiddle(14, 23);

			SetMiddlePosition(pacManWorld.x, pacManWorld.y);
		}

		if (!m_isMale)
			SetState(PacManState::m_idleState.get());
		else
			SetState(PacManState::m_mrIdleState.get());

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
		if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
			return false;
		int cellSize = m_pGridComponent->GetCellSize()-1;
		utils::Vector2f middlePos = GetMiddlePosition();
		return m_pGridComponent->GetUpCellFromWorldPos(utils::Vector2f(middlePos.x , middlePos.y + cellSize / 2)).isWalkable;
		//return m_pGridComponent->GetUpCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize/2 +1, middlePos.y + cellSize / 2)).isWalkable && m_pGridComponent->GetUpCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2 -1, middlePos.y + cellSize / 2)).isWalkable;
	}

	bool PacManCharacter::CanMoveDown() const
	{
		if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
			return false;

		int cellSize = m_pGridComponent->GetCellSize()-1;
		utils::Vector2f middlePos = GetMiddlePosition();
		return m_pGridComponent->GetDownCellFromWorldPos(utils::Vector2f(middlePos.x , middlePos.y - cellSize / 2)).isWalkable;
		//return m_pGridComponent->GetDownCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize/2 +1, middlePos.y - cellSize / 2)).isWalkable && m_pGridComponent->GetDownCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2 -1, middlePos.y - cellSize / 2)).isWalkable;
	}

	bool PacManCharacter::CanMoveLeft() const
	{
		if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
			return false;

		int cellSize = m_pGridComponent->GetCellSize()-1;
		utils::Vector2f middlePos = GetMiddlePosition();
		return m_pGridComponent->GetLeftCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2, middlePos.y)).isWalkable;
		//return m_pGridComponent->GetLeftCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2, middlePos.y - cellSize/2 +1)).isWalkable && m_pGridComponent->GetLeftCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2, middlePos.y + cellSize / 2-1)).isWalkable;
	}

	bool PacManCharacter::CanMoveRight() const
	{
		if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
			return false;

		int cellSize = m_pGridComponent->GetCellSize()-1;
		utils::Vector2f middlePos = GetMiddlePosition();
		return m_pGridComponent->GetRightCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize / 2, middlePos.y)).isWalkable;
		//return m_pGridComponent->GetRightCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize / 2, middlePos.y - cellSize/2 +1)).isWalkable && m_pGridComponent->GetRightCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize / 2, middlePos.y + cellSize / 2 -1)).isWalkable;
	}

	void PacManCharacter::MoveLeft(float deltaTime)
	{
		if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
		{
			GetOwner()->AddWorldOffset(glm::vec3(-m_movementSpeed * deltaTime, 0, 0));

			if ((GetMiddlePosition().x + m_DestRect.width / 2) < 0)
			{
				SetMiddlePosition(GetMiddlePosition().x + m_pGridComponent->GetWidth() * m_pGridComponent->GetCellSize() + m_DestRect.width /2, GetMiddlePosition().y);
			}
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
					SetSrcRect(utils::Rect{ 456, 17 + 143.f*m_isMale, 15, 15 });
					break;
				case 1:
					SetSrcRect(utils::Rect{ 472, 17 + 143.f * m_isMale, 15, 15 });
					break;
				case 2:
					SetSrcRect(utils::Rect{ 488, 17 + 143.f * m_isMale, 15, 15 });
					if (m_isMale)
					{
						SetSrcRect(utils::Rect{ 488, 1 + 143.f * m_isMale, 15, 15 });
					}
					break;
				case 3:
					SetSrcRect(utils::Rect{ 472, 17 + 143.f * m_isMale, 15, 15 });

					break;
				default:
					std::cerr << "Invalid sprite index" << std::endl;
					break;
				}
			}
		}
		else if (CanMoveLeft()) {
			PickUp(deltaTime);

			auto midPos = GetMiddlePosition();
			float snapHeight = m_pGridComponent->GetSnapPos(midPos.x, midPos.y).y;

			SetMiddlePosition(midPos.x, snapHeight);

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
					SetSrcRect(utils::Rect{ 456, 17 + 143.f * m_isMale, 15, 15 });
					break;
				case 1:
					SetSrcRect(utils::Rect{ 472, 17 + 143.f * m_isMale, 15, 15 });
					break;
				case 2:
					SetSrcRect(utils::Rect{ 488, 17 + 143.f * m_isMale, 15, 15 });
					if (m_isMale)
					{
						SetSrcRect(utils::Rect{ 488, 1 + 143.f * m_isMale, 15, 15 });
					}
					break;
				case 3:
					SetSrcRect(utils::Rect{ 472, 17 + 143.f * m_isMale, 15, 15 });
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
		if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
		{
			GetOwner()->AddWorldOffset(glm::vec3(m_movementSpeed * deltaTime, 0, 0));

			if ((GetMiddlePosition().x - m_DestRect.width / 2 ) > m_pGridComponent->GetWidth() * m_pGridComponent->GetCellSize())
			{
				SetMiddlePosition(1, GetMiddlePosition().y);
			}

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
					SetSrcRect(utils::Rect{ 456, 1 + 143.f * m_isMale, 15, 15 });
					break;
				case 1:
					SetSrcRect(utils::Rect{ 472, 1 + 143.f * m_isMale, 15, 15 });
					break;
				case 2:
					SetSrcRect(utils::Rect{ 488, 1 + 143.f * m_isMale, 15, 15 });
					if (m_isMale)
					{
						SetSrcRect(utils::Rect{ 488, 1 + 143.f * m_isMale, 15, 15 });
					}
					break;
				case 3:
					SetSrcRect(utils::Rect{ 472, 1 + 143.f * m_isMale, 15, 15 });
					break;
				default:
					std::cerr << "Invalid sprite index" << std::endl;
					break;
				}
			}
		}
		else if (CanMoveRight()) 
		{
			PickUp(deltaTime);

			auto midPos = GetMiddlePosition();
			float snapHeight = m_pGridComponent->GetSnapPos(midPos.x, midPos.y).y;

			SetMiddlePosition(midPos.x, snapHeight);

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
					SetSrcRect(utils::Rect{ 456, 1 + 143.f * m_isMale, 15, 15 });
					break;
				case 1:
					SetSrcRect(utils::Rect{ 472, 1 + 143.f * m_isMale, 15, 15 });
					break;
				case 2:
					SetSrcRect(utils::Rect{ 488, 1 + 143.f * m_isMale, 15, 15 });
					if (m_isMale)
					{
						SetSrcRect(utils::Rect{ 488, 1 + 143.f * m_isMale, 15, 15 });
					}
					break;
				case 3:
					SetSrcRect(utils::Rect{ 472, 1 + 143.f * m_isMale, 15, 15 });
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
			PickUp(deltaTime);

			auto midPos = GetMiddlePosition();
			float snapWidth = m_pGridComponent->GetSnapPos(midPos.x, midPos.y).x;

			SetMiddlePosition(snapWidth, midPos.y);

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
					SetSrcRect(utils::Rect{ 456, 33 + 143.f * m_isMale, 15, 15 });
					break;
				case 1:
					SetSrcRect(utils::Rect{ 472, 33 + 143.f * m_isMale, 15, 15 });
					break;
				case 2:
					SetSrcRect(utils::Rect{ 488, 33 + 143.f * m_isMale, 15, 15 });
					if (m_isMale)
					{
						SetSrcRect(utils::Rect{ 488, 1 + 143.f * m_isMale, 15, 15 });
					}
					break;
				case 3:
					SetSrcRect(utils::Rect{ 472, 33 + 143.f * m_isMale, 15, 15 });
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
			PickUp(deltaTime);

			auto midPos = GetMiddlePosition();
			float snapWidth = m_pGridComponent->GetSnapPos(midPos.x, midPos.y).x;

			SetMiddlePosition(snapWidth, midPos.y);

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
					SetSrcRect(utils::Rect{ 456, 49 + 143.f * m_isMale, 15, 15 });
					break;
				case 1:
					SetSrcRect(utils::Rect{ 472, 49 + 143.f * m_isMale, 15, 15 });
					break;
				case 2:
					SetSrcRect(utils::Rect{ 488, 49 + 143.f * m_isMale, 15, 15 });
					if (m_isMale)
					{
						SetSrcRect(utils::Rect{ 488, 1 + 143.f * m_isMale, 15, 15 });
					}
					break;
				case 3:
					SetSrcRect(utils::Rect{ 472, 49 + 143.f * m_isMale, 15, 15 });
					break;
				default:
					std::cerr << "Invalid sprite index" << std::endl;
					break;
				}
			}
		}
	}

	void PacManCharacter::PickUp(float )
	{
		auto middlePos = GetMiddlePosition();
		auto cell = m_pGridComponent->GetCellFromWorldPos(middlePos);
		if (cell.hasPellet)
		{
			auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
			auto id = soundSystem.LoadSound("Sounds/ms_eat_dot.wav");
			soundSystem.Play(id, 1.0f);
			m_pGridComponent->PickUpPellet(cell);
		}
		if (cell.hasPowerPill)
		{
			auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
			soundSystem.StopAllSounds();
			auto id = soundSystem.LoadSound("Sounds/ms_eat_dot.wav");
			soundSystem.Play(id, 1.0f);

			auto id2 = soundSystem.LoadSound("Sounds/ms_fright.wav");
			soundSystem.Play(id2, 1.0f);
			m_pGridComponent->PickUpPowerPill(cell);
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

	void PacManCharacter::SetMovementDirection(glm::vec3 movementDir)
	{
		m_movementDirection = movementDir;
	}

	void dae::PacManCharacter::Update(float deltaTime)
	{
		RenderComponent::Update(deltaTime);

		//if (m_pacManState) //Set a IDLE state in the constructor and delete this null check!
		{
			m_pacManState->Update(*this, deltaTime);
		}

		if (m_eatGhostTimer > 0.0f)
		{
			m_eatGhostTimer -= deltaTime;
		}
		else
		{
			m_ghostMultiplier = 0;
		}

		if (m_lives == -1)
		{
			if (m_onDeath == nullptr) return;

			if (m_score > 0)
			{
				g_globalScore = m_score;
			}

			SceneManager::GetInstance().QueueSceneLoad("GameOverScene", m_onDeath);
		}
	}

	void PacManCharacter::AddOtherPlayer(PacManCharacter* otherCharacter)
	{
		m_otherPlayer = otherCharacter;
	}

	void PacManCharacter::OnCollision(GameObject* )
	{
		DecreaseLives();

		if (m_otherPlayer != nullptr)
		{
			m_otherPlayer->DecreaseLives();
		}
	}

	void PacManCharacter::LevelUp()
	{
		if (!m_isMale)
		{
			auto pacManWorld = m_pGridComponent->GetWorldCoordinatesMiddle(13, 23);

			SetMiddlePosition(pacManWorld.x, pacManWorld.y);
		}
		else
		{
			auto pacManWorld = m_pGridComponent->GetWorldCoordinatesMiddle(14, 23);

			SetMiddlePosition(pacManWorld.x, pacManWorld.y);
		}

		if (!m_isMale)
			SetState(PacManState::m_idleState.get());
		else
			SetState(PacManState::m_mrIdleState.get());

		if (!m_isMale)
		{
			if (m_otherPlayer != nullptr)
				m_otherPlayer->LevelUp();
		}
	}

	void PacManCharacter::EatGhost()
	{
		auto score = 200 * static_cast<int>(powf(2.f, m_ghostMultiplier * 1.f));
		AddScore(score);
		++m_ghostMultiplier;
		auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
		auto id = soundSystem.LoadSound("Sounds/ms_eat_ghost.wav");
		soundSystem.Play(id, 1.0f);
		
	}

	void PacManCharacter::SetEndScreen()
	{
		if (m_onDeath == nullptr) return;

		if (m_score > 0)
		{
			g_globalScore = m_score;
		}

		SceneManager::GetInstance().QueueSceneLoad("GameOverScene", m_onDeath);
	}

	void MovingState::Update(PacManCharacter& player, float deltaTime)
	{
		auto dir = player.GetMovementDirection();
		if (dir.x > 0)
		{
			player.MoveRight(deltaTime);
		}
		else if (dir.x < 0)
		{
			player.MoveLeft(deltaTime);
		}
		else if (dir.y > 0)
		{
			player.MoveDown(deltaTime);
		}
		else if (dir.y < 0)
		{
			player.MoveUp(deltaTime);
		}
	}

	void MovingState::OnEnter(PacManCharacter& player)
	{
		auto dir = player.GetMovementDirection();
		if (dir.x > 0)
		{
			player.MoveRight(0);
		}
		else if (dir.x < 0)
		{
			player.MoveLeft(0);
		}
		else if (dir.y > 0)
		{
			player.MoveDown(0);
		}
		else if (dir.y < 0)
		{
			player.MoveUp(0);
		}
		
	}

	void MovingState::OnExit(PacManCharacter& )
	{
		// Logic when exiting DownState
	}
}
