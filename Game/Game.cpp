#include "stdafx.h"
#include "Game.h"
#include"Player.h"
#include"GameCamera.h"
#include"BackGround.h"
#include"BackGround2.h"
#include"Enemy.h"
#include"Enemy2.h"
#include"BOSS.h"
#include"Star.h"
#include"GameOver.h"
#include"GameClear.h"
#include"Title.h"
static Game* s_gameInstance;

Game::~Game() {
	
}

bool Game::Start()
{
	////ポイントライトを作ってみる
	//auto* PointLight = g_sceneLight->NewPointLight();
	//PointLight->SetPosition(Vector3(5.0f, 100.0f, 0.0f));
	//PointLight->SetColor(10000000000.0f, 0.1f, 0.1f);
	//PointLight->SetRange(100.0f);
	//PointLight->SetAffectPowParam(3.0f);
	//PointLight->Update();

	s_gameInstance = this;

	m_player = NewGO<Player>(0, "player");
	m_gameCamera = NewGO<GameCamera>(0);
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_timeLimit = 120.0f; // ステージ開始時に制限時間をリセット
	SetupStage1();
		
	return true;
}

void Game::SetupStage1() {
	m_backGround = NewGO<BackGround>(0);
	m_timeLimit = 120.0f; // ステージ1用の制限時間
	// …他の敵もここでまとめる
	Enemy2* noko1 = NewGO<Enemy2>(0, "enemy2");
	noko1->SetStartPosition({ 6265.0f,1062.0f,-2177.0f });
	noko1->SetStartDirection(Vector3::Front); // ←左へ
	m_enemy2s.push_back(noko1);

	Enemy2* noko2 = NewGO<Enemy2>(0, "enemy2");
	noko2->SetStartPosition({ 2753.0f,262.0f,-260.0f });
	noko2->SetStartDirection(Vector3::Right); 
	m_enemy2s.push_back(noko2);

	Enemy2* noko3 = NewGO<Enemy2>(0, "enemy2");
	noko3->SetStartPosition({6325.0f,1035.0f,-2934.0f });
	noko3->SetStartDirection(Vector3::Back); // ←左へ
	m_enemy2s.push_back(noko3);

	Enemy*enemy1 = NewGO<Enemy>(0, "enemy");
	enemy1->SetStartPosition({400.0f,2.0f,110.0f});
	m_enemies.push_back(enemy1);

	Enemy* enemy2 = NewGO<Enemy>(0, "enemy");
	enemy2->SetStartPosition ({ 800.0f,2.0f,-10.0f });
	m_enemies.push_back(enemy2);

	Enemy* enemy4 = NewGO<Enemy>(0, "enemy");
	enemy4->SetStartPosition( { 400.0f,2.0f,400.0f });
	m_enemies.push_back(enemy4);

	Enemy* enemy5 = NewGO<Enemy>(0, "enemy");
	enemy5->SetStartPosition({ 600.0f,2.0f,0.0f });
	m_enemies.push_back(enemy5);

	Enemy* enemy6 = NewGO<Enemy>(0, "enemy");
	enemy6->SetStartPosition({ 2070.0f,182.0f,160.0f });
	m_enemies.push_back(enemy6);

	Enemy* enemy7 = NewGO<Enemy>(0, "enemy");
	enemy7->SetStartPosition({ 2300.0f,182.0f,-230.0f });
	m_enemies.push_back(enemy7);

	Enemy* enemy8 = NewGO<Enemy>(0, "enemy");
	enemy8->SetStartPosition({ 3035.0f,337.0f,99.0f });
	m_enemies.push_back(enemy8);

	Enemy* enemy9 = NewGO<Enemy>(0, "enemy");
	enemy9->SetStartPosition({ 5375.0f,749.0f,-77.0f });
	m_enemies.push_back(enemy9);

	Enemy* enemy10 = NewGO<Enemy>(0, "enemy");
	enemy10->SetStartPosition({ 3346.0f,419.0f,-64.0f });
	m_enemies.push_back(enemy10);

	Enemy*enemy11 = NewGO<Enemy>(0, "enemy");
	enemy11->SetStartPosition({3778.0f,533.0f,-384.0f});
	m_enemies.push_back(enemy11);

	Enemy* enemy12 = NewGO<Enemy>(0, "enemy");
	enemy12->SetStartPosition ({ 4219.0f,596.0f,-361.0f });
	m_enemies.push_back(enemy12);

	Enemy* enemy13 = NewGO<Enemy>(0, "enemy");
	enemy13->SetStartPosition({ 4205.0f,596.0f,163.0f });
	m_enemies.push_back(enemy13);

	Enemy* enemy14 = NewGO<Enemy>(0, "enemy");
	enemy14->SetStartPosition( { 4646.0f,596.0f,-36.0f });
	m_enemies.push_back(enemy14);

	Enemy* enemy15 = NewGO<Enemy>(0, "enemy");
	enemy15->SetStartPosition({ 5375.0f,749.0f,-77.0f });
	m_enemies.push_back(enemy15);

	Enemy* enemy16 = NewGO<Enemy>(0, "enemy");
	enemy16->SetStartPosition({ 6053.0f,939.0f,-56.0f });
	m_enemies.push_back(enemy16);

	Enemy* enemy17 = NewGO<Enemy>(0, "enemy");
	enemy17->SetStartPosition({ 6329.0f,1062.0f,-837.0f });
	m_enemies.push_back(enemy17);

	Enemy* enemy18 = NewGO<Enemy>(0, "enemy");
	enemy18->SetStartPosition({ 6405.0f,1062.0f,-1399.0f });
	m_enemies.push_back(enemy18);

	Enemy* enemy19 = NewGO<Enemy>(0, "enemy");
	enemy19->SetStartPosition({ 6254.0f,1035.0f,-3385.0f });
	m_enemies.push_back(enemy19);

	Enemy* enemy20 = NewGO<Enemy>(0, "enemy");
	enemy20->SetStartPosition({ 6361.0f,1035.0f,-3746.0f });
	m_enemies.push_back(enemy20);

	Enemy* enemy21 = NewGO<Enemy>(0, "enemy");
	enemy21->SetStartPosition({ 6303.0f,1035.0f,-4062.0f });
	m_enemies.push_back(enemy21);

	Enemy* enemy22 = NewGO<Enemy>(0, "enemy");
	enemy22->SetStartPosition({ 6344.0f,1035.0f,-4530.0f });
	m_enemies.push_back(enemy22);

	Enemy* enemy23 = NewGO<Enemy>(0, "enemy");
	enemy23->SetStartPosition({ 6461.0f,1035.0f,-5140.0f });
	m_enemies.push_back(enemy23);

	Enemy* enemy24 = NewGO<Enemy>(0, "enemy");
	enemy24->SetStartPosition({ 6298.0f,1035.0f,-5671.0f });
	m_enemies.push_back(enemy24);

	Enemy* enemy25 = NewGO<Enemy>(0, "enemy");
	enemy25->SetStartPosition({ 6331.0f,1223.0f,-6660.0f });
	m_enemies.push_back(enemy25);

	Enemy* enemy26 = NewGO<Enemy>(0, "enemy");
	enemy26->SetStartPosition({ 6530.0f,1079.0f,-7865.0f });
	m_enemies.push_back(enemy26);

	Enemy* enemy27 = NewGO<Enemy>(0, "enemy");
	enemy27->SetStartPosition({ 6256.0f,785.0f,-9068.0f });
	m_enemies.push_back(enemy27);

	Star*star1 = NewGO<Star>(0, "star");
	star1->position = { 6053.0f,1366.0f,-12836.0f };
}

void Game::SetupStage2() {
	m_timeLimit = 120.0f; // ステージ2用の制限時間（例）
	m_backGround2 = NewGO<BackGround2>(0);
	// 敵セットB
	Enemy* enemyA = NewGO<Enemy>(0, "enemy");
	enemyA->SetStartPosition({ 1000.0f,200.0f,0.0f });
	m_enemies.push_back(enemyA);

	Enemy* enemyAb = NewGO<Enemy>(0, "enemy");
	enemyAb->SetStartPosition({ 1266.0f,-158.0f,168.0f });
	m_enemies.push_back(enemyAb);

	Enemy* enemyAc = NewGO<Enemy>(0, "enemy");
	enemyAc->SetStartPosition({ 1494.0f,-158.0f,-118.0f });
	m_enemies.push_back(enemyAc);

	Enemy* enemyAd = NewGO<Enemy>(0, "enemy");
	enemyAd->SetStartPosition({ 1470.0f,-158.0f,-108.0f });
	m_enemies.push_back(enemyAd);

	Enemy* enemyAe = NewGO<Enemy>(0, "enemy");
	enemyAe->SetStartPosition({ 2101.0f,-158.0f,-637.0f });
	m_enemies.push_back(enemyAe);

	Enemy* enemyAf = NewGO<Enemy>(0, "enemy");
	enemyAf->SetStartPosition({ 2259.0f,-158.0f,-821.0f });
	m_enemies.push_back(enemyAf);

	Enemy* enemyAg = NewGO<Enemy>(0, "enemy");
	enemyAg->SetStartPosition({ 2493.0f,-156.0f,704.0f });
	m_enemies.push_back(enemyAg);

	Enemy* enemyAh = NewGO<Enemy>(0, "enemy");
	enemyAh->SetStartPosition({ 3059.0f,-158.0f,-111.0f });
	m_enemies.push_back(enemyAh);

	Enemy* enemyAi = NewGO<Enemy>(0, "enemy");
	enemyAi->SetStartPosition({ 3411.0f,-158.0f,105.0f });
	m_enemies.push_back(enemyAi);

	Enemy* enemyAj = NewGO<Enemy>(0, "enemy");
	enemyAj->SetStartPosition({ 3866.0f,-158.0f,-28.0f });
	m_enemies.push_back(enemyAj);

	Enemy* enemyAk = NewGO<Enemy>(0, "enemy");
	enemyAk->SetStartPosition({ 4762.0f,-137.0f,-158.0f });
	m_enemies.push_back(enemyAk);

	Enemy* enemyAl = NewGO<Enemy>(0, "enemy");
	enemyAl->SetStartPosition({ 4746.0f,-132.0f,195.0f });
	m_enemies.push_back(enemyAl);

	Enemy* enemyAm = NewGO<Enemy>(0, "enemy");
	enemyAm->SetStartPosition({ 5180.0f,-23.0f,123.0f });
	m_enemies.push_back(enemyAm);

	Enemy* enemyAn = NewGO<Enemy>(0, "enemy");
	enemyAn->SetStartPosition({ 5307.0f,-23.0f,-267.0f });
	m_enemies.push_back(enemyAn);

	Enemy* enemyAo = NewGO<Enemy>(0, "enemy");
	enemyAo->SetStartPosition({ 5177.0f,-23.0f,-95.0f });
	m_enemies.push_back(enemyAo);

	Enemy* enemyAp = NewGO<Enemy>(0, "enemy");
	enemyAp->SetStartPosition({ 5746.0f,-22.0f,-291.0f });
	m_enemies.push_back(enemyAp);

	Enemy* enemyAq = NewGO<Enemy>(0, "enemy");
	enemyAq->SetStartPosition({ 6119.0f,-20.0f,-291.0f });
	m_enemies.push_back(enemyAq);

	Enemy* enemyAr = NewGO<Enemy>(0, "enemy");
	enemyAr->SetStartPosition({ 6705.0f,-6.0f,-291.0f });
	m_enemies.push_back(enemyAr);

	Enemy* enemyAs = NewGO<Enemy>(0, "enemy");
	enemyAs->SetStartPosition({ 6847.0f,-6.0f,105.0f });
	m_enemies.push_back(enemyAs);

	Enemy* enemyAt = NewGO<Enemy>(0, "enemy");
	enemyAt->SetStartPosition({ 7322.0f,27.0f,151.0f });
	m_enemies.push_back(enemyAt);

	Enemy* enemyAu = NewGO<Enemy>(0, "enemy");
	enemyAu->SetStartPosition({ 7941.0f,27.0f,-262.0f });
	m_enemies.push_back(enemyAu);

	Enemy* enemyAw = NewGO<Enemy>(0, "enemy");
	enemyAw->SetStartPosition({ 8215.0f,27.0f,-38.0f });
	m_enemies.push_back(enemyAw);

	Enemy* enemyAv = NewGO<Enemy>(0, "enemy");
	enemyAv->SetStartPosition({ 10624.0f,140.0f,-112.0f });
	m_enemies.push_back(enemyAv);

	Enemy* enemyAy = NewGO<Enemy>(0, "enemy");
	enemyAy->SetStartPosition({ 10935.0f,140.0f,42.0f });
	m_enemies.push_back(enemyAy);

	Enemy* enemyAx = NewGO<Enemy>(0, "enemy");
	enemyAx->SetStartPosition({ 11273.0f,138.0f,-94.0f });
	m_enemies.push_back(enemyAx);

	Enemy* enemyAz = NewGO<Enemy>(0, "enemy");
	enemyAz->SetStartPosition({ 11636.0f,140.0f,-5.0f });
	m_enemies.push_back(enemyAz);

	Enemy* enemyAA = NewGO<Enemy>(0, "enemy");
	enemyAA->SetStartPosition({ 11636.0f,140.0f,-5.0f });
	m_enemies.push_back(enemyAA);

	Enemy* enemyAB = NewGO<Enemy>(0, "enemy");
	enemyAB->SetStartPosition({ 12222.0f,140.0f,-98.0f });
	m_enemies.push_back(enemyAB);

	Enemy* enemyAC = NewGO<Enemy>(0, "enemy");
	enemyAC->SetStartPosition({ 12558.0f,140.0f,46.0f });
	m_enemies.push_back(enemyAC);

	Enemy* enemyAD = NewGO<Enemy>(0, "enemy");
	enemyAD->SetStartPosition({ 12684.0f,140.0f,-162.0f });
	m_enemies.push_back(enemyAD);

	Enemy* enemyAE = NewGO<Enemy>(0, "enemy");
	enemyAE->SetStartPosition({ 13363.0f,140.0f,52.0f });
	m_enemies.push_back(enemyAE);

	Enemy* enemyAF = NewGO<Enemy>(0, "enemy");
	enemyAF->SetStartPosition({ 13702.0f,140.0f,-84.0f });
	m_enemies.push_back(enemyAF);

	Enemy* enemyAG = NewGO<Enemy>(0, "enemy");
	enemyAG->SetStartPosition({ 13940.0f,140.0f,63.0f });
	m_enemies.push_back(enemyAG);

	Enemy* enemyAH = NewGO<Enemy>(0, "enemy");
	enemyAH->SetStartPosition({ 14210.0f,140.0f,-25.0f });
	m_enemies.push_back(enemyAH);

	Enemy* enemyAI = NewGO<Enemy>(0, "enemy");
	enemyAI->SetStartPosition({ 14266.0f,140.0f,87.0f });
	m_enemies.push_back(enemyAI);

	Enemy* enemyAJ = NewGO<Enemy>(0, "enemy");
	enemyAJ->SetStartPosition({ 14964.0f,140.0f,-111.0f });
	m_enemies.push_back(enemyAJ);

	Enemy* enemyAK = NewGO<Enemy>(0, "enemy");
	enemyAK->SetStartPosition({ 15307.0f,140.0f,-24.0f });
	m_enemies.push_back(enemyAK);

	Enemy* enemyAL = NewGO<Enemy>(0, "enemy");
	enemyAL->SetStartPosition({ 15636.0f,140.0f,33.0f });
	m_enemies.push_back(enemyAL);

	Enemy* enemyAM = NewGO<Enemy>(0, "enemy");
	enemyAM->SetStartPosition({ 15884.0f,140.0f,-43.0f });
	m_enemies.push_back(enemyAM);

	Enemy* enemyAN = NewGO<Enemy>(0, "enemy");
	enemyAN->SetStartPosition({ 16738.0f,140.0f,45.0f });
	m_enemies.push_back(enemyAN);

	Enemy* enemyAO = NewGO<Enemy>(0, "enemy");
	enemyAO->SetStartPosition({ 16815.0f,140.0f,-136.0f });
	m_enemies.push_back(enemyAO);

	Enemy* enemyAP = NewGO<Enemy>(0, "enemy");
	enemyAP->SetStartPosition({ 17070.0f,140.0f,-27.0f });
	m_enemies.push_back(enemyAP);

	Enemy* enemyAQ = NewGO<Enemy>(0, "enemy");
	enemyAQ->SetStartPosition({ 17301.0f,140.0f,90.0f });
	m_enemies.push_back(enemyAQ);

	Enemy* enemyAR = NewGO<Enemy>(0, "enemy");
	enemyAR->SetStartPosition({ 17550.0f,140.0f,-90.0f });
	m_enemies.push_back(enemyAR);

	Enemy* enemyAS = NewGO<Enemy>(0, "enemy");
	enemyAS->SetStartPosition({ 18215.0f,140.0f,7.0f });
	m_enemies.push_back(enemyAS);

	Enemy* enemyAT = NewGO<Enemy>(0, "enemy");
	enemyAT->SetStartPosition({ 18194.0f,140.0f,-132.0f });
	m_enemies.push_back(enemyAT);

	Enemy* enemyAU = NewGO<Enemy>(0, "enemy");
	enemyAU->SetStartPosition({ 18447.0f,140.0f,-37.0f });
	m_enemies.push_back(enemyAU);

	Enemy* enemyAW = NewGO<Enemy>(0, "enemy");
	enemyAW->SetStartPosition({ 18585.0f,140.0f,82.0f });
	m_enemies.push_back(enemyAW);

	Enemy* enemyAV = NewGO<Enemy>(0, "enemy");
	enemyAV->SetStartPosition({ 18721.0f,140.0f,-154.0f });
	m_enemies.push_back(enemyAV);

	Enemy* enemyAX = NewGO<Enemy>(0, "enemy");
	enemyAX->SetStartPosition({ 18908.0f,140.0f,-24.0f });
	m_enemies.push_back(enemyAX);

	Enemy* enemyAY = NewGO<Enemy>(0, "enemy");
	enemyAY->SetStartPosition({ 19125.0f,140.0f,90.0f });
	m_enemies.push_back(enemyAY);

	Enemy* enemyAZ = NewGO<Enemy>(0, "enemy");
	enemyAZ->SetStartPosition({ 19165.0f,140.0f,-180.0f });
	m_enemies.push_back(enemyAZ);
	// …ステージ2用の敵をここでまとめる
	BOSS* boss1 = NewGO<BOSS>(0, "boss");
	boss1->SetStartPosition({ 11343.0f,139.0f,-35.0f });
	m_bosses.push_back(boss1);

	//Starクラスのオブジェクトを作る。
	Star* star2 = NewGO<Star>(0, "star");
	star2->position = { 23533.0f,600.0f,51.0f };
}


void Game::Update() {
	// ★制限時間を減らす
	if (!m_isCleared) {
		m_timeLimit -= g_gameTime->GetFrameDeltaTime();
		if (m_timeLimit <= 0.0f) {
			ChangeScene("GameOver"); // 時間切れでゲームオーバー
			return;
		}
	}
	auto& enemies = GetEnemies();
	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(),
			[](Enemy* e) { return e == nullptr || e->IsDead(); }),
		enemies.end()
	);

	// ★スターを監視 → クリア画面へ
	if (m_player != nullptr && !m_player->IsDead() && m_player->starCount >= 1) {
		m_isCleared = true;          // ← クリアフラグを立てる
		ChangeScene("GameClear");
		return;
	}

}



Game* Game::GetInstance() {
	return s_gameInstance;
}

void Game::ChangeScene(const std::string& sceneName) {
	// プレイヤー削除は一度だけ
	if (m_player) {
		DeleteGO(m_player);
		m_player = nullptr;
	}

	if (m_gameCamera) { DeleteGO(m_gameCamera); m_gameCamera = nullptr; }
	if (m_backGround) { DeleteGO(m_backGround); m_backGround = nullptr; }
	if (m_skyCube) { DeleteGO(m_skyCube);    m_skyCube = nullptr; }
	if (m_star) { DeleteGO(m_star);       m_star = nullptr; }
	if (m_backGround2) { DeleteGO(m_backGround2); m_backGround2 = nullptr; }

	for (auto enemy : m_enemies) {
		if (enemy) DeleteGO(enemy);
	}
	m_enemies.clear();

	for (auto enemy2 : m_enemy2s) {
		if (enemy2) DeleteGO(enemy2);
	}
	m_enemy2s.clear();

	for (auto boss : m_bosses) {
		if (boss) DeleteGO(boss);
	}
	m_bosses.clear();

	// ★ここでは Game 自体を削除しない
	// DeleteGO(this); ←削除！

	if (sceneName == "GameOver") {
		m_gameover = NewGO<GameOver>(0);
	}
	else if (sceneName == "GameClear") {
		NewGO<GameClear>(0, "gameClear"); // ここでクリア画面を出す
	}
}

void Game::RestartStage() {
	m_isCleared = false; // 再スタート時にリセット
	if (m_stageIndex == 1) {
		m_player = NewGO<Player>(0, "player");
		m_gameCamera = NewGO<GameCamera>(0);
		m_skyCube = NewGO<SkyCube>(0, "skycube");
		SetupStage1();
	}
	else if (m_stageIndex == 2) {
		m_player = NewGO<Player>(0, "player");
		m_gameCamera = NewGO<GameCamera>(0);
		m_skyCube = NewGO<SkyCube>(0, "skycube");
		SetupStage2();
	}
}


void Game::NextStage() {
	m_isCleared = false; // 再スタート時にリセット
	m_stageIndex++;
	if (m_stageIndex == 2) {
		// ステージ2開始
		m_player = NewGO<Player>(0, "player");
		m_gameCamera = NewGO<GameCamera>(0);
		m_skyCube = NewGO<SkyCube>(0, "skycube");
		SetupStage2();
	}
	else if (m_stageIndex == 3) {
		// ステージ2クリア後 → タイトルへ
		NewGO<Title>(0, "title");
	}
}


void Game::Render(RenderContext& rc)
{
	// フォント更新
	wchar_t buf[64];
	swprintf(buf, 64, L"TIME: %.0f", m_timeLimit);
	m_timeFont.SetText(buf);
	m_timeFont.SetPosition({ -800.0f, 400.0f, 0.0f }); // 画面右上
	m_timeFont.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	m_timeFont.Draw(rc); // 制限時間を描画
}