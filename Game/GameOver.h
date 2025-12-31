#pragma once
class GameOver:public IGameObject
{
public:
	GameOver();
	~GameOver();
	void Update();
	bool Start();
	void Render(RenderContext& rc);
	SpriteRender m_spriteRender;
	float m_inputWaitTimer = 0.0f;
	bool m_canInput = false;
	FontRender m_font;
	bool m_isBlinking = false;
	float m_blinkTimer = 0.0f;
};

