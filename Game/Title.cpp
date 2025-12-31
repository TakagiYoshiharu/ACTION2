#include "stdafx.h"
#include "Title.h"
#include"Game.h"
#include "sound/SoundEngine.h"
Title::Title() {
	m_spriteRender.Init("Assets/sprite/ti.dds", 1920.0f, 1080.0f);

	//タイトルのBGMを読み込む。
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/AS_1122633_マリオ風の楽しい8bitファミコンBGM.wav");
	//タイトルのBGMを再生する。
	titleBGM = NewGO<SoundSource>(0);
	titleBGM->Init(0);
	titleBGM->Play(true);
	// ★ここでGameインスタンスが存在するかチェックしてから登録！ if (Game::GetInstance() != nullptr) { Game::GetInstance()->SetBGM(titleBGM); }
}

Title::~Title() {
	DeleteGO(this);
}

void Title::Update() {
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// ★BGMを止めて削除
		if (titleBGM) {
			titleBGM->Stop();
			DeleteGO(titleBGM);
			titleBGM = nullptr;
		}

		NewGO<Game>(0);
		DeleteGO(this);
	}
}


void Title::Render(RenderContext& rc) {
	m_spriteRender.Draw(rc);
}