////////////////           界面或菜单跳转框架程序ifs_class   V2.1.0  ///////////////////////////
////////////////           by changser    20170331              	   ///////////////////////////
////////////////           changser@139.com                          ///////////////////////////

#include "ifs_class.h"

//定义NULL，为了适用性强，不用#include <arduino.h>
#ifndef NULL
#ifdef __cplusplus
#define NULL  0
#else
#define NULL ((void*)0)
#endif
#endif


//IFS构造函数。初始化相关变量。
//输入：
//     interfaceStr *firstp         界面数组的第一个结构体地址（第一个界面）
//     unsigned char interfacenum   界面个数（数组个数）
//输出：存输入的两个参数，仅供本模块使用
//一个公共变量switchCMD
//其余5个为私有变量
//
IFS::IFS(interfaceStr const *firstp, unsigned char interfacenum)
{
  firstIfsP = firstp;             //指向第一个界面结构体数组元素。
  interfaceNum = interfacenum;    //界面个数，即界面结构体数组个数
  desInterface = 0;               //用户不通过界面结构体数组和switchCMD变量来跳转时，存储目的界面序号
  switchCMD = SNONE;
  curIfsP = NULL;                 //当前界面对应的结构体地址
  curInterface = 0;               //记录当前处于的界面序号。缺省为0,上电起始界面为1。
  preInterface = 0;               //记录跳转前的界面序号
}

//功能：提供直接改变目标界面号的一个接口。最终的改变也是在执行doSwitchCMD()时。
//输入：
//    unsigned char ifsnum     目标界面号
//输出：
//    私有变量 desInterface
void IFS::gotoInterface(unsigned char ifsnum)
{
  desInterface = ifsnum;
}

//功能：界面跳转功能具体过程。
//输入：
//    unsigned char ifsnum     目标界面号
//输出：
//    动作-调用老界面的退出程序
//    动作-调用新界面的进入程序
//    伴随上述动作，更新各类变量。主要是
//    私有变量preInterface;curInterface;curIfsP;
void IFS::dojump(unsigned char ifsnum)
{
  //调用退出函数
  if (curIfsP->ExitOperate != NULL) //存在不为空的退出函数
  {
    curIfsP->ExitOperate();//调用
  };

  //保存跳转前的界面在表中的序号，即数组下标
  preInterface = curInterface;

  //将当前界面号改为目标界面号，并指向其在结构体数组上的位置
  curInterface = ifsnum;   //curIfsP->gotoInterface[switchCMD - 1];

  curIfsP = firstIfsP;
  unsigned char i;
  for (i = 0; i < interfaceNum; i++)
  {
    if (curIfsP->snInterface == curInterface)
    {
      break;  //也找到了新的当前指针
    };
    curIfsP++;
  };

  //如果都没有找到，则界面结构体表有误，报错。暂不处理

  //调用目的界面的进入函数
  if (curIfsP->EnterOperate != NULL) //存在不为空的进入函数
  {
    curIfsP->EnterOperate();//调用
  };

}

//类成员函数。界面切换命令解析执行函数，本模块的核心
//输入：switchCMD
//输出：
//公共变量     switchCMD=SNONE         命令，全局变量，处理后复位
//5个私有变量也更新
void IFS::doSwitchCMD()
{
  unsigned char i;
  //加入第一次调用的特殊处理，主要是调用上电起始界面的入口函数。
  if (curInterface == 0)
  {
    curIfsP = firstIfsP;
    curInterface = curIfsP->snInterface; //指向的结构体数组第一个的界面号。

    //调用目的界面的进入函数
    if ((firstIfsP->EnterOperate) != NULL) //存在不为空的进入函数
    {
      firstIfsP->EnterOperate();//调用进口函数
    };
  };

  //优先执行用户设置的目标界面的跳转
  if (desInterface)
  {
    dojump(desInterface);
    desInterface = 0;
  }

  //当前界面下有切换界面命令
  if (switchCMD != SNONE)
  {
    if (switchCMD == SBACK)
    {
      if ((preInterface == 0) || (preInterface == curInterface)) //跳入第一个界面时存的是0，跳不回0，命令不正常
        //若跳转前存的是当前的界面值，不正常，不应存在
      {}   //报错？
      else    //执行跳回
      {
        unsigned char temp = preInterface;
        dojump(temp);
      }
    } //end of "if (switchCMD == SBACK)"
    else  //else of "if (switchCMD == SBACK)"
    {
      if (curIfsP->gotoInterface[switchCMD - 1]) //  判断是否是有效的目标界面值（非0）
      {
        unsigned char temp = curIfsP->gotoInterface[switchCMD - 1];
        dojump(temp);
      } //end of if (curIfsP->gotoInterface[switchCMD - 1]) //  判断是否是有效的目标界面值（非0）
      else //有错，  报错类型赋值，便于返回。
      {
      };
    };//end of "else if (switchCMD == SBACK)"
    //跳转命令复位
    switchCMD = SNONE;
  } //end of if (switchCMD != SNONE)
  else  //当前界面下没有切换界面命令
  {
    if (curIfsP->RunOperate != NULL) //存在不为空的界面即时运行函数
    {
      curIfsP->RunOperate();
    };
  };
}

