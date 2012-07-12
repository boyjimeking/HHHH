//
//  NDAutoPath.h
//  MapDataPool
//
//  Created by jhzheng on 10-12-13.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#include <Foundation/Foundation.h>
#include "NDCompare.h"
#include <list>

@interface NDPathNode : NSObject <NDCompare>
{
	uint			_x,_y;			
	int			_h;				
	int			_g;
	int			_f;
	//int		_dir;
	//int		_flag;
	NSUInteger	_number;
	NDPathNode  *_parent;
}

@property(nonatomic, assign)uint x;						//�ڵ��x����
@property(nonatomic, assign)uint y;						//�ڵ��y����
@property(nonatomic, assign)int	h;						//�ڵ�Ĺ�ֵ
@property(nonatomic, assign)int	g;						//·���ĳ���
@property(nonatomic, assign)int f;						//g+h
//@property(nonatomic, assign)int	dir;				//����(��������)
//@property(nonatomic, assign)int	flag;				//��չ�����ı�־
@property(nonatomic, assign)NSUInteger number;			//����key
@property(nonatomic, assign)NDPathNode *parent;			//��չ�����ı�־

@end


typedef std::list<void*>			nd_pnode_list;
typedef nd_pnode_list::iterator		nd_pnode_list_iter;

// ���ִ洢����(ֵС�ĵĿ�ǰ)
@interface NDParttionList : NSObject
{
	// ·��������
	nd_pnode_list	ndPnodeList;
}
- (void)pop_front;

- (id)front;

- (BOOL)partitionInsertObject:(id)obj;

- (void)clear;

@end