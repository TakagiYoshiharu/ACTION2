#pragma once
class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();
	void Update();
	bool Start();
	void Move();
	void Render(RenderContext& rc);
	void OnContact(IGameObject* other);
	bool IsAirborne();
	virtual void OnStepped();
	bool IsDead() const;
	void SetStartPosition(const Vector3& pos);
	enum EnemyState {
		Patrol,//徘徊中
		Idle,//立ち止まり中
		Chase,//追尾中
		Return//戻り中
	};
	EnemyState m_state = Patrol;
	ModelRender m_modelRender;
	RigidBody m_rigidBody;
	Vector3 m_position;
	Vector3 m_moveDirection = { 0.0f, 0.0f, 0.0f }; // 初期は右方向
	Vector3 m_initialDirection;
	CharacterController m_characterController;
	Vector3 m_initialPosition;
	float m_moveSpeed = 30.0f;
	Vector3 m_moveGravity;
	float m_moveTimer = 0.0f;  // 経過時間
	float m_switchInterval = 100.0f; // 方向切り替え間隔（秒）
	float m_idleTimer = 0.0f;       // 立ち止まり時間の計測
	float m_idleDuration = 1.5f;    // 立ち止まる時間（秒）
	Vector3 m_velocity = Vector3::Zero; // 実際の移動速度（慣性含む）
	float m_friction = 0.85f;           // 減速係数（0.0〜1.0）
	float m_chaseLostTimer = 0.0f;
	float m_chaseLostThreshold = 1.0f; // 1秒間見失ったらIdleに切り替える
	float m_enemyHeight = 0.0f;
	float m_enemyRadius = 0.0f;
	bool m_isDead = false;
	CharacterController* GetCharacterController() {
		return&m_characterController;
	}
	Vector3 GetPosition()const {
		return m_characterController.GetPosition();
	}
	float m_deathTimer = 0.0f;//飛んで消える
};
