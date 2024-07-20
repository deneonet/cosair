#define RUN_FLAPPYBIRD
#ifdef RUN_FLAPPYBIRD
#include <Cosair.h>

#include <cmath>
#include <chrono>
#include <random>

#include <imgui/imgui.h>

class GameLayer : public Cosair::Layer {
public:
    virtual void OnAttach() override {
        m_Gen = std::mt19937(m_Rd());
        m_Dis = std::uniform_real_distribution<>(-0.45, 0.35f);

        // Transforms
        Cosair::TransformRef playerTransform = MAKE_TRANSFORM_REF();
        playerTransform->SetScale(glm::vec3(0.15f));

        Cosair::TransformRef backgroundTransform = MAKE_TRANSFORM_REF();
        backgroundTransform->SetScale(glm::vec3(3.3f, 2, 3.3f));

        Cosair::TransformRef scoreTransform = MAKE_TRANSFORM_REF(glm::vec3(0, 0.7f, 0));
        scoreTransform->SetScale(glm::vec3(0.1f));

        m_ScoreDigitOneTransform = MAKE_TRANSFORM_REF(glm::vec3(-0.063, 0.7f, 0));
        m_ScoreDigitOneTransform->SetScale(glm::vec3(0.1f));
        m_ScoreDigitTwoTransform = MAKE_TRANSFORM_REF(glm::vec3(0.063, 0.7f, 0));
        m_ScoreDigitTwoTransform->SetScale(glm::vec3(0.1f));

        // Object and Text Textures
        Cosair::MaterialRef backgroundMaterial = MAKE_MATERIAL_REF(backgroundTransform);
        backgroundMaterial->SetTexture(CREATE_TEXTURE2D("assets/textures/background.png"));
        Cosair::MaterialLibrary::Add("Background", backgroundMaterial);

        Cosair::MaterialRef groundMaterial = MAKE_MATERIAL_REF();
        groundMaterial->SetTexture(CREATE_TEXTURE2D("assets/textures/ground.png"));
        Cosair::MaterialLibrary::Add("Ground", groundMaterial);

        Cosair::MaterialRef pillarMaterial = MAKE_MATERIAL_REF();
        pillarMaterial->SetTexture(CREATE_TEXTURE2D("assets/textures/pillar.png"));
        Cosair::MaterialLibrary::Add("Pillar", pillarMaterial);

        Cosair::MaterialRef gameOverMaterial = MAKE_MATERIAL_REF();
        gameOverMaterial->SetTexture(CREATE_TEXTURE2D("assets/textures/gameover.png"));
        Cosair::MaterialLibrary::Add("GameOver", gameOverMaterial);

        // Player Textures
        m_PlayerMaterial = MAKE_MATERIAL_REF(playerTransform);
        Cosair::Texture2dLibrary::Add("Player_Up", CREATE_TEXTURE2D("assets/textures/player-upflap.png"));
        Cosair::Texture2dLibrary::Add("Player_Mid", CREATE_TEXTURE2D("assets/textures/player-midflap.png"));
        Cosair::Texture2dLibrary::Add("Player_Down", CREATE_TEXTURE2D("assets/textures/player-downflap.png"));

        // Number Textures
        m_ScoreMaterial = MAKE_MATERIAL_REF(scoreTransform);
        Cosair::Texture2dLibrary::Add("0", CREATE_TEXTURE2D("assets/textures/0.png"));
        Cosair::Texture2dLibrary::Add("1", CREATE_TEXTURE2D("assets/textures/1.png"));
        Cosair::Texture2dLibrary::Add("2", CREATE_TEXTURE2D("assets/textures/2.png"));
        Cosair::Texture2dLibrary::Add("3", CREATE_TEXTURE2D("assets/textures/3.png"));
        Cosair::Texture2dLibrary::Add("4", CREATE_TEXTURE2D("assets/textures/4.png"));
        Cosair::Texture2dLibrary::Add("5", CREATE_TEXTURE2D("assets/textures/5.png"));
        Cosair::Texture2dLibrary::Add("6", CREATE_TEXTURE2D("assets/textures/6.png"));
        Cosair::Texture2dLibrary::Add("7", CREATE_TEXTURE2D("assets/textures/7.png"));
        Cosair::Texture2dLibrary::Add("8", CREATE_TEXTURE2D("assets/textures/8.png"));
        Cosair::Texture2dLibrary::Add("9", CREATE_TEXTURE2D("assets/textures/9.png"));

        m_PlayerState = PlayerState::Idle;
    }

    virtual void OnUpdate(Cosair::Timestep ts) override {
        // Player Falling and Moving Up
        m_StateStartTime += ts.GetMilliseconds();

        switch (m_PlayerState) {
        case PlayerState::GameOver:
            if (Cosair::Input::IsMouseButtonPressed(CR_MOUSE_BUTTON_LEFT)) {
                ResetGame();
            }
            break;
        case PlayerState::Idle:
        case PlayerState::Falling:
            if (Cosair::Input::IsKeyPressed(CR_KEY_SPACE) && m_StateStartTime >= 100) {
                m_StateStartTime = 0;
                m_PlayerState = PlayerState::MovingUp;
                m_Velocity = m_PlayerJumpPower;
            }

            // Apply gravity
            m_Velocity -= m_Gravity * ts;
            m_PlayerMaterial->SetPositionY(
                m_PlayerMaterial->GetPositionY() + m_Velocity * ts
            );

            // Ground hit check
            if (m_PlayerMaterial->GetPositionY() <= -0.6f) {
                SetGameOver();
            }
            break;

        case PlayerState::MovingUp:
            m_Velocity -= m_Gravity * ts;
            m_PlayerMaterial->SetPositionY(
                m_PlayerMaterial->GetPositionY() + m_Velocity * ts
            );

            if (m_Velocity <= 0) {
                m_PlayerState = PlayerState::Falling;
            }
            break;
        }

        if (m_PlayerState != PlayerState::GameOver) {
            // Collision Check
            CheckCollision();

            // Fall and Move up rotation
            float targetRotation = -50.0f;
            if (m_Velocity > -0.4)
                targetRotation = 25.0f;

            float rotationSpeed = 5.f;
            float currentRotation = m_PlayerMaterial->GetRotation().z;
            float newRotation = currentRotation + (targetRotation - currentRotation) * ts * rotationSpeed;
            m_PlayerMaterial->SetRotation(glm::vec3(0, 0, newRotation));

            // Generating new pillars & grounds
            m_LastGeneratedPillarTime += ts.GetMilliseconds();
            UpdatePillars();

            m_LastGeneratedGroundTime += ts.GetMilliseconds();
            UpdateGrounds();

            // Player Animation
            m_LastAnimTime += ts.GetMilliseconds();
            if (m_LastAnimTime > 100) {
                if (m_CurrentAnimIndex == 3)
                    m_CurrentAnimIndex = 0;

                m_PlayerMaterial->SetTexture(Cosair::Texture2dLibrary::Load(
                    m_CurrentAnimIndex == 0 ? "Player_Up" :
                    m_CurrentAnimIndex == 1 ? "Player_Mid" :
                    "Player_Down"
                ));

                m_CurrentAnimIndex++;
                m_LastAnimTime = 0;
            }
        }

        // Rendering
        Cosair::Renderer2D::BeginScene(m_Camera);
        Cosair::Renderer2D::DrawQuad("Background");

        if (m_PlayerState != PlayerState::GameOver) {
            Cosair::Renderer2D::DrawQuad(m_PlayerMaterial);
            RenderPillarsAndGround(ts);
        }
        else
            // Could have made a new transform for the gameover text, but why though?
            Cosair::Renderer2D::DrawQuad("GameOver", m_PlayerMaterial->GetTransform());

        RenderScore();
        Cosair::Renderer2D::EndScene();
    }

private:
    void UpdatePillars() {
        if (m_LastGeneratedPillarTime >= 2700) {
            m_Score++;
            m_LastGeneratedPillarTime = 0;

            float pillarGap = 0.37f;
            float pillarWidth = 0.25f;

            float randomHeight = m_Dis(m_Gen);
            float pillarPositionY = randomHeight + .5f;

            Cosair::TransformRef upperPillar = MAKE_TRANSFORM_REF(
                glm::vec3(1.7f, pillarPositionY + pillarGap, randomHeight), glm::vec3(0, 0, 180), glm::vec3(pillarWidth, 1, 1)
            );

            Cosair::TransformRef lowerPillar = MAKE_TRANSFORM_REF(
                glm::vec3(1.7f, randomHeight-.5f, 0), glm::vec3(0), glm::vec3(pillarWidth, 1, 1)
            );

            if (m_PillarsOffset == 3)
                m_PillarsOffset = 0;
            m_Pillars[m_PillarsOffset++] = { upperPillar, lowerPillar };
        }
    }

    void UpdateGrounds() {
        if (m_LastGeneratedGroundTime >= 3000) {
            m_LastGeneratedGroundTime = 0;

            float groundWidth = 2.5f;
            float groundHeight = 0.27f;

            Cosair::TransformRef transform = MAKE_TRANSFORM_REF(
                glm::vec3(groundWidth, -0.77, 0)
            );
            transform->SetScale(glm::vec3(groundWidth, groundHeight, groundWidth));

            if (m_GroundsOffset == 3)
                m_GroundsOffset = 0;
            m_Grounds[m_GroundsOffset++] = transform;
        }
    }

    void RenderPillarsAndGround(Cosair::Timestep ts) {
        for (int i = 0; i < 4; i++) {
            auto& [topPillar, bottomPillar] = m_Pillars[i];
            if (!topPillar)
                break;

            float newPositionX = topPillar->GetPositionX() - m_ObjectMoveSpeed * ts;
            topPillar->SetPositionX(newPositionX);
            bottomPillar->SetPositionX(newPositionX);
            Cosair::Renderer2D::DrawQuad("Pillar", topPillar);
            Cosair::Renderer2D::DrawQuad("Pillar", bottomPillar);
        }

        for (int i = 0; i < 4; i++) {
            auto& ground = m_Grounds[i];
            if (!ground)
                break;

            float newPositionX = ground->GetPositionX() - m_ObjectMoveSpeed * ts;
            ground->SetPositionX(newPositionX);
            Cosair::Renderer2D::DrawQuad("Ground", ground);
        }
    }

    void RenderScore() {
        if (m_Score == -1)
            return;

        std::string scoreStr = std::to_string(m_Score);

        if (scoreStr.length() == 1) {
            m_ScoreMaterial->SetTexture(
                Cosair::Texture2dLibrary::Load(scoreStr.c_str())
            );
            Cosair::Renderer2D::DrawQuad(m_ScoreMaterial);
            return;
        }

        if (scoreStr.length() == 2) {
            m_ScoreMaterial->SetTexture(
                Cosair::Texture2dLibrary::Load(scoreStr.substr(0, 1).c_str())
            );
            Cosair::Renderer2D::DrawQuad(m_ScoreMaterial, m_ScoreDigitOneTransform);

            m_ScoreMaterial->SetTexture(
                Cosair::Texture2dLibrary::Load(scoreStr.substr(1, 1).c_str())
            );
            Cosair::Renderer2D::DrawQuad(m_ScoreMaterial, m_ScoreDigitTwoTransform);
            return;
        }

        CR_ERROR("Exceeded score of 99, score is not being rendered anymore.");
    }

    void SetGameOver() {
        m_PlayerState = PlayerState::GameOver;
        m_PlayerMaterial->SetTransform(MAKE_TRANSFORM_REF(glm::vec3(-0.035, 0, 0), glm::vec3(0), glm::vec3(1.3f, 0.4f, 1.3f)));
    }

    void ResetGame() {
        m_Score = -1;
        m_PillarsOffset = 0;
        m_GroundsOffset = 2;
        m_StateStartTime = 0;
        m_LastGeneratedPillarTime = 2500.f;
        m_LastGeneratedGroundTime = 3000.f;

        m_Velocity = 0;
        m_PlayerMaterial->SetTransform(MAKE_TRANSFORM_REF(glm::vec3(0), glm::vec3(0), glm::vec3(0.15f)));

        std::fill(m_Pillars, m_Pillars+4, std::pair<Cosair::TransformRef, Cosair::TransformRef>());

        m_Grounds[0] = MAKE_TRANSFORM_REF(glm::vec3(-2.5f, -0.77, 0), glm::vec3(0), glm::vec3(2.5f, 0.27f, 2.5f));
        m_Grounds[1] = MAKE_TRANSFORM_REF(glm::vec3(0, -0.77, 0), glm::vec3(0), glm::vec3(2.5f, 0.27f, 2.5f));

        m_PlayerState = PlayerState::Idle;
    }

    void CheckCollision() {
        for (int i = 0; i < 4; i++) {
            auto& [topPillar, bottomPillar] = m_Pillars[i];
            if (!topPillar)
                break;

            float playerWidth = 0.15f;
            float yPosOffset = 0.55f;

            float playerPosX = m_PlayerMaterial->GetPositionX();
            float playerPosY = m_PlayerMaterial->GetPositionY();

            if (playerPosX + playerWidth >= topPillar->GetPositionX() &&
                playerPosX <= topPillar->GetPositionX() + 0.25f &&
                playerPosY + yPosOffset >= topPillar->GetPositionY())
                SetGameOver();
            else if (playerPosX + playerWidth >= bottomPillar->GetPositionX() &&
                playerPosX <= bottomPillar->GetPositionX() + 0.25f &&
                playerPosY - yPosOffset <= bottomPillar->GetPositionY())
                SetGameOver();
        }
    }
private:
    // Number generator
    std::mt19937 m_Gen;
    std::random_device m_Rd;
    std::uniform_real_distribution<> m_Dis;

    // Player
    enum class PlayerState {
        Idle,
        MovingUp,
        Falling,
        GameOver
    };

    Cosair::OrthographicCamera m_Camera;

    Cosair::TransformRef m_ScoreDigitOneTransform;
    Cosair::TransformRef m_ScoreDigitTwoTransform;

    Cosair::MaterialRef m_ScoreMaterial;
    Cosair::MaterialRef m_PlayerMaterial;

    int m_Score = -1;
    float m_Gravity = 2.f;
    float m_Velocity = 0.0f;
    float m_PlayerJumpPower = 0.9f;

    int m_CurrentAnimIndex = 0;
    float m_LastAnimTime = 0.f;

    PlayerState m_PlayerState;
    float m_StateStartTime = 0.f;

    // Objects (Pillars + Ground)
    float m_ObjectMoveSpeed = 0.6f;

    // Pillars
    int m_PillarsOffset = 0;
    std::pair<Cosair::TransformRef, Cosair::TransformRef> m_Pillars[4];

    float m_LastGeneratedPillarTime = 2500.f;

    // Ground
    int m_GroundsOffset = 2;
    Cosair::TransformRef m_Grounds[4]{
        MAKE_TRANSFORM_REF(glm::vec3(-2.5f, -0.77, 0), glm::vec3(0), glm::vec3(2.5f, 0.27f, 2.5f)),
        MAKE_TRANSFORM_REF(glm::vec3(0, -0.77, 0), glm::vec3(0), glm::vec3(2.5f, 0.27f, 2.5f))
    };

    float m_LastGeneratedGroundTime = 3000.f;
};

class FlappyBird : public Cosair::Application {
public:
    FlappyBird() {
        PushLayer(new GameLayer());
    }

    ~FlappyBird() {

    }
};

Cosair::Application* Cosair::CreateApplication() {
    return new FlappyBird();
}
#endif