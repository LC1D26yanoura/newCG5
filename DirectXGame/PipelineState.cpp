#include "PipelineState.h"
#include "KamataEngine.h"

using namespace KamataEngine;

void PipelineState::Create(D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ID3D12PipelineState* graphicsPipeLineState = nullptr;
	HRESULT hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipeLineState));
	assert(SUCCEEDED(hr));
	// 生成した PipelineState をとっておく
	pipelineState_ = graphicsPipeLineState;
}

ID3D12PipelineState* PipelineState::Get() { return pipelineState_; }

PipelineState::PipelineState() {}

PipelineState::~PipelineState() {
	if (pipelineState_) {
		pipelineState_ -> Release();
		pipelineState_ = nullptr;
	}
}
