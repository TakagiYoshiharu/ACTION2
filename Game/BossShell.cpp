#include "stdafx.h"
#include "BossShell.h"
#include"Player.h"
#include"Game.h"

bool BossShell::Start() {
	m_modelRender.Init("Assets/modelData/nokonoko3.tkm");
	return true;
}

void BossShell::SetStart(const Vector3& pos, const Vector3& dir) {
	Vector3 basePos = pos;
	m_characterController.Init(30.0f, 30.0f, basePos);
	m_velocity = dir * m_speed;
	m_invincibleTime = 0.1f;
	m_wasIntersecting = false;
	if (auto* player = Game::GetInstance()->GetPlayer()) {
		player->m_ignoreShellThisFrame = true;
	}
}

void BossShell:: Update() {

	if (!m_characterController.GetCollider() || !m_characterController.GetCollider()->GetBody()) {
		DeleteGO(this);
		return;
	}
	float dt = g_gameTime->GetFrameDeltaTime();
	Vector3 newPos = m_characterController.Execute(m_velocity,dt);
	Vector3 modelOffset = { 0.0f,30.0f,0.0f };
	m_modelRender.SetPosition(newPos+modelOffset);
	m_modelRender.Update();
	bool justBecameVulnerable = false;
	if (m_invincibleTime > 0.0f) {
		m_invincibleTime -= dt;
		if (m_invincibleTime <= 0.0f) {
			justBecameVulnerable = true;
		}
	}
	Player* player = Game::GetInstance()->GetPlayer();
	bool isIntersecting = false;
	auto* myCollider = m_characterController.GetCollider();
	auto* playerCC = player ? player->GetCharacterController() : nullptr;
	auto* playerCollider = playerCC ? playerCC->GetCollider() : nullptr;
	if (player && myCollider && myCollider->GetBody() && playerCollider && playerCollider->GetBody()) {
		isIntersecting = myCollider->IsIntersect(*playerCollider);
	}
	if (justBecameVulnerable) {
		m_wasIntersecting = false;
	}
	m_wasIntersecting = isIntersecting;
	m_lifetime -= dt;
	if (m_lifetime<=0.0f) {
		DeleteGO(this);
	}
	if (m_invincibleTime <= 0.0f && m_velocity.LengthSq() > 1.0f) {
		if (isIntersecting && !m_wasIntersecting) {
			if (player && !player->IsDead() && !player->m_isInvincible) {
				player->OnDamaged();
			}
			DeleteGO(this);
			return;
		}
	}
}

Vector3 BossShell::GetPosition() const {
	return m_characterController.GetPosition();
}

void BossShell::Stop() {
	m_velocity = Vector3::Zero;
	m_isStopped = true;
}

void BossShell::Kick(const Vector3& dir) {
	m_velocity = dir * m_speed;
	m_isStopped = false;
}

void BossShell::Render(RenderContext& rc) {
        m_modelRender.Draw(rc);
}