#include "stdafx.h"
#include "BackGround.h"
#include"Game.h"

BackGround::BackGround()
{
    m_modelRender.Init("Assets/modelData/stage/stargOOO.tkm");
    m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

    g_soundEngine->ResistWaveFileBank(10, "Assets/sound/AS_983281_迫力・豪華・暗い＿ジャズビッグバンド_.wav");
    //PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

    // ★既存のBGMを止める
    if (Game::GetInstance() && Game::GetInstance()->GetBGM()) {
        Game::GetInstance()->GetBGM()->Stop();
        DeleteGO(Game::GetInstance()->GetBGM());
        Game::GetInstance()->SetBGM(nullptr);
    }

    // ★新しいBGMを再生
    SoundSource* stageBGM = NewGO<SoundSource>(0);
    stageBGM->Init(10); // ステージBGMのID
    stageBGM->SetVolume(2.0f);
    stageBGM->Play(true); // ループ再生
    Game::GetInstance()->SetBGM(stageBGM);
}


BackGround::~BackGround()
{

}

void BackGround::Render(RenderContext& rc)
{
    //モデルを描画する。
    m_modelRender.Draw(rc);
}
