#include "Stage.h"
#include <cassert>
#include <fstream>
#include "SafeDelete.h"

//using namespace std;
Camera* Stage::cameraStage_ = nullptr;

Stage::~Stage() {
	SafeDelete(model_);
	SafeDelete(modelFloor_);
	SafeDelete(modelGoal_);

	SafeDelete(obj_);
	SafeDelete(objFloor_);
	SafeDelete(objGoal_);
}

void Stage::Initialize(Camera* camera) {

	this->cameraStage_ = camera;

	//�C���X�^���X����
	model_ = new Model();
	modelFloor_ = new Model();
	modelGoal_ = new Model();

	obj_ = new Object3d();
	objFloor_ = new Object3d();
	objGoal_ = new Object3d();

	// ���f���ǂݍ���
	model_ = Model::LoadFromOBJ("cube", true);
	modelFloor_ = Model::LoadFromOBJ("floor", true);
	modelGoal_ = Model::LoadFromOBJ("cubeB", true);

	//3D�I�u�W�F�N�g�ƃJ�����̃Z�b�g
	obj_ = Object3d::Create();
	obj_->SetModel(model_);
	obj_->SetCamera(cameraStage_);

	objFloor_ = Object3d::Create();
	objFloor_->SetModel(modelFloor_);
	objFloor_->SetCamera(cameraStage_);

	objGoal_ = Object3d::Create();
	objGoal_->SetModel(modelGoal_);
	objGoal_->SetCamera(cameraStage_);

	// �X�e�[�W�̏���������
	LoadFloorBlock();
}

void Stage::StageInitialize(const std::string& stageNum) {
	// �ŏ��Ɏc���Ă���v�f���폜
	stageBlocks_.clear();

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

	isGoal_ = false;

	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		//block->worldTransform_.UpdateMatrix();
		block->obj->SetWorldTransform(block->worldTransform_);
		block->obj->Update();
	}

	for (std::unique_ptr<StageData>& block : floorBlocks_) {
		//block->worldTransform_.UpdateMatrix();
		block->obj->SetWorldTransform(block->worldTransform_);
		block->obj->Update();
	}
}

void Stage::Draw() {
	// �X�e�[�W�`��
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		if (block->type_ == BLOCK) {
			// �Ǖ`��
			obj_->Draw(block->worldTransform_);
		}
	
		else
			if (block->type_ == GOAL) {
				// �S�[���`��
			/*	block->worldTransform_.position_.y = -15.5f;*/
				objGoal_->Draw(block->worldTransform_);
			}


	}

	// ���`��
	for (std::unique_ptr<StageData>& block : floorBlocks_) {
		objFloor_->Draw(block->worldTransform_);
	}

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
				PushStageBlockList(stageBlocks_, obj_, NONE, mapLine, mapRow, stageDepth_);
				// �C���N�������g
				mapLine++;
			}
			else if (word.find("BLOCK") == 0 || word.find("1") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_, objFloor_, BLOCK, mapLine, mapRow, stageDepth_);
				// �C���N�������g
				mapLine++;
			}
			else if (word.find("GOAL") == 0 || word.find("6") == 0) {
				// �X�e�[�W�̃u���b�N��ǉ�
				PushStageBlockList(stageBlocks_, objGoal_, GOAL, mapLine, mapRow, stageDepth_);
				// �C���N�������g
				mapLine++;
			}
			// ���̓��e��
			getline(line_stream, word, ',');

		}
		// �}�b�v�`�b�vLine��10�𒴂����烊�Z�b�g����Row���C���N�������g
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
			PushStageBlockList(floorBlocks_, objFloor_, BLOCK, j, i, floorDepth_);
		}
	}
}

void Stage::InitializeStageBlock(std::unique_ptr<StageData>& block, Object3d* obj, Vector3& pos, int line, int row) {
	// ���[���h�g�����X�t�H�[���̏������ݒ�
	block->worldTransform_.Initialize();
	block->obj = obj;

	// �X�P�[���ݒ�
	block->worldTransform_.scale_ = block->obj->GetScale();
	block->worldTransform_.scale_ = { magnification_, magnification_, magnification_ };

	// ���W�ݒ�
	block->worldTransform_.position_ = block->obj->GetPosition();
	block->worldTransform_.position_ = pos;

	block->line_ = line;
	block->row_ = row;

	obj->SetWorldTransform(block->worldTransform_);

	obj->Update();

}

void Stage::PushStageBlockList(std::list<std::unique_ptr<StageData>>& blocks_, Object3d* obj, int type, int line, int row, float depth) {
	// ���X�g�ɓ���邽�߂ɐV�����錾
	std::unique_ptr<StageData> newBlock = std::make_unique<StageData>();
	// �u���b�N�̎��
	newBlock->type_ = type;
	// ���W
	Vector3 pos;

	//���������ƂȂ�l�ɍ��W���v�Z
	pos.x = -36.0f + (8.0f * line);
	pos.y = depth;
	pos.z = 92.0f - (8.0f * row);

	// ����������
	InitializeStageBlock(newBlock, obj, pos, line, row);
	// ���X�g�ɒǉ�
	blocks_.push_back(std::move(newBlock));

}

void Stage::CheckBlock(int line, int row) {
	// �͈�for
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		if (block->type_ == GOAL) {
			// �w�肵���ԍ��ɍ��������W��Ԃ�
			if (block->line_ == line && block->row_ == row) {
				isGoal_ = true;
			}
		}
	}
}

bool Stage::CheckFloorBlock(int line, int row) {
	if (line < 0 || line > 10 || row < 0 || row > 10) {
		return true;
	}
	return false;
}

Vector3 Stage::GetBlockPosition(int line, int row) {
	// �͈�for
	for (std::unique_ptr<StageData>& block : stageBlocks_) {
		// �u���b�N�ƕǂ̎��͕Ԃ�
		if (block->type_ == BLOCK) {
			// �w�肵���ԍ��ɍ��������W��Ԃ�
			if (block->line_ == line && block->row_ == row) {
				return block->worldTransform_.position_;
			}
		}
	}
	// �Ȃ�������0��Ԃ�
	return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 Stage::GetFloorPosition(int line, int row) {
	// �͈�for
	for (std::unique_ptr<StageData>& block : floorBlocks_) {
		// �w�肵���ԍ��ɍ��������W��Ԃ�
		if (block->line_ == line && block->row_ == row) {
			return block->worldTransform_.position_;
		}
	}
	// �Ȃ�������0��Ԃ�
	return Vector3(0.0f, 0.0f, 0.0f);
}
