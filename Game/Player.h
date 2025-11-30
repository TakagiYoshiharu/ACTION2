#pragma once
class Player :public IGameObject
{
public:
    Player();
    ~Player();
    void Update();
    void Render(RenderContext& rc);
    void Move();
    void Rotation();
    void ManageState();
    void PlayAnimation();
    void CheckStepOnEnemy();
    void OnDamaged();
    bool IsAirborne();
    bool IsStepHit(const Vector3& playerPos, const Vector3& enemyPos);
    void ApplyStepJump();
    Quaternion GetRotation() const { return m_rotation; }
    ModelRender m_modelRender;

    Vector3 m_position;
    Vector3 GetPosition() const;
    CCapsuleCollider* GetCollider() {
        return m_characterController.GetCollider();
    }
    Vector3 GetForwardDir()const;
    enum EnAnimationClip {
        enAnimationClip_Idle,
        enAnimationClip_Walk,
        enAnimationClip_Jump,
        enAnimationClip_Run,
        enAnimationClip_Num,
    };

    AnimationClip animationClips[enAnimationClip_Num];
    CharacterController m_characterController;
    Vector3 m_moveSpeed;
    Quaternion m_rotation;
    FontRender m_fontRender;
    FontRender m_fontrender;
    Vector3 m_prevPosition;
    nsK2EngineLow::CharacterController* GetCharacterController() {
        return &m_characterController;
    }
    int m_hp = 3;
    int m_maxHP = 3;
    bool m_isInvincible = false;
    float m_invincibleTimer = 0.0f;
    float g_timeScale = 1.0f; // í èÌÇÕ1.0Åiìôî{Åj
    float g_hitStopTimer = 0.0f;
    int m_playerState = 0;
    bool m_isDashing = false;
    bool m_isFallingToDeath = false;
    float m_fallTimer = 0.0f;
       static const int maxHP = 3;
    SpriteRender m_spriteRender[maxHP];
    bool m_isDead = false;
    float m_deathTimer = 0.0f;
    bool m_hasDeathJumped = false;
    bool IsDead()const { return m_isDead; }
    int GetHP()const { return m_hp; }
    int starCount = 0;
};

