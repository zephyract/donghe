Revision: 29040
Author: lansheng
Date: 2014��4��17�� 11:04:37
Message:
(1)��SD0��SD1�̷�������ʱ���޷�ͨ���ļ�ϵͳ����APP������
(2)�����µ��ƽ�3360��ʽ

----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 28977
Author: lansheng
Date: 2014��4��15�� 10:29:29
Message:
(1)����BlueCastle.dll���Ѹ�����������޷��Ҷ�����
(2)������Ƶ:����Ѩλ.avi������¼��-���汣����.avi����˸����
(3)�������ʽģ����ӣ�ͨ��CVBS�ź�ǿ���ж�����̨����

----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 28736
Author: lansheng
Date: 2014��4��4�� 14:24:49
Message:

----
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360_Golden.rar

Revision: 28452
Author: lansheng
Date: 2014��3��27�� 10:14:25
Message:
(1) ���ӵ���������IC GT911��֧��(���յ�)
ʹ�÷�����
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
������IC GT911��Ӳ�����Ӿ�һ��I2C���ߣ��жϽţ��ϵ�ţ���λ�ţ��ͻ����Ը����Լ���Ӳ�������ò�ͬ��IO��
�ж�ֻ֧��EINT1(GPIO48),EINT2(GPIO49)
����������������������IC�����Ժ�������
(2)�͹�WD�ƶ�Ӳ�̣����ſ������������̷�����(����)
(3)�͹�SanDisk cruzer 4GB U����ǰUSB�ڲ���ʶ������(���յ�)

----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 28319
Author: lansheng
Date: 2014��3��21�� 10:56:41
Message:
(1)����͹�1TBϣ��Ӳ������ʶ������
(2)USB IPOD���ܣ�Iphone5,5s���ڸ�¥���绰�ҶϺ�,ipod����ʧЧ����
(3)ipod�һ�����������������
(4)����������ĺ��İ壬����֧���µ��������
��ͻ��з����õľɵĺ��İ���ֲ���������������ѯ��˾��Ŀ���񴰿�
(5)ͨ���ļ�ϵͳ����ʱ������YC002.img���ſ���ǰ25���ļ������������ļ����ſ���100���ļ�
(��ǰ��ֻ����ǰ��18���ļ�). 
(6)MC HotDog&������_�x�_(KTV).mpg���Ž�����APP hungס����
(7)����VideoPatterns_xvid.avi�󣬵���MediaGraph_GetMediaInfo�ᵼ���쳣����
----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 28020
Author: lansheng
Date: 2014��3��6�� 16:19:36
Message:
2014-03-06
(1)��������ʶ����������ѹ���(audin.lib,audio.lib,waveform2.lib,waveform.lib)
(2)ConFigFile_Tool.exe����LCD pclkƵ�ʺͼ�������ѡ��
(3)��������������bMirror����cvbsͨ��ѡ��
��4λ��ά��֮ǰ״̬,����ѡ��:
bMirror: 0 Ĭ��ֵ������������
			 1 ���Ҿ���
			 2 ���¾���
			 3 ���� & ���¾���
			 ��4λ��CVBSͨ��ѡ��
			 10-14�ֱ����CVBS0-CVBS4
			 (4)����DVD��������ĳ�ͻ��ֳۻ�о��ĳ��DVD-Video����������(�˵�Ƭ��¼�����ϲRFO�źŲ��Գ�)
			 (5)��������MT6622FW�������Ϊ7150�ֻ��������Ӻ���һ�������ڳ������޷��Ҷϵ�bug

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27868
Author: lansheng
Date: 2014��2��26�� 10:44:13
Message:
		 (1)���Ӷ�128MB nandflash: MX30LF1G08AA ��֧��
			 (2)SD�������ṩIOCTL_DISK_SET_SDCARD_ID_FIX�ӿڣ�SD��ID�ſ�����������
#define IOCTL_DISK_SET_SDCARD_ID_FIX  CTL_CODE(0x7, 0x501, METHOD_BUFFERED, FILE_ANY_ACCESS)
			 typedef struct _YG_SDCARDID {
				 BOOL   bEnable;			// �Ƿ�ʹ��
				 USHORT ManufacturerID;		// ������ID
				 DWORD  ProductSerialNumber;		// ��ƷID
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
		 (3)���Ӽ���оƬ��֧��
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27732
Author: lansheng
Date: 2014��2��18�� 14:37:14
Message:
		 (1)������IC:AU6465R��ȡSDID,������USBʱ���޷���ȡ����
			 (2)�������RXΪ�͵�ƽ���Ῠ������
			 (3)��԰����ǵķ�������:
		 MSDK.DLL������3���ӿڣ�(ͬ������APP�����SDK��)
			 MRESULT     AvinCtrl_OpenRDS(HMEDIAGRAPH hMediaGraph, E_MSDK_AVIN_AINDEX eAIndex);//����RDS����
		 MRESULT     AvinCtrl_CloseRDS(HMEDIAGRAPH hMediaGraph, E_MSDK_AVIN_AINDEX eAIndex);//�ر�RDS����
		 BOOL 	    GClientSetRDSVolume(GHANDLE hAudioDev,GUINT32 u4Vol);//����RDS����
		 �����ǹ��ܱ������ǽ�һ·Line-In��Waveformͨ����Ȼ����DSP����������ӣ�������ʹ�õ�ϵͳ��Դ����������
			 Դ���ڹ�������ʹ��ʱ��Ҫע�����¼��������
			 1.  ����SD/USB��DVPʱ����RDS
			 ��ʱ��������Դ��ͻ��ֻ���ڲ���RDS����ʱ������GClientSetVolumn��С��ǰ��ý�岥��������RDS�������Ž�����
			 �ָ���ý��Դ����������
			 2.  ����AUX-In�ź�ʱ����RDS
			 ������Դ��ͻ��Driver�ѶԴ�������д���RDS������ʼ����ʱ��ǿ��ֹͣ��ǰAXU-In�źŵĲ��ţ�����RDS��������
			 ����ҪAPP����ȥ�ָ�AXU-In�źŵĲ��š�
			 3.  �����������ֻ��绰ʱ����RDS
			 ������Դ��ͻ������������ʱ����RDS�����Ե���BZ_A2DPActive(FALSE)ֹͣ��Audioģ�����������������ݣ�RDS������
			 ����BZ_A2DPActive(TRUE)�ָ����ţ���������ͨ��ʱ����RDS�����Ե���BZ_SetAudioOutput(TRUE)�������е��ֻ��ˣ�RDS
			 ���������BZ_SetAudioOutput(FALSE)�ָ�ͨ����
			 (4)����LVDS 480X272����

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27562
Author: lansheng
Date: 2014��1��24�� 14:34:37
Message:
		 (1)DVP��ʼ������ʼ��2000��,CD�����Ų�������
			 (2)����ͨ��ʱ���ж�������(Enable PLC���ɿ�����)�޸����ù���ConFigFile_Tool.exe
			 (3)����USB IPODʱ��AVIN����ᶶ������(2013-2-28���Ĺ������⣬����7�·ݵİ汾�ֱ�����)����ȫ��display��
			 (4)Ѹ�������USB���ó��̷�����һ��ʱ����USB�̷���ΪNavi
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27274
Author: lansheng
Date: 2014��1��21�� 13:46:38
Message:
		 (1)���Ӷ�����IC:AU6465R��ȡSDID�Ľӿ�
			 (2)ͨ��APP�ȸ���һ��BMP logo��Ȼ���ٸ���һ��GIF logo��������ʱʱ���ָ���1������
			 (3)USB1.1������Ϣ���ƣ�USB1���Ŷ�����(����SD��),USB0����U�̣�������Ȼ��γ�����������EMI��Ϣ��������
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27135
Author: lansheng
Date: 2014��1��15�� 17:00:59
Message:
		 (1)SD���ٶ����ã��Ե��ٿ���Ч
			 ֮ǰ�����ù�����SD�����Clockֻ�Ը��ٿ���Ч�����ٿ��ǹ̶���20MHZ��
			 Ŀǰ�����ٿ���������֮ǰ�ģ������ٿ�ʱ��������ù���������Clock<20MHZʱ����ô�����õ�clock��ʹ��,������20MHZ
			 ��Ϊ����Ϊ�˼���SD�����ָ��ţ�����EMI�����⣬�ͻ���Ѵ�clock���ıȽϵͣ�����Ŀǰ���ٿ��͵��ٿ������ǽ�ȥ

			 (2)���ù��߸��ģ�����TFTѡ�Ŀ�ģ�
			 ֮ǰ���ó�LVDS��ʱ��TFTͬʱҲ������ģ���TFT�����clock����Щ�ͻ����������ŵ�GPS���ǣ�����Ŀǰ���ó�LVDS��ʱ��Ĭ�ϰ�TFT�ص�
			 ����֮ǰҲ�пͻ��ǽ���������һ��LVDS��һ��TFT������Ϊ�˼��ݴ�����������ù���������TFTѡ�ѡ��TFT�������ó�LVDS��ʱ��TFTҲ���������

			 (3)1024x600��AVIN�޷�������ʾ������

			 (4)RNDIS,֧�����ǣ�С�׵�andriod�ֻ�
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 27084
Author: lansheng
Date: 2014��1��14�� 11:00:28
Message:
		 (1)Wma ID3�޷���ȡ������
			 (2)֧��RNDIS,�ֻ��복����USB���ӣ������˿���ʵ���������ܣ����ܴ����ֻ������Ե����⣩
			 (3)usb ipod������κ��޷���ʼ������(ipod���lib��)
			 (4)naviking��ͼ����PNGӲ���޷�������������(imgdec_png.lib)
			 (5)���һ���ͻ�16�ֽ�ID�Ķ���д�Ľӿ� (�Ǻ�����)
			 (6)USB IPOD���Ƶ�׻�ȡ
			 (7)�Ա�ɽ_��ΰ_����.wma�޷���������
			 (8)��line_in����¼������·�������ͨ��ʱ������ͨ����������ȼ�ռ��Audio��Դ��֤��������

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 26815
Author: lansheng
Date: 2014��1��3�� 14:36:09
Message:
		 (1)����SPI������SPI0��SPI1������ʹ��
			 (2)USB1.1������Ϣ���ƣ����Ŷ�����(����SD��)������Ȼ��γ�����������EMI��Ϣ��������
			 (3)SMSC-USB2244������֧��IOCTL_DISK_GET_STORAGEID�ӿ�

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 26707
Author: lansheng
Date: 2013��12��30�� 18:40:28
Message:
		 1.68�汾��
			 (1)������PBAP���ص绰��������PullPhoneBook�ķ�ʽ����BZ_Init�����BZ_PBMGRSetPBAPMethod(TRUE)����
			 (2)�����ַ�����.avi
			 (3)����800X600 lvds����,ConFigFile_Tool.exeͬ������
			 (4)���������ȡ��������:
		 HANDLE hHookEvent = CreateEvent(NULL,FALSE,FALSE,L"GetTouchEvent");
		 SetEventData(hHookEvent,0x89);ʱ��ֻ��main���ڷ���Ϣ��ϵͳ����Ӧ������Ϣ
			 ֮ǰ��SetEventData(hHookEvent,0x1);��Ȼ��������eventdata��1ʱ��main���ں�ϵͳ������Ӧ������Ϣ
			 (5)USB1.1����Ϣ�汾����ʹ�ö�����ʱ���ڵ�����
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 26428
Author: lansheng
Date: 2013��12��19�� 18:28:29
Message:
		 1.68�汾��
			 (1)���ӿ��������켣����(ѡ����ļ�backcar_Track.mrf)
			 (2)������յ��ṩ��һ��KLD��ͼ������������
			 (3)֧������SPI��,Createfile(L"SPI0:"...),CreateFile(L"SPI1:"....)
			 (4)����DVD���,����OGG�ļ�,֧��ID��Ƭ�Ļ�ȡ

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 26243
Author: lansheng
Date: 2013��12��13�� 10:52:16
Message:
		 1.68�汾��
			 (1)EMI������Ϣ���ƣ������γ�USB����ʱ��Ҳ�ᷢ�����ŵ���Ϣ���ǰγ���Ϣ
			 (2)��������(δ���򿪱���ָ��)����£�����ָ�������һ���������ȵĲ���������ͻ�MCU�������ñ�������
			 ֮ǰ��Ĭ��������ʾ
			 (3)�޸Ķ�����̣����opera�������������ʱ����ʾ������ͬ��ĸ����
			 (4)�޸�I2C�⣬���ż��SCA��SDL��ʱ����ֵ͵�ƽ������
			 (5)�޸����������߼���������������ǰ���ƣ������˸һ�£������������ƣ���Ʋ������м�����ʧ�ܣ�����ƣ������ɹ���OS���������У�������(��Ҫ���ڲ��������ܿ�)
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 25706
Author: lansheng
Date: 2013��11��27�� 11:39:47
Message:
		 1.68�汾��
			 (1)���ݵ���128MB DDR��֧��
			 (2)�͹�ape��Ƶ�ļ���ID3��ʾ����(�����OS��APP��msdk.dll)
			 (3)��MTKͬ��ϵͳ�����������
			 (4)��Ƶ���벿�ָ���MTK��Բ�ͬоƬH.264�����Ĳ���
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 25610
Author: lansheng
Date: 2013��11��22�� 9:16:53
Message:

		 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/MT3360.pbxml

Revision: 25586
Author: lansheng
Date: 2013��11��21�� 14:46:36
Message:
		 1.68�汾��
			 (1)�����µ�ͼ��Щ������Ϊ��ɫ���ɱ�Ե�����б�Ƴ���
			 (2)2012-10-29���.Net�ؼ�������ʾ�쳣����(2D�����㷨����)���bitbltˢ��λͼ��������

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 25534
Author: lansheng
Date: 2013��11��19�� 19:30:18
Message:
		 1.68�汾��
			 (1)����̩�ϵ�USB CMMBģ�����������������ҵ���������
			 (2)USB EMI��������֮ǰPORT1�޷�����FS_DAEMON_MOUNT��Ϣ����
			 (3)���������Ӧʱ��USB2.0��ת��USB1.1��ʱ�򣬷����¼���
Port1:L"USB1HighSpeedToFullSpeed"
Port0:L"USB0HighSpeedToFullSpeed"
	  (4)��ʱ��move���������ӣ����Զ�up������
	  (5)���USB WIFI��3Gģ�����γ��¼�֪ͨL"Usb3GDeviceEvent",L"UsbWifiDeviceEvent"
	  (6)������������֧�ֶ���̣�֮ǰֻ֧��ע��һ�����̡�

	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24882
Author: lansheng
Date: 2013��11��6�� 10:39:04
Message:
		 1.68�汾��
			 (1)����target_NAND_1200XH_DL31.bin�����DL31�������е�������������
			 (2)800x600�ֱ���ƽ̨������CVBSʱ����MediaGraph_SetScale()�޷�ͬʱ������ü�������(�����msdk.dll)
			 (3)�Ż�480x272�ֱ���ƽ̨�����(��Ƶ����Ч����CVBS-IN�ź�Ч��)

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24667
Author: lansheng
Date: 2013��10��29�� 15:51:24
Message:
		 1.68�汾��
			 (1)���Ӷ��ִ�512MB SLC FLASH��H27U4G8F2DTR��֧��
			 (2).Net�ؼ�������ʾ�쳣����(2D�����㷨����)
			 (3)����"����.mp4"�л���"����֮��.rmvb"���ţ���һ�����ʶ����������ݡ�
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24639
Author: lansheng
Date: 2013��10��28�� 10:04:26
Message:
		 1.68�汾��
			 (1)����480X272�ֱ��ʵ�֧��
			 (2)���Ӷ�����512MB SLC FLASH: K9F4G08u0D��֧��
			 (3)������ſ͹���Ƶ:��չ6.flv��Ƶ��ס�����겻����һ������(����Ƶ�ļ����ڴ����������ݣ��м������ӱ�־λ
			 �ж���Audio����)
			 (4)���������USB WIFIģ��
			 (5)����simcom��3Gģ��

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24459
Author: lansheng
Date: 2013��10��15�� 11:04:22
Message:
		 1.68�汾��
			 (1)���ĳ�ͻ�һ��GIF��LOGO��ѭ�����ػ������л�����ͣ�ڿ���LOGO����
			 (2)SD1�ӿ�֧��ͨ���ļ�ϵͳ�Ŀ�����OS��APP(��Щ�ͻ��Ļ���ֻ��SD1�ӿڣ����ַ�ʽ�ṩ���������֧��)
			 (3)Ӳ������DAC��ϵͳĬ�����ó�DAC��APP���ò���DAC��RGB�л���ʱ�������������
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24341
Author: lansheng
Date: 2013��10��9�� 14:01:20
Message:
		 1.68�汾��
			 (1) ������Ƶ�ļ�������ʮ��_1080P.mov��������
			 (2) ĳ������FLASH���в�д�ɹ�������������д�����⣬��Ѹ�����⣩
			 (3) USB1���ó�USB1.1��USB0���ó�����Ӧʱ��USB1��������,����USB2.0
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 24212
Author: lansheng
Date: 2013��9��27�� 10:52:23
Message:
		 1.68�汾��
			 (1)����AUDIO������֧��SPDIF (��Ҫ��Ӧ����DVD�����APP SDK)
			 ֮ǰ��SPDIF Driver֧�ֽ�USB/SD downmix���ΪPCM��
			 ���ڵ�SPDIF Driver�Ѿ�֧�ֽ�DVD��USB/SD��Line in downmixΪPCM����Raw data��Raw fata��֧��AC3��DTS���롣
			 GSysServices.h��ӽӿ�GSetSpdifOutputType��DVP_GSetSpdifOutputType�ֱ�USB/SD��DVP�������Ƶ�ź�ͨ��SPDIF�ӿ������
			 SPDIFOUTPUT_T�ṹ��ȷ������PCM����Raw data��ʽ�����Raw Data��֧��AC3��DTS�������ƵԴ��

			 (2)����USB����������붥������һ��U�̶���������
			 (3)����BOOT�������Ϊɨ�軵����ɺ��İ��������ɹ������⡣(eboot.nb0,sdagent.nb0��Ҫ����)
			 (4)���Ӽ��SD��GPSUPDATE.PACK�ļ���ʱ����
			 (5)EBOOT�м���ļ����ݳ�ʱ��16�ĳ�18

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 23925
Author: lansheng
Date: 2013��9��13�� 14:13:06
Message:
		 1.68�汾��
			 (1)USB�������£�֧��USB1.1,USB2.0,����Ӧ�������л�
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 23800
Author: lansheng
Date: 2013��9��9�� 9:55:28
Message:
		 1.68�汾��
			 (1)����USB1.1��������Ļ������
			 ���һЩ���ͣ������ִ���ϵ������K5��BYD F3�����ڰ����Ȼ򿪴�ƣ�U���̷���ж�ص�����
			 ���������һ�ִ���(APP������޸�)����Ȼ�̷����ֶ��ݵ�ж�أ��������û����ܲ�����
			 (2)��LOGOΪBMPͼƬ��ʱ��֧�ֽ�������ʾ�����������ͻ�����ͨ��PndFlashtool1.6.8\Tool\Rcbuild��������
			 ��bitmap.rc,
			 6            150        395        Bitmap                 Slider.bmp
			 ID��Ϊ6�����������ͼƬ��150,395Ϊ����������ʼ���꣬������ó�0,0����֧�ֽ���������
			 �ͻ������Լ��������滻Slider.bmp���޸���ʼ���꼴��
			 (3)��ϵͳ���ó��ⲿDAC����ʱ,APP����PWM������Ч����
			 (4) 6��25���Ժ�汾��CMMB�һ���������
			 (5) ����DVD���ݵ�ID3��Ϣ����codepage�Ľӿ�,����֧�֣�APP�ṩ�ӿ�

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 23428
Author: lansheng
Date: 2013��8��23�� 13:58:16
Message:
		 1.68�汾��
			 (1)GPS�˿�������18��19
			 (2)16K ˥�����������
			 (3)ipod ���data abort����
			 (4)BSP������MT3332��MT3336��֧��
			 (5)����fastcamera.cpp��FSC_IOControl���ӻ��⣬����ܵͻ����Ե�ͣ�ڿ���LOGO����
			 (6)�޸�SD����ⳬʱ����,3�θ�Ϊ1��,ʱ���3.5S��Ϊ8S
			 (7)�����Զ�����Residentflash2\App\YGAutorun.ini��·������Ļ���
			 (8)YCAPI.DLL���ӻ�ȡSD��ͨѶģʽ�Ľӿ�
			 (9)YCAPI.DLL�����޸���һ�»�ȡоƬ���͵Ľӿ�
			 (10)����ɭ��USB HUB����������,�ṩһ��:����ɭ��_ConFigFile_Tool.exe
			 (11)����1200XH�����о����汾target_NAND_1200XH_XW200.bin
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 22844
Author: lansheng
Date: 2013��8��12�� 10:34:47
Message:
		 1.68�汾��
			 (1)����GPS�˿����ã�ConFigFile_Tool.exe�޸�
			 (2)����target_NAND_1200XH_DL31.bin,���DL31��о��������
			 (3)ͨ���ļ�ϵͳ����metazone������ϵͳ����һ�β�����У����������

			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 22778
Author: lansheng
Date: 2013��8��8�� 19:58:21
Message:
		 1.68�汾��
			 (1)���ݵ���256MB DDR3
			 (2)800X600�ֱ��ʵ�ʱ��ͨ��APP��������800X600��BMP��ʽLOGO����(�붥)
			 (3)����SD���������Ѹ�صİ汾��SD������timeoutʱ����3.5M��Ϊ20S
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 22380
Author: lansheng
Date: 2013��8��1�� 8:52:06
Message:
		 1.68�汾
			 (1)����ĳ̨��Ƽ�32G U����Ƶ�ļ��Ῠ������(��U�̼���������)
			 (2)�޸�Launch->AppWriter.cpp,APP����ʱ֧��>32MB�ĵ��ļ�����
			 (3)����PCB���ܷ�ʽ
			 (4)����SD0��SD1�̷����ƻ��������ã�ConFigFile_Tool.exe����
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 21874
Author: lansheng
Date: 2013��7��16�� 18:49:51
Message:
		 1.68�汾
			 (1)Ѹ��U�����ָ��£����ܲ�USB0����USB1����һ������̷����ֽ�USBDisk
			 (2)����"001��ʤ��-Ե.flac"��MP3֮���л���ʱ���������⣬����aud_adsp.lib��
			 (3)���������⣬֧������ͬʱ�������豸��һ���̶���ĳ�������豸��SPP profile��һ����������HF����
			 ���ӽӿ�BZ_SPPCreateConnectByBDAddr(REMOTE_DEVICE_SPP_ADDR); ����֧��,app��BlueZMW.DLL��ͬ������
��Ҫ��;: ��������,��������OBD�����豸,��ȡһЩ��Ϣ,ͬʱ�ֲ�Ӱ�����������ֻ�HF����
	  (4)Metazone���߸��£�֧������USB�ӿھ����ó�USB1.1�ٶ�
	  (5)����CMMB(���CVBSʽ)������������Ͽ���������,����msdkcore.dll, tvd_ce_al.lib
	  (6)ͨ���ļ�ϵͳ��������backcar.mrf�ļ�

	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 21489
Author: lansheng
Date: 2013��7��5�� 9:43:12
Message:
		 1.68�汾
			 (1)3G��������			���붥���⣩
			 (2)�����ȥ����������塱����	���붥���⣩
			 (3)������BlueCastle.dll���£����GT-S3710�ֻ�ͨ���쳣
			 (4)����WAVEע���,֮ǰ˵��MIC�����һ��ֵ��0x6e�Ļ�0x68
			 ��������ͻ�Ҫ����audio��MIC����,����ͨ������,���봦��,������ͨ��APP�ӿ�ʵ��
��ο��ĵ�:Waveform Introduction(For Yecon).pdf
	  (5)�޸�HW_MSDK.dll, IPOD���⣺IPOD�����б��ʱ����Ҫ��ר��ͼƬ�������Һ͸���ʱ���Լ���������ʾ����
	  (6)���¿�display_tcon.lib,tcon.h,֧��pclk���Կ�����
	  (7)��flashtool��������APPʱ��APP�ļ�����������ʱ��ʾ�������⣨Ѹ�����⣩
	  (8)����USBת��������,IC�ͺ�:FL232RL	��������⣩
	  (9)����spi_mt3360_v1.lib�����APP�����޷�����SPI�����⣨������⣩
	  (10)����flashtool��appwriter�����0kb�ļ����ܴ�����⣨Ѹ�����⣩
	  (11)������ʾ���������AVINͼ��������Ӱ����
	  ��һ��Ķ��ϴ󣬻�����overlay��ʾ��һЩbug�����������ͻ����һЩ�������Ͻ���������APP�п�����Ҫ����޸�
	  �ͻ���overlay��ͬ��ˢ�¿������Ⱥ�֮��
	  (12)����ĳ����Ƶ"��ɽ.avi"��ִ�жϵ㲥�Ź���MediaCtrl_LastMemStart�Ứ�� ���⣨�������⣩
	  (13)���ISDBT�޷�����¼���ļ�����	��������⣩
	  (14)���SD��1bitģʽʱ����Щ���忨�����data abort����
	  (15)����USB�������������465G�ƶ�Ӳ�̲�ʶ��&SD�������ٶ��������� ���������⣩
	  (16)ΪѸ������SYSINFO.TXT�ļ���windowsĿ¼	��Ѹ�����⣩
	  (17)Metazone���ù��߸��£�����video memory size����(���Ʒ�Χ8MB-30MB)�����յ����⣩
	  (18)UFT8��������ʾ��������		���붥���⣩
	  (19)����DVD��WMV��DIVX��WMA���뿪�ſ���
	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 21335
Author: lansheng
Date: 2013��7��2�� 17:28:04
Message:
		 1.68�汾
			 (1)3G��������
			 (2)�����ȥ����������塱����
			 (3)������BlueCastle.dll���£����GT-S3710�ֻ�ͨ���쳣
			 (4)����WAVEע���,֮ǰ˵��MIC�����һ��ֵ��0x6e�Ļ�0x68
			 ��������ͻ�Ҫ����audio��MIC����,����ͨ������,���봦��,������ͨ��APP�ӿ�ʵ��
��ο��ĵ�:Waveform Introduction(For Yecon).pdf
	  (5)�޸�HW_MSDK.dll, IPOD���⣺IPOD�����б��ʱ����Ҫ��ר��ͼƬ�������Һ͸���ʱ���Լ���������ʾ����
	  (6)���¿�display_tcon.lib,tcon.h,֧��pclk���Կ�����
	  (7)��flashtool��������APPʱ��APP�ļ�����������ʱ��ʾ��������
	  (8)����USBת��������,IC�ͺ�:FL232RL
	  (9)����spi_mt3360_v1.lib�����APP�����޷�����SPI������
	  (10)����flashtool��appwriter�����0kb�ļ����ܴ������
	  (11)������ʾ���������AVINͼ��������Ӱ����
	  ��һ��Ķ��ϴ󣬻�����overlay��ʾ��һЩbug�����������ͻ����һЩ�������Ͻ���������APP�п�����Ҫ����޸�
	  �ͻ���overlay��ͬ��ˢ�¿������Ⱥ�֮��
	  (12)����ĳ����Ƶ"��ɽ.avi"��ִ�жϵ㲥�Ź���MediaCtrl_LastMemStart�Ứ�� ����
	  (13)���ISDBT�޷�����¼���ļ�����
	  (14)���SD��1bitģʽʱ����Щ���忨�����data abort����

	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 21287
Author: lansheng
Date: 2013��7��1�� 15:17:38
Message:
		 1.68�汾
			 (1)3G��������
			 (2)�����ȥ����������塱����
			 (3)������BlueCastle.dll���£����GT-S3710�ֻ�ͨ���쳣
			 (4)����WAVEע���,֮ǰ˵��MIC�����һ��ֵ��0x6e�Ļ�0x68
			 ��������ͻ�Ҫ����audio��MIC����,����ͨ������,���봦��,������ͨ��APP�ӿ�ʵ��
��ο��ĵ�:Waveform Introduction(For Yecon).pdf
	  (5)�޸�HW_MSDK.dll, IPOD���⣺IPOD�����б��ʱ����Ҫ��ר��ͼƬ�������Һ͸���ʱ���Լ���������ʾ����
	  (6)���¿�display_tcon.lib,tcon.h,֧��pclk���Կ�����
	  (7)��flashtool��������APPʱ��APP�ļ�����������ʱ��ʾ��������
	  (8)����USBת��������,IC�ͺ�:FL232RL
	  (9)����spi_mt3360_v1.lib�����APP�����޷�����SPI������
	  (10)����flashtool��appwriter�����0kb�ļ����ܴ������
	  (11)������ʾ���������AVINͼ��������Ӱ����
	  (12)����ĳ����Ƶ"��ɽ.avi"��ִ�жϵ㲥�Ź���MediaCtrl_LastMemStart�Ứ�� ����
	  (13)���ISDBT�޷�����¼���ļ�����
	  ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 20868
Author: lansheng
Date: 2013��6��14�� 11:40:13
Message:
		 1.68�汾
			 (1)eboot֧��SD��1bitģʽ
			 (2)UART6������˿ںŴӣ���Ϊ��,Ϊ�˼��ݹ����һЩ��ͼ�������������
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 20633
Author: lansheng
Date: 2013��6��8�� 10:45:53
Message:
		 1.68�汾
			 (1)���¿�DivxParserFilter.dll,msdkcore.dll ���ӻ�ȡ��Ƶ�����Ľӿ�
APPͬ�����������ļ�:
		 MSDK.DLL,MSDK.LIB,MSDK_Defs.h,MSDK_MediaGraph.h
			 (2)����i2c_mt3360_v1.lib,֧��ADV7180����
			 (3)���¿�tvd_ce_al.lib��tvd_hal_lib.lib��tvd_lib.lib��֧��APP��camera inһЩ������������
			 ���Լ���ATV 320x240ͼ���Լ�ADV7180 720x480ͼ��
			 (4)������ƵĬ�����PWM����I2S�źŵ����ã���Ĭ��ʹ��ʹ��DAC�����ù���ConFigFile_Tool.exeͬʱ�޸�
			 (5)������SDͨѶģʽ1bit or 4bit mode�����ã����ù���ConFigFile_Tool.exeͬʱ�޸�
			 (6)3G��������,�����Ը���Щ
			 (7)���������Ż�������0����ʾ
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 20354
Author: lansheng
Date: 2013��5��31�� 11:17:28
Message:
		 1.68�汾
			 (1)MT3353֧��cvbs out���
			 (2)֧�ֿ��������״���ʾ

			 �˰�BSPδ���µ������(1)���display������
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 19881
Author: lansheng
Date: 2013��5��23�� 9:52:45
Message:
		 1.68�汾
			 (1)�޸�ע���custom.reg,��������ƽ�������ܣ�ClearType
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 19841
Author: lansheng
Date: 2013��5��22�� 11:07:38
Message:

		 ----
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/Aribic/MT3360.pbxml

Revision: 19840
Author: lansheng
Date: 2013��5��22�� 11:06:53
Message:

		 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/MT3360.pbxml

Revision: 19837
Author: lansheng
Date: 2013��5��22�� 10:59:19
Message:
		 1.68�汾
			 (1)����I2C����i2c_mt3360_v1.lib,i2c.h,�����붥��ADV7180оƬͨѶ���� camera�ӿ�
			 (2)���¿�Mhcdmdd4hub.lib���޸�USB1 WIFI�������ȶ�����
			 (3)���¿�display_gpelib.lib,ϵͳ����mouse��������������ι��ͼ�ꡣ�����ͻ����Ե���GetCursorPos�ȹ����ؽӿ�
			 (4)����DVD���������붥��������repeat off��ʱ�� ��һ�����ٵ��ʱ��������Ӧ�����⣬���θ��µİ汾���� 0113212(5)1
			 (5)���¿�com_mdd2_1.lib���������ӡ��Ϣuart drop����index��ʶ�����ڴ��ڴ�ӡ����LOGʱ�����ж����ĸ�����
			 (6)����������BlueCastle.dll,Blueman.dll �����и��ͻ�������SPPģ���豸
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 19419
Author: lansheng
Date: 2013��5��9�� 13:57:08
Message:
		 1.68�汾
			 (1)����LOGO�����λ������(���õ���mtk_dvp_load�ķ�ʽ)
			 (2)MSDK��������codepage�Ľӿ�
			 (3)����USB��λ�Ľӿ�:
#define IOCTL_HAL_USB_HOST_RESET CTL_CODE(FILE_DEVICE_HAL, 62, METHOD_BUFFERED, FILE_ANY_ACCESS)
		 CreateFile(L"HCD0",��); //For usb port0
		 CreateFile(L"HCD1",��); //For usb port1
		 (4)MT3353B֧�ֵ���
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 19050
Author: lansheng
Date: 2013��4��27�� 9:14:40
Message:

		 ----
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/MT3360.pbxml
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/Aribic
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/CalibrUi.cpp
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/CalibrUi.pbpxml
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/CalibrUi.reg
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/CalibrUi_clone.rc
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/GetCaText.cpp
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/GetCaText.h
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/Readme.txt
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/TouchCalibrate.txt
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/calibrrc.h
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/makefile
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/makefile.inc
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/postlink.bat
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/prelink.bat
Added : /PND/Software/OS/WinCE6.0/BSP/MT3360/�����ļ�/CH/CalibrUi/sources

Revision: 18880
Author: lansheng
Date: 2013��4��24�� 10:52:11
Message:
		 1.68�汾
			 (1)Anycall GT-S3710 ���ص绰�����������,������Ӣ����������⣬�����������ڲ����޸�����
			 (2)��ͼ��ȡUSB IPod��һЩ��Ϣ��ӡ Not Support�������������
			 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 18703
Author: lansheng
Date: 2013��4��19�� 15:54:39
Message:
		 1.68�汾
			 (1)��MTK����Ҫ�����FLASH����
�ӿ���logo��ȷ��FMD�汾����: 2013041200��mtkflash�汾���ǣ�2013011701
				   (2)����AUDIO������֧����һ�������µ�ץ���ݷ�ʽ
				   ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 18325
Author: lansheng
Date: 2013��4��11�� 16:48:14
Message:
		 1.68�汾
			 (1)����������
			 �ṩ�ж϶Է��ֻ��Ƿ�֧��ID3�Ľӿ�
			 �޸��˶Ͽ���������������֮���ղ���ID3ʱ�䲻�ٸ��µ�©��	SPP���ֵĸ��£����ӵ�ʱ���֪ͨ�ϲ����ӵĴ��ڣ����ҶϿ���ʱ���֪ͨ�Է��Ͽ�����	
			 SPP �ϲ�APP ��ȡ���ںŵ�ʾ�����룺
			 if (BZ_IND_SPP_CONNECTED == dwIndID)
			 {
				 if (lParam == sizeof(SPPVCOMINFO_T))
				 {
					 P_SPPVCOMINFO_T info = (P_SPPVCOMINFO_T)wParam;
				 }
			 }

			 (2)����DVD FW
				 (3)���붥Ҫ���붥ϵͳֻ�ܰ��붥��������APP
				 ����ͬ����Ҫ����metazone���ù��߸��붥

				   ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 18047
Author: lansheng
Date: 2013��4��2�� 15:40:18
Message:
			 1.68�汾
				 (1)�����붥��������YC_BackLightEnable(0)���ٵ�YC_BackLightEnable(1)����һ�°�������
				 (2)�Ź������Ĳ�����Ƶ��ʱ�򲥷Žϴ�GIF��DBGCHK FAIL�����⣬���¿�imgdec_gif.lib
				 (3)��һ��256MB FLASH������д���⣬����FLASH��
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17999
Author: lansheng
Date: 2013��4��1�� 15:50:48
Message:
			 1.67�汾
				 (1)����MSDKCORE.DLL,����붥�ṩ��һ����Ƶ˵�Ῠ�ܾ�����
				 (2)֧��MT3353B
				 (3)����YCAPIDLL,KNCG֧�֣�D����
				 (4)֧��DDR512MB�ڴ�
				 (5)�����붥��������YC_BackLightEnable(0)���ٵ�YC_BackLightEnable(1)����һ�°�������
				 (6)�Ź������Ĳ�����Ƶ��ʱ�򲥷Žϴ�GIF��DBGCHK FAIL�����⣬���¿�imgdec_gif.lib
				 (7)��һ��256MB FLASH������д���⣬����FLASH��
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17789
Author: lansheng
Date: 2013��3��25�� 16:10:35
Message:
			 1.68�汾
				 (1)����MSDKCORE.DLL,����붥�ṩ��һ����Ƶ˵�Ῠ�ܾ�����
				 (2)֧��MT3353B
				 (3)����YCAPIDLL,KNCG֧�֣�D����
				 (4)֧��DDR512MB�ڴ�
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17684
Author: lansheng
Date: 2013��3��21�� 17:24:22
Message:
			 1.67�汾
				 (1)CMMB��Ϊֻ֧��CMMB05(SM2180)ģ��
				 (2)���USB1.1������U�̸�����Ƶʱ����findfirstfile�Ῠס����
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17681
Author: lansheng
Date: 2013��3��21�� 17:15:29
Message:
			 1.68�汾
				 (1)CMMB��Ϊֻ֧��CMMB05(SM2180)ģ��
				 (2)���USB1.1������U�̸�����Ƶʱ����findfirstfile�Ῠס����

				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17585
Author: lansheng
Date: 2013��3��19�� 11:57:26
Message:
			 1.67�汾
				 (1) �����storage card�ĳ�inandʱ���״β���ʽ��inand����
				 (2) �Ż����񵼺�DTMF����
				 (3) ��WMV��չ����ΪWMA���Խ�����Ⲣ�ղ���Finish Event ������
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17426
Author: lansheng
Date: 2013��3��13�� 9:50:22
Message:
			 1.67�汾
				 (1)����TOUCH,����붥������DOWN��UP�����⡣
				 (2)���Ļ��귴����Ƶ�ļ�93370455.h264_1.MP4 seek��ͣ������
				 (3)USB IPOD֧����Ƶ

				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17217
Author: lansheng
Date: 2013��3��4�� 15:21:10
Message:
			 1.68�汾
				 ͬ����ǰ��.�����汾����
				 �����������붥WIFI������������WIFI��������
				 ������1.68�汾֧��ISDBT05(2130)ģ��
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17176
Author: lansheng
Date: 2013��3��1�� 10:13:28
Message:
			 1.67�汾
				 Ϊ����֧��BICGоƬ��׼�������Կ�����BICG֧��H.264Ӳ����
				 ��Ĭ��BICGоƬ�ǲ�֧��H.264�ģ�
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17157
Author: lansheng
Date: 2013��2��28�� 11:54:13
Message:
			 1.67�汾
				 (1)����TOUCH,����붥������ΪӲ����������Ʈ���⡣
				 (2)����3G ע���
				 (3)����display_pmx.lib�⣬���AVIN ��IPod һ�𲥷ţ�AVIN���Ъ�Զ���һ������
				 (4)���������⣬BlueCastle.dll��Blueman.dll��BlueZMW.dll
				 ����������ѯ�ֻ��Ƿ�֧�ִ������ֻ������Ľӿ�

				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 17017
Author: lansheng
Date: 2013��2��16�� 11:59:19
Message:
			 1.67�汾
				 (1)�ӿ�APP����LOGO�ٶ�(����һ��LOGO��֮ǰ��1.5S��ߵ�0.6S)
				 (2)���WYD������720P�ļ�����seek��1:33:42ʱ������������seek��ȥҲû����������
				 (3)���AVIN ��IPod һ�𲥷ţ�AVIN���Ъ�Զ���һ��
				 (4)֧������ͨ��ʱ��ȡPCM��Ƶ���ݣ�����һ��������Ҫ�˹��ܣ�
				 (4)����DVD FW
				 DVP  FW ���µ����°� 
				 ����Servo�汾��ȷ��
				 1200W +DL30    3f0D0060
				 1200XH+DL30    9b090060
				 HD860 +DL30    83070060
				 1200XH+DL31    e3010060
				 (5)��MTKͬ��BSP��������APP������ָ�DLL:DVPAgent.dll��MSDK.dll
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16810
Author: lansheng
Date: 2013��1��29�� 11:00:50
Message:
			 1.67�汾
				 (1)����ע�������GPS��������������
GPS: COM9->COM10
����: COM3->COM11
	�����ļ���platform.reg
	(2)���ӻ����ṩ�Ļ�Ϊ3Gģ��֧��
	�����ļ���platform.bib,platform.reg,MT3360\FILESĿ¼����3G_Huawei�ļ���
	(3)16Bit�Դ��6MB����10MB
	----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16776
Author: lansheng
Date: 2013��1��27�� 10:56:19
Message:
			 1.67�汾
				 ���´���IPOD���BUG��֮ǰû�аѼ�����Ϊ����š�
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16728
Author: lansheng
Date: 2013��1��25�� 14:29:13
Message:
			 1.67�汾
				 (1)����display_gpelib.lib,���WYD�����Ĳ�����Ƶ"����.mp4"�ٲ�"����֮��.rmvb"��ͼ���������
				 (2)���´���IPOD������,�״ε��ýӿڣ�������Ϣ��ֱ�Ӹ�����ֵ��
				 (3)����HW_MSDK.DLL�����USB IPODǰ���ŵ�һЩ����
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16533
Author: lansheng
Date: 2013��1��21�� 15:41:21
Message:
			 1.67�汾
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16311
Author: lansheng
Date: 2013��1��11�� 15:58:49
Message:
			 1.67 DVD IN NOR�汾��
				 2012-01-11
				 (1)�ر���������cfa log��־�ļ��Ĺ���
				 (2)TFT��LVDS�����ʵ��ͳһ(֮ǰSDAgent_24bit.nb0�Ƿְ汾��),�����ù�������LCD���
				 (3)���ù���ConFigFile_Tool.exe����,LCD֧��:LVDS 1024X600 
				 (4)Ϊ�����㲻ͬ�ͻ��Դ���У׼����ĸ��ģ�����ͬʱ����800x480��1024x600����������У׼���������ģ�
				 Ŀǰ֧������ָʾ�ͺ�ͼƬָʾ�ͣ�
				 <1> ����ָʾ�ͣ�����У׼����ʱ�����Ȼ���\\Residentflash2\\app\\TouchCalibrate.txt�ļ������д��ļ�
				 ���������ָʾ��У׼����
				 <2> ����Ҳ���TouchCalibrate.txt�ļ�������\\Residentflash2\\app\\CalibraUi.bmpͼƬ,����д�ͼƬ����У׼����
				 ����ʾ��ͼƬ����������ʵ�ֿͻ����ƴ�У׼����
				 <3> ����Ҳ���CalibraUi.bmpͼƬ��������ϵͳĬ�ϵ�ͼƬУ׼����

				 �ο��ļ�TouchCalibrate.txt��CalibraUi.bmp��ToolĿ¼��
				 (5)֧��WIFI RT2780��RTL8192CU
				 (6)����USB IPOD����
				 (7)֧��newport��ATVоƬ:ATV168,Ŀǰ�����ṩ�ο�APP���ͻ�����ʹ��
				 (8)����������,���� servicesd.exe��������
				 (9)���¿�YCAPIDLL.dll
				 (10) APP����MSDK.DLL�ļ�
				 (11)����AUDIO�����������wma �����л�AVIN֮�� seek �������������⣬���Ż���֮ǰA2DP�л���������
				 (12)DTV power��reset��֧�ֿ�����
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16206
Author: lansheng
Date: 2013��1��9�� 16:46:38
Message:
			 1.68��
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 16034
Author: lansheng
Date: 2013��1��2�� 10:30:06
Message:
			 1.68��
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 15819
Author: lansheng
Date: 2012��12��21�� 17:36:37
Message:
			 1.67��
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

Revision: 15730
Author: lansheng
Date: 2012��12��19�� 9:34:12
Message:
			 1.68�װ�CODE
				 ----
Modified : /PND/Software/OS/WinCE6.0/BSP/MT3360/MT3360.rar

