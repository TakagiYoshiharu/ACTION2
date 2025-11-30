#pragma once
class Player;
class GameCamera :public IGameObject
{
public:
	GameCamera();
	void Update();
	bool Start();
private:
	Player* m_player;
	Vector3 m_toCameraPos;
	float m_currentTargetYOffset = 100.0f;
	bool m_isCameraFrozen = false;
	Vector3 m_frozenPos;
	Vector3 m_frozenTarget;
	Vector3 m_lastPos;
	Vector3 m_lastTarget;
};

