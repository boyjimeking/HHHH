// ConverToMK.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ConverToMK.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	options_description kOptions("Tool Options");
	CConverToMK* pkMK = CConverToMK::initWithIniFile("ConvertToMKConfig.xml");

	kOptions.add_options()("help","���ǰ����ĵ�")("XX","XXXX");
	variables_map kVM;
	store(parse_command_line(argc,argv,kOptions),kVM);

	if (kVM.count("help"))
	{
		CConverToMK::StringVector kHelpStringSet;

		kHelpStringSet = pkMK->GetHelpComment();

		for (unsigned int uiIndex = 0;uiIndex < kHelpStringSet.size();uiIndex++)
		{
			cout << kHelpStringSet[uiIndex] << endl;
		}

		cout << "����Ϊ��������" << endl;
		cout << kOptions << endl;
		return 0;
	}

	pkMK->WriteToMKFile();

	getchar();

	return 0;
}