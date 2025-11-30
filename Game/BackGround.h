#pragma once
class BackGround :public IGameObject
{
public:
    BackGround();
    ~BackGround();

    //描画関数。
    void Render(RenderContext& rc);

    //モデルレンダ―。        
    ModelRender m_modelRender;
    PhysicsStaticObject m_physicsStaticObject;
};

