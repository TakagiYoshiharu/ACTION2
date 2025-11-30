#pragma once
class GameClear: public IGameObject
{
public:
	GameClear();
	~GameClear();
	void Update();
	void Render(RenderContext&rc);
	SpriteRender m_spriteRender;
};

