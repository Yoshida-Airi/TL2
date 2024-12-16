#include "TextureComverter.h"
#include<Windows.h>

TextureComverter::TextureComverter()
{
}

TextureComverter::~TextureComverter()
{
}


void TextureComverter::ConvertTextureWICoDDS(const std::string& filePath, int numOptions, char* options[])
{
	//テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);

	//DDS形式に変換して書き出す
	SaveDDSTextureToFile(numOptions, options);
}

void TextureComverter::OutputUsage()
{
	printf("画像ファイルをWIC形式からDDS形式に変換します。\n");
	printf("\n"); //空白行
	printf("TextureConverter [ドライブ:][パス][ファイル名] [-ml level]\n");
	printf("\n"); //空白行
	printf(" [ドライブ:][パス][ファイル名]: 変換したいWIC形式の画像ファイルを指定します。\n");
	printf("\n");
	printf("[-ml level]: ミップレベルを指定します。0を指定すると1x1までのフルミップマップチェーンを生成します。");
}


void TextureComverter::LoadWICTextureFromFile(const std::string& filePath)
{
	//ファイルパスをワイド文字列に変換
	std::wstring wFilePath = ConvertMultiByteStringToWideStirng(filePath);

	//テクスチャを読み込む
	HRESULT result = LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(result));

	SeparateFilePath(wFilePath);
}

std::wstring TextureComverter::ConvertMultiByteStringToWideStirng(const std::string& mString)
{
	//ワイド文字列に変換した際の文字数を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	//ワイド文字列
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureComverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1;
	std::wstring exceptExt;

	//区切り文字’,’が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		//区切り文字の後ろをファイル拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		//区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}
	else
	{
		fileExt_ = L"";
		exceptExt = filePath;
	}

	//区切り文字’\\’が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		//区切り文字の後ろをファイル拡張子として保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の前までを抜き出す
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	
	//区切り文字’\\’が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		//区切り文字の後ろをファイル拡張子として保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の前までを抜き出す
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//区切り文字がないのでファイル名のみとして扱う
	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureComverter::SaveDDSTextureToFile(int numOptions, char* options[])
{
	size_t mipLevel = 0;

	//ミップマップレベル指定を検索
	for (int i = 0; i < numOptions; i++)
	{
		if (std::string(options[i]) == "-ml")
		{
			//ミップレベル指定
			mipLevel = std::stoi(options[i + 1]);
			break;
		}
	}

	HRESULT result;
	DirectX::ScratchImage mipChain;
	//ミップマップ生成
	result = DirectX::GenerateMipMaps(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_,
		DirectX::TEX_FILTER_DEFAULT, mipLevel, mipChain);

	if (SUCCEEDED(result))
	{
		//イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metaData_ = scratchImage_.GetMetadata();
	}

	DirectX::ScratchImage converted;
	//ミップマップ生成
	result = DirectX::Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_,
		DXGI_FORMAT_BC7_UNORM_SRGB, DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT |
		DirectX::TEX_COMPRESS_PARALLEL, 1.0f, converted);

	if (SUCCEEDED(result))
	{
		//イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(converted);
		metaData_ = scratchImage_.GetMetadata();
	}



	//読み込んだテクスチャをSRGBとして扱う
	metaData_.format = DirectX::MakeSRGB(metaData_.format);

	//出力ファイル名を設定する
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	result = DirectX::SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}


