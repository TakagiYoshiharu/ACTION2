#include "stdafx.h"
#include "BackGround2.h"
#include"Game.h"


BackGround2::BackGround2()
{
    m_modelRender.Init("Assets/modelData/stage/stargBOSS.tkm");

    m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
    g_soundEngine->ResistWaveFileBank(14, "Assets/sound/AS_1665072_任天堂風ボス戦曲_ストリングスとオルガン.wav");
    //当たり判定を可視化する。
  // PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
       // ★既存のBGMを止める
    if (Game::GetInstance() && Game::GetInstance()->GetBGM() != nullptr) {
        Game::GetInstance()->GetBGM()->Stop();
        DeleteGO(Game::GetInstance()->GetBGM());
        Game::GetInstance()->SetBGM(nullptr);
    }

    // ★新しいBGMを再生
    SoundSource* stageBGM = NewGO<SoundSource>(0);
    stageBGM->Init(14); // ステージBGMのID
    stageBGM->SetVolume(2.0f);
    stageBGM->Play(true); // ループ再生
    Game::GetInstance()->SetBGM(stageBGM);
}

BackGround2::~BackGround2()
{

}

void BackGround2::Render(RenderContext& rc)
{
    //モデルを描画する。
    m_modelRender.Draw(rc);
}