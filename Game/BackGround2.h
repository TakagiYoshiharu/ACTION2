#pragma once
class BackGround2:public IGameObject
{ public:
     BackGround2();
    ~BackGround2();

    //描画関数。
    void Render(RenderContext& rc);

    //モデルレンダ―。        
    ModelRender m_modelRender;
    PhysicsStaticObject m_physicsStaticObject;
};

