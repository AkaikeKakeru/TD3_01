#include "ParticleManager.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <MyMath.h>
#include <WinApp.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const float ParticleManager::radius_ = 5.0f;				// 底面の半径
const float ParticleManager::prizmHeight_ = 8.0f;			// 柱の高さ
ID3D12Device* ParticleManager::device_ = nullptr;
UINT ParticleManager::descriptorHandleIncrementSize_ = 0;
ID3D12GraphicsCommandList* ParticleManager::cmdList_ = nullptr;
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature_;
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate_;
ComPtr<ID3D12DescriptorHeap> ParticleManager::descHeap_;
ComPtr<ID3D12Resource> ParticleManager::vertBuff_;
ComPtr<ID3D12Resource> ParticleManager::texbuff_;
CD3DX12_CPU_DESCRIPTOR_HANDLE ParticleManager::cpuDescHandleSRV_;
CD3DX12_GPU_DESCRIPTOR_HANDLE ParticleManager::gpuDescHandleSRV_;
Matrix4 ParticleManager::matView_{};
Matrix4 ParticleManager::matProjection_{};
Vector3 ParticleManager::eye_ = { 0, 0, -5.0f };
Vector3 ParticleManager::target_ = { 0, 0, 0 };
Vector3 ParticleManager::up_ = { 0, 1, 0 };
D3D12_VERTEX_BUFFER_VIEW ParticleManager::vbView_{};

ParticleManager::VertexPos ParticleManager::vertices_[vertexCount_];

Matrix4 ParticleManager::matBillboard_ = Matrix4Identity();
Matrix4 ParticleManager::matBillboardY_ = Matrix4Identity();

void ParticleManager::StaticInitialize(ID3D12Device* device, int window_width, int window_height) {
	// nullptrチェック
	assert(device);

	ParticleManager::device_ = device;

	// デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	// カメラ初期化
	InitializeCamera(window_width, window_height);

	// パイプライン初期化
	InitializeGraphicsPipeline();

	// テクスチャ読み込み
	LoadTexture();

	// モデル生成
	CreateModel();

}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList) {
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ParticleManager::cmdList_ == nullptr);

	// コマンドリストをセット
	ParticleManager::cmdList_ = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate_.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature_.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleManager::PostDraw() {
	// コマンドリストを解除
	ParticleManager::cmdList_ = nullptr;
}

ParticleManager* ParticleManager::Create() {
	// 3Dオブジェクトのインスタンスを生成
	ParticleManager* object3d = new ParticleManager();
	if (object3d == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize()) {
		delete object3d;
		assert(0);
		return nullptr;
	}

	return object3d;
}

void ParticleManager::SetEye(Vector3 eye) {
	ParticleManager::eye_ = eye;

	UpdateViewMatrix();
}

void ParticleManager::SetTarget(Vector3 target) {
	ParticleManager::target_ = target;

	UpdateViewMatrix();
}

void ParticleManager::CameraMoveVector(Vector3 move) {
	Vector3 eye_moved = GetEye();
	Vector3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void ParticleManager::CameraMoveEyeVector(Vector3 move) {
	Vector3 eye_moved = GetEye();
	Vector3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void ParticleManager::InitializeDescriptorHeap() {
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap_));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void ParticleManager::InitializeCamera(int window_width, int window_height) {

	//ビュー行列の計算
	UpdateViewMatrix();

	//垂直視野角
	float angle_ = ConvertToRadian(60.0f);
	//アスペクト比
	float aspect_ = (float)WinApp::Win_Width/ WinApp::Win_Height;
	//ニアクリップ
	float nearClip_ = 0.1f;
	//ファークリップ
	float farClip_ = 1000.0f;

	Vector4 pers = {
		1.0f / (static_cast<float>(tanf(angle_ / 2.0f))) / aspect_,
		1.0f / (static_cast<float>(tanf(angle_ / 2.0f))),
		1.0f / (farClip_ - nearClip_) * farClip_,
		-nearClip_ / (farClip_ - nearClip_) * farClip_,
	};

	matProjection_ = Matrix4Identity();
	matProjection_ = {
		pers.x,0.0f,0.0f,0.0f,
		0.0f,pers.y,0.0f,0.0f,
		0.0f,0.0f,pers.z,1.0f,
		0.0f,0.0f,pers.w,0.0f
	};
}

void ParticleManager::InitializeGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob; // ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/shader/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/shader/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/shader/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//スケール
			"SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0

		},
		{ // 色
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
													  // ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	//半透明合成
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;*/
	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	//減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

									//AlphaToCoverageEnableを有効化
	gpipeline.BlendState.AlphaToCoverageEnable = true;

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

															  // ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature_.Get();

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate_));
	assert(SUCCEEDED(result));
}

void ParticleManager::LoadTexture() {
	HRESULT result = S_FALSE;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// WICテクスチャのロード
	result = LoadFromWICFile(L"Resource/particle.png", WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// テクスチャ用バッファの生成
	result = device_->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr, IID_PPV_ARGS(&texbuff_));
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = texbuff_->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV_= CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap_->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);
	gpuDescHandleSRV_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap_->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff_->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView(texbuff_.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV_
	);
}

void ParticleManager::CreateModel() {
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(vertices_));

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = device_->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices_, sizeof(vertices_));
		vertBuff_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices_);
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}

void ParticleManager::UpdateViewMatrix() {
	Vector3 axisZ = Vector3Normalize(target_ - eye_);
	Vector3 axisX = Vector3Normalize(Vector3Cross(up_, axisZ));
	Vector3 axisY = Vector3Cross(axisZ, axisX);

	//カメラ回転行列
	Matrix4 matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列0
	matCameraRot = {
		axisX.x,axisX.y,axisX.z,0.0f,
		axisY.x,axisY.y,axisY.z,0.0f,
		axisZ.x,axisZ.y,axisZ.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//転置により逆行列(逆回転)を計算
	matView_ = Matrix4Transposed(matCameraRot);

	//視点座標に-1を掛けた座標
	Vector3 reverseEyePosition = eye_ * -1;
	//カメラの位置からワールド原点へのベクトル(カメラ座標系)
	Vector3 cameraMoveVal_ = {
		Vector3Dot(reverseEyePosition,axisX),
		Vector3Dot(reverseEyePosition,axisY),
		Vector3Dot(reverseEyePosition,axisZ)
	};

	//ビュー行列に平行移動成分を設定
	matView_.m[3][0] = cameraMoveVal_.x;
	matView_.m[3][1] = cameraMoveVal_.y;
	matView_.m[3][2] = cameraMoveVal_.z;

	// ビルボード行列
	matBillboard_ = {
		axisX.x,axisX.y,axisX.z,0.0f,
		axisY.x,axisY.y,axisY.z,0.0f,
		axisZ.x,axisZ.y,axisZ.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// カメラX軸、Y軸、Z軸
	Vector3 ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X軸は共通
	ybillCameraAxisX = axisX;
	// Y軸はワールド座標系のY軸
	ybillCameraAxisY = Vector3Normalize(up_);
	// Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = Vector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	// Y軸回りビルボード行列
	matBillboardY_ = {
		ybillCameraAxisX.x,ybillCameraAxisX.y,ybillCameraAxisX.z,0.0f,
		ybillCameraAxisY.x,ybillCameraAxisY.y,ybillCameraAxisY.z,0.0f,
		ybillCameraAxisZ.x,ybillCameraAxisZ.y,ybillCameraAxisZ.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

void ParticleManager::Add(int life, 
	Vector3 position, Vector3 velocity, Vector3 accel,
	float start_scale,float end_scale,
	Vector4 start_color, Vector4 end_color){
	//リストに要素を追加
	particles_.emplace_front();
	//追加した要素の参照
	Particle& p = particles_.front();
	//値のセット
	p.position_ = position;
	p.velocity_ = velocity;
	p.accel_ = accel;
	p.num_frame_ = life;

	p.s_scale_ = start_scale;
	p.e_scale_ = end_scale;
	p.scale_ = p.s_scale_;

	p.s_color_ = start_color;
	p.e_color_ = end_color;
	p.s_color_ = start_color;
}

void ParticleManager::Config(
	float rnd_pos,
	float rnd_vel,
	float rnd_acc,
	float rnd_scale,
	float rnd_color) {
	//X,Y,Z全てランダムに分布
	Vector3 pos{};
	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	//X,Y,Z全てランダムに分布
	Vector3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	//重力に見立ててYのみランダムに分布
	Vector3 acc{};
	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	//[1.0f,2.0f]で分布
	float s_scale = 0.0f;
	s_scale = (float)rand() / RAND_MAX  * rnd_scale + rnd_scale;

	//全て、[64.0f,192.0f]で分布
	Vector4 s_color, e_color = {};
	s_color.x = (float)rand() / RAND_MAX  * rnd_color ;//+( rnd_color/2);
	s_color.y = (float)rand() / RAND_MAX  * rnd_color ;//+( rnd_color/2);
	s_color.z = (float)rand() / RAND_MAX  * rnd_color ;//+( rnd_color/2);
													   //s_color.w = (float)rand() / RAND_MAX  * rnd_color ;//+( rnd_color/2);
	s_color.w = 1.0f;
	e_color.x = (float)rand() / RAND_MAX  * rnd_color ;//+( rnd_color/2);
	e_color.y = (float)rand() / RAND_MAX  * rnd_color ;//+( rnd_color/2);
	e_color.z = (float)rand() / RAND_MAX  * rnd_color ;//+( rnd_color/2);
													   //e_color.w = (float)rand() / RAND_MAX  * rnd_color ;//+( rnd_color/2);
	e_color.w = 1.0f;

	//追加
	Add(60,
		pos, vel, acc,
		s_scale,0.0f,
		s_color,e_color);
}

bool ParticleManager::Initialize() {
	// nullptrチェック
	assert(device_);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	return true;
}

void ParticleManager::Update() {
	HRESULT result;
	Matrix4 matScale;

	//寿命が尽きたパーティクルを全削除
	particles_.remove_if(
		[](Particle& x) {
			return x.frame_ >= x.num_frame_;
		}
	);

	//全パーティクル更新
	for (std::forward_list<Particle>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		//経過フレームをカウント
		it->frame_++;

		//速度に加速度を加算
		it->velocity_ = it->velocity_ + it->accel_;
		//速度による移動
		it->position_ = it->position_ + it->velocity_;

		//進行度を0～1の範囲に変換
		float f = (float)it->frame_ / it->num_frame_;
		//スケールの線形補間
		it->scale_ = (it->e_scale_ - it->s_scale_) * f;
		it->scale_ += it->s_scale_;

		//色の線形補間
		it->color_.x = (it->e_color_.x - it->s_color_.x) * f;
		it->color_.y = (it->e_color_.y - it->s_color_.y) * f;
		it->color_.z = (it->e_color_.z - it->s_color_.z) * f;
		it->color_.w = 1.0f;// (it->e_color.w - it->s_color.w)* f;

		it->color_.x += it->s_color_.x;
		it->color_.y += it->s_color_.y;
		it->color_.z += it->s_color_.z;
		//it->color.w += it->s_color.w;
	}

	//頂点バッファへデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//パーティクルの情報を一つずつ反映
		for (std::forward_list<Particle> ::iterator it = particles_.begin();
			it != particles_.end();
			it++) {
			//座標
			vertMap->pos_ = it->position_;
			//スケール
			vertMap->scale_ = it->scale_;
			//色
			vertMap->color_ = it->color_;
			//次の頂点へ
			vertMap++;
		}
		vertBuff_->Unmap(0, nullptr);
	}

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	// 行列の合成
	constMap->mat_ = matView_ * matProjection_;
	constMap->matBillboard_ = matBillboard_;
	constBuff_->Unmap(0, nullptr);
}

void ParticleManager::Draw() {
	// nullptrチェック
	assert(device_);
	assert(ParticleManager::cmdList_);

	// 頂点バッファの設定
	cmdList_->IASetVertexBuffers(0, 1, &vbView_);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap_.Get() };
	cmdList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList_->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV_);
	// 描画コマンド
	cmdList_->DrawInstanced((UINT)std::distance(particles_.begin(), particles_.end()), 1, 0, 0);
}