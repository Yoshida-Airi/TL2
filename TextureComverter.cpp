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
	//テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);

	//DDS形式に変換して書き出す
}

void TextureComverter::LoadWICTextureFromFile(const std::string& filePath)
{
	//ファイルパスをワイド文字列に変換
	std::wstring wFilePath = ConvertMultiByteStringToWideStirng(filePath);

	////テクスチャを読み込む
	//HRESULT result = LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData_, scratchImage_);
	//assert(SUCCEEDED(result));

	//SeparateFilePath(wFilePath);
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
	pos1 = filePath.rfind(',');
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
		
	}

	//区切り文字がないのでファイル名のみとして扱う
	directoryPath_ = L"";
	fileName_ = exceptExt;
}
