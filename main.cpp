#include<cstdio>
#include<cstdlib>

int main(int argc,char*argv[])
{
	//argc‚Ì”‚¾‚¯ŒJ‚è•Ô‚·
	for (int i = 0; i < argc; i++)
	{
		//•¶š—ñargv‚Ìi”Ô‚ğ•\¦
		printf(argv[i]);
		//‰üs
		printf("\n");
	}

	system("pause");
	return 0;
}