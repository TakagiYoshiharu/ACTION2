#include "stdafx.h"
#include "Player.h"
#include"Enemy.h"
#include"Enemy2.h"
#include"Game.h"
#include"GameOver.h"
#include"BossShell.h"
#include"CollisonGroup.h"
#include"sound/SoundEngine.h"
#include"sound/soundSource.h"


Player::Player()
{
    //アニメーションクリップをロードする。
    animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
    animationClips[enAnimationClip_Idle].SetLoopFlag(true);
    animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
    animationClips[enAnimationClip_Walk].SetLoopFlag(true);
    animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
    animationClips[enAnimationClip_Jump].SetLoopFlag(false);
    animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
    animationClips[enAnimationClip_Run].SetLoopFlag(true);
    //ユニティちゃんのモデルを読み込む。
    m_modelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);

    // 初期向きを設定（例：右向き）
    m_rotation.SetRotationDeg(Vector3::AxisY, 90.0f);
    m_modelRender.SetRotation(m_rotation);

    //キャラコンを初期化する。
    m_characterController.Init(25.0f, 75.0f, m_position);

    // HP表示（テキスト）
    const int maxHP = 3;
    for (int i = 0; i < maxHP; ++i) {
        m_spriteRender[i].Init("Assets/sprite/kara.DDS", 100.0f, 100.0f);
        if (i < m_hp) {
            m_spriteRender[i].Init("Assets/sprite/full.DDS", 100.0f, 100.0f);
        }

    }
    g_soundEngine->ResistWaveFileBank(4, "Assets/sound/AS_135672_ピヨヨヨン（マシーン、脱力）.wav");
}

Player::~Player()
{
}

//更新処理。
void Player::Update()
{
    if (m_isDead) {
        if (auto* game = Game::GetInstance()) {
            game->ChangeScene("gameOver");
        }
        DeleteGO(this);
        return;
    }

    Move();

    ManageState();

    PlayAnimation();

    Rotation();

    m_modelRender.Update();

    m_modelRender.SetPosition(m_position);

    CheckStepOnEnemy(); // ← ここで踏み判定をチェック

    if (m_isInvincible) {
        m_invincibleTimer -= g_gameTime->GetFrameDeltaTime();
        if (m_invincibleTimer <= 0.0f) {
            m_isInvincible = false;
        }
    }

    //落下判定
    if (m_position.y < -1000.0f) {
        m_isDead = true;
        m_prevPosition = m_position;
        if (auto* game = Game::GetInstance()) {
            game->ChangeScene("gameOver");
        }
        DeleteGO(this);
        return;
    }
    m_prevPosition = m_position;
    m_ignoreShellThisFrame = false;
}

void Player::Move()
{
    const float friction = 0.85f;
    m_moveSpeed.x *=friction ;
    m_moveSpeed.z *=friction ;

    if (fabsf(m_moveSpeed.x) < 1.0f)m_moveSpeed.x = 0.0f;
    if (fabsf(m_moveSpeed.z) < 1.0f)m_moveSpeed.z = 0.0f;

    //xzの移動速度を0.0fにする。

    //m_moveSpeed.x = 0.0f;
    //m_moveSpeed.z = 0.0f;

    //左スティックの入力量を取得。
    Vector3 stickl;
    stickl.x = g_pad[0]->GetLStickXF();
    stickl.y = g_pad[0]->GetLStickYF();

    Vector3 forward = g_camera3D->GetForward();
    Vector3 right = g_camera3D->GetRight();

    //y方向には移動させない。
    forward.y = 0.0f;
    right.y = 0.0f;

    forward.Normalize();
    right.Normalize();

    m_isDashing = g_pad[0]->IsPress(enButtonB);

    // 移動速度を決定
    float movePower = m_isDashing ? 80.0f :70.0f;
    Vector3 moveDir = right * stickl.x + forward * stickl.y;
    if (moveDir.LengthSq() > 0.01f && m_moveSpeed.LengthSq() > 1.0f) {
        Vector3 moveDirNorm = moveDir;
        moveDirNorm.Normalize();
        Vector3 velocityXZ = m_moveSpeed;
        velocityXZ.y = 0.0f;
        Vector3 velocityNorm = velocityXZ;
        velocityNorm.Normalize();
        float dot = moveDirNorm.Dot(velocityNorm);
        bool isBraking = (!m_isDashing && dot < 0.0f);
        float inputScale = isBraking ? 0.3f : 1.0f;
        if (isBraking) {

            // 入力方向の正規化
            Vector3 moveDirNorm2 = moveDir;
            moveDirNorm2.Normalize();

            // 現在の速度（XZ）
            Vector3 velocityXZ = m_moveSpeed;
            velocityXZ.y = 0.0f;

            // 速度を「前後成分」と「横成分」に分解
            float forwardSpeed = velocityXZ.Dot(moveDirNorm2);
            Vector3 forwardVec = moveDirNorm2 * forwardSpeed;
            Vector3 sideVec = velocityXZ - forwardVec;

            // ★逆方向（forwardSpeed < 0）だけブレーキ
            if (forwardSpeed < 0.0f) {
                forwardSpeed *= 0.6f;
            }

            // ★横滑りは残す
            Vector3 newVel = moveDirNorm2 * forwardSpeed + sideVec;

            m_moveSpeed.x = newVel.x;
            m_moveSpeed.z = newVel.z;
            m_moveSpeed.x += moveDirNorm2.x * 100.0f;
            m_moveSpeed.z += moveDirNorm2.z * 100.0f;
        }

        m_moveSpeed.x += moveDir.x * movePower * inputScale;
        m_moveSpeed.z += moveDir.z * movePower * inputScale;
    }
    else
    {
        m_moveSpeed.x += moveDir.x * movePower;
        m_moveSpeed.z += moveDir.z * movePower;
    }
    
    //地面に付いていたら。
    if (m_characterController.IsOnGround())
    {
        //重力を無くす。
        m_moveSpeed.y = 0.0f;

        //Aボタンが押されたら。
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            //ジャンプさせる。
            m_moveSpeed.y = 450.0f;
        }
    }

    //地面に付いていなかったら。
    else
    {
        //重力を発生させる。
        m_moveSpeed.y -= 17.8f;
    }

    //キャラクターコントローラーを使って座標を移動させる。
    m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

    //絵描きさんに座標を教える。
    m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
    //xかzの移動速度があったら(スティックの入力があったら)。
    if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
    {
        //キャラクターの方向を変える。
        m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);

        //絵描きさんに回転を教える。
        m_modelRender.SetRotation(m_rotation);
    }
}

void Player::ManageState()
{
    //地面に付いていなかったら。
    if (m_characterController.IsOnGround() == false)
    {
        //ステートを1(ジャンプ中)にする。
        m_playerState = 1;
        //ここでManageState関数の処理を終わらせる。
        return;
    }
    if (m_isDashing && (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)) {
        m_playerState = 3;
    }
    else if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
    {
        m_playerState = 2;
    }
    else
    {
        m_playerState = 0;
    }
}

Vector3 GetForwardFromQuaternion(const Quaternion& q) {
    Vector3 forward(0.0f, 0.0f, 1.0f);
    q.Apply(forward);
    return forward;
}


void Player::CheckStepOnEnemy()
{
    Vector3 playerPos = m_characterController.GetPosition();

    // Enemy（クリボー風）
    auto& enemies = Game::GetInstance()->GetEnemies();
    for (Enemy* enemy : enemies) {
        if (enemy == nullptr || enemy->IsDead()) continue;

        // ★ここに安全チェックを入れる
        auto* enemyCC = enemy->GetCharacterController();
        if (!enemyCC) continue; // キャラコンが未初期化ならスキップ

        Vector3 enemyPos = enemyCC->GetPosition();

        // 踏み判定
        if (IsStepHit(playerPos, enemyPos)) {
            enemy->OnStepped();
            ApplyStepJump();
            continue;
        }
    }

    // Enemy2（ノコノコ風）
    for (Enemy2* enemy2 : Game::GetInstance()->GetEnemy2s()) {
        if (!enemy2 || enemy2->IsDead()) continue;

        if (enemy2->GetState() == Enemy2::EnemyState::ShellMoving && enemy2->GetInvincibleTimer() > 0.0f) {
            continue;
        }
        Vector3 enemyPos = enemy2->GetPosition(); // Enemy2にGetPosition()を追加
        //踏む判定
        if (IsStepHit(playerPos, enemyPos)) {
            if (enemy2->GetState() == Enemy2::EnemyState::Walking) {
                enemy2->OnStepped(Vector3::Zero);
                ApplyStepJump();
            }
            else if (enemy2->GetState() == Enemy2::EnemyState::ShellStill)
            {
                Vector3 forwardDir = GetForwardFromQuaternion(m_rotation);
                forwardDir.y = 0.0f;
                forwardDir.Normalize();
                enemy2->OnStepped(forwardDir);
            }
            else if (enemy2->GetState() == Enemy2::EnemyState::ShellMoving)
            {
                enemy2->OnStepped(Vector3::Zero);
                ApplyStepJump();
            }
        }
        //ぶつかる
        else
        {
            float dx = playerPos.x - enemyPos.x;
            float dz = playerPos.z - enemyPos.z;
            float dy = fabs(playerPos.y - enemyPos.y);
            float horizontalDist = sqrtf(dx * dx + dz * dz);

            if (horizontalDist < 52.0f && dy < 10.0f) {
                auto state = enemy2->GetState();
                if (state == Enemy2::EnemyState::ShellStill) {
                    // 甲羅を蹴る
                    Enemy2::CollisionResult result;
                    result.hitPlayer = true;
                    Vector3 forwardDir = GetForwardFromQuaternion(m_rotation);
                    forwardDir.y = 0.0f;
                    forwardDir.Normalize();
                    result.playerForward = forwardDir;
                    enemy2->OnCollision(result);
                    m_ignoreShellThisFrame = true;
                    // ★ このフレームは絶対にダメージ判定をしない
                    continue;
                }
                else if (state == Enemy2::EnemyState::ShellMoving || state == Enemy2::EnemyState::Walking) {
                    if (!m_ignoreShellThisFrame) {
                        OnDamaged();
                    }
                }

            }

        }
    }
    for (BossShell* shell : Game::GetInstance()->GetBossShells()) {
        if (!shell)continue;
        Vector3 shellPos = shell->GetPosition();
        float dx = playerPos.x - shellPos.x;
        float dz = playerPos.z - shellPos.z;
        float dy = fabs(playerPos.y - shellPos.y);
        float horizontalDist = sqrtf(dx * dx + dz * dz);
        if (horizontalDist < 80.0f && dy < 30.0f) {
            if (m_prevPosition.y > shellPos.y && m_position.y <= shellPos.y + 60.0f && m_moveSpeed.y < 0.0f) {
                shell->Stop();
                ApplyStepJump();
            }
            else if (!m_ignoreShellThisFrame && !m_isInvincible) {
                OnDamaged();
            }
        }
        else if (shell->m_isStopped)
        {
            if (horizontalDist < 52.0f && dy < 10.0f) {
                    Vector3 forwardDir = GetForwardFromQuaternion(m_rotation);
                    forwardDir.y = 0.0f;
                    forwardDir.Normalize();
                    shell->Kick(forwardDir);
                    m_ignoreShellThisFrame = true;
                    return;
            }
        }
      
    }
}
bool Player::IsStepHit(const Vector3& playerPos, const Vector3& enemyPos) {
    float horizontalDist = sqrtf(pow(playerPos.x - enemyPos.x, 2) + pow(playerPos.z - enemyPos.z, 2));
    float verticalDiffPrev = m_prevPosition.y - enemyPos.y;
    float verticalDiffNow = playerPos.y - enemyPos.y;

    return horizontalDist < 70.0f
        && verticalDiffPrev > 0.0f    // 前フレームで敵より上ならOK
        && verticalDiffNow <= 60.0f   // 今フレームで敵と近い高さならOK
        && m_moveSpeed.y < 0.0f;      // 落下中
}

Vector3 Player::GetForwardDir()const {
    Vector3 forward = GetForwardFromQuaternion(m_rotation);
    forward.y = 0.0f;
    forward.Normalize();
    return forward;
}

void Player::ApplyStepJump() {
    // ★加算ではなく固定値で上書き
    if (g_pad[0]->IsPress(enButtonA)) {
        m_moveSpeed.y = 600.0f; // 高めジャンプ
    }
    else {
        m_moveSpeed.y = 400.0f; // 通常ジャンプ
    }
}


void Player::PlayAnimation()
{
    switch (m_playerState) {
    case 0:
        m_modelRender.PlayAnimation(enAnimationClip_Idle);
        break;
    case 1:
        m_modelRender.PlayAnimation(enAnimationClip_Jump);
        break;
    case 2:
        m_modelRender.PlayAnimation(enAnimationClip_Walk);
        break;
    case 3:
        m_modelRender.PlayAnimation(enAnimationClip_Run);
        break;
    }
}

void Player::OnDamaged() {
    if (m_isInvincible || m_hp <= 0 || m_isDead) return;

    m_hp--;
    printf("ダメージを受けた！ 残りHP: %d\n", m_hp);

    m_isInvincible = true;
    m_invincibleTimer = 2.0f;
    Game::GetInstance()->StartHitStop(0.2f);

    // 効果音再生
    SoundSource* se = NewGO<SoundSource>(0);
    se->Init(4);
    if (se->GetXAudio2SourceVoice() != nullptr) {
        se->SetVolume(3.0f);
        se->Play(false);
    }

    if (m_hp <= 0) {
        m_isDead = true;
        m_deathTimer = 1.0f;
        m_moveSpeed = Vector3::Zero;
        m_moveSpeed.y -= 100.0f;
    }
}

Vector3 Player::GetPosition() const {
    return m_position;
}

bool Player::IsAirborne() {
    return !m_characterController.IsOnGround();
}

//描画処理
void Player::Render(RenderContext& rc)
{
    if (m_isDead) {
        return;
}
    // 無敵中は点滅させる
    if (m_isInvincible) {
        // 0.2秒周期で点滅
        float blinkCycle = fmod(m_invincibleTimer, 0.2f);

        if (blinkCycle < 0.1f) {
            // この区間は完全に非表示 → Drawしない
        }
        else {
            // この区間は通常表示
            m_modelRender.Draw(rc);
        }
    }
    else {
        // 通常時は常に表示
        m_modelRender.Draw(rc);
    }
   
    for (int i = 0; i < maxHP; i++) {
        if (i < m_hp) {
            m_spriteRender[i].SetMulColor(Vector4(1, 1, 1, 1));
        }
        else
        {
            m_spriteRender[i].SetMulColor(Vector4(0.5f, 0.5f, 0.5f, 1));
        }
            m_spriteRender[i].SetPosition(Vector3{-800.0f + i * 80.0f, 450.0f, 0.0f}); // 画面左上
            m_spriteRender[i].Update();
           
        m_spriteRender[i].Draw(rc);
    }
   //  座標表示
   /* wchar_t posText[64];
    swprintf(posText, 64, L"Pos: X=%.1f Y=%.1f Z=%.1f", m_position.x, m_position.y, m_position.z);
    m_fontrender.SetText(posText);
    m_fontrender.SetPosition({ 300.0f, 400.0f, 0.0f });
    m_fontrender.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    m_fontrender.Draw(rc);*/
}
