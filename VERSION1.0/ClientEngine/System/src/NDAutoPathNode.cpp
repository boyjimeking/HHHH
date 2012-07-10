//
//  NDAutoPath.mm
//  MapDataPool
//
//  Created by jhzheng on 10-12-13.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "NDAutoPathNode.h"

@implementation NDPathNode

@synthesize x = _x, y = _y, g = _g, h = _h, f = _f, number = _number, parent = _parent;

-(BOOL) compareEqualOtherObj:(id) otherObj
{
	return _h == [otherObj h];
}

-(BOOL) compareSmallThanOtherObj:(id) otherObj
{
	return _h < [otherObj h];
}

@end

////////////////////////////////////////////////////////

@interface NDParttionList(hidden)

- (nd_pnode_list_iter) findIterBegin:(nd_pnode_list_iter)begin To:(nd_pnode_list_iter)end ByObject:(id)obj;

@end

@implementation NDParttionList

//���ַ�Ѱ�Һ��ʵĲ���λ��
- (nd_pnode_list_iter) findIterBegin:(nd_pnode_list_iter)begin To:(nd_pnode_list_iter)end ByObject:(id)obj
{
	int nAmout = std::distance(begin, end);
	
	// ֻ��һ��Ԫ��
	if ( nAmout == 1 ) {
		return begin;
	}
	
	// ��begin���
	if ([(id)(*begin) compareEqualOtherObj:obj]) {
		return begin;
	}
	
	// ��end���
	if ([(id)(*end) compareEqualOtherObj:obj]) {
		return end;
	}
	
	// ֻ�������ڵ�
	if ( nAmout == 2 ) {
		// ��beginС
		if (![(id)(*begin) compareSmallThanOtherObj:obj]) {
			return begin;
		}
		
		return end;
	}
	
	int i = nAmout/2;
	
	nd_pnode_list_iter	midIter = begin;
	
	while (i--) {
		midIter++;
	}
	
	// ��mid���
	if ([(id)(*midIter) compareEqualOtherObj:obj]) {
		begin = midIter;
		return begin;
	}
	
	if([(id)(*midIter) compareSmallThanOtherObj:obj])
	{
		// С��
		begin = midIter;
	}
	else
	{
		// ����
		end = midIter;
	}
	
	return [self findIterBegin:begin To:end ByObject:obj];
}

- (BOOL)partitionInsertObject:(id)obj
{
	// һ��Ԫ�ض�û��
	if (ndPnodeList.empty()) {
		ndPnodeList.push_back(obj);
		return YES;
	}
	
	// �ҳ��ʺϵ�λ��
	nd_pnode_list_iter	begin = ndPnodeList.begin();
	nd_pnode_list_iter	end   = ndPnodeList.end()--;
	nd_pnode_list_iter	insert;

	insert = [self findIterBegin:begin To:end ByObject:obj];
	
	BOOL bRet = NO;
	
	if ( [(id)(*insert) compareEqualOtherObj:obj]
		|| ![(id)(*insert) compareSmallThanOtherObj:obj] ) {
		// С����,���뵽ǰ��
		ndPnodeList.insert(insert, obj);
		bRet = YES;
	}else{
		// ����
		if (insert == ndPnodeList.end()--) {
			ndPnodeList.push_back(obj);
			bRet = YES;
		}else {
			insert++;
			ndPnodeList.insert(insert, obj);
			bRet = YES;
		}
		
	}
	
	return bRet;
}

- (void)pop_front
{
	if (!ndPnodeList.size()) {
		return;
	}
	ndPnodeList.pop_front();
}

- (id)front
{
	if (!ndPnodeList.size()) {
		return nil;
	}
	return (id)( ndPnodeList.front() );
}

- (void)clear
{
	ndPnodeList.clear();
}

- (id)init
{
	if ((self = [super init])) {
		ndPnodeList.clear();
	}
	return self;
}

@end