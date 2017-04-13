1、主题、版权说明
////////////////           界面或菜单跳转框架程序库             ///////////////////////////
////////////////             ifs_class_flash    V3.0.0 			///////////////////////////
////////////////             ifs_class    		V2.1.0 			///////////////////////////
////////////////             ifs          		V1.0.9  		///////////////////////////
////////////////      Copyright 2016-2017      by changser      ///////////////////////////
////////////////             changser@139.com                   ///////////////////////////
=========================================================================================================================  

2、版本说明
V3.0.0 by changser 20170405  在ifs_class（V2.1.0）的基础上，修改成结构体数组存于ARDUINO UNO的FLASH区域。
                             否则占用RAM空间，数组稍大一点就不够用了。当然这样也将适用性减少了。
V2.1.0 by changser 20170331  1>修改doSwitchCMD()，不再默认上电时的界面号为1，但还是必需为firstIfsP所指向的。
                             一般指向界面数组，则元素下标号为0。当然也可以指向数组首地址+1的，不推荐。
							 2>增加一个私有函数void dojump(unsigned char ifsnum)，执行具体跳转过程，
                                只是便于多处调用。
							 3>增加switchCMD的命令数为10+1个，10个为界面跳转分支数，原来6个不够用。
                             4>完善原来空着的SBACK命令（即那个+1）。用于退回保存的上一次的界面。不用用户保存。
                             5>增加一个私有变量unsigned char desInterface，及操作此变量的公有函数
                                void gotoInterface(unsigned char ifsnum); 用于不通过结构体数组执行跳转。
                                在某些一跳多的情况下，跳转支路数远远超过10个，比如弹出菜单，可以使用。
V2.0.1 by changser 20170228  修改说明；指向const结构体指针加const。在IDE1.8.1，且UNO下通过
V1.0.9 by changser 20170228  指向const结构体指针加const。在IDE1.8.1，且UNO下通过
V2.0.0 by changser 20161005  建立ifs_class，即C++类库。V2.0以前为c风格，V2.0以后为C++类库
V1.0.8 by changser 20161005  建立ifs库，C风格。在IDE1.6.11，UNO下通过

3、文件列表
..\interfaceswitch\
                  -examples\ifs_serial_example\ifs_serial_example.ino       
				                        C风格例程，ifs库的应用
                  -examples\ifs_class_serial_example\ifs_class_serial_example.ino
				                        C++风格例程，ifs_class库的应用
				  -readme.txt           本文件，说明文档
				  -ifs.h                C风格库头文件
                  -ifs.cpp              C风格库
				  -ifs_class.h          C++风格库头文件
				  -ifs_class.cpp        C++风格库
				  
4、 功能描述： 
    界面或菜单跳转框架。跳转时机、逻辑关系由用户在结构体数组中定义。
	跳转前中后具体操作需由用户填充数组中的函数。
	
5、	应用方法(仅以C++类库来说明)：
   先看类IFS声明，它将相关的变量都封装了进去。
	class IFS {
	  public:
		IFS(interfaceStr const *firstp, unsigned char interfacenum);
		enum switchCMDenum switchCMD;   //界面跳转命令
		void doSwitchCMD();             //界面跳转命令解析执行。有无命令都执行。
	  private:
		unsigned char interfaceNum;     //界面个数，也即界面结构体数组个数
		interfaceStr const *firstIfsP;        //界面1对应的结构体地址
		interfaceStr const *curIfsP;          //当前界面对应的结构体地址
		unsigned char curInterface;     //记录当前处于的界面序号。缺省为0,上电起始界面为1。
		unsigned char preInterface;     //记录跳转前的界面序号
	};
	其中，命令变量是枚举型的，枚举声明如下：
	enum   switchCMDenum  {SNONE = 0, SUP = 1, SDOWN = 2, SLEFT = 3, SRIGHT = 4, SEXIT = 5, SENTER = 6, SBACK = 7} ;
  
  具体应用过程
    一、将本库解压到arduino的库目录，比如将本库解压到arduino的库目录，比如"C:\Users\ch\Documents\Arduino\libraries"	
	注意：1>不要有两级interfaceswitch目录。本说明文件应在"C:\Users\ch\Documents\Arduino\libraries\interfaceswitch\"下面
	      2>若有多个版本的interfaceswitch库，请删除其它版本，否则会有问题
		  3>本模块并不依赖于arduino，可用于其他c场合。但例子是用于arduino
		  4>不当作库的话，可以将ifs_class.h和ifs_class.cpp拷入用户的项目目录，作为项目的一部分也可以。
	二、由用户包含ifs_class.h，即#include <ifs_class.h>
	三、用户项目中定义结构体数组，及个数
	四、实例化IFS，构造函数初始化IFS(interfaceStr *firstp, unsigned char interfacenum);参数改为合适的值。
	五、循环中调用对象XXX.doSwitchCMD(); 
	   注意两点：一是时间的配合，二是在调用之前需根据按键或者通信情况，合成命令，给switchCMD赋值。
	            合成命令可以在每个界面的后台，即doSwitchCMD()被调用之前，也可以在每个界面的即时运行函数之中。
				前者一目了然，后者便于具体问题具体分析。
	参见例程ifs_class_serial_example.ino等。


========================================================================================================================
6、历史说明（有点乱）
V1.0.8   by changser 20161001  往C++类库方向发展，但仍为c风格
描述：
    以前版本是基于c风格的编程，有两个全局变量（const interfaceStr interfaceTable[]，unsigned char interfaceNUM）
要定义在用户程序中，是种隐含设置；而且这个类只能用一次，如果同时要用于界面和菜单，要升级成C++类库。一步一步来，本版本为
C++类库打下基础。
    先引入ifsSetup()函数，将这两个全局变量的引用变成参数传递。传递用指针会合适些。在模块中定义两个变量
	              static interfaceStr *firstIfsP;      //界面1对应的结构体地址
                  static unsigned char interfaceNum;    //界面个数，也即界面结构体数组个数
	doSwitchCMD()内部，用指针替代curArraySn(当前界面所在数组的序号)
	         static interfaceStr *curIfsP;        //当前界面对应的结构体地址
			 所有对结构体数组的操作，都用指针的引用来实现。
应用办法：
    1、将本库解压到arduino的库目录，比如"C:\Users\ch\Documents\Arduino\libraries"
	注意：1>不要有两级interfaceswitch1.08目录。本说明文件应在"C:\Users\ch\Documents\Arduino\libraries\interfaceswitch1.08\"下面
	      2>若有多个版本的interfaceswitch库，请删除其它版本，否则会有问题
		  3>本模块并不依赖于arduino，可用于其他c场合。但例子是用于arduino
		  4>不当作库的话，可以将ifs.h和ifs.cpp拷入用户的项目目录，作为项目的一部分也可以。
		     若用户项目是基于C(编译器只支持C)，将ifs.cpp改名为ifs.c。本模块V1.08本来就是c风格。
	2、由用户包含ifs.h，即#include <ifs.h>
	3、用户项目中定义结构体数组，及个数
	4、初始化中调用ifsSetup(interfaceStr *firstp, unsigned char interfacenum);参数改为合适的值。
	5、循环中调用doSwitchCMD(); 
	   注意两点：一是时间的配合，二是在调用之前需根据按键或者通信情况，合成命令，给switchCMD赋值。
	            合成命令可以在每个界面的后台，即doSwitchCMD()被调用之前，也可以在每个界面的即时运行函数之中。
				前者一目了然，后者便于具体问题具体分析。
	参见例程ifs_serial_example.ino等。
	
======================================================================================================================
V1.0.7   by changser 20161001  建立并修改成可用状态
//编写思路为：
/*
  1、首先数一数，一个界面跳转到其他界面的支路数。最大的定义为NUMofpathMAX，缺省为6，一般够用。
  2、定义一个枚举全局类型，用于存储跳转命令。本质就是有多少种跳转，就有多少种命令再加1（为什么加1见下面）。
  同一时间只有一种跳转。
   enum {SNONE=0,SUP=1,SDOWN=2,SLEFT=3,SRIGHT=4,SEXIT=5,SBACK=6,SENTER=7}  switchCMD = SNONE;
   每个枚举值从0~7赋值，1~6是因为结构体表中每个界面对应有一个gotoInterface[NUMofpathMAX]数组，这可以方便调用此数组中的值。
   命令SBACK不根据这个表来跳转，而是代表回退，由程序保存上个界面的序号，不用查表跳转。比如用于多个界面的弹出框。
  3、定义结构体来表明每一界面可能的跳转去向及三种事件函数。如下所示：
  typedef struct{
  unsigned char snInterface;
  unsigned char gotoInterface[NUMofpathMAX];
  void (*EnterOperate)();
  void (*RunOperate)();
  void (*ExitOperate)();
  } interfaceStr;
    其中，snInterface为界面序号。可以不定义这一项，以结构体数组的序号来表示，但维护结构体数组时不方便。
          gotoInterface[NUMofpathMAX]为跳转列表。在此界面下，每个命令对应的目的界面。有几个跳转就填几项为非0
                           数组序号与命令值有一个缺省的对应关系。
         (*EnterOperate)()，进入界面时的操作函数，可以没有指向空。比如各种初始化，特别是界面重建。
                           解析函数一次性调用后退出，不能是循环的。
         (*RunOperate)()，在此界面时的即时操作。可以没有指向空。
                          解析函数多次调用并退出，不能是循环的。若是循环的，则需退机制，因为要执行命令解析函数，并保证实时性。
         (*ExitOperate)()，退出界面时的操作函数。可以没有指向空。比如各种销毁和参数保存。
                           解析函数一次性调用后退出，不能是循环的。
  4、定义结构体数组，把所有的界面跳转关系，及要执行的函数写进去。
   const interfaceStr interfaceTable[] = {
  //{1,{0,0,0,0,0,0},NULL,NULL,NULL},标准格式，0或空为初始值
  {1, {3, 2, 0, 0, 0, 0}, enter1, run1, exit1}, //缺省为开机的入口界面1。必须是数组第一个（下标为0）
  {2, {1, 3, 0, 0, 0, 0}, enter2, run2, exit2},
  {3, {2, 1, 0, 0, 0, 0}, enter3, run3, exit3}
  };
  注意：一个界面占一个下标，有几个界面数组就有几个元素（一个元素是一个结构体）。
        界面1必须为数组第一个，为上电界面。初始化可以放在enter1()里，会自动执行一次。
    因为跳转后需遍历数组确定新的元素下标，所以需给出数组个数，
    用sizeof(interfaceTable)的话，需数组在本模块中定义，否则出错，因为数组由用户维护，不在本模块中，不可行。
    那么定义数组个数unsigned char interfaceNUM进行参数传替。不知可变数组行不行？
  5、编写命令解析函数void doSwitchCMD();
  第一步，定义两个静态变量，分别存储当前界面号和对应在结构体数组interfaceTable[]中的下标。
  static unsigned char curInterface = 0; //记录当前处于的界面序号。缺省为0,上电起始界面为1。
  static unsigned char curArraySn = 0; //记录当前界面在interfaceTable[]表中的数组序号，便于数组操作。
  再定义一个记录上一个界面对应的数组序号（记录上一个界面序号也可以，但还是得查找数组序号）
  static unsigned char preArraySn=0;
  第二步，实现上电时自动执行enter1()一次（若不为空），并更新curInterface = 1；curArraySn = 0;
  第三步，执行命令解析，输入为switchCMD.
         若有正确命令，命令是回退（SBACK），则根据记录的来跳转
         其它命令则根据则根据结构体表interfaceTable[] 来实现跳转。
           先执行当前界面对应的退出函数。
           再更新curInterface各curArraySn，执行目的界面的进入函数。
           最后清除命令switchCMD = SNONE
         若无命令，则调用当前界面的即时运行函数。
*/

//应用思路
/*
  一、一般情况下，不用更改 #define NUMofpathMAX 6，若更改，则需更改结构体数组表。
  二、维护结构体数据表：
  const interfaceStr interfaceTable[] = {
  //{0,{0,0,0,0,0,0},NULL,NULL,NULL},标准格式，0或空为初始值
  {1, {3, 2, 0, 0, 0, 0}, enter1, run1, exit1}, //缺省为开机的入口界面1。必须是数组第一个（下标为0）
  {2, {1, 3, 0, 0, 0, 0}, enter2, run2, exit2},
  {3, {2, 1, 0, 0, 0, 0}, enter3, run3, exit3}
  };
  定义数组个数并赋初值。注意，一定根据实际表的个数（也就是界面个数）赋初值。
  unsigned char interfaceNUM = 3;
  三、编写各界面的进入函数、即时运行函数、退出函数，可以没有。与结构体数据表相对应。
        enter1()...
        run1()...
        exit1()...
  四、将窗口解析函数doSwitchCMD()放在主循环里，让其大约50ms执行一次。若有OS，放在任务里，50ms间隔会比较准。
    要求这个时间，是因为要保证各种按键、通信、定时器反应的即时性。当然可以更短。
  五、在其他时间或任务里，根据按键值、通信协议、定时器结果，得到跳转命令，给switchCMD赋值。
    若针对每个界面，这个有通用性，则给switchCMD赋值可以放在switchCMD()外面。
    若要最灵活，可以放在每个界面的即时处理函数里。
    因为doswitchCMD()中没对switchCMD进行保护，不建议在定时器中直接更改switchCMD值。同理，实时操作OS中也需注意。
*/
===============================================================================================================