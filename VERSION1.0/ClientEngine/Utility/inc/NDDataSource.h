/***
*
*/

#ifndef NDDATASOURCE_H
#define NDDATASOURCE_H

#include "..\..\ui\inc\NDDataSource.h"

using namespace NDEngine;

class NDDataSource
{
public:
	NDDataSource();
	virtual ~NDDataSource();

//		������AddSection
//		���ã���ӷ���
//		������section��������ָ��
//		����ֵ����
	void AddSection(NDSection* section);
//		
//		������InsertSection
//		���ã���ĳһ���������������
//		������index��������
//		����ֵ����
	void InsertSection(unsigned int index, NDSection* section);
//		
//		������RemoveSection
//		���ã�ɾ������
//		������index��������
//		����ֵ����
	void RemoveSection(unsigned int index);
//		
//		������Section
//		���ã���ȡ��������ָ��
//		������index��������
//		����ֵ������
	NDSection* Section(unsigned int index);		
//		
//		������Clear
//		���ã���շ���
//		��������
//		����ֵ����
	void Clear();
//		
//		������Count
//		���ã���ȡ��������Ŀ
//		��������
//		����ֵ��������Ŀ
	unsigned int Count();
//		
//		������Copy
//		���ã���ȿ������ݼ�
//		��������
//		����ֵ�����ݼ�
	NDDataSource* Copy();		

protected:
private:
};

#endif