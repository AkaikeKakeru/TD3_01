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
	//�C���X�^���X����
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

	// ���f���ǂݍ���
	model_ = Model::LoadFromOBJ("cubeB", true);
	modelFloor_ = Model::LoadFromOBJ("floor", true);
	modelSwitchR_ = Model::LoadFromOBJ("rswitch", true);
	modelSwitchB_ = Model::LoadFromOBJ("bswitch", true);
	modelWallR_ = Model::LoadFromOBJ("cubeR", true);
	modelWallB_ = Model::LoadFromOBJ("cubeB", true);
	modelGoal_ = Model::LoadFromOBJ("goal", true);

	//3D�I�u�W�F�N�g�ƃJ�����̃Z�b�g
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

	// �X�C�b�`
	switchR_ = new Switch();
	switchB_ = new Switch();
	switchR_->Initialize(modelSwitchR_, objSwitchR_);
	switchB_->Initialize(modelSwitchB_, objSwitchB_);


	// �X�e�[�W�̏���������
	LoadFloorBlock();
}

void Stage::StageInitialize(const std::string& stageNum) {
	// �ŏ��Ɏc���Ă���v�f���폜
	stageBlocks_.clear();

	isSwitchDrawR_ = false;
	isSwitchDrawB_ = false;

	// �o�b�t�@���N���A
	stageCommands.str("");
	// ��Ԃ��N���A
	stageCommands.clear(std::stringstream::goodbit);

	// �X�e�[�W�p�t�@�C���ǂݍ���
	LoadStageData(stageNum);
	// �X�e�[�W�t�@�C���Ŋe�v�f��ݒ�
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
	// �X�e�[�W�`��
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		if (block->type_ == BLOCK) {
			// �Ǖ`��
			obj_->Draw();
		}
		else if (block->type_ == WALLR) {
			// �ԕǕ`��
			objWallR_->Draw();
		}
		else if (block->type_ == WALLB) {
			// �Ǖ`��
			objWallB_->Draw();
		}
		else 
			if (block->type_ == GOAL) {
			// �S�[���`��
			block->worldTransform_.position_.y = -15.5f;
			objGoal_->Draw();
		}
	}

	// ���`��
	for (std::unique_ptr<StageData>& block : floorBlocks_) {
		objFloor_->Draw();
	}

	// �X�C�b�`�`��
	if (isSwitchDrawR_) switchR_->Draw();
	if (isSwitchDrawB_) switchB_->Draw();
}

void Stage::LoadStageData(const std::string& stageNum) {
	// �t�@�C��
	std::ifstream file;
	// �p�X���擾
	const std::string name = "stage";
	const std::string filename = name + stageNum + ".csv";
	const std::string directoryPath = "Resource/" + name + "/";
	// �t�@�C�����J��
	file.open(directoryPath + filename);
	assert(file.is_open());
	// �t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	stageCommands << file.rdbuf();

	// �t�@�C�������
	file.close();
}

void Stage::LoadStageCommands() {
	// 1�s���̕����������ϐ�
	std::string line;
	// �}�b�v�`�b�v�p�ϐ�
	int mapLine = 0;	// X
	int mapRow = 0;	// Y

	// �R�}���h���s���[�v
	while (getline(stageCommands, line)) {
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);
		// ������
		std::string word;
		// ,��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		// "//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			// �R�����g�s��ǂݔ�΂�
			continue;
		}

		while (mapLine != STAGE_WIDTH) {
			// �R�}���h�ǂݍ���
			if (word.find("NONE") == 0 || word.find("0") == 0 || word.find("7") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_, obj_, NONE, mapLine, mapRow, -10.0f);
				// �C���N�������g
				mapLine++;
			}
			else if (word.find("BLOCK") == 0 || word.find("1") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_,objFloor_, BLOCK, mapLine, mapRow, -10.0f);
				// �C���N�������g
				mapLine++;
			}
			else if (word.find("SWITCH") == 0 || word.find("2") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_,objSwitchR_, SWITCHR, mapLine, mapRow, -10.0f);
				// �C���N�������g
				mapLine++;
			}
			else if (word.find("WALL") == 0 || word.find("3") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_,objWallR_, WALLR, mapLine, mapRow, -10.0f);
				// �C���N�������g
				mapLine++;
			}
			else if (word.find("SWITCH") == 0 || word.find("4") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_,objSwitchB_, SWITCHB, mapLine, mapRow, -10.0f);
				// �C���N�������g
				mapLine++;
			}
			else if (word.find("WALL") == 0 || word.find("5") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_,objWallB_, WALLB, mapLine, mapRow, -10.0f);
				// �C���N�������g
				mapLine++;
			}
			else if (word.find("GOAL") == 0 || word.find("6") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_,objGoal_, GOAL, mapLine, mapRow, -10.0f);
				// �C���N�������g
				mapLine++;
			}
			// ���̓��e��
			getline(line_stream, word, ',');
			
		}
		// �}�b�v�`�b�vLine��20�𒴂����烊�Z�b�g����Row���C���N�������g
		if (mapLine == STAGE_WIDTH) {
			mapLine = 0;
			mapRow++;
		}
	}
}

void Stage::LoadFloorBlock() {
	for (int i = 0; i < STAGE_HEIGHT; i++) {
		for (int j = 0; j < STAGE_WIDTH; j++) {
			// �X�e�[�W�̃u���b�N��ǉ�
			PushStageBlockList(floorBlocks_, objFloor_, BLOCK, j, i, -14.0f);
		}
	}
}

void Stage::InitializeStageBlock(std::unique_ptr<StageData>& block, Object3d* obj, Vector3& pos, int line, int row) {
	// ���[���h�g�����X�t�H�[���̏������ݒ�
	//block->worldTransform_.Initialize();
	block->obj = obj;

	// �X�P�[���ݒ�
	block->worldTransform_.scale_ = block->obj->GetScale();
	block->worldTransform_.scale_ = { magnification_, magnification_, magnification_ };
	block->obj->SetScale(block->worldTransform_.scale_);
	
	// ���W�ݒ�
	block->worldTransform_.position_ = block->obj->GetPosition();
	block->worldTransform_.position_ = pos;
	block->obj->SetPosition(block->worldTransform_.position_);
	
	block->line_ = line;
	block->row_ = row;

	block->obj->SetWorldTransform(block->worldTransform_);

}

void Stage::PushStageBlockList(std::list<std::unique_ptr<StageData>>& blocks_, Object3d* obj, int type, int line, int row, float depth) {
	// ���X�g�ɓ���邽�߂ɐV�����錾
	std::unique_ptr<StageData> newBlock = std::make_unique<StageData>();
	// �u���b�N�̎��
	newBlock->type_ = type;
	// ���W
	Vector3 pos;
	
	pos.x = 2.0f + (4.0f * line);
	pos.y = depth;
	pos.z = 78.0f - (4.0f * row);

	// ����������
	InitializeStageBlock(newBlock, obj, pos, line, row);
	// ���X�g�ɒǉ�
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
	// �͈�for
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		// NONE�͕Ԃ��Ȃ�
		if (block->type_ == SWITCHR) {
			// �w�肵���ԍ��ɍ��������W��Ԃ�
			if (block->line_ == line && block->row_ == row) {
				switchR_->OnCollisionSwitch();
			}
		}
		else if (block->type_ == SWITCHB) {
			// �w�肵���ԍ��ɍ��������W��Ԃ�
			if (block->line_ == line && block->row_ == row) {
				switchB_->OnCollisionSwitch();
			}
		}
		else if (block->type_ == GOAL) {
			// �w�肵���ԍ��ɍ��������W��Ԃ�
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
	// �͈�for
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		// �u���b�N�ƕǂ̎��͕Ԃ�
		if (block->type_ == BLOCK || block->type_ == WALLR || block->type_ == WALLB) {
			// �w�肵���ԍ��ɍ��������W��Ԃ�
			if (block->line_ == line && block->row_ == row) {
				return block->worldTransform_.position_;
			}
		}
	}
	// �Ȃ�������0��Ԃ�
	return Vector3(0.0f, 0.0f, 0.0f);
}