#include "stdafx.h"
#include "Star.h"
#include"Player.h"
#include"Game.h"

Star::Star()
{

    //☆のモデルを読み込む。
    modelRender.Init("Assets/modelData/star.tkm");
    position = { 0.0f,0.0f,0.0f };
}



Star::~Star()

{



}



void Star::Update()

{

    //移動処理。

    Move();



    //絵描きさんの更新処理。

    modelRender.Update();
        Player* player = Game::GetInstance()->GetPlayer();
        if (!player || player->IsDead()) return;

        Vector3 diff = player->GetPosition() - position;
        if (diff.Length() < 120.0f) {
            player->starCount += 1;
            DeleteGO(this);
            Game::GetInstance()->ChangeScene("GameClear");
        }
}



void Star::Move()

{

    //絵描きさんに座標を教える。

    modelRender.SetPosition(position);

}



void Star::Render(RenderContext& rc)

{

    //☆を描画する。

    modelRender.Draw(rc);

}
