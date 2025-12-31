#include "stdafx.h"
#include "GameOver.h"
#include"Game.h"
#include"Title.h"
GameOver::GameOver(){
	m_spriteRender.Init("Assets/sprite/gemu.dds", 1920.0f, 1080.0f);
}

GameOver::~GameOver(){

}

void GameOver::Update(){
    float deltaTime = g_gameTime->GetFrameDeltaTime();

    if (!m_canInput) {
        m_inputWaitTimer -= deltaTime;
        if (m_inputWaitTimer <= 0.0f) {
            m_canInput = true;
        }
        return; // 入力を受け付けない
    }

    if (g_pad[0]->IsTrigger(enButtonA)) {
        Game::GetInstance()->RestartStage();
        DeleteGO(this);
    }
}
bool GameOver::Start(){
    // BGMを止める
    if (Game::GetInstance() && Game::GetInstance()->GetBGM()) {
        Game::GetInstance()->GetBGM()->Stop();
        DeleteGO(Game::GetInstance()->GetBGM());
        Game::GetInstance()->SetBGM(nullptr);
    }

    // ゲームオーバーBGMを再生
    g_soundEngine->ResistWaveFileBank(6, "Assets/sound/AS_1615044_ゲームオーバー・コミカル風.wav"); // IDは空いてる番号で！

    SoundSource* gameOverBGM = NewGO<SoundSource>(0);
    gameOverBGM->Init(6);
    gameOverBGM->SetVolume(2.0f);
    gameOverBGM->Play(false);

    Game::GetInstance()->SetBGM(gameOverBGM); // 登録しておくと後で止められる！

    m_inputWaitTimer = 4.0f; // 1秒待つ
    m_canInput = false;


    return true;
}
void GameOver::Render(RenderContext& rc){
	m_spriteRender.Draw(rc);
    if (m_canInput) {
        wchar_t buf[64]; swprintf(buf, 64, L"Aボタンでスタート");
        m_font.SetText(buf);
        m_font.SetPosition({ -150.0f, -250.0f, 0.0f });
        m_font.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
        m_font.Draw(rc);
    }
}