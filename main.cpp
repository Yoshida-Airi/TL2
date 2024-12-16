#include<cstdio>
#include<cstdlib>
#include<cassert>
#include"TextureComverter.h"
#include <Windows.h> 



enum Argument
{
	kApplicationPath,	//�A�v���P�[�V�����̃p�X
	kFilePath,			//�n���ꂽ�t�@�C���̃p�X

	NumArgument
};

int main(int argc,char*argv[])
{
	
	//�R�}���h���C�������w��Ȃ�
	if (argc < NumArgument)
	{
		//�g������\������
		TextureComverter::OutputUsage();
		return 0;
	}

	//COM���C�u�����̏�����
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//�e�N�X�`���R���o�[�^
	TextureComverter converter;

	//�I�v�V�����̐�
	int numOptions = argc - NumArgument;
	//�I�v�V�����z��
	char** options = argv + NumArgument;

	//�e�N�X�`���ϊ�
	converter.ConvertTextureWICoDDS(argv[kFilePath],numOptions,options);

	//COM���C�u�����̏I��
	CoUninitialize();


	//system("pause");
	return 0;
}