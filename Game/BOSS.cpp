#include "stdafx.h"
#include "BOSS.h"
#include"Player.h"
#include"Game.h"

BOSS::BOSS() {
	m_modelRender.Init("Assets/modelData/BOSS.tkm");
}

BOSS::~BOSS() {

}

bool BOSS::Start() {
    m_isVisible = false; // 最初は隠れている

	return true;
}

void BOSS::Move(float deltaTime,const Vector3&playerPos,const Vector3&bossPos) {
	float chaseSpeed = 30.0f;//圧をかけるスピード
	float stopDistance = 0.0f;//距離制限
	Vector3 velocity = { 0.0f,0.0f,0.0f };
	if (fabs(playerPos.z - bossPos.z) > stopDistance) {
		if (playerPos.z < bossPos.z) {
			velocity.z = -chaseSpeed;
		}
		else if(playerPos.z>bossPos.z)
		{
				velocity.z = chaseSpeed;
		}
	}
	Vector3 newPos = m_characterController.Execute(velocity, deltaTime);
	Vector3 modelOffset = { 0.0f,-m_BOSSHeight * 0.0f,18.0f };
	m_modelRender.SetPosition(newPos + modelOffset);
}

void BOSS::Update() {
    m_modelRender.Update();
    float deltaTime = g_gameTime->GetFrameDeltaTime(); // ここで一度だけ宣言

    Player* player = Game::GetInstance()->GetPlayer();
    if (!player) return;

    Vector3 playerPos = player->GetPosition();
    Vector3 bossPos = m_characterController.GetPosition();
    float dist = (playerPos - bossPos).Length();

    const float appearRange = 1000.0f;
    if (!m_isVisible && dist < appearRange) {
        m_isVisible = true;
    }
    if (!m_isVisible) return;

    // --- 重力処理 ---
    if (m_characterController.IsOnGround()) {
        m_moveGravity.y = 0.0f;
    }
    else {
        m_moveGravity.y -= 9.8f * deltaTime;
    }
    m_velocity.y += m_moveGravity.y;

    Vector3 newPos = m_characterController.Execute(m_velocity, deltaTime);
    Vector3 modelOffset = { 0.0f, -m_BOSSHeight * 0.0f, 18.0f };
    m_modelRender.SetPosition(newPos + modelOffset);

    // 落下チェック
    float fallDistance = m_startY - newPos.y;
    if (fallDistance > 1000.0f) {
        DeleteGO(this);
        return;
    }

    // 認知・ワープ処理
    dist = (playerPos - newPos).Length(); // ここは再代入でOK
    const float awarenessRange = 600.0f;
    m_hasDetectedPlayer = (dist < awarenessRange);

    if (m_hasDetectedPlayer && playerPos.x > m_nextThreshold && !m_pendingWarp) {
        m_pendingWarp = true;
        m_warpTimer = 1.0f;
        m_reservedWarpPos = newPos;
        m_reservedWarpPos.x = m_nextThreshold + m_bossStep;
    }
    if (m_pendingWarp) {
        m_warpTimer -= deltaTime;
        if (m_warpTimer <= 0.0f) {
            m_reservedWarpPos.x = newPos.x + m_bossStep;
            m_characterController.SetPosition(m_reservedWarpPos);
            m_modelRender.SetPosition(m_reservedWarpPos + modelOffset);
            m_nextThreshold = m_reservedWarpPos.x;
            m_pendingWarp = false;
        }
    }

    // 接触判定
    float hitRange = player->IsAirborne() ? 178.0f : 155.0f;
    if (!m_isDead && dist < hitRange &&
        m_characterController.GetCollider()->IsIntersect(*player->GetCharacterController()->GetCollider())) {
        OnContact(player);
    }

    // 近距離なら移動
    if (dist < 250.0f) {
        Move(deltaTime, playerPos, newPos);
    }
}


void BOSS::SetStartPosition(const Vector3& pos) {
	m_initialDirection = pos;
	m_BOSSRadius = 130.0f;
	m_BOSSHeight = 100.0f;

	Vector3 offset = { 0.0f,-m_BOSSHeight * 0.5f,0.0f };
	Vector3 basePos = pos + offset;
	m_characterController.Init(m_BOSSRadius, m_BOSSHeight, basePos);
	m_modelRender.SetPosition(basePos);
    m_startY = basePos.y;
}

void BOSS::OnContact(IGameObject* other) {
	if (auto player = dynamic_cast<Player*>(other)) {
		player->OnDamaged();
	}
}

void BOSS::Render(RenderContext& rc) {
    if (!m_isVisible) return; // 非表示なら描画しない
	m_modelRender.Draw(rc);
}