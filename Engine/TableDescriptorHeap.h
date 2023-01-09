#pragma once
class TableDescriptorHeap
{
private:

	ComPtr<ID3D12DescriptorHeap> _descHeap;
	uint64 _handleSize{ 0 };
	uint64 _groupSize{ 0 };
	uint64 _groupCount{ 0 };

	uint32 _currentGroupIndex{ 0 };

public:

	void Init(uint32 count);

	void Clear();
	void setCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void setSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);

	void CommitTable();

	ComPtr<ID3D12DescriptorHeap> getDescriptorHeap() { return _descHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(SRV_REGISTER reg);

private:

	D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(uint8 reg);


};

