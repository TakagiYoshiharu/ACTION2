#pragma once
#include"Enemy.h"
class Enemy2:public IGameObject
{
public:
	Enemy2();
	~Enemy2();
	void Update();
	bool Start();
	void Move();
	void OnStepped(const Vector3&playerForward);
	void SetPlayerGhost(bool ghost);
	void Render(RenderContext& rc);
	void SetStartPosition(const Vector3& pos);
	void ExitShellMode();
	float GetInvincbleTimer()const { return m_invincibleTimer;};
	void OnDefeated();
	void SetStartDirection(const Vector3& dir);
	enum class EnemyState
	{
		Walking,
		ShellStill,
		ShellMoving,
		Defeated
	};
	struct  CollisionResult
	{
		bool hitWall = false;
		bool hitWallX = false;
		bool hitWallZ = false;
		bool hitEnemy = false;
		bool hitPlayer = false;
		Enemy2* enemy = nullptr; // Õ“Ë‚µ‚½“G‚ğ•Û‚µ‚½‚¢ê‡
		Vector3 playerForward = Vector3::Zero;
		Vector3 hitNormal;
	};
	void OnCollision(const CollisionResult& result);
	EnemyState GetState()const { return m_state; }
	Vector3 m_position=Vector3::Zero;
	Vector3 m_velocity=Vector3::Zero;
	Vector3 GetPosition()const;
	bool IsDead()const;
	EnemyState m_state = EnemyState::Walking;
	bool m_isSteppedOn = false;
	bool m_isGhostForPlayer = false;
	PhysicsGhostObject m_shellGhost;
	bool m_shellGhostActive = false;
	CharacterController m_characterController;
	ModelRender m_modelRender;
	ModelRender m_shellModelRender;
	RigidBody m_rigidBody;
	float m_radius = 25.0f;
	float m_height = 20.0f;
	float m_turtleRadius = 30.0f;
	float m_turtleHeight = 60.0f;

	Vector3 m_initialPosition;
	Vector3 m_moveGravity;
	float m_moveTimer = 0.0f;  // Œo‰ßŠÔ
	float m_shellRadius = 100.0f;
	float m_shellHeight = 100.0f;
	float m_walkSpeed = 50.0f;//nokoˆÚ“®‘¬“x
	float m_shellSpeed = 500.0f;//b—…‚ÌˆÚ“®‘¬“x
	float m_stuckTimer = 0.0f;//ƒXƒ^ƒbƒNŠÔ
	Vector3 m_lastPosition;
	float m_invincibleTimer = 0.0f;//b—…‚ª“®‚­‚Æ‚«‚Ì–³“G
	float m_shellStillTimer = 0.0f;//b—…’â~ŠÔ
	CharacterController* GetCharacterController();
	float m_deathTimer = 0.0f;//”ò‚ñ‚ÅÁ‚¦‚é
	bool m_initialized = false;
	float m_stepCooldown = 0.0f;
	float m_shakeTimer = 0.0f;//k‚¦ŠÔ
	Vector3 m_initialDirection = Vector3::Right;
	nsK2EngineLow::CCapsuleCollider* m_capsuleCollider = nullptr;
};

