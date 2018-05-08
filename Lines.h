#pragma once
#include<iostream>
#include <dbhatch.h>
#include <vector>
#include <algorithm>

using namespace std;


class Lines
{
public:
	Lines(void);
	~Lines(void);
	void CreateTestHash();
	void CreateTestHash2();
	AcDbObjectIdArray CreateTestHash3();
	void lineChangeColor(AcDbObjectIdArray objIds);
	AcDbObjectId PostToModelSpace(AcDbEntity* pEnt);
	AcDbObjectId CreateLine(AcGePoint3d ptStart,AcGePoint3d ptEnd);
	//填充
	AcDbObjectId CreateHatch(AcDbObjectIdArray objIds,const char* patName, bool bAssociative);
	void AddHatch(AcDbObjectIdArray objIds);

	//求交点函数
	bool LineIntersect(AcGeLineSeg3d line1,AcGeLineSeg3d line2,AcGePoint3d &point);

	//深度优先遍历构建所有面域
	AcDbObjectIdArray CreateRegion(const AcDbObjectIdArray& curveIds,double &area);
	void AddRegion(AcDbObjectIdArray objIds);
	void optimizePointsArray(AcGePoint3dArray &currPointsArray);

	AcDbObjectIdArray BreakLine();
	void FindTuBao();
};
