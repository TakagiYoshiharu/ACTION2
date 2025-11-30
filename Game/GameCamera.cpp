#include "stdafx.h"
#include "GameCamera.h"
#include"Player.h"
template<typename T>
T Clamp(const T& value, const T& minVal, const T& maxVal) {
    return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
}

GameCamera::GameCamera() {

}

bool GameCamera::Start() {
    m_toCameraPos.Set(0.0f, 200.0f, -300.0f);

    g_camera3D->SetNear(0.1f);

    g_camera3D->SetFar(100000.0f);

    m_player = FindGO<Player>("player");

    if (m_player) {
        // プレイヤーの初期回転を取得
        Quaternion playerRot = m_player->GetRotation();

        // カメラの相対位置をプレイヤーの回転で回す
        playerRot.Apply(m_toCameraPos);
    }

    return true;
}

void GameCamera::Update()
{
  
    if (!m_player) return;

    if (m_player->IsDead()) {
        if (!m_isCameraFrozen) {
            m_frozenPos = m_lastPos;
                m_frozenTarget = m_lastTarget;
            m_isCameraFrozen = true;
        }
        g_camera3D->SetPosition(m_frozenPos);
        g_camera3D->SetTarget(m_frozenTarget);
        g_camera3D->Update();
        return;
    }

    // プレイヤーが落下していたらカメラ追尾を停止
    if (m_player->GetPosition().y < -100.0f) {
        return; // カメラ更新しない
    }

    // プレイヤー位置を注視点に
    Vector3 target = m_player->m_position;
  
    // ジャンプ中かどうかで目標高さを決定
    float targetYOffset = m_player->IsAirborne() ? 150.0f : 100.0f;

    // ★ 高さを滑らかに補間（Lerp）
    m_currentTargetYOffset = m_currentTargetYOffset * 0.9f + targetYOffset * 0.1f;

    target.y += m_currentTargetYOffset;

    // 入力取得
    float x = g_pad[0]->GetRStickXF();
    float y = g_pad[0]->GetRStickYF();

    // Y軸回転
    Quaternion qRot;
    qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
    qRot.Apply(m_toCameraPos);

    // X軸回転
    Vector3 axisX;
    axisX.Cross(Vector3::AxisY, m_toCameraPos);
    if (axisX.Length() > 0.001f) {
        axisX.Normalize();
        qRot.SetRotationDeg(axisX, 1.3f * y);
        qRot.Apply(m_toCameraPos);
    }

    // カメラ位置
    Vector3 pos = target + m_toCameraPos;

    g_camera3D->SetTarget(target);
    g_camera3D->SetPosition(pos);
    g_camera3D->Update();
    m_lastPos = pos;
    m_lastTarget = target;
}