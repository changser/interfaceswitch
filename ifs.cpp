////////////////           界面或菜单跳转框架程序ifs   V1.0.9   ///////////////////////////
////////////////           by changser    20170228              ///////////////////////////
////////////////           changser@139.com                     ///////////////////////////

#include "ifs.h"

//定义NULL，为了适用性强，不用#include <arduino.h>
#ifndef NULL
#ifdef __cplusplus  //c++模式，见http://blog.csdn.net/iitvip/article/details/8618120
#define NULL  0
#else
#define NULL ((void*)0)
#endif
#endif

enum switchCMDenum switchCMD = SNONE;

//需由用户通过ifsSetup()传递过来的参数：
static const interfaceStr *firstIfsP;      //界面1对应的结构体地址
static unsigned char interfaceNum;    //界面个数，也即界面结构体数组个数

//界面设置函数，设置界面个数和界面1对应的数组里的结构体的地址
//输入：
//     interfaceStr const *firstp         界面数组的第一个结构体地址（第一个界面）
//     unsigned char interfacenum   界面个数（数组个数）
//输出：存输入的两个参数，仅供本模块使用
//firstIfsP
//interfaceNum
//
void ifsSetup(interfaceStr const *firstp, unsigned char interfacenum)
{
  firstIfsP = firstp;
  interfaceNum = interfacenum;
}

//界面切换命令解析执行函数，本模块的核心
//输入：switchCMD
//输出：
//     switchCMD=SNONE         命令，全局变量，处理后复位
//     curInterface            当前界面，函数内部静态变量
//     preInterface            前一个界面，函数内部静态变量
//
void doSwitchCMD()
{
  unsigned char i;
  static unsigned char curInterface = 0; //记录当前处于的界面序号。缺省为0,上电起始界面为1。
  static unsigned char preInterface = 0; //记录跳转前的界面序号
  static interfaceStr const *curIfsP;        //当前界面对应的结构体地址

  //加入第一次调用的特殊处理，主要是调用上电起始界面的入口函数。
  if (curInterface == 0)
  {
    curInterface = 1;
    curIfsP = firstIfsP;
    //调用目的界面的进入函数
    if ((firstIfsP->EnterOperate) != NULL) //存在不为空的进入函数
    {
      firstIfsP->EnterOperate();//调用进口函数
    };
  };

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
        //暂时不处理。
      }
    }
    else //end of "if (switchCMD == SBACK)"
    {
      if (curIfsP->gotoInterface[switchCMD - 1]) //  判断是否是有效的目标界面值（非0）
      {

        //调用退出函数
        if (curIfsP->ExitOperate != NULL) //存在不为空的退出函数
        {
          curIfsP->ExitOperate();//调用
        };

        //保存跳转前的界面在表中的序号，即数组下标
        preInterface = curInterface;

        //通过查表更改当前界面序号和其在结构体数组中的下标号，方便各元素的应用。
        curInterface = curIfsP->gotoInterface[switchCMD - 1];

        curIfsP = firstIfsP;
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
        //跳转命令复位
        switchCMD = SNONE;
      } //end of if (curIfsP->gotoInterface[switchCMD - 1]) //  判断是否是有效的目标界面值（非0）
      else //有错，  报错类型赋值，便于返回。
      {
      };
    };//end of "else if (switchCMD == SBACK)"
  } //end of if (switchCMD != SNONE)
  else  //当前界面下没有切换界面命令
  {
    if (curIfsP->RunOperate != NULL) //存在不为空的界面即时运行函数
    {
      curIfsP->RunOperate();
    };
  };
}

