// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include   <time.h>   
#include   <stdlib.h> 
#include <math.h>
#include<vector>
#include<list>
#include "Lines.h"
//#include <aced.h>
//#include <rxregsvc.h>

//-----------------------------------------------------------------------------
#define szRDS _RXST("")	
const int MaxCoordinate=2500;
const int MinCoordinate=0;
const int MaxLineLen=2000;
const int MinLineLen=100;
const int MaxLineNum=200;
const int MinLineNum=150;
//const int MaxLineNum=20;
//const int MinLineNum=15;
static Lines lineObject;
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CMyTestApp : public AcRxArxApp {
private:
	
public:
	
	CMyTestApp () : AcRxArxApp () {}

	
	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;


		// TODO: Add your initialization code here

		return (retCode) ;
	}

	
	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	
	virtual void RegisterServerComponents () {
	}
	
	/*
	随机生成直线：注册一个arx命令myRandomLineCreate，随机生成150-200条线段。
	要求：线段的起点坐标范围在[0.0,0.0],[2500.0,2500.0]区域内，方向随机，长度分布在100 – 2000之间。*/

	static void myRandomLineCreate(){
		acutPrintf(L"\n随机生成直线");
		srand((unsigned)time(NULL)); 
		//先随机生成线段条数
		int num=rand()%(MaxLineNum-MinLineNum+1)+MinLineNum;
		AcDbBlockTable *pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable,AcDb::kForRead);
		AcDbBlockTableRecord *pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,AcDb::kForWrite);
		AcDbObjectId lineId;

		for(int i=0;i<num;){
			float ptStartX=rand()%(MaxCoordinate-MinCoordinate+1)+MinCoordinate;
			float ptStartY=rand()%(MaxCoordinate-MinCoordinate+1)+MinCoordinate;
			float ptEndX=rand();
			float ptEndY=rand();
			int len=sqrt( abs( (ptEndY-ptStartY)*(ptEndY-ptStartY)+(ptEndX-ptStartX)*(ptEndX-ptStartX) ) );
			if(len<MinLineLen || len>MaxLineLen) 
				continue;
			AcGePoint3d ptStart(ptStartX,ptStartY,0);
			AcGePoint3d ptEnd(ptEndX,ptEndY,0);
			
			AcDbLine *pLine =new AcDbLine(ptStart,ptEnd);

			pBlockTableRecord->appendAcDbEntity(lineId,pLine);
			i++;
			pLine->close();
		}
		pBlockTable->close();
		pBlockTableRecord->close();
		
	}
	
	static void createTestRegion(){
		AcDbObjectIdArray objIds;
		objIds=lineObject.CreateTestHash3();
		lineObject.AddRegion(objIds);
	}
	static void createTestLine(){
		lineObject.CreateTestHash2();
	}
	static void myFindMaxClosedArea(){
		lineObject.BreakLine();
		lineObject.FindTuBao();
	}
	static void myFindTuBao(){
		acutPrintf(L"\n寻找打断线段的凸包");
		lineObject.FindTuBao();
	}
	
	
	static void ArxMyBlock_MyPickEntity(void)
	{
		acutPrintf(L"\n创建填充测试命令：AA");
		acedRegCmds->addCommand(L"myRandomLineCreate1",L"AA",L"AA",ACRX_CMD_MODAL ,myRandomLineCreate);
		acutPrintf(L"\n创建打断,并寻找凸包测试命令：BB");
		acedRegCmds->addCommand(L"myRandomLineCreate1",L"BB",L"BB",ACRX_CMD_MODAL ,myFindMaxClosedArea);
		/*acutPrintf(L"\n凸包测试命令：CC");
		acedRegCmds->addCommand(L"myRandomLineCreate1",L"CC",L"CC",ACRX_CMD_MODAL ,myFindTuBao);
		acutPrintf(L"\n添加面域测试命令：CC");
		acedRegCmds->addCommand(L"myRandomLineCreate1",L"CC",L"CC",ACRX_CMD_MODAL ,createTestRegion);*/
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CMyTestApp)

extern "C" _declspec(dllexport) void xh()
{
	CMyTestApp::ArxMyBlock_MyPickEntity();
}

ACED_ARXCOMMAND_ENTRY_AUTO(CMyTestApp, ArxMyBlock, _MyPickEntity, xh, ACRX_CMD_TRANSPARENT, NULL)
