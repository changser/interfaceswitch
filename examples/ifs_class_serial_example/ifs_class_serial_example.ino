////////////////      界面或菜单跳转框架程序例程                                      ////////////
////////////////      ifs_class_serial_example   V161005                              ////////////
////////////////      by changser    20161005                                         ////////////
////////////////      changser@139.com                                                ////////////
//说明：
//界面跳转演示，共三个界面，通过串口文本来显示各界面进入、即时运行和退出的时机
//控制也是通过键盘输入串口发送来实现。上翻w,下翻s
//enter1............................................
//running1..
//bye1..
//enter2..........................................
//...

#include "ifs_class.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//内部函数声明：
//功能：根据串口命令解析得到界面切换命令
//输入：串口缓冲区
//输出：static enum switchCMDenum类型
static enum switchCMDenum getIfsCMD();

//功能：界面1的进入、即时运行、退出函数
static void enter1();
static void run1();
static void exit1();

//功能：界面2的进入、即时运行、退出函数
static void enter2();
static void run2();
static void exit2();

//功能：界面3的进入、即时运行、退出函数
static void enter3();
static void run3();
static void exit3();

//////////////////////////////////////////////////////////////////////////////////////////////////
//变常量定义：
//界面结构体数组
#define IFSNUM  3
static const   interfaceStr   interfaceTable[IFSNUM]
= {
  //{0,{0,0,0,0,0,0},NULL,NULL,NULL},标准格式，0或空为初始值
  {1, {3, 2, 0, 0, 0, 0}, enter1, run1, exit1}, //缺省为开机的入口界面1。必须是数组第一个（下标为0）
  {2, {1, 3, 0, 0, 0, 0}, enter2, run2, exit2},
  {3, {2, 1, 0, 0, 0, 0}, enter3, run3, exit3}
};

//类IFS实例为对象ifs
IFS ifs = IFS(&interfaceTable[0], IFSNUM);

/////////////////////////////////////////////////////////////////////////////////////////////
//arduino开机初始化，系统默认。
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("hello,welcome");
}

//arduino循环，系统默认。
void loop() {
  // put your main code here, to run repeatedly:
  ifs.switchCMD = getIfsCMD();
  ifs.doSwitchCMD();  //执行窗口解析函数
  delay(48);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//函数实现：
//功能：根据串口命令解析得到界面切换命令
//输入：串口缓冲区
//输出：static enum switchCMDenum类型
static enum switchCMDenum getIfsCMD()
{
  enum switchCMDenum temp = SNONE;
  if (Serial.available() > 0)
  {
    switch (Serial.read())
    {
      case 'w':
        temp = SUP;
        break;
      case 's':
        temp = SDOWN;
        break;
      default:
        temp = SNONE;
        break;
    }
  }
  return temp;
}

static void enter1()
{
  //界面初始化，如重生成新屏、参数初始化
  Serial.println("hello");
  Serial.println("enter1............................................");
}

static void run1()
{
  //显示涮新或其他实时动作
  static unsigned int  i = 0;
  i++;
  if (i >= 20)
  {
    i = 0;
    Serial.println("running1..");
  }
}

static void exit1()
{
  //退出界面时参数销毁或保存等
  Serial.println("bye1");
}

static void enter2()
{
  Serial.println("enter2...................................................");
}

static void run2()
{
  static unsigned int i = 0;
  i++;
  if (i >= 20)
  {
    i = 0;
    Serial.println("running2..");
  }
}

static void exit2()
{
  Serial.println("bye2");
}

static void enter3()
{
  Serial.println("enter3.....................................................");
}

static void run3()
{
  static unsigned int i = 0;
  i++;
  if (i >= 20)
  {
    i = 0;
    Serial.println("running3..");
  }
}

static void exit3()
{
  Serial.println("bye3");
}

