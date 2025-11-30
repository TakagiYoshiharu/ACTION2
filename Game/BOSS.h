#pragma once
class BOSS:public IGameObject
{
public:
	BOSS();
	~BOSS();
	void Update();
	bool Start();
	void Move(float deltaTime,const Vector3&playerPos,const Vector3&bossPos);
	void Render(RenderContext& rc);
	void SetStartPosition(const Vector3& pos);
	void OnContact(IGameObject* other);
	ModelRender m_modelRender;
	CharacterController m_characterController;
	Vector3 m_initialDirection;
	Vector3 m_velocity = Vector3::Zero;
	float m_BOSSRadius = 0.0f;
	float m_BOSSHeight = 0.0f;
	float m_moveTimer = 0.0f;
	float m_shootTimer = 0.0f;
	bool m_hasDetectedPlayer = true;
	float m_nextThreshold = 300.0f;//チェック座標
	float m_bossStep = 1625.0f;//出現座標
	bool m_pendingWarp = false;
	float m_warpTimer = 0.0f;//ワープ時間
	Vector3 m_reservedWarpPos;
	float m_startY = 0.0f; // BOSS.h に追加
	// BOSS.h（メンバに追加）
	Vector3 m_moveGravity = Vector3::Zero;
	bool m_isVisible = false;
};

