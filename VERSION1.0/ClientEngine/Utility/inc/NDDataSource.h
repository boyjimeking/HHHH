/***
*
*/

#ifndef NDDATASOURCE_H
#define NDDATASOURCE_H

#include "..\..\ui\inc\NDDataSource.h"

using namespace NDEngine;

namespace NDEngine
{
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

	class NDSection : public NDObject
	{
		DECLARE_CLASS(NDSection)

		NDSection();
		~NDSection();

	public:
		//		
		//		������SetTitle
		//		���ã��������ı���
		//		������title����
		//		����ֵ����
		void SetTitle(const char* title){ m_title = title; }
		//		
		//		������GetTitle
		//		���ã���ȡ�����ı���
		//		��������
		//		����ֵ������
		std::string GetTitle(){ return m_title; }
		//		
		//		������SetColumnCount
		//		���ã����÷���������
		//		������columnCount����
		//		����ֵ����
		void SetColumnCount(unsigned int columnCount);
		//		
		//		������GetColumnCount
		//		���ã���ȡ����������
		//		��������
		//		����ֵ������
		unsigned int GetColumnCount(){ return m_columnCount; }
		//		
		//		������SetRowHeight
		//		���ã�����ÿһ�еĸ߶�
		//		������rowHeight�߶�
		//		����ֵ����
		void SetRowHeight(unsigned int rowHeight){ m_rowHeight = rowHeight; }
		//		
		//		������GetRowHeight
		//		���ã���ȡÿһ�еĸ߶�
		//		��������
		//		����ֵ���߶�
		unsigned int GetRowHeight(){ return m_rowHeight; }

		void UseCellHeight(bool useCellHeight){ m_useCellHeight = useCellHeight; }
		bool IsUseCellHeight(){ return m_useCellHeight; }
		//		
		//		������SetFocusOnCell
		//		���ã����ý�����ĳһ��Ԫ��
		//		��������Ԫ������
		//		����ֵ����
		void SetFocusOnCell(unsigned int cellIndex){ m_focusCellIndex = cellIndex; }
		//		
		//		������GetFocusCellIndex
		//		���ã���ȡ�������ڵĵ�Ԫ������
		//		��������
		//		����ֵ����Ԫ������
		unsigned int GetFocusCellIndex(){ return m_focusCellIndex; }
		//		
		//		������AddCell
		//		���ã���ӵ�Ԫ��
		//		������cell��Ԫ�����ָ��
		//		����ֵ����
		void AddCell(NDUINode* cell);
		//		
		//		������InsertCell
		//		���ã��ڵڼ�������λ�ò��뵥Ԫ��
		//		������index��Ԫ��������cell��Ԫ��
		//		����ֵ����
		void InsertCell(unsigned int index, NDUINode* cell);
		//		
		//		������RemoveCell
		//		���ã�ɾ����Ԫ��
		//		������index��Ԫ������
		//		����ֵ����
		void RemoveCell(unsigned int index);
		//		
		//		������RemoveCell
		//		���ã�ɾ����Ԫ��
		//		������index��Ԫ������
		//		����ֵ����
		void RemoveCell(NDUINode* cell);
		//		
		//		������Cell
		//		���ã���ȡ��Ԫ��
		//		������index��Ԫ������
		//		����ֵ����Ԫ��
		NDUINode* Cell(unsigned int index);		
		//		
		//		������Clear
		//		���ã���յ�Ԫ��
		//		��������
		//		����ֵ����
		void Clear();
		//		
		//		������Count
		//		���ã���ȡ��Ԫ������
		//		��������
		//		����ֵ����Ԫ������
		unsigned int Count();

	private:

		std::vector<NDUINode*> m_cells;
		std::string m_title;
		unsigned int m_columnCount, m_rowHeight, m_focusCellIndex;
		bool m_useCellHeight;
	};
}

#endif