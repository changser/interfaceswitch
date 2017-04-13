////////////////     界面或菜单跳转框架程序ifs_class_flash    V3.0.0   ///////////////////////////
////////////////           by changser    20170405                     ///////////////////////////
////////////////           changser@139.com                            ///////////////////////////

#ifndef __ifs_class_flash_H__
#define __ifs_class_flash_H__
#include <avr/pgmspace.h>

#define NUMofpathMAX 10//6   //从某界面换到其他页面的支路数中，这个是所有界面中最大的。

//定义界面结构体
typedef struct {
  unsigned char snInterface;  //界面序号
  //下面：跳转命令对应的目的界面，与枚举值{SNONE,SUP,SDOWN,SLEFT,SRIGHT,SEXIT,SENTER,SBACK}中的中间6项对应
  unsigned char gotoInterface[NUMofpathMAX];
  void (*EnterOperate)(); //函数指针，指向进入界面时的操作函数
  void (*RunOperate)();   //函数指针，指向此界面下的即时操作函数
  void (*ExitOperate)();  //函数指针，指向退出界面时的操作函数
} interfaceStr;

/*
  //界面结构体数组，名字自由定义，放在用户项目文件中定义由用户维护，其定义形式如下：
  #define IFSNUM  3    //界面个数，也即数组个数
  static const interfaceStr interfaceTable[IFSNUM] PROGMEM
  = {
  //{0,{0,0,0,0,0,0,0,0,0,0},NULL,NULL,NULL},标准格式，0或空为初始值
  {1, {3, 2, 0, 0, 0, 0, 0, 0, 0, 0}, enter1, run1, exit1}, //缺省为开机的入口界面1。必须是数组第一个（下标为0）
  {2, {1, 3, 0, 0, 0, 0, 0, 0, 0, 0}, enter2, run2, exit2},
  {3, {2, 1, 0, 0, 0, 0, 0, 0, 0, 0}, enter3, run3, exit3}
  };
*/

//定义一个枚举类型
enum   switchCMDenum  {SNONE = 0, SUP = 1, SDOWN = 2, SLEFT = 3, SRIGHT = 4, SEXIT = 5,
                       SENTER = 6, SF1 = 7, SF2 = 8, SF3 = 9, SF4 = 10, SBACK = 11
                      } ;
//声明枚举变量来存储跳转命令，定义在本模块里，不由用户定义。SBACK暂时不处理
/*
  //存的是非0，则此命令有效，进行相应的换页；为0（SNONE）则不跳转。
  //命令一大来源就是键盘输入，可以和键盘一一对应，也可以是虚拟键，以便处理复合键、定时跳转等问题。
  //一般界面转换有左、右（或上、下）、退出、进入某特定界面等四个命令就够了。
  //这里有6+1个，是考虑如果应用到下拉菜单，一个菜单项的变化就当作一个界面的话，会用到6个。
  //还有一个是可能会碰到通用弹出菜单，多个界面下会弹出，这时有两个办法，一个是将弹出菜单当作多个界面；
  //另一个办法是运用回退命令，不用表还跳转，靠程序存储的上一个界面来跳转。
  //实际上，就看最多有多少种跳转，有多少就定义多少再加1。而且取名F1CMD、F2CMD......也行。
*/

//声明类IFS
class IFS_F {
  public:
    IFS_F(interfaceStr const *firstp PROGMEM, unsigned char interfacenum);
    enum switchCMDenum switchCMD;   //界面跳转命令
    void doSwitchCMD();             //界面跳转命令解析执行。有无命令都执行。
    void gotoInterface(unsigned char ifsnum);   
  private:
    unsigned char interfaceNum;     //界面个数，也即界面结构体数组个数
    interfaceStr const *firstIfsP PROGMEM;        //界面1对应的结构体地址
    interfaceStr const *curIfsP PROGMEM;          //当前界面对应的结构体地址
    unsigned char curInterface;     //记录当前处于的界面序号。缺省为0,上电起始界面为1。
    unsigned char preInterface;     //记录跳转前的界面序号
    unsigned char desInterface;     //用gotoInterface()指令直接跳转时（不用switchCMD变量存储命令）
    void dojump(unsigned char ifsnum);//功能：界面跳转功能具体过程。
};

#endif
