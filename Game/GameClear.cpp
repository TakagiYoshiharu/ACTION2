#include "stdafx.h"
#include "GameClear.h"
#include"Title.h"
#include"Game.h"

GameClear::GameClear() {
	m_spriteRender.Init("Assets/sprite/Clear.dds", 1920.0f, 1080.0f);
}

GameClear::~GameClear() {

}

void GameClear::Update() {
        if (g_pad[0]->IsTrigger(enButtonA)) {
            Game::GetInstance()->NextStage(); // ★Gameに次ステージ開始を依頼
            DeleteGO(this); // ★GameClear画面を閉じる
        }
}

void GameClear::Render(RenderContext&rc) {
	m_spriteRender.Draw(rc);
}