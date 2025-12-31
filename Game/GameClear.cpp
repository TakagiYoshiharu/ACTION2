#include "stdafx.h"
#include "GameClear.h"
#include"Title.h"
#include"Game.h"

GameClear::GameClear() {
    m_spriteRender.Init("Assets/sprite/kuri.dds", 1920.0f, 1080.0f);
    g_soundEngine->ResistWaveFileBank(11, "Assets/sound/AS_1394560_マリオ風ステージクリア（ファミコン音源）.wav"); // IDは空いてる番号でOK！
    // ★既存のBGMを止める
    if (Game::GetInstance() && Game::GetInstance()->GetBGM()) {
        Game::GetInstance()->GetBGM()->Stop();
        DeleteGO(Game::GetInstance()->GetBGM());
        Game::GetInstance()->SetBGM(nullptr);
    }
    // ★クリアBGMを再生 
    SoundSource* clearBGM = NewGO<SoundSource>(0);
    clearBGM->Init(11); // クリアBGMのID clearBGM->SetVolume(2.0f);
    clearBGM->Play(false);
    Game::GetInstance()->SetBGM(nullptr);
    m_inputWaitTimer = 3.0f; // 2秒待つ（お好みで調整OK！）
    m_canInput = false;
}

GameClear::~GameClear() {

}

void GameClear::Update() {
    float deltaTime = g_gameTime->GetFrameDeltaTime();

    if (!m_canInput) {
        m_inputWaitTimer -= deltaTime;
        if (m_inputWaitTimer <= 0.0f) {
            m_canInput = true;
        }
        return; // 入力を受け付けない
    }

    if (g_pad[0]->IsTrigger(enButtonA)) {
        Game::GetInstance()->NextStage();
        DeleteGO(this);
    }
}

void GameClear::Render(RenderContext&rc) {
	m_spriteRender.Draw(rc);
    m_spriteRender.Draw(rc);
    if (m_canInput) {
        wchar_t buf[64]; swprintf(buf, 64, L"Aボタンでスタート");
        m_font.SetText(buf);
        m_font.SetPosition({ -150.0f, -250.0f, 0.0f });
        m_font.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
        m_font.Draw(rc);
    }
}