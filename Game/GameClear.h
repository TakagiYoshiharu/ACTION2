#pragma once
class GameClear: public IGameObject
{
public:
	GameClear();
	~GameClear();
	void Update();
	void Render(RenderContext&rc);
	SpriteRender m_spriteRender;
	float m_inputWaitTimer = 0.0f;
	bool m_canInput = false;
	FontRender m_font;
};

