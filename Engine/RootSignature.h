#pragma once

//Rootsignature : �׸��� ȣ�� �� �������α׷��� �ݵ�� Rendering Pipeline�� binding�ؾ�
//�ϴ� �ڿ����� �����̰�, �� �ڿ����� ���̴� �Է� �������͵鿡 ��� �����ϴ����� �����Ѵ�.
//

class RootSignature
{
private:

	ComPtr<ID3D12RootSignature> _signature;

public:

	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature> getSignature() { return _signature; }

};

