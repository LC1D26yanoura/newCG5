#include "RootSignature.h"
#include "KamataEngine.h"

using namespace KamataEngine;

void RootSignature::create() {
	if (rootSignature_) {
		rootSignature_->Release();
		rootSignature_ = nullptr;
	}
	DirectXCommon* dxcommon = DirectXCommon::GetInstance();
	D3D12_ROOT_SIGNATURE_DESC descriotionRootSignature{};
	descriotionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* ErrorBlog = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriotionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &ErrorBlog);
	if (FAILED(hr)) {
		DebugText ::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(ErrorBlog->GetBufferPointer()));
		assert(false);
	}
	// バイナリをもとに生成
	ID3D12RootSignature* rootSignature = nullptr;
	hr = dxcommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
	signatureBlob->Release();
	rootSignature_ = rootSignature;
}
	// 生成した RootSignatureを返す
	ID3D12RootSignature* RootSignature::Get() { return rootSignature_; }

	// コンストラクタ
    RootSignature::RootSignature() {}
    // デストラクタ
    RootSignature ::~RootSignature() {
	    if (rootSignature_) {
		    rootSignature_ -> Release();
		    rootSignature_ = nullptr;
	    }
    }