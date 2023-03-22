#include "Stage.h"
#include <cassert>
#include <fstream>
#include "SafeDelete.h"

//using namespace std;

Stage::~Stage() {
	SafeDelete(model_);
	SafeDelete(modelFloor_);
	SafeDelete(modelGoal_);
	SafeDelete(modelSwitchR_);
	SafeDelete(modelSwitchB_);
	SafeDelete(modelWallR_);
	SafeDelete(modelWallB_);

	SafeDelete(obj_);
	SafeDelete(objFloor_);
	SafeDelete(objGoal_);
	SafeDelete(objSwitchR_);
	SafeDelete(objSwitchB_);
	SafeDelete(objWallR_);
	SafeDelete(objWallB_);

	SafeDelete(switchR_);
	SafeDelete(switchB_);


}

void Stage::Initialize(Camera* camera) {

	this->cameraStage_ = camera;
	stageBlocks_.begin()->get()->worldTransform_.Initialize();
	floorBlocks_.begin()->get()->worldTransform_.Initialize();
	//インスタンス生成
	model_ = new Model();
	modelFloor_ = new Model();
	modelSwitchR_ = new Model();
	modelSwitchB_ = new Model();
	modelWallR_ = new Model();
	modelWallB_ = new Model();
	modelGoal_ = new Model();

	obj_ = new Object3d();
	objFloor_ = new Object3d();
	objSwitchR_ = new Object3d();
	objSwitchB_ = new Object3d();
	objWallR_ = new Object3d();
	objWallB_ = new Object3d();
	objGoal_ = new Object3d();

	// モデル読み込み
	model_ = Model::LoadFromOBJ("cubeB", true);
	modelFloor_ = Model::LoadFromOBJ("floor", true);
	modelSwitchR_ = Model::LoadFromOBJ("rswitch", true);
	modelSwitchB_ = Model::LoadFromOBJ("bswitch", true);
	modelWallR_ = Model::LoadFromOBJ("cubeR", true);
	modelWallB_ = Model::LoadFromOBJ("cubeB", true);
	modelGoal_ = Model::LoadFromOBJ("goal", true);

	//3Dオブジェクトとカメラのセット
	obj_ = Object3d::Create();
	obj_->SetModel(model_);
	obj_->SetCamera(cameraStage_);

	objFloor_ = Object3d::Create();
	objFloor_->SetModel(modelFloor_);
	objFloor_->SetCamera(cameraStage_);

	objSwitchR_ = Object3d::Create();
	objSwitchR_->SetModel(modelSwitchR_);
	objSwitchR_->SetCamera(cameraStage_);

	objSwitchB_ = Object3d::Create();
	objSwitchB_->SetModel(modelSwitchB_);
	objSwitchB_->SetCamera(cameraStage_);

	objWallR_ = Object3d::Create();
	objWallR_->SetModel(modelWallR_);
	objWallR_->SetCamera(cameraStage_);

	objWallB_ = Object3d::Create();
	objWallB_->SetModel(modelWallB_);
	objWallB_->SetCamera(cameraStage_);

	objGoal_ = Object3d::Create();
	objGoal_->SetModel(modelGoal_);
	objGoal_->SetCamera(cameraStage_);

	// スイッチ
	switchR_ = new Switch();
	switchB_ = new Switch();
	switchR_->Initialize(modelSwitchR_, objSwitchR_);
	switchB_->Initialize(modelSwitchB_, objSwitchB_);


	// ステージの床を初期化
	LoadFloorBlock();
}

void Stage::StageInitialize(const std::string& stageNum) {
	// 最初に残っている要素を削除
	stageBlocks_.clear();

	isSwitchDrawR_ = false;
	isSwitchDrawB_ = false;

	// バッファをクリア
	stageCommands.str("");
	// 状態をクリア
	stageCommands.clear(std::stringstream::goodbit);

	// ステージ用ファイル読み込み
	LoadStageData(stageNum);
	// ステージファイルで各要素を設定
	LoadStageCommands();

	isGoal_ = false;
}

void Stage::Update() {
	if (switchR_->GetFlag()) {
		for (std::unique_ptr<StageData>& block : stageBlocks_) {
			if (block->type_ == WALLR) {
				block->type_ = NONE2;
			}
		}
	}
	else if (!switchR_->GetFlag()) {
		for (std::unique_ptr<StageData>& block : stageBlocks_) {
			if (block->type_ == NONE2) {
				block->type_ = WALLR;
			}
		}
	}

	if (switchB_->GetFlag()) {
		for (std::unique_ptr<StageData>& block : stageBlocks_) {
			if (block->type_ == WALLB) {
				block->type_ = NONE3;
			}
		}
	}
	else if (!switchB_->GetFlag()) {
		for (std::unique_ptr<StageData>& block : stageBlocks_) {
			if (block->type_ == NONE3) {
				block->type_ = WALLB;
			}
		}
	}

	switchR_->Update();
	switchB_->Update();

	isGoal_ = false;

	cameraStage_->Update();
}

void Stage::Draw() {
	// ステージ描画
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		if (block->type_ == BLOCK) {
			// 壁描画
			obj_->Draw();
		}
		else if (block->type_ == WALLR) {
			// 赤壁描画
			objWallR_->Draw();
		}
		else if (block->type_ == WALLB) {
			// 青壁描画
			objWallB_->Draw();
		}
		else 
			if (block->type_ == GOAL) {
			// ゴール描画
			block->worldTransform_.position_.y = -15.5f;
			objGoal_->Draw();
		}
	}

	// 床描画
	for (std::unique_ptr<StageData>& block : floorBlocks_) {
		objFloor_->Draw();
	}

	// スイッチ描画
	if (isSwitchDrawR_) switchR_->Draw();
	if (isSwitchDrawB_) switchB_->Draw();
}

void Stage::LoadStageData(const std::string& stageNum) {
	// ファイル
	std::ifstream file;
	// パスを取得
	const std::string name = "stage";
	const std::string filename = name + stageNum + ".csv";
	const std::string directoryPath = "Resource/" + name + "/";
	// ファイルを開く
	file.open(directoryPath + filename);
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stageCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void Stage::LoadStageCommands() {
	// 1行分の文字列を入れる変数
	std::string line;
	// マップチップ用変数
	int mapLine = 0;	// X
	int mapRow = 0;	// Y

	// コマンド実行ループ
	while (getline(stageCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		// 文字列
		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を読み飛ばす
			continue;
		}

		while (mapLine != STAGE_WIDTH) {
			// コマンド読み込み
			if (word.find("NONE") == 0 || word.find("0") == 0 || word.find("7") == 0) {
				// ステージのブロックを追加
				PushStageBlockList(stageBlocks_, obj_, NONE, mapLine, mapRow, -10.0f);
				// インクリメント
				mapLine++;
			}
			else if (word.find("BLOCK") == 0 || word.find("1") == 0) {
				// ステージのブロックを追加
				PushStageBlockList(stageBlocks_,objFloor_, BLOCK, mapLine, mapRow, -10.0f);
				// インクリメント
				mapLine++;
			}
			else if (word.find("SWITCH") == 0 || word.find("2") == 0) {
				// ステージのブロックを追加
				PushStageBlockList(stageBlocks_,objSwitchR_, SWITCHR, mapLine, mapRow, -10.0f);
				// インクリメント
				mapLine++;
			}
			else if (word.find("WALL") == 0 || word.find("3") == 0) {
				// ステージのブロックを追加
				PushStageBlockList(stageBlocks_,objWallR_, WALLR, mapLine, mapRow, -10.0f);
				// インクリメント
				mapLine++;
			}
			else if (word.find("SWITCH") == 0 || word.find("4") == 0) {
				// ステージのブロックを追加
				PushStageBlockList(stageBlocks_,objSwitchB_, SWITCHB, mapLine, mapRow, -10.0f);
				// インクリメント
				mapLine++;
			}
			else if (word.find("WALL") == 0 || word.find("5") == 0) {
				// ステージのブロックを追加
				PushStageBlockList(stageBlocks_,objWallB_, WALLB, mapLine, mapRow, -10.0f);
				// インクリメント
				mapLine++;
			}
			else if (word.find("GOAL") == 0 || word.find("6") == 0) {
				// ステージのブロックを追加
				PushStageBlockList(stageBlocks_,objGoal_, GOAL, mapLine, mapRow, -10.0f);
				// インクリメント
				mapLine++;
			}
			// 次の内容へ
			getline(line_stream, word, ',');
			
		}
		// マップチップLineが20を超えたらリセットしてRowをインクリメント
		if (mapLine == STAGE_WIDTH) {
			mapLine = 0;
			mapRow++;
		}
	}
}

void Stage::LoadFloorBlock() {
	for (int i = 0; i < STAGE_HEIGHT; i++) {
		for (int j = 0; j < STAGE_WIDTH; j++) {
			// ステージのブロックを追加
			PushStageBlockList(floorBlocks_, objFloor_, BLOCK, j, i, -14.0f);
		}
	}
}

void Stage::InitializeStageBlock(std::unique_ptr<StageData>& block, Object3d* obj, Vector3& pos, int line, int row) {
	// ワールドトランスフォームの初期化設定
	//block->worldTransform_.Initialize();
	block->obj = obj;

	// スケール設定
	block->worldTransform_.scale_ = block->obj->GetScale();
	block->worldTransform_.scale_ = { magnification_, magnification_, magnification_ };
	block->obj->SetScale(block->worldTransform_.scale_);
	
	// 座標設定
	block->worldTransform_.position_ = block->obj->GetPosition();
	block->worldTransform_.position_ = pos;
	block->obj->SetPosition(block->worldTransform_.position_);
	
	block->line_ = line;
	block->row_ = row;

	block->obj->SetWorldTransform(block->worldTransform_);

}

void Stage::PushStageBlockList(std::list<std::unique_ptr<StageData>>& blocks_, Object3d* obj, int type, int line, int row, float depth) {
	// リストに入れるために新しく宣言
	std::unique_ptr<StageData> newBlock = std::make_unique<StageData>();
	// ブロックの種類
	newBlock->type_ = type;
	// 座標
	Vector3 pos;
	
	pos.x = 2.0f + (4.0f * line);
	pos.y = depth;
	pos.z = 78.0f - (4.0f * row);

	// 初期化する
	InitializeStageBlock(newBlock, obj, pos, line, row);
	// リストに追加
	blocks_.push_back(std::move(newBlock));

	if (type == SWITCHR) {
		pos.x -= 2.0f;
		pos.z += 2.0f;
		switchR_->Seting(pos, magnification_);
		isSwitchDrawR_ = true;
	}
	if (type == SWITCHB) {
		pos.x -= 2.0f;
		pos.z += 2.0f;
		switchB_->Seting(pos, magnification_);
		isSwitchDrawB_ = true;
	}
	for (std::unique_ptr<StageData>& stage : blocks_)
	{
		stage->obj->Update();
	}
}

void Stage::CheckBlock(int line, int row) {
	// 範囲for
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		// NONEは返さない
		if (block->type_ == SWITCHR) {
			// 指定した番号に合った座標を返す
			if (block->line_ == line && block->row_ == row) {
				switchR_->OnCollisionSwitch();
			}
		}
		else if (block->type_ == SWITCHB) {
			// 指定した番号に合った座標を返す
			if (block->line_ == line && block->row_ == row) {
				switchB_->OnCollisionSwitch();
			}
		}
		else if (block->type_ == GOAL) {
			// 指定した番号に合った座標を返す
			if (block->line_ == line && block->row_ == row) {
				isGoal_ = true;
			}
		}
	}
}

bool Stage::CheckFloorBlock(int line, int row) {
	if (line < 0 || line > 19 || row < 0 || row > 19) {
		return true;
	}
	return false;
}

Vector3 Stage::GetBlockPosition(int line, int row) {
	// 範囲for
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		// ブロックと壁の時は返す
		if (block->type_ == BLOCK || block->type_ == WALLR || block->type_ == WALLB) {
			// 指定した番号に合った座標を返す
			if (block->line_ == line && block->row_ == row) {
				return block->worldTransform_.position_;
			}
		}
	}
	// なかったら0を返す
	return Vector3(0.0f, 0.0f, 0.0f);
}