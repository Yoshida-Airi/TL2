#include "TextureComverter.h"
#include<Windows.h>

TextureComverter::TextureComverter()
{
}

TextureComverter::~TextureComverter()
{
}

void TextureComverter::ConvertTextureWICoDDS(const std::string& filePath)
{
	//�e�N�X�`���t�@�C����ǂݍ���
	LoadWICTextureFromFile(filePath);

	//DDS�`���ɕϊ����ď����o��
	SaveDDSTextureToFile();
}

void TextureComverter::LoadWICTextureFromFile(const std::string& filePath)
{
	//�t�@�C���p�X�����C�h������ɕϊ�
	std::wstring wFilePath = ConvertMultiByteStringToWideStirng(filePath);

	//�e�N�X�`����ǂݍ���
	HRESULT result = LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(result));

	SeparateFilePath(wFilePath);
}

std::wstring TextureComverter::ConvertMultiByteStringToWideStirng(const std::string& mString)
{
	//���C�h������ɕϊ������ۂ̕��������v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	//���C�h������
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureComverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1;
	std::wstring exceptExt;

	//��؂蕶���f,�f���o�Ă����ԍŌ�̕���������
	pos1 = filePath.rfind('.');
	//�������q�b�g������
	if (pos1 != std::wstring::npos)
	{
		//��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		//��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = filePath.substr(0, pos1);
	}
	else
	{
		fileExt_ = L"";
		exceptExt = filePath;
	}

	//��؂蕶���f\\�f���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('\\');
	//�������q�b�g������
	if (pos1 != std::wstring::npos)
	{
		//��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̑O�܂ł𔲂��o��
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	
	//��؂蕶���f\\�f���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('/');
	//�������q�b�g������
	if (pos1 != std::wstring::npos)
	{
		//��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̑O�܂ł𔲂��o��
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//��؂蕶�����Ȃ��̂Ńt�@�C�����݂̂Ƃ��Ĉ���
	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureComverter::SaveDDSTextureToFile()
{
	HRESULT result;

	//�ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metaData_.format = DirectX::MakeSRGB(metaData_.format);

	//�o�̓t�@�C������ݒ肷��
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	result = DirectX::SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}
