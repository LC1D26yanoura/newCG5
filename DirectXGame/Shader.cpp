#include "Shader.h"
#include <cassert>


void Shader::Load(const std::wstring& filePath, const std::wstring& shaderModel) {
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	std::string mbShaderModel = ConvertString(shaderModel);

	    HRESULT hr = D3DCompileFromFile(
	    filePath.c_str(), // シェーダーファイル名
	    nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
	    // インクルード可能にする
	    "main", mbShaderModel.c_str(),
	    // エントリーポイント名、シェーダモデル指定
	    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shaderBlob, &errorBlob);
	// エラーが発生した場合、止める
	if (FAILED(hr)) {
		if (errorBlob) {
			OutputDebugStringA(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}
		assert(false);
	}
	blob_ = shaderBlob;
}

void Shader::LoadDxc(const std::wstring& filepath, const std::wstring& ShaderModel) { 
	static IDxcUtils* dxcUtils = nullptr;
	static IDxcCompiler3* dxcCompiler = nullptr;
	static IDxcIncludeHandler* includeHandler = nullptr;

	HRESULT hr;
	if(dxcUtils == nullptr) {
		hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		assert(SUCCEEDED(hr));
	}
	if (dxcCompiler == nullptr) {
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		assert(SUCCEEDED(hr)); 
	}
	if (includeHandler == nullptr) {
		hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
		assert(SUCCEEDED(hr));
	}

	IDxcBlobEncoding* shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filepath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));

	// 読み込んだファイルの内容をDxcBufferに設定する
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = {
	    filepath.c_str(), L"-E", L"main", L"-T", ShaderModel.c_str(), L"-Zi", L"-Qembed_debug", L"-Od", L"-Zpr",
	};

	IDxcResult* shaderResult = nullptr;

	hr = dxcCompiler->Compile(
	    &shaderSourceBuffer, arguments, _countof(arguments), includeHandler,
	    IID_PPV_ARGS(&shaderResult) 
	);
	assert(SUCCEEDED(hr));

		IDxcBlobUtf8* shaderError = nullptr;
	IDxcBlobWide* nameBlob = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), &nameBlob);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		OutputDebugStringA(shaderError->GetStringPointer());
		assert(false);
	}
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), &nameBlob);
	assert(SUCCEEDED(hr));

	shaderSource->Release();
	shaderResult->Release();
	dxcBlob_ = shaderBlob;
}


ID3DBlob* Shader::GetBlob() {
	    return blob_; 
}

ID3DBlob* Shader::GetDxcBlob() {
	return blob_;
}

Shader::Shader() {}

Shader::~Shader() {
	if (blob_ != nullptr) {
		blob_ = nullptr;
	}
	if (dxcBlob_ != nullptr) {
		dxcBlob_->Release();
		dxcBlob_ = nullptr;
	}
}
