/**
htmlcxx: DOM(DocumentObject Model �ĵ�����ģ��)��ʽ����XML��ʽ���ĵ������ڴ棬��Ϊÿ���ڵ㽨��һ������֮�����ǿ���ͨ����������ķ�ʽ���Խڵ���в��������ǽ�XML�Ľڵ�ӳ�䵽���ڴ��е�һϵ�ж���֮�ϡ�
�ŵ�:�ڽ�����ģ�ͺ�Ĳ�ѯ���޸��ٶȽϿ졣
ȱ��:�ڴ���֮ǰ��Ҫ������XML�ĵ�ȫ�������ڴ���н�����������XML������һ������ģ�ͣ����ĵ��ܴ�ʱ��DOM��ʽ�ͻ�ͻ�ֳ����״�����ԣ�һ��300KB��XML�ĵ����Ե���RAM���������ڴ���3000000KB��DOM��ģ�͡�

**/
#ifndef __HTML_PARSER_DOM_H__
#define __HTML_PARSER_DOM_H__

#include "ParserSax.h"
#include "tree.h"

namespace htmlcxx
{
	namespace HTML
	{
		class ParserDom : public ParserSax
		{
			public:
				ParserDom() {}
				~ParserDom() {}

				const tree<Node> &parseTree(const std::string &html);
				const tree<Node> &getTree()
				{ return mHtmlTree; }

			protected:
				virtual void beginParsing();

				virtual void foundTag(Node node, bool isEnd);
				virtual void foundText(Node node);
				virtual void foundComment(Node node);

				virtual void endParsing();
				
				tree<Node> mHtmlTree;
				tree<Node>::iterator mCurrentState;
		};

		std::ostream &operator<<(std::ostream &stream, const tree<HTML::Node> &tr);
	} //namespace HTML
} //namespace htmlcxx

#endif
