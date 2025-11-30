#pragma once

#include "Level3DRender/LevelRender.h"
class Player;
class GameCamera;
class BackGround;
class BackGround2;
class Enemy;
class Enemy2;
class BOSS;
class GameOver;
class GameClear;
class Star;
class Game : public IGameObject
{
public:
    Game() {}
    ~Game();
    bool Start();
    void Update();
    void Render(RenderContext& rc);
    static Game* GetInstance();
    void SetupStage1();
    void SetupStage2();
    void StartHitStop(float duration) { m_hitStopTimer = duration; }
    bool IsHitStop() const { return m_hitStopTimer > 0.0f; }
    void NextStage();
    void RestartStage();
    btDiscreteDynamicsWorld* GetPhysicsWorld() { return m_physicsWorld; }
    ModelRender m_modelRender;
    Vector3 m_pos;
    Player* m_player; //プレイヤー。
    GameCamera* m_gameCamera; //ゲームカメラ。
    BackGround* m_backGround;  //背景。
    BackGround2* m_backGround2;
    SkyCube* m_skyCube;
    GameOver* m_gameover;
    GameClear* m_gameclear;
    BOSS* m_boss;
    Star* m_star;
    Player* GetPlayer() const { return m_player; } // プレイヤー取得
    void ChangeScene(const std::string& sceneName);
    std::vector<Enemy*> m_enemies;
    std::vector<Enemy2*> m_enemy2s;
    std::vector<BOSS*> m_bosses;

    std::vector<Enemy*>& GetEnemies() { return m_enemies; }
    std::vector<Enemy2*>& GetEnemy2s() { return m_enemy2s; }
    std::vector<BOSS*>& GetBOSSes() { return m_bosses; }


    float m_hitStopTimer = 0.0f; // ヒットストップ残り時間
    btDiscreteDynamicsWorld* m_physicsWorld;
    int m_stageIndex = 1;
    float m_timeLimit = 120.0f;
    FontRender m_timeFont;
    bool m_isCleared = false; // クリア済みかどうか

};
