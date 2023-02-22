//マテリアル
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;//色(RGBA)
};

//3D変換行列
cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;//3D変換行列
};

//頂点シェーダの出力構造体
// (頂点シェーダからピクセルシェーダへのやり取りに使用)
struct VSOutput {
	//システム用頂点構造体
	float4 svpos : SV_POSITION;
	//uv値
	float2 uv : TEXCOORD;
};