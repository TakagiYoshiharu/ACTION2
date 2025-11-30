#include "stdafx.h"
#include "BackGround2.h"


BackGround2::BackGround2()
{
    m_modelRender.Init("Assets/modelData/stage/stargBOSS.tkm");

    m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

    //“–‚½‚è”»’è‚ğ‰Â‹‰»‚·‚éB
   //PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

BackGround2::~BackGround2()
{

}

void BackGround2::Render(RenderContext& rc)
{
    //ƒ‚ƒfƒ‹‚ğ•`‰æ‚·‚éB
    m_modelRender.Draw(rc);
}