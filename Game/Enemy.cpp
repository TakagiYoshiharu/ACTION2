#include "stdafx.h"
#include "Enemy.h"
#include"CollisonGroup.h"
#include"Player.h"
#include"Game.h"
#include"Enemy2.h"
 //Bullet → K2Engine Vector3 変換関数
//inline nsK2EngineLow::Vector3 ToVector3(const btVector3& v) {
//    return nsK2EngineLow::Vector3(v.getX(), v.getY(), v.getZ());
//}

Enemy::Enemy()
{
  m_modelRender.Init("Assets/modelData/enemyDX.tkm");

}

Enemy::~Enemy() {

}

bool Enemy::Start() {

    m_moveDirection = { 0.0f, 0.0f, 0.0f };

    return true;
}

void Enemy::Move() {
    //地面に付いていたら。
    if (m_characterController.IsOnGround())
    {
        //重力を無くす。
        m_moveGravity.y = 0.0f;
    }

    //地面に付いていなかったら。
    else
    {
        //重力を発生させる。
        m_moveGravity.y -= 6.8f;
    }
}


void Enemy::Update() {
    if (m_isDead) {
        if (m_deathTimer > 0.0f) {
            m_deathTimer -= g_gameTime->GetFrameDeltaTime();
            if (m_deathTimer <= 0.0f) {
                DeleteGO(this);
                return;
            }
        }
        else
        {
            DeleteGO(this);
            return;
        }
    }
   
    Move();

    float deltaTime = g_gameTime->GetFrameDeltaTime();
    m_moveTimer += deltaTime;
  
    // プレイヤーとの距離を計算
    Player* player = Game::GetInstance()->GetPlayer();
    if (player == nullptr) {
        return; // もしくは適切な処理
    }
    Vector3 playerPos = player->GetPosition();
    Vector3 enemyPos = m_characterController.GetPosition();
    float distanceToPlayer=(playerPos-enemyPos).Length();
    switch (m_state) {
    case Patrol:
        if (distanceToPlayer < 300.0f) {
            m_state = Chase;
            if (m_characterController.IsOnGround()) {
                m_velocity.y = 400.0f;
            }
            // プレイヤー方向に向けて移動開始
            Vector3 toPlayer = playerPos - enemyPos;
            toPlayer.Normalize();
            m_moveDirection = { toPlayer.x, 0.0f, toPlayer.z };
            m_moveSpeed = 100.0f;
        }
        else
        {
            m_moveSpeed = 30.0f;
            if (m_moveTimer >= m_switchInterval) {
                m_moveDirection.x *= -1.0f;
                m_moveTimer = 0.0f;
            }
        }
        break;
    case Idle:
        m_idleTimer += deltaTime;
        if (m_idleTimer >= m_idleDuration) {
            if (distanceToPlayer < 300.0f) {
                m_state = Chase;

                // ★ ジャンプ処理を追加（地面にいるときのみ）
                if (m_characterController.IsOnGround()) {
                    m_velocity.y = 400.0f;
                }

                // プレイヤー方向に向けて移動開始
                Vector3 toPlayer = playerPos - enemyPos;
                toPlayer.Normalize();
                m_moveDirection = { toPlayer.x, 0.0f, toPlayer.z };
                m_moveSpeed = 100.0f;
            }
            else {
                m_state = Return;
            }
        }
        break;

    case Chase:
        if (distanceToPlayer >= 300.0f) {
            m_chaseLostTimer += deltaTime;
            if (m_chaseLostTimer >= m_chaseLostThreshold) {
                m_state = Idle;
                m_idleTimer = 0.0f;
                m_chaseLostTimer = 0.0f;
            }
        }
        else
        {
            m_chaseLostTimer = 0.0f;
            Vector3 toPlayer = playerPos - enemyPos;
            toPlayer.Normalize();
            m_moveDirection = { toPlayer.x,0.0f,toPlayer.z };
            m_moveSpeed = 100.0f;
        }
        break;
    case Return:
        // プレイヤーが近くにいるなら即座に追尾に切り替える
        if (distanceToPlayer < 300.0f) {
            m_state = Chase;
            // ★ ジャンプ処理を追加（地面にいるときのみ）
            if (m_characterController.IsOnGround()) {
                m_velocity.y = 400.0f;
            }

            // プレイヤー方向に向けて移動開始
            Vector3 toPlayer = playerPos - enemyPos;
            toPlayer.Normalize();
            m_moveDirection = { toPlayer.x, 0.0f, toPlayer.z };
            m_moveSpeed = 100.0f;
            break;
        }

        Vector3 toHome = m_initialPosition - enemyPos;
        float distanceToHome = toHome.Length();
        if (distanceToHome < 10.0f) {
            m_state = Patrol;
        }
        else
        {
            toHome.Normalize();
            m_moveDirection = { toHome.x,0.0f,toHome.z };
            m_moveSpeed = 30.0f;
        }
        break;
    }
    Vector3 moveVec = Vector3::Zero;
    if (m_state != Idle) {
        // 通常の移動方向に速度を加える
        Vector3 targetVelocity = m_moveDirection * m_moveSpeed;

        // 慣性を加味した速度更新
        m_velocity = m_velocity * m_friction + targetVelocity * (1.0f - m_friction);
    }
    else {
        // Idle中は慣性だけで滑るように止まる
        m_velocity = m_velocity * m_friction;
    }

    // 重力を加算
    m_velocity.y += m_moveGravity.y;

    // 実際の移動
    Vector3 newPos = m_characterController.Execute(m_velocity, deltaTime);

    float dist = (player->GetPosition() - m_characterController.GetPosition()).Length();
    if (!m_isDead && dist < 55.0f && m_characterController.GetCollider()->IsIntersect(*player->GetCharacterController()->GetCollider())) {
        OnContact(player);
    }

    auto& shells = Game::GetInstance()->GetEnemy2s();
    for (Enemy2* shell : shells) {
        if (shell == nullptr) continue;              // ← まずチェック
        if (shell->IsDead()) continue;               // 死んでいるならスキップ

        CharacterController* shellCC = shell->GetCharacterController();
        if (shellCC == nullptr) continue;            // ← ここも安全に

        if (shell->GetState() == Enemy2::EnemyState::ShellMoving) {
            Vector3 shellPos = shell->GetPosition();
            float distShell = (shellPos - enemyPos).Length();
            if (distShell < 55.0f &&
                m_characterController.GetCollider()->IsIntersect(*shellCC->GetCollider()))
            {
                m_isDead = true;
                if (m_characterController.IsOnGround()) {
                    m_velocity.y = 1500.0f;
                }
                m_deathTimer = 0.5f;
            }
        }
    }


    Vector3 modelOffset = { 0.0f, -m_enemyHeight * 0.5f, 0.0f };
    m_modelRender.SetPosition(newPos + modelOffset);

    // 落下判定：Y座標が一定以下なら消す
    if (!m_isDead && newPos.y < -1000.0f) {
        m_isDead = true;
        DeleteGO(this);
        return; // 以降の処理をスキップ
    }
    
    m_modelRender.Update();
}

bool Enemy::IsAirborne() {
    return !m_characterController.IsOnGround();
}

void Enemy::OnStepped() {
    if (m_isDead) return;
    m_isDead = true;
    m_deathTimer = 0.0f;
}

void Enemy::SetStartPosition(const Vector3& pos) {
    m_initialPosition = pos;

    m_enemyRadius = 25.0f;
    m_enemyHeight = 30.0f;

    Vector3 offset = { 0.0f, -m_enemyHeight * 0.5f, 0.0f };
    Vector3 basePos = pos + offset;

    m_characterController.Init(m_enemyRadius, m_enemyHeight, basePos);
    m_modelRender.SetPosition(basePos);
}

void Enemy::OnContact(IGameObject* other) {
    if (auto player = dynamic_cast<Player*>(other)) {
        player->OnDamaged(); // ← これだけでOK！

        // プレイヤーに衝突したら移動方向を反転
        m_moveDirection.x *= -1.0f;
    }
}

void Enemy::Render(RenderContext& rc) {
 m_modelRender.Draw(rc);
}