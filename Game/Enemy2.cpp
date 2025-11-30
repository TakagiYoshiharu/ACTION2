#include "stdafx.h"
#include "Enemy2.h"
#include"Game.h"
#include"Player.h"
#include"Enemy.h"
#include"CollisonGroup.h"

CharacterController* Enemy2::GetCharacterController() {
    return &m_characterController;
}


Enemy2::Enemy2()
{
    m_modelRender.Init("Assets/modelData/nokonoko1.tkm");
    m_shellModelRender.Init("Assets/modelData/nokonoko3.tkm"); // çbóÖÉÇÉfÉãÇÃÉpÉX
}

Enemy2::~Enemy2() {

}

bool Enemy2::Start() {
    m_velocity = m_initialDirection * m_walkSpeed;
    return true;
}

inline bool IsZero(const Vector3& v) {
    return fabs(v.x) < 0.0001f && fabs(v.y) < 0.0001f && fabs(v.z) < 0.0001f;
}

void Enemy2::Move() {
    float deltaTime = g_gameTime->GetFrameDeltaTime();

    // èÛë‘Ç≤Ç∆ÇÃà⁄ìÆÉxÉNÉgÉãåàíË
    Vector3 moveVec = Vector3::Zero;
    switch (m_state) {
    case EnemyState::Walking:
        moveVec = m_velocity;
        break;
    case EnemyState::ShellStill:
        moveVec = Vector3::Zero;
        break;
    case EnemyState::ShellMoving:
        moveVec = m_velocity;
        break;
    }

    // èdóÕèàóù
    if (m_characterController.IsOnGround()) {
        m_moveGravity.y = 0.0f;
    }
    else {
        m_moveGravity.y -= 6.8f;
    }

    // é¿ç€ÇÃà⁄ìÆ
    Vector3 totalVelocity = m_velocity + m_moveGravity;
    Vector3 newPos = m_characterController.Execute(totalVelocity, deltaTime);
    m_position = newPos;

    // Åö ÉÇÉfÉãà íuÇñàÉtÉåÅ[ÉÄîΩâfÅiÇ±Ç±ÅIÅj
    Vector3 modelOffset = { 0.0f, -m_height * 0.1f, 0.0f };
    Vector3 modelPos = m_position + modelOffset;
    if (m_state == EnemyState::Walking) {
        m_modelRender.SetPosition(modelPos);
    }
    else {
        m_shellModelRender.SetPosition(modelPos);
    }
}


void Enemy2::Update() {
    Move();

    float deltaTime = g_gameTime->GetFrameDeltaTime();
    m_shakeTimer += deltaTime;

    if (m_stepCooldown > 0.0f) {
        m_stepCooldown -= g_gameTime->GetFrameDeltaTime();
    }

    //çbóÖïúäàéûä‘
    if (m_state == EnemyState::ShellStill) {
        if (m_shellStillTimer > 0.0f) {
            m_shellStillTimer -= g_gameTime->GetFrameDeltaTime();
            if (m_shellStillTimer <= 0.0f) {
                m_state = EnemyState::Walking;
                m_velocity = m_initialDirection * m_walkSpeed; // Å©Ç±Ç±ÇèCê≥
                ExitShellMode();
            }
        }
    }
    if (m_state == EnemyState::ShellStill && m_shellStillTimer <= 2.0f) {
        float angleDeg = sin(m_shakeTimer * 25.0f) * 100.0f;
        Quaternion rot;
        rot.SetRotationDeg(g_vec3AxisY, angleDeg);
        m_shellModelRender.SetRotation(rot);
    }
    else
    {
        m_shellModelRender.SetRotation(Quaternion::Identity);
    }

    auto& enemies = Game::GetInstance()->GetEnemies();
    for (Enemy* enemy : enemies) {
        if (!enemy || enemy->IsDead())continue;
        if (m_state == EnemyState::ShellMoving) {
            float dist = (enemy->GetPosition() - m_position).Length();
            if (dist < 40.0f && m_characterController.GetCollider()->IsIntersect(*enemy->GetCharacterController()->GetCollider())) {
                enemy->OnStepped();
            }
        }
    }
    auto& turtles = Game::GetInstance()->GetEnemy2s();
    for (Enemy2* turtle : turtles) {
        if (!turtle || turtle->IsDead() || turtle == this)continue;
        if (m_state == EnemyState::ShellMoving && turtle->m_state == EnemyState::Walking) {
            float dist = (turtle->GetPosition() - m_position).Length();
            if (dist < 60.0f && m_characterController.GetCollider()->IsIntersect(*turtle->GetCharacterController()->GetCollider())) {
                turtle->OnDefeated();
                if (m_state == EnemyState::Walking) {
                    if (m_characterController.IsOnGround()) {
                        m_velocity.y = 1500.0f;
                    }
                    m_deathTimer = 0.5f;
                }
            }
        }
    }
    auto& turtles2 = Game::GetInstance()->GetEnemy2s();
    for (Enemy2* turtle2 : turtles2) {
        if (!turtle2 || turtle2->IsDead() || turtle2 == this)continue;
        if (m_state == EnemyState::ShellMoving && turtle2->m_state == EnemyState::ShellStill) {
            float dist = (turtle2->GetPosition() - m_position).Length();
            if (dist < 60.0f && m_characterController.GetCollider()->IsIntersect(*turtle2->GetCharacterController()->GetCollider())) {
                turtle2->OnDefeated();
                if (m_state == EnemyState::ShellStill) {
                    if (m_characterController.IsOnGround()) {
                        m_velocity.y = 1500.0f;
                    }
                    m_deathTimer = 0.5f;
                }
            }
        }
    }

    if (m_state == EnemyState::ShellMoving && m_invincibleTimer > 0.0f) {
        m_invincibleTimer -= g_gameTime->GetFrameDeltaTime();
        if (m_invincibleTimer < 0.0f)m_invincibleTimer = 0.0f;
    }

    m_lastPosition = m_position;
    m_modelRender.Update();
    m_shellModelRender.Update();
    bool ghost = (m_state == EnemyState::ShellMoving);
    SetPlayerGhost(ghost);
    m_isGhostForPlayer = ghost;
}


void Enemy2::OnStepped(const Vector3& playerForward) {
    if (m_stepCooldown > 0.0f)return;
    switch (m_state) {
    case EnemyState::Walking:
        // ï‡Ç¢ÇƒÇ¢ÇÈãTÇì•Çﬁ Å® çbóÖí‚é~
        m_state = EnemyState::ShellStill;
        m_velocity = Vector3::Zero;
        ExitShellMode();
        m_shellStillTimer = 10.0f;
        break;
   case EnemyState::ShellStill: {
    // í‚é~íÜÇÃçbóÖÇì•Çﬁ Å® èRÇ¡Çƒì]Ç™ÇÈÅiíµÇÀÇ»Ç¢Åj
    m_state = EnemyState::ShellMoving;

    SetPlayerGhost(true);
    m_characterController.RemoveRigidBoby();
    if (!m_shellGhostActive) {
        m_shellGhost.CreateCapsule(m_position, Quaternion::Identity, m_radius, m_height);
        m_shellGhostActive = true;
    }
    Vector3 playerPos = Game::GetInstance()->GetPlayer()->GetPosition();
    Vector3 forwardDir = Game::GetInstance()->GetPlayer()->GetForwardDir();
    forwardDir.y = 0.0f;
    forwardDir.Normalize();
    Vector3 shellStartPos = playerPos + forwardDir * 60.0f;
    m_position = shellStartPos;
    m_characterController.SetPosition(shellStartPos);
    if (m_shellGhostActive) {
        m_shellGhost.SetPosition(shellStartPos);
    }
    m_velocity = forwardDir * m_shellSpeed;
    m_invincibleTimer = 0.0f;
    PhysicsGhostObject::CollisionFilter filter{ enCollisionGroup_Sensor,enCollisionGroup_Player };
    m_shellGhost.SetCollisionFilter(filter);
    m_stepCooldown = 0.0f;
    break;
}

    case EnemyState::ShellMoving:
        // ìÆÇ¢ÇƒÇ¢ÇÈçbóÖÇì•Çﬁ Å® í‚é~ Å{ ÉvÉåÉCÉÑÅ[ÇíµÇÀÇ≥ÇπÇÈ
        m_state = EnemyState::ShellStill;
        m_velocity = Vector3::Zero;
        SetPlayerGhost(false);
        // ÅöÇ±Ç±Çí«â¡
        if (m_rigidBody.GetBody() != nullptr) {
            m_rigidBody.Release();
        }
        if (m_shellGhostActive) {
            m_shellGhost.Release();
            m_shellGhostActive = false;
        }

        // ÅöÇ±Ç±Ç≈ÉvÉåÉCÉÑÅ[ÇíµÇÀÇ≥ÇπÇÈ
        Game::GetInstance()->GetPlayer()->ApplyStepJump();
        m_stepCooldown = 0.2f;
        m_shellStillTimer = 10.0f;
        break;
    default:
        break;
    }

}

void Enemy2::SetPlayerGhost(bool ghost) {
    RigidBody* body = m_characterController.GetRigidBody();
    int group = body->GetCollisionGroup();
    int mask = body->GetCollisionMask();
    if (ghost) {
        mask &= ~enCollisionGroup_Player;
        body->GetBody()->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

    }
    else
    {
        mask |= enCollisionGroup_Player;
        body->GetBody()->setCollisionFlags(0);
        body->GetBody()->setCollisionFlags(body->GetBody()->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }
    body->SetCollisionGroupAndMask(group, mask);
    body->GetBody()->activate(true);

}

Vector3 Enemy2::GetPosition() const {
    return m_characterController.GetPosition();
}

bool Enemy2::IsDead() const {
    return m_isDead;
}

void Enemy2::SetStartPosition(const Vector3& pos) {
    m_initialPosition = pos;

    Vector3 offset = { 0.0f, -m_height * 0.5f, 0.0f };
    Vector3 basePos = pos + offset;

    m_characterController.Init(m_radius, m_height, basePos);
    m_position = basePos;

    int mask = enCollisionGroup_Player | enCollisionGroup_Ground | enCollisionGroup_World;
    m_characterController.GetRigidBody()->SetCollisionGroupAndMask(enCollisionGroup_Enemy2, mask);
}


void Enemy2::OnCollision(const CollisionResult& result) {
    //Ç‘Ç¬Ç©ÇÈÇ∆Ç´
    if (m_state == EnemyState::ShellStill && result.hitPlayer) {
        m_state = EnemyState::ShellMoving;
        if (m_characterController.GetRigidBody()->GetBody() != nullptr) {
            m_characterController.GetRigidBody()->Release();
        }
        RigidBodyInitData rbInfo;
        rbInfo.pos = m_position;
        rbInfo.rot = Quaternion::Identity;
        auto* capsule = new nsK2EngineLow::CCapsuleCollider();
        capsule->Init(m_radius, m_height);
        rbInfo.collider = capsule;
        rbInfo.mass = 1.0f;
        rbInfo.restitution = 0.0f;
        rbInfo.collisionGroup = enCollisionGroup_Enemy2;
        rbInfo.collisionMask = enCollisionGroup_Player | enCollisionAttr_Ground | enCollisionGroup_World;
        m_rigidBody.Init(rbInfo);
        Vector3 moveDir = result.playerForward;
        moveDir.y = 0.0f;
        moveDir.Normalize();
        m_velocity = moveDir * m_shellSpeed;
        m_rigidBody.SetLinearVelocity(m_velocity);
        m_invincibleTimer = 0.2f;
        SetPlayerGhost(true);
        if (!m_shellGhostActive) {
            m_shellGhost.CreateCapsule(m_position, Quaternion::Identity, m_radius, m_height);
            m_shellGhostActive = true;
        }
        PhysicsGhostObject::CollisionFilter filter{ enCollisionGroup_Sensor,enCollisionGroup_Player };
        m_shellGhost.SetCollisionFilter(filter);
    }
}
void Enemy2::ExitShellMode() {
    if (m_rigidBody.GetBody() != nullptr) {
        m_rigidBody.Release();
    }
    if (m_shellGhostActive) {
        m_shellGhost.Release();
        m_shellGhostActive = false;
    }
    RigidBody* ccBody = m_characterController.GetRigidBody();
    int group = ccBody->GetCollisionGroup();
    int mask = ccBody->GetCollisionMask();
    mask |= enCollisionGroup_Player;
    ccBody->GetBody()->setCollisionFlags(ccBody->GetBody()->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
    ccBody->SetCollisionGroupAndMask(group, mask);
    ccBody->GetBody()->activate(true);
}

void Enemy2::OnDefeated() {
    if (m_isDead)return;
    m_state = EnemyState::Defeated;
    m_isDead = true;
    if (m_characterController.IsOnGround()) {
        m_velocity.y = 1500.0f;
    }
    m_deathTimer = 0.5f;
}

bool Enemy::IsDead() const {
    return m_isDead;
}


void Enemy2::SetStartDirection(const Vector3& dir) {
    m_initialDirection = dir;
    m_velocity = dir * m_walkSpeed;
}


void Enemy2::Render(RenderContext& rc) {
    if (m_isDead)return;
    if (m_state == EnemyState::Defeated)return;
    if (m_state == EnemyState::Walking) {
        m_modelRender.Draw(rc);
    }
    else {
        m_shellModelRender.Draw(rc); // çbóÖÉÇÉfÉã
    }
}