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
    m_shellModelRender.Init("Assets/modelData/nokonoko3.tkm"); // 甲羅モデルのパス
    g_soundEngine->ResistWaveFileBank(3, "Assets/sound/AS_1473759_ゲーム_レトロなSE_（踏む）.wav");     // ポヨン
    g_soundEngine->ResistWaveFileBank(7, "Assets/sound/AS_1387216_【環境音】地面に転がる空き缶.wav");
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

    // 状態ごとの移動ベクトル決定
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

    // 重力処理
    if (m_characterController.IsOnGround()) {
        m_moveGravity.y = 0.0f;
    }
    else {
        m_moveGravity.y -= 6.8f;
    }

    // 実際の移動
    Vector3 totalVelocity = m_velocity + m_moveGravity;
    Vector3 newPos = m_characterController.Execute(totalVelocity, deltaTime);
    m_position = newPos;

    // ★ モデル位置を毎フレーム反映（ここ！）
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

    if (m_state == EnemyState::ShellMoving) {
        float moveDist = (m_position - m_lastPosition).Length();
        if (moveDist < 1.0f) {
            m_stuckTimer += deltaTime;
        }
        else {
            m_stuckTimer = 0.0f;
        }

        if (m_stuckTimer > 0.001f && !IsZero(m_lastHitNormal)) {
            // ★ 反射処理！
            Vector3 normal = m_lastHitNormal;
            normal.y = 0.0f;
            normal.Normalize();

            Vector3 reflectDir = m_velocity - normal * (2.0f * m_velocity.Dot(normal));
            reflectDir.y = 0.0f;
            reflectDir.Normalize();

            m_velocity = reflectDir * m_shellSpeed;
            m_stuckTimer = 0.0f;
            m_lastHitNormal = Vector3::Zero;
        }
    }

    //甲羅復活時間
    if (m_state == EnemyState::ShellStill) {
        if (m_shellStillTimer > 0.0f) {
            m_shellStillTimer -= g_gameTime->GetFrameDeltaTime();
            if (m_shellStillTimer <= 0.0f) {
                m_state = EnemyState::Walking;
                m_velocity = m_initialDirection * m_walkSpeed; // ←ここを修正
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
}


void Enemy2::OnStepped(const Vector3& playerForward) {
    if (m_stepCooldown > 0.0f)return;
    switch (m_state) {
    case EnemyState::Walking:
        // 歩いている亀を踏む → 甲羅停止
        m_state = EnemyState::ShellStill;
        m_velocity = Vector3::Zero;
        m_shellStillTimer = 10.0f;
        {
            SoundSource* se = NewGO<SoundSource>(0); se->Init(3);
            se->SetVolume(3.0f);
            se->Play(false);
        }
        break;
    case EnemyState::ShellStill: {
        m_state = EnemyState::ShellMoving;

        Vector3 playerPos = Game::GetInstance()->GetPlayer()->GetPosition();
        Vector3 dir = m_position - playerPos;
        dir.y = 0.0f;
        if (dir.LengthSq() < 0.001f) {
            dir = Vector3::AxisX;
        }
        dir.Normalize();

        // ★ プレイヤーの前方に60ユニット出してから発射！
        Vector3 shellStartPos = m_position + dir * 60.0f;
        m_position = shellStartPos;
        m_characterController.SetPosition(shellStartPos);
        m_velocity = dir * m_shellSpeed;

        m_invincibleTimer = 0.0f;
        m_stepCooldown = 0.0f;

        {
            SoundSource* se = NewGO<SoundSource>(0);
            se->Init(7);
            se->SetVolume(3.0f);
            se->Play(false);
        }
        break;
    }

    case EnemyState::ShellMoving:
        // 動いている甲羅を踏む → 停止 ＋ プレイヤーを跳ねさせる
        m_state = EnemyState::ShellStill;
        m_velocity = Vector3::Zero;
        // ★ここを追加
        {
            SoundSource* se = NewGO<SoundSource>(0);
            se->Init(3);
            se->SetVolume(3.0f);
            se->Play(false);
        }

        // ★ここでプレイヤーを跳ねさせる
        Game::GetInstance()->GetPlayer()->ApplyStepJump();
        m_stepCooldown = 0.2f;
        m_shellStillTimer = 10.0f;
        break;
    default:
        break;
    }

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
    if (m_state == EnemyState::ShellMoving && result.hitWall) {
        m_lastHitNormal = result.hitNormal;
    }
    //ぶつかるとき
    if (m_state == EnemyState::ShellStill && result.hitPlayer) {
        m_state = EnemyState::ShellMoving;
        if (m_characterController.GetRigidBody()->GetBody() != nullptr) {
            m_characterController.GetRigidBody()->Release();
        }
        Vector3 moveDir = result.playerForward;
        moveDir.y = 0.0f;
        moveDir.Normalize();
        m_velocity = moveDir * m_shellSpeed;
        m_invincibleTimer = 0.2f;
        PhysicsGhostObject::CollisionFilter filter{ enCollisionGroup_Sensor,enCollisionGroup_Player };
    }
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
        m_shellModelRender.Draw(rc); // 甲羅モデル
    }
}