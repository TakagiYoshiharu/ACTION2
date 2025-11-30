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
};

