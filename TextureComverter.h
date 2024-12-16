#pragma once
#include<string>
#include"Externals/DirectXTex/DirectXTex.h"

class TextureComverter
{
public:

	TextureComverter();
	~TextureComverter();

	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="numOptions">オプションの数</param>
	/// <param name="options">オプション配列</param>
	void ConvertTextureWICoDDS(const std::string& filePath, int numOptions = 0, char* options[] = nullptr);

	/// <summary>
	/// 使用方法を出力（表示）する
	/// </summary>
	static void OutputUsage();


private:

	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertMultiByteStringToWideStirng(const std::string& mString);

	/// <summary>
	/// フォルダパスとファイル名を分離する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void SeparateFilePath(const std::wstring& filePath);

	/// <summary>
	/// DDSテクスチャとしてファイル書き出し
	/// </summary>
	/// <param name="numOptions">オプションの数</param>
	/// <param name="options">オプション配列</param>
	void SaveDDSTextureToFile(int numOptions, char* options[]);

private:

	//画像の情報
	DirectX::TexMetadata metaData_;
	//画像イメージのコンテナ
	DirectX::ScratchImage scratchImage_;
	//ディレクトリパス
	std::wstring directoryPath_;
	//ファイル名
	std::wstring fileName_;
	//ファイル拡張子
	std::wstring fileExt_;

};

