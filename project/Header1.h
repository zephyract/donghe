Revision: 29040
Author: lansheng
Date: 2014年4月17日 11:04:37
Message:
(1)当SD0和SD1盘符名互换时，无法通过文件系统升级APP的问题
(2)增加新的破解3360方式

----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 28977
Author: lansheng
Date: 2014年4月15日 10:29:29
Message:
(1)更新BlueCastle.dll解决迅族蓝牙盒子无法挂断问题
(2)播放视频:其他穴位.avi，重新录像-常规保健法.avi会闪烁问题
(3)增加外挂式模拟电视，通过CVBS信号强弱判断有无台功能

----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 28736
Author: lansheng
Date: 2014年4月4日 14:24:49
Message:

----
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360_Golden.rar

Revision: 28452
Author: lansheng
Date: 2014年3月27日 10:14:25
Message:
(1) 增加电容屏触摸IC GT911的支持(威艺德)
使用方法：
typedef struct
{
	unsigned EintPin	 : 8;
	unsigned ResetPin    : 8;
	unsigned PowerPin    : 8;
	unsigned Reserved    : 8;
} YGTOUCH_EVENTDATA, *PYGTOUCH_EVENTDATA;
HANDLE hTouchEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("YG_EnableCPTouchEvent"));
if(hTouchEvent)
{
	DWORD bData;
	YGTOUCH_EVENTDATA  gEvenData;
	gEvenData.EintPin =48;	// GPIO 48
	gEvenData.PowerPin =29;	// GPIO 29
	gEvenData.ResetPin =39;	// GPIO 39
	memcpy(&bData,&gEvenData,sizeof(DWORD));
	SetEventData(hTouchEvent,bData);
	SetEvent(hTouchEvent);
}
电容屏IC GT911的硬件连接就一个I2C总线，中断脚，上电脚，复位脚，客户可以根据自己的硬件，配置不同的IO口
中断只支持EINT1(GPIO48),EINT2(GPIO49)
后续如有其他电容屏触摸IC，调试后再增加
(2)客供WD移动硬盘，插着开机，不加载盘符问题(主线)
(3)客供SanDisk cruzer 4GB U盘在前USB口不能识别问题(威艺德)

----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 28319
Author: lansheng
Date: 2014年3月21日 10:56:41
Message:
(1)解决客供1TB希捷硬件不能识别问题
(2)USB IPOD功能，Iphone5,5s，在高楼层打电话挂断后,ipod功能失效问题
(3)ipod煲机，机率性死机问题
(4)非益光生产的核心板，将不支持新的软件升级
如客户研发部用的旧的核心板出现不能升级现象，请咨询我司项目服务窗口
(5)通过文件系统升级时，查找YC002.img，放宽至前25个文件，其他更新文件，放宽至100个文件
(以前是只查找前面18个文件). 
(6)MC HotDog&張震岳_離開(KTV).mpg播放结束后APP hung住问题
(7)播放VideoPatterns_xvid.avi后，调用MediaGraph_GetMediaInfo会导致异常问题
----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 28020
Author: lansheng
Date: 2014年3月6日 16:19:36
Message:
2014-03-06
(1)增加语音识别的语音唤醒功能(audin.lib,audio.lib,waveform2.lib,waveform.lib)
(2)ConFigFile_Tool.exe增加LCD pclk频率和极性设置选项
(3)开机倒车，参数bMirror增加cvbs通道选择
低4位：维持之前状态,镜像选择:
bMirror: 0 默认值，不做镜像处理
			 1 左右镜像
			 2 上下镜像
			 3 左右 & 上下镜像
			 高4位：CVBS通道选择
			 10-14分别代码CVBS0-CVBS4
			 (4)更新DVD软件，解决某客户乐驰机芯对某张DVD-Video不读的问题(此碟片记录质量较差，RFO信号不对称)
			 (5)更新蓝牙MT6622FW，解决华为7150手机蓝牙连接后有一定概率在车机段无法挂断的bug

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27868
Author: lansheng
Date: 2014年2月26日 10:44:13
Message:
		 (1)增加对128MB nandflash: MX30LF1G08AA 的支持
			 (2)SD卡驱动提供IOCTL_DISK_SET_SDCARD_ID_FIX接口，SD卡ID号可以自由设置
#define IOCTL_DISK_SET_SDCARD_ID_FIX  CTL_CODE(0x7, 0x501, METHOD_BUFFERED, FILE_ANY_ACCESS)
			 typedef struct _YG_SDCARDID {
				 BOOL   bEnable;			// 是否使用
				 USHORT ManufacturerID;		// 制造商ID
				 DWORD  ProductSerialNumber;		// 产品ID
		 } YG_SDCARDID, *PYG_SDCARDID;  
		 HANDLE hDskHandle=NULL;	
		 hDskHandle= CreateFile(_T("DSK1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
		 if(hDskHandle)
		 {
			 g_SDCardId.bEnable = 1;
			 g_SDCardId.ManufacturerID = 0x82;
			 g_SDCardId.ProductSerialNumber=0x16602EA9;
			 DeviceIoControl(hDskHandle,IOCTL_DISK_SET_SDCARD_ID_hYGAPIHandleardId,sizeof(YG_SDCARDID),NULL,0,NULL,NULL);
			 CloseHandle(hYGAPIHandle);
		 }
		 (3)增加加密芯片的支持
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27732
Author: lansheng
Date: 2014年2月18日 14:37:14
Message:
		 (1)读卡器IC:AU6465R获取SDID,插两个USB时，无法获取问题
			 (2)解决串口RX为低电平，会卡死问题
			 (3)针对安吉星的发声需求:
		 MSDK.DLL新增了3个接口：(同步更新APP下面的SDK库)
			 MRESULT     AvinCtrl_OpenRDS(HMEDIAGRAPH hMediaGraph, E_MSDK_AVIN_AINDEX eAIndex);//开启RDS功能
		 MRESULT     AvinCtrl_CloseRDS(HMEDIAGRAPH hMediaGraph, E_MSDK_AVIN_AINDEX eAIndex);//关闭RDS功能
		 BOOL 	    GClientSetRDSVolume(GHANDLE hAudioDev,GUINT32 u4Vol);//设置RDS音量
		 安吉星功能本质上是将一路Line-In走Waveform通道，然后与DSP输出声音叠加，由于其使用的系统资源与其他播放
			 源存在共享，所以使用时需要注意以下几种情况：
			 1.  播放SD/USB或DVP时开启RDS
			 此时不存在资源冲突，只需在播放RDS语音时，调用GClientSetVolumn调小当前多媒体播放音量；RDS语音播放结束后
			 恢复多媒体源播放音量。
			 2.  播放AUX-In信号时开启RDS
			 存在资源冲突，Driver已对此情况进行处理，RDS语音开始播放时会强行停止当前AXU-In信号的播放，但在RDS语音结束
			 后需要APP主动去恢复AXU-In信号的播放。
			 3.  播放蓝牙音乐或打电话时开启RDS
			 存在资源冲突，播蓝牙音乐时开启RDS，可以调用BZ_A2DPActive(FALSE)停止向Audio模块输送蓝牙音乐数据，RDS结束后
			 调用BZ_A2DPActive(TRUE)恢复播放；进行蓝牙通话时开启RDS，可以调用BZ_SetAudioOutput(TRUE)将声音切到手机端，RDS
			 结束后调用BZ_SetAudioOutput(FALSE)恢复通话。
			 (4)增加LVDS 480X272配置

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27562
Author: lansheng
Date: 2014年1月24日 14:34:37
Message:
		 (1)DVP初始化反初始化2000次,CD碟播放不了问题
			 (2)蓝牙通话时会有断音问题(Enable PLC做成可配置)修改配置工具ConFigFile_Tool.exe
			 (3)插入USB IPOD时，AVIN画面会抖动问题(2013-2-28更改过此问题，后来7月份的版本又被改了)更新全部display库
			 (4)迅族的两个USB设置成盘符名不一样时，后USB盘符名为Navi
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27274
Author: lansheng
Date: 2014年1月21日 13:46:38
Message:
		 (1)增加读卡器IC:AU6465R获取SDID的接口
			 (2)通过APP先更新一个BMP logo，然后再更新一个GIF logo，动画延时时间变恢复成1秒问题
			 (3)USB1.1干扰消息机制，USB1插着读卡器(部分SD卡),USB0插着U盘，开机，然后拔出读卡器，无EMI消息发出问题
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27135
Author: lansheng
Date: 2014年1月15日 17:00:59
Message:
		 (1)SD卡速度配置，对低速卡有效
			 之前：配置工具里SD卡最大Clock只对高速卡有效，低速卡是固定成20MHZ的
			 目前：高速卡继续沿用之前的，当低速卡时，如果配置工具配的最大Clock<20MHZ时，那么以配置的clock来使用,否则还是20MHZ
			 因为可能为了减少SD卡部分干扰，或者EMI等问题，客户会把此clock降的比较低，所以目前高速卡和低速卡都考虑进去

			 (2)配置工具更改，增加TFT选项，目的：
			 之前设置成LVDS的时候，TFT同时也有输出的，而TFT输出的clock，有些客户的主板会干扰到GPS收星，所以目前配置成LVDS的时候，默认把TFT关掉
			 但是之前也有客户是接两个屏，一个LVDS，一个TFT，所以为了兼容此情况，在配置工具里增加TFT选项，选择TFT，则配置成LVDS的时候，TFT也是有输出的

			 (3)1024x600的AVIN无法居中显示的问题

			 (4)RNDIS,支持三星，小米等andriod手机
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27084
Author: lansheng
Date: 2014年1月14日 11:00:28
Message:
		 (1)Wma ID3无法获取的问题
			 (2)支持RNDIS,手机与车机用USB连接，车机端可以实现上网功能（可能存在手机兼容性的问题）
			 (3)usb ipod反复插拔后无法初始化问题(ipod相关lib库)
			 (4)naviking地图由于PNG硬解无法正常运行问题(imgdec_png.lib)
			 (5)添加一个客户16字节ID的读和写的接口 (智航需求)
			 (6)USB IPOD添加频谱获取
			 (7)赵本山_范伟_功夫.wma无法播放问题
			 (8)在line_in或者录音情况下发生蓝牙通话时，蓝牙通话以最高优先级占用Audio资源保证功能正常

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 26815
Author: lansheng
Date: 2014年1月3日 14:36:09
Message:
		 (1)更新SPI驱动，SPI0和SPI1都可以使用
			 (2)USB1.1干扰消息机制，插着读卡器(部分SD卡)开机，然后拔出读卡器，无EMI消息发出问题
			 (3)SMSC-USB2244读卡器支持IOCTL_DISK_GET_STORAGEID接口

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 26707
Author: lansheng
Date: 2013年12月30日 18:40:28
Message:
		 1.68版本：
			 (1)蓝牙用PBAP下载电话本已增加PullPhoneBook的方式，在BZ_Init后调用BZ_PBMGRSetPBAPMethod(TRUE)即可
			 (2)推拿手法讲解.avi
			 (3)增加800X600 lvds设置,ConFigFile_Tool.exe同步更改
			 (4)触摸坐标获取方法增加:
		 HANDLE hHookEvent = CreateEvent(NULL,FALSE,FALSE,L"GetTouchEvent");
		 SetEventData(hHookEvent,0x89);时，只向main窗口发消息，系统不响应触摸消息
			 之前的SetEventData(hHookEvent,0x1);依然保留，当eventdata是1时，main窗口和系统都可响应触摸消息
			 (5)USB1.1干消息版本，当使用读卡器时存在点问题
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 26428
Author: lansheng
Date: 2013年12月19日 18:28:29
Message:
		 1.68版本：
			 (1)增加开机倒车轨迹功能(选择此文件backcar_Track.mrf)
			 (2)解决威艺德提供的一个KLD地图不能运行问题
			 (3)支持两个SPI口,Createfile(L"SPI0:"...),CreateFile(L"SPI1:"....)
			 (4)更新DVD软件,过滤OGG文件,支持ID碟片的获取

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 26243
Author: lansheng
Date: 2013年12月13日 10:52:16
Message:
		 1.68版本：
			 (1)EMI干扰消息机制：正常拔出USB，有时候也会发出干扰的消息，非拔出消息
			 (2)开机倒车(未发打开背光指令)情况下，倒车指令，再增加一个背光亮度的参数，方便客户MCU可以设置背光亮度
			 之前是默认最亮显示
			 (3)修改俄语键盘，解决opera浏览器输入文字时会显示两个相同字母问题
			 (4)修改I2C库，解决偶尔SCA和SDL闲时会出现低电平的问题
			 (5)修改升级闪灯逻辑，进入升级界面前蓝灯，红灯闪烁一下，升级过程蓝灯，红灯不亮，中间升级失败，亮红灯，升级成功，OS能正常运行，亮蓝灯(主要用于产线升级管控)
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 25706
Author: lansheng
Date: 2013年11月27日 11:39:47
Message:
		 1.68版本：
			 (1)兼容单颗128MB DDR的支持
			 (2)客供ape音频文件的ID3显示乱码(需更新OS和APP的msdk.dll)
			 (3)跟MTK同步系统下面的蓝牙库
			 (4)视频解码部分更新MTK针对不同芯片H.264给出的补丁
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 25610
Author: lansheng
Date: 2013年11月22日 9:16:53
Message:

		 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/MT3360.pbxml

Revision: 25586
Author: lansheng
Date: 2013年11月21日 14:46:36
Message:
		 1.68版本：
			 (1)凯立德地图有些界面因为颜色过渡边缘锐化造成斜纹出现
			 (2)2012-10-29解决.Net控件滑动显示异常问题(2D加速算法问题)造成bitblt刷新位图变慢问题

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 25534
Author: lansheng
Date: 2013年11月19日 19:30:18
Message:
		 1.68版本：
			 (1)集成泰合的USB CMMB模块驱动，播放器需找第三方开发
			 (2)USB EMI驱动修正之前PORT1无法发送FS_DAEMON_MOUNT消息问题
			 (3)当配成自适应时，USB2.0跳转到USB1.1的时候，发送事件：
Port1:L"USB1HighSpeedToFullSpeed"
Port0:L"USB0HighSpeedToFullSpeed"
	  (4)有时候move触摸几分钟，会自动up的问题
	  (5)添加USB WIFI，3G模块插入拔出事件通知L"Usb3GDeviceEvent",L"UsbWifiDeviceEvent"
	  (6)导航兼听功能支持多进程，之前只支持注册一个进程。

	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24882
Author: lansheng
Date: 2013年11月6日 10:39:04
Message:
		 1.68版本：
			 (1)更新target_NAND_1200XH_DL31.bin，解决DL31低温所有碟读不到的问题
			 (2)800x600分辨率平台在输入CVBS时调用MediaGraph_SetScale()无法同时缩放与裁剪的问题(需更新msdk.dll)
			 (3)优化480x272分辨率平台的软件(视频播放效果和CVBS-IN信号效果)

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24667
Author: lansheng
Date: 2013年10月29日 15:51:24
Message:
		 1.68版本：
			 (1)增加对现代512MB SLC FLASH：H27U4G8F2DTR的支持
			 (2).Net控件滑动显示异常问题(2D加速算法问题)
			 (3)播放"剑灵.mp4"切换到"康美之恋.rmvb"播放，有一定概率读到错误数据。
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24639
Author: lansheng
Date: 2013年10月28日 10:04:26
Message:
		 1.68版本：
			 (1)增加480X272分辨率的支持
			 (2)增加对三星512MB SLC FLASH: K9F4G08u0D的支持
			 (3)解决播放客供视频:车展6.flv视频卡住，播完不切下一首问题(此视频文件存在大量垃圾数据，有几个包从标志位
			 判断是Audio数据)
			 (4)兼容雷凌的USB WIFI模块
			 (5)兼容simcom的3G模块

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24459
Author: lansheng
Date: 2013年10月15日 11:04:22
Message:
		 1.68版本：
			 (1)解决某客户一个GIF的LOGO，循环开关机，会有机率性停在开机LOGO问题
			 (2)SD1接口支持通过文件系统的卡升级OS和APP(有些客户的机子只有SD1接口，此种方式提供软件升级的支持)
			 (3)硬件不带DAC，系统默认配置成DAC，APP设置不带DAC，RGB切换的时候会有噪音问题
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24341
Author: lansheng
Date: 2013年10月9日 14:01:20
Message:
		 1.68版本：
			 (1) 播放视频文件：金陵十三_1080P.mov死机问题
			 (2) 某个板上FLASH，有插写成功，但不能正常写入问题，（迅族问题）
			 (3) USB1配置成USB1.1，USB0配置成自适应时，USB1不起作用,还是USB2.0
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24212
Author: lansheng
Date: 2013年9月27日 10:52:23
Message:
		 1.68版本：
			 (1)更新AUDIO驱动，支持SPDIF (需要相应更新DVD软件，APP SDK)
			 之前的SPDIF Driver支持将USB/SD downmix输出为PCM。
			 现在的SPDIF Driver已经支持将DVD、USB/SD、Line in downmix为PCM或者Raw data。Raw fata仅支持AC3和DTS编码。
			 GSysServices.h添加接口GSetSpdifOutputType和DVP_GSetSpdifOutputType分别将USB/SD和DVP输出的音频信号通过SPDIF接口输出。
			 SPDIFOUTPUT_T结构体确定是以PCM还是Raw data形式输出。Raw Data仅支持AC3和DTS编码的音频源。

			 (2)更新USB驱动，解决齐顶反馈的一个U盘读不到问题
			 (3)更新BOOT，解决因为扫描坏块造成核心板升级不成功的问题。(eboot.nb0,sdagent.nb0需要更新)
			 (4)增加检测SD卡GPSUPDATE.PACK文件延时机制
			 (5)EBOOT中检测文件数据超时由16改成18

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 23925
Author: lansheng
Date: 2013年9月13日 14:13:06
Message:
		 1.68版本：
			 (1)USB驱动更新，支持USB1.1,USB2.0,自适应，三种切换
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 23800
Author: lansheng
Date: 2013年9月9日 9:55:28
Message:
		 1.68版本：
			 (1)增加USB1.1干扰问题的缓冲机制
			 针对一些车型，比如现代车系，起亚K5，BYD F3出现在按喇叭或开大灯，U盘盘符会卸载的问题
			 软件上做了一种处理(APP需配合修改)，虽然盘符出现短暂的卸载，但是让用户感受不到。
			 (2)当LOGO为BMP图片的时候，支持进度条显示，进度条，客户可以通过PndFlashtool1.6.8\Tool\Rcbuild进行配置
			 打开bitmap.rc,
			 6            150        395        Bitmap                 Slider.bmp
			 ID号为6即代表进度条图片，150,395为进度条的起始坐标，如果设置成0,0，则不支持进度条功能
			 客户根据自己的需求，替换Slider.bmp和修改起始座标即可
			 (3)当系统配置成外部DAC功能时,APP设置PWM功能无效问题
			 (4) 6月25号以后版本，CMMB煲机死机问题
			 (5) 增加DVD数据碟ID3信息设置codepage的接口,驱动支持，APP提供接口

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 23428
Author: lansheng
Date: 2013年8月23日 13:58:16
Message:
		 1.68版本：
			 (1)GPS端口再增加18和19
			 (2)16K 衰减不达标问题
			 (3)ipod 插拔data abort问题
			 (4)BSP包增加MT3332和MT3336的支持
			 (5)更改fastcamera.cpp，FSC_IOControl增加互斥，解决很低机率性的停在开机LOGO问题
			 (6)修改SD卡检测超时机制,3次改为1次,时间从3.5S改为8S
			 (7)增加自动运行Residentflash2\App\YGAutorun.ini里路径程序的机制
			 (8)YCAPI.DLL增加获取SD卡通讯模式的接口
			 (9)YCAPI.DLL重新修改了一下获取芯片类型的接口
			 (10)安徽森力USB HUB加载慢问题,提供一版:安徽森力_ConFigFile_Tool.exe
			 (11)新增1200XH湘武机芯软件版本target_NAND_1200XH_XW200.bin
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 22844
Author: lansheng
Date: 2013年8月12日 10:34:47
Message:
		 1.68版本：
			 (1)增加GPS端口配置，ConFigFile_Tool.exe修改
			 (2)更新target_NAND_1200XH_DL31.bin,解决DL31机芯异响问题
			 (3)通过文件系统升级metazone后启动系统，第一次不弹出校屏界面问题

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 22778
Author: lansheng
Date: 2013年8月8日 19:58:21
Message:
		 1.68版本：
			 (1)兼容单颗256MB DDR3
			 (2)800X600分辨率的时候，通过APP不能升级800X600的BMP格式LOGO问题(齐顶)
			 (3)更改SD驱动，针对迅簇的版本把SD卡检测的timeout时间由3.5M改为20S
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 22380
Author: lansheng
Date: 2013年8月1日 8:52:06
Message:
		 1.68版本
			 (1)播放某台电科技32G U盘视频文件会卡死问题(属U盘兼容性问题)
			 (2)修改Launch->AppWriter.cpp,APP升级时支持>32MB的单文件升级
			 (3)增加PCB加密方式
			 (4)增加SD0和SD1盘符名称互换的配置，ConFigFile_Tool.exe更新
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 21874
Author: lansheng
Date: 2013年7月16日 18:49:51
Message:
		 1.68版本
			 (1)迅簇U盘名字更新，不管插USB0还是USB1，第一个插的盘符名字叫USBDisk
			 (2)播放"001高胜美-缘.flac"和MP3之间切换的时候死机问题，更改aud_adsp.lib库
			 (3)更新蓝牙库，支持蓝牙同时连两部设备，一部固定连某个蓝牙设备的SPP profile，一部连接其他HF功能
			 增加接口BZ_SPPCreateConnectByBDAddr(REMOTE_DEVICE_SPP_ADDR); 如需支持,app库BlueZMW.DLL需同步更改
主要用途: 车机蓝牙,可以连接OBD蓝牙设备,获取一些信息,同时又不影响正常的与手机HF连接
	  (4)Metazone工具更新，支持两个USB接口均配置成USB1.1速度
	  (5)播放CMMB(外挂CVBS式)，会造成蓝牙断开连接问题,更新msdkcore.dll, tvd_ce_al.lib
	  (6)通过文件系统可以升级backcar.mrf文件

	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 21489
Author: lansheng
Date: 2013年7月5日 9:43:12
Message:
		 1.68版本
			 (1)3G驱动更新			（齐顶问题）
			 (2)软键盘去掉“输入面板”四字	（齐顶问题）
			 (3)蓝牙库BlueCastle.dll更新，解决GT-S3710手机通话异常
			 (4)更新WAVE注册表,之前说调MIC增益的一个值从0x6e改回0x68
			 后续如果客户要更改audio的MIC增益,蓝牙通话音量,降噪处理,均可以通过APP接口实现
请参考文档:Waveform Introduction(For Yecon).pdf
	  (5)修改HW_MSDK.dll, IPOD问题：IPOD播放列表的时候，需要将专辑图片，艺术家和歌曲时间以及歌名都显示出来
	  (6)更新库display_tcon.lib,tcon.h,支持pclk极性可配置
	  (7)用flashtool工具升级APP时，APP文件夹如有中文时显示乱码问题（迅簇问题）
	  (8)增加USB转串口驱动,IC型号:FL232RL	（徐岗问题）
	  (9)更新spi_mt3360_v1.lib，解决APP下面无法操作SPI的问题（徐岗问题）
	  (10)更新flashtool和appwriter，解决0kb文件不能打包问题（迅簇问题）
	  (11)更新显示驱动，解决AVIN图像会出现重影问题
	  这一点改动较大，还改了overlay显示的一些bug，即把其他客户解的一些问题整合进来，但是APP有可能需要配合修改
	  客户的overlay不同层刷新可能有先后之分
	  (12)播放某个视频"登山.avi"，执行断点播放功能MediaCtrl_LastMemStart会花屏 问题（正鼎问题）
	  (13)解决ISDBT无法播放录制文件问题	（益光问题）
	  (14)解决SD卡1bit模式时，有些卡插卡会产生data abort问题
	  (15)更新USB驱动，解决西数465G移动硬盘不识别&SD读卡器速度慢的问题 （皇玛问题）
	  (16)为迅簇增加SYSINFO.TXT文件到windows目录	（迅簇问题）
	  (17)Metazone配置工具更新，增加video memory size配置(限制范围8MB-30MB)（威艺德问题）
	  (18)UFT8编码歌词显示乱码问题		（齐顶问题）
	  (19)增加DVD里WMV，DIVX，WMA解码开放开关
	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 21335
Author: lansheng
Date: 2013年7月2日 17:28:04
Message:
		 1.68版本
			 (1)3G驱动更新
			 (2)软键盘去掉“输入面板”四字
			 (3)蓝牙库BlueCastle.dll更新，解决GT-S3710手机通话异常
			 (4)更新WAVE注册表,之前说调MIC增益的一个值从0x6e改回0x68
			 后续如果客户要更改audio的MIC增益,蓝牙通话音量,降噪处理,均可以通过APP接口实现
请参考文档:Waveform Introduction(For Yecon).pdf
	  (5)修改HW_MSDK.dll, IPOD问题：IPOD播放列表的时候，需要将专辑图片，艺术家和歌曲时间以及歌名都显示出来
	  (6)更新库display_tcon.lib,tcon.h,支持pclk极性可配置
	  (7)用flashtool工具升级APP时，APP文件夹如有中文时显示乱码问题
	  (8)增加USB转串口驱动,IC型号:FL232RL
	  (9)更新spi_mt3360_v1.lib，解决APP下面无法操作SPI的问题
	  (10)更新flashtool和appwriter，解决0kb文件不能打包问题
	  (11)更新显示驱动，解决AVIN图像会出现重影问题
	  这一点改动较大，还改了overlay显示的一些bug，即把其他客户解的一些问题整合进来，但是APP有可能需要配合修改
	  客户的overlay不同层刷新可能有先后之分
	  (12)播放某个视频"登山.avi"，执行断点播放功能MediaCtrl_LastMemStart会花屏 问题
	  (13)解决ISDBT无法播放录制文件问题
	  (14)解决SD卡1bit模式时，有些卡插卡会产生data abort问题

	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 21287
Author: lansheng
Date: 2013年7月1日 15:17:38
Message:
		 1.68版本
			 (1)3G驱动更新
			 (2)软键盘去掉“输入面板”四字
			 (3)蓝牙库BlueCastle.dll更新，解决GT-S3710手机通话异常
			 (4)更新WAVE注册表,之前说调MIC增益的一个值从0x6e改回0x68
			 后续如果客户要更改audio的MIC增益,蓝牙通话音量,降噪处理,均可以通过APP接口实现
请参考文档:Waveform Introduction(For Yecon).pdf
	  (5)修改HW_MSDK.dll, IPOD问题：IPOD播放列表的时候，需要将专辑图片，艺术家和歌曲时间以及歌名都显示出来
	  (6)更新库display_tcon.lib,tcon.h,支持pclk极性可配置
	  (7)用flashtool工具升级APP时，APP文件夹如有中文时显示乱码问题
	  (8)增加USB转串口驱动,IC型号:FL232RL
	  (9)更新spi_mt3360_v1.lib，解决APP下面无法操作SPI的问题
	  (10)更新flashtool和appwriter，解决0kb文件不能打包问题
	  (11)更新显示驱动，解决AVIN图像会出现重影问题
	  (12)播放某个视频"登山.avi"，执行断点播放功能MediaCtrl_LastMemStart会花屏 问题
	  (13)解决ISDBT无法播放录制文件问题
	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 20868
Author: lansheng
Date: 2013年6月14日 11:40:13
Message:
		 1.68版本
			 (1)eboot支持SD卡1bit模式
			 (2)UART6的软件端口号从０改为６,为了兼容国外的一些地图软件或第三方软件
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 20633
Author: lansheng
Date: 2013年6月8日 10:45:53
Message:
		 1.68版本
			 (1)更新库DivxParserFilter.dll,msdkcore.dll 增加获取视频码流的接口
APP同步更新如下文件:
		 MSDK.DLL,MSDK.LIB,MSDK_Defs.h,MSDK_MediaGraph.h
			 (2)更新i2c_mt3360_v1.lib,支持ADV7180调试
			 (3)更新库tvd_ce_al.lib，tvd_hal_lib.lib，tvd_lib.lib，支持APP层camera in一些参数设置问题
			 可以兼容ATV 320x240图像以及ADV7180 720x480图像
			 (4)增加音频默认输出PWM还是I2S信号的配置，即默认使不使用DAC，配置工具ConFigFile_Tool.exe同时修改
			 (5)发增加SD通讯模式1bit or 4bit mode的配置，配置工具ConFigFile_Tool.exe同时修改
			 (6)3G驱动更新,兼容性更好些
			 (7)开机倒车优化，增加0格显示
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 20354
Author: lansheng
Date: 2013年5月31日 11:17:28
Message:
		 1.68版本
			 (1)MT3353支持cvbs out输出
			 (2)支持开机倒车雷达显示

			 此版BSP未更新到解决第(1)点的display三个库
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 19881
Author: lansheng
Date: 2013年5月23日 9:52:45
Message:
		 1.68版本
			 (1)修改注册表custom.reg,增加字体平滑处理功能，ClearType
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 19841
Author: lansheng
Date: 2013年5月22日 11:07:38
Message:

		 ----
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/Aribic/MT3360.pbxml

Revision: 19840
Author: lansheng
Date: 2013年5月22日 11:06:53
Message:

		 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/MT3360.pbxml

Revision: 19837
Author: lansheng
Date: 2013年5月22日 10:59:19
Message:
		 1.68版本
			 (1)更新I2C驱动i2c_mt3360_v1.lib,i2c.h,用于齐顶的ADV7180芯片通讯，接 camera接口
			 (2)更新库Mhcdmdd4hub.lib，修改USB1 WIFI上网不稳定问题
			 (3)更新库display_gpelib.lib,系统增加mouse组件，驱动层屏蔽光标图标。这样客户可以调用GetCursorPos等光标相关接口
			 (4)更新DVD软件，针对齐顶反馈读碟repeat off的时候 上一曲后，再点击时操作无响应的问题，本次更新的版本号是 0113212(5)1
			 (5)更新库com_mdd2_1.lib，驱动层打印信息uart drop增加index标识，用于串口打印出此LOG时可以判断是哪个串口
			 (6)更新蓝牙库BlueCastle.dll,Blueman.dll 兼容有个客户的蓝牙SPP模块设备
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 19419
Author: lansheng
Date: 2013年5月9日 13:57:08
Message:
		 1.68版本
			 (1)更新LOGO后屏参会变问题(采用调用mtk_dvp_load的方式)
			 (2)MSDK增加设置codepage的接口
			 (3)增加USB复位的接口:
#define IOCTL_HAL_USB_HOST_RESET CTL_CODE(FILE_DEVICE_HAL, 62, METHOD_BUFFERED, FILE_ANY_ACCESS)
		 CreateFile(L"HCD0",…); //For usb port0
		 CreateFile(L"HCD1",…); //For usb port1
		 (4)MT3353B支持倒车
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 19050
Author: lansheng
Date: 2013年4月27日 9:14:40
Message:

		 ----
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/MT3360.pbxml
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/Aribic
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/CalibrUi.cpp
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/CalibrUi.pbpxml
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/CalibrUi.reg
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/CalibrUi_clone.rc
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/GetCaText.cpp
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/GetCaText.h
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/Readme.txt
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/TouchCalibrate.txt
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/calibrrc.h
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/makefile
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/makefile.inc
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/postlink.bat
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/prelink.bat
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/工程文件/CH/CalibrUi/sources

Revision: 18880
Author: lansheng
Date: 2013年4月24日 10:52:11
Message:
		 1.68版本
			 (1)Anycall GT-S3710 下载电话本乱码的问题,修正了英文乱码的问题，中文乱码属于不可修复问题
			 (2)试图获取USB IPod的一些信息打印 Not Support的问题现已添加
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 18703
Author: lansheng
Date: 2013年4月19日 15:54:39
Message:
		 1.68版本
			 (1)按MTK最新要求更改FLASH驱动
从开机logo请确认FMD版本号是: 2013041200；mtkflash版本号是：2013011701
				   (2)更新AUDIO驱动，支持翼卡一键导航新的抓数据方式
				   ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 18325
Author: lansheng
Date: 2013年4月11日 16:48:14
Message:
		 1.68版本
			 (1)更新蓝牙库
			 提供判断对方手机是否支持ID3的接口
			 修复了断开蓝牙连接再连上之后收不到ID3时间不再更新的漏洞	SPP部分的更新，连接的时候会通知上层连接的串口，并且断开的时候会通知对方断开连接	
			 SPP 上层APP 获取串口号的示例代码：
			 if (BZ_IND_SPP_CONNECTED == dwIndID)
			 {
				 if (lParam == sizeof(SPPVCOMINFO_T))
				 {
					 P_SPPVCOMINFO_T info = (P_SPPVCOMINFO_T)wParam;
				 }
			 }

			 (2)更新DVD FW
				 (3)按齐顶要求，齐顶系统只能按齐顶方工升级APP
				 　　同步城要更新metazone配置工具给齐顶

				   ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 18047
Author: lansheng
Date: 2013年4月2日 15:40:18
Message:
			 1.68版本
				 (1)更新齐顶反馈调用YC_BackLightEnable(0)后再调YC_BackLightEnable(1)会闪一下白屏问题
				 (2)张工反馈的播放视频的时候播放较大GIF会DBGCHK FAIL的问题，更新库imgdec_gif.lib
				 (3)有一个256MB FLASH不能烧写问题，更新FLASH库
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17999
Author: lansheng
Date: 2013年4月1日 15:50:48
Message:
			 1.67版本
				 (1)更新MSDKCORE.DLL,解决齐顶提供的一个视频说会卡很久问题
				 (2)支持MT3353B
				 (3)更新YCAPIDLL,KNCG支持３D功能
				 (4)支持DDR512MB内存
				 (5)更新齐顶反馈调用YC_BackLightEnable(0)后再调YC_BackLightEnable(1)会闪一下白屏问题
				 (6)张工反馈的播放视频的时候播放较大GIF会DBGCHK FAIL的问题，更新库imgdec_gif.lib
				 (7)有一个256MB FLASH不能烧写问题，更新FLASH库
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17789
Author: lansheng
Date: 2013年3月25日 16:10:35
Message:
			 1.68版本
				 (1)更新MSDKCORE.DLL,解决齐顶提供的一个视频说会卡很久问题
				 (2)支持MT3353B
				 (3)更新YCAPIDLL,KNCG支持３D功能
				 (4)支持DDR512MB内存
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17684
Author: lansheng
Date: 2013年3月21日 17:24:22
Message:
			 1.67版本
				 (1)CMMB改为只支持CMMB05(SM2180)模块
				 (2)解决USB1.1，播放U盘高清视频时调用findfirstfile会卡住问题
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17681
Author: lansheng
Date: 2013年3月21日 17:15:29
Message:
			 1.68版本
				 (1)CMMB改为只支持CMMB05(SM2180)模块
				 (2)解决USB1.1，播放U盘高清视频时调用findfirstfile会卡住问题

				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17585
Author: lansheng
Date: 2013年3月19日 11:57:26
Message:
			 1.67版本
				 (1) 华宏把storage card改成inand时，首次不格式化inand问题
				 (2) 优化塞格导航DTMF功能
				 (3) 把WMV扩展名改为WMA可以进入软解并收不到Finish Event 的问题
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17426
Author: lansheng
Date: 2013年3月13日 9:50:22
Message:
			 1.67版本
				 (1)更改TOUCH,解决齐顶触摸有DOWN无UP的问题。
				 (2)更改华宏反馈视频文件93370455.h264_1.MP4 seek会停顿问题
				 (3)USB IPOD支持视频

				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17217
Author: lansheng
Date: 2013年3月4日 15:21:10
Message:
			 1.68版本
				 同步当前１.６７版本更新
				 （１）更新齐顶WIFI驱动，解决插拔WIFI出错问题
				 （２）1.68版本支持ISDBT05(2130)模块
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17176
Author: lansheng
Date: 2013年3月1日 10:13:28
Message:
			 1.67版本
				 为后续支持BICG芯片作准备，可以开启动BICG支持H.264硬解码
				 （默认BICG芯片是不支持H.264的）
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17157
Author: lansheng
Date: 2013年2月28日 11:54:13
Message:
			 1.67版本
				 (1)更改TOUCH,解决齐顶触摸因为硬件反而倒致飘问题。
				 (2)更改3G 注册表
				 (3)更新display_pmx.lib库，解决AVIN 和IPod 一起播放，AVIN会间歇性抖动一下问题
				 (4)更新蓝牙库，BlueCastle.dll，Blueman.dll，BlueZMW.dll
				 蓝牙新增查询手机是否支持传过来手机铃声的接口

				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17017
Author: lansheng
Date: 2013年2月16日 11:59:19
Message:
			 1.67版本
				 (1)加快APP更新LOGO速度(更新一张LOGO从之前的1.5S提高到0.6S)
				 (2)解决WYD反馈的720P文件播放seek到1:33:42时无声音，导致seek回去也没声音的问题
				 (3)解决AVIN 和IPod 一起播放，AVIN会间歇性抖动一下
				 (4)支持蓝牙通话时获取PCM音频数据（翼卡的一键导航需要此功能）
				 (4)更新DVD FW
				 DVP  FW 更新到最新版 
				 其中Servo版本号确认
				 1200W +DL30    3f0D0060
				 1200XH+DL30    9b090060
				 HD860 +DL30    83070060
				 1200XH+DL31    e3010060
				 (5)跟MTK同步BSP包，包括APP下面的现个DLL:DVPAgent.dll，MSDK.dll
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16810
Author: lansheng
Date: 2013年1月29日 11:00:50
Message:
			 1.67版本
				 (1)更新注册表，更改GPS和蓝牙的物理串口
GPS: COM9->COM10
蓝牙: COM3->COM11
	更改文件：platform.reg
	(2)增加皇玛提供的华为3G模块支持
	更改文件：platform.bib,platform.reg,MT3360\FILES目录增加3G_Huawei文件夹
	(3)16Bit显存从6MB增大到10MB
	----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16776
Author: lansheng
Date: 2013年1月27日 10:56:19
Message:
			 1.67版本
				 更新串口IPOD检测BUG，之前没有把检测脚做为输入脚。
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16728
Author: lansheng
Date: 2013年1月25日 14:29:13
Message:
			 1.67版本
				 (1)更新display_gpelib.lib,解决WYD反馈的播放视频"剑雨.mp4"再播"康美之恋.rmvb"会图像错乱问题
				 (2)更新串口IPOD检测机制,首次调用接口，不发消息，直接给返回值。
				 (3)更新HW_MSDK.DLL，解决USB IPOD前后排的一些问题
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16533
Author: lansheng
Date: 2013年1月21日 15:41:21
Message:
			 1.67版本
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16311
Author: lansheng
Date: 2013年1月11日 15:58:49
Message:
			 1.67 DVD IN NOR版本：
				 2012-01-11
				 (1)关闭蓝牙生成cfa log日志文件的功能
				 (2)TFT和LVDS屏软件实现统一(之前SDAgent_24bit.nb0是分版本的),由配置工具配置LCD项即可
				 (3)配置工具ConFigFile_Tool.exe更改,LCD支持:LVDS 1024X600 
				 (4)为了满足不同客户对触摸校准界面的更改，并且同时兼容800x480和1024x600的屏，触摸校准界面做更改：
				 目前支持文字指示型和图片指示型：
				 <1> 文字指示型，进入校准界面时，首先会找\\Residentflash2\\app\\TouchCalibrate.txt文件，如有此文件
				 则进入文字指示型校准界面
				 <2> 如果找不到TouchCalibrate.txt文件，会找\\Residentflash2\\app\\CalibraUi.bmp图片,如果有此图片，则校准界面
				 会显示此图片，这样可以实现客户订制此校准界面
				 <3> 如果找不到CalibraUi.bmp图片，则会进入系统默认的图片校准界面

				 参考文件TouchCalibrate.txt和CalibraUi.bmp见Tool目录下
				 (5)支持WIFI RT2780和RTL8192CU
				 (6)更新USB IPOD驱动
				 (7)支持newport的ATV芯片:ATV168,目前可以提供参考APP给客户开发使用
				 (8)更新蓝牙库,出现 servicesd.exe错误问题
				 (9)更新库YCAPIDLL.dll
				 (10) APP更新MSDK.DLL文件
				 (11)更新AUDIO驱动，解决了wma 播放切换AVIN之后 seek 会有噪声的问题，并优化了之前A2DP切换死机问题
				 (12)DTV power和reset脚支持可配置
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16206
Author: lansheng
Date: 2013年1月9日 16:46:38
Message:
			 1.68版
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16034
Author: lansheng
Date: 2013年1月2日 10:30:06
Message:
			 1.68版
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 15819
Author: lansheng
Date: 2012年12月21日 17:36:37
Message:
			 1.67版
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 15730
Author: lansheng
Date: 2012年12月19日 9:34:12
Message:
			 1.68首版CODE
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

