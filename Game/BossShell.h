#pragma once
class BossShell:public IGameObject
{
public:
	bool Start();
	void Update();
	void SetStart(const Vector3& pos, const Vector3& dir);
	void Stop();
	void Kick(const Vector3& dir);
	void Render(RenderContext& rc);
	Vector3 GetPosition() const;
	bool IsDeleted()const { return m_isDeleted; }
	bool m_isStopped = false;
private:
	ModelRender m_modelRender;
	CharacterController m_characterController;
	Vector3 m_velocity = Vector3::Zero;
	float m_speed = 600.0f;
	float m_lifetime = 1.5f;
	float m_invincibleTime = 0.05f;
	bool m_wasIntersecting = false;
	bool m_isDeleted = false;
};

