#include<cstdio>
#include<cstdlib>
#include<cassert>
#include"TextureComverter.h"
#include <Windows.h> 



enum Argument
{
	kApplicationPath,	//アプリケーションのパス
	kFilePath,			//渡されたファイルのパス

	NumArgument
};

int main(int argc,char*argv[])
{
	assert(argc >= NumArgument);

	//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//テクスチャコンバータ
	TextureComverter converter;

	//テクスチャ変換
	converter.ConvertTextureWICoDDS(argv[kFilePath]);

	//COMライブラリの終了
	CoUninitialize();


	system("pause");
	return 0;
}