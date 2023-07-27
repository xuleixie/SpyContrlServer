//FILE: icsneo40DLLAPI.cpp
#include "icsneo40DLLAPI.h"

//Basic Functions
FINDNEODEVICES icsneoFindNeoDevices;
OPENNEODEVICE icsneoOpenNeoDevice;
CLOSEPORT icsneoClosePort;
FREEOBJECT icsneoFreeObject;
OPENPORTEX icsneoOpenPortEx;

//Message Functions
GETMESSAGES icsneoGetMessages;
TXMESSAGES icsneoTxMessages;
WAITFORRXMSGS icsneoWaitForRxMessagesWithTimeOut;
GETTSFORMSG icsneoGetTimeStampForMsg;
ENABLERXQUEUE icsneoEnableNetworkRXQueue;
GETISO15765STATUS icsneoGetISO15765Status;
SETISO15765RXPARMS icsneoSetISO15765RxParameters;
ENABLENETWORKCOM icsneoEnableNetworkCom;
DOWNLOADISO15765_2_TXSCRIPT icsneoDownloadISO15765_2_TXScript;
CLEARISO15765_2_TXSCRIPT icsneoClearISO15765_2_TxScript;


//Device Functions
GETCONFIG icsneoGetConfiguration;
SENDCONFIG icsneoSendConfiguration;
GETFIRESETTINGS icsneoGetFireSettings;
SETFIRESETTINGS icsneoSetFireSettings;
GETVCAN3SETTINGS icsneoGetVCAN3Settings;
SETVCAN3SETTINGS icsneoSetVCAN3Settings;
SETBITRATE icsneoSetBitRate;
GETDEVICEPARMS icsneoGetDeviceParameters;
SETDEVICEPARMS icsneoSetDeviceParameters;

//Error Functions
GETLASTAPIERROR icsneoGetLastAPIError;
GETERRMSGS icsneoGetErrorMessages;
GETERRORINFO icsneoGetErrorInfo;

//General Utility Functions
VALIDATEHOBJECT icsneoValidateHObject;
GETDLLVERSION icsneoGetDLLVersion;
GETSERIALNUMBER icsneoGetSerialNumber;
STARTSOCKSERVER icsneoStartSockServer;
STOPSOCKSERVER icsneoStopSockServer;

//CoreMini Functions
SCRIPTLOAD icsneoScriptLoad;
SCRIPTSTART icsneoScriptStart;
SCRIPTSTOP icsneoScriptStop;
SCRIPTCLEAR icsneoScriptClear;
SCRIPTSTARTFBLOCK icsneoScriptStartFBlock;
SCRIPTSTOPFBLOCK icsneoScriptStopFBlock;
SCRIPTGETFBLOCKSTATUS icsneoScriptGetFBlockStatus;
SCRIPTGETSCRIPTSTATUS icsneoScriptGetScriptStatus;
SCRIPTREADAPPSIGNAL icsneoScriptReadAppSignal;
SCRIPTWRITEAPPSIGNAL icsneoScriptWriteAppSignal;
SCRIPTREADRXMESSAGE icsneoScriptReadRxMessage;
SCRIPTREADTXMESSAGE icsneoScriptReadTxMessage;
SCRIPTWRITERXMESSAGE icsneoScriptWriteRxMessage;
SCRIPTWRITETXMESSAGE icsneoScriptWriteTxMessage;


//The following are valid strings for setting parameters on devices 
//using the icsneoGetDeviceParameters() and icsneoSetDeviceParameters() functions
const char *FireParameters[] =
{
	"can1", "can2", "can3", "can4", "swcan", "lsftcan", "lin1", "lin2",
	"lin3", "lin4", "cgi_baud", "cgi_tx_ifs_bit_times",
	"cgi_rx_ifs_bit_times", "cgi_chksum_enable", "network_enables", 
	"network_enabled_on_boot", "pwm_man_timeout", "pwr_man_enable", 
	"misc_io_initial_ddr", "misc_io_initial_latch", "misc_io_analog_enable", 
	"misc_io_report_period", "misc_io_on_report_events", "ain_sample_period", 
	"ain_threshold", "iso15765_separation_time_offset", "iso9141_kwp_settings", 
	"perf_en", "iso_parity", "iso_msg_termination", "network_enables_2"
};


const char *VCAN3Parameters[] =
{
	"can1", "can2", "network_enables", "network_enabled_on_boot", "iso15765_separation_time_offset",
	"perf_en", "misc_io_initial_ddr", "misc_io_initial_latch", "misc_io_report_period", 
	"misc_io_on_report_events"
};

const char *CANParameters[] =
{
     "Mode", "SetBaudrate", "Baudrate", "NetworkType", "TqSeg1",
	 "TqSeg2", "TqProp", "TqSync", "BRP", "auto_baud"
};

const char *SWCANParameters[] =
{
	 "Mode", "SetBaudrate", "Baudrate", "NetworkType", "TqSeg1", "TqSeg2", 
	 "TqProp", "TqSync", "BRP", "high_speed_auto_switch", "auto_baud"
};

const char *LINParameters[] =
{
	 "Baudrate", "spbrg", "brgh", "MasterResistor", "Mode"
};

const char *ISOKWPParms[] =
{
	 "Baudrate", "spbrg", "brgh", "init_steps", "init_step_count", 
	 "p2_500us", "p3_500us", "p4_500us", "chksum_enabled"
};


bool LoadDLLAPI(HINSTANCE &hAPIDLL)
{
 	if((hAPIDLL = LoadLibrary(L"icsneo40.dll")) == NULL)
		return false;

	icsneoFindNeoDevices =    (FINDNEODEVICES) GetProcAddress(hAPIDLL,              "icsneoFindNeoDevices");
	icsneoOpenNeoDevice =     (OPENNEODEVICE) GetProcAddress(hAPIDLL,               "icsneoOpenNeoDevice");
	icsneoClosePort =         (CLOSEPORT) GetProcAddress(hAPIDLL,                   "icsneoClosePort");	
	icsneoFreeObject =        (FREEOBJECT) GetProcAddress(hAPIDLL,                  "icsneoFreeObject");
    icsneoOpenPortEx =        (OPENPORTEX) GetProcAddress(hAPIDLL,                  "icsneoOpenPortEx");
	icsneoEnableNetworkCom = (ENABLENETWORKCOM) GetProcAddress(hAPIDLL,             "icsneoEnableNetworkCom");
	icsneoGetDLLVersion = (GETDLLVERSION) GetProcAddress(hAPIDLL,					"icsneoGetDLLVersion");

    icsneoTxMessages =        (TXMESSAGES) GetProcAddress(hAPIDLL,                  "icsneoTxMessages");
	icsneoGetMessages =       (GETMESSAGES) GetProcAddress(hAPIDLL,                 "icsneoGetMessages");
	icsneoWaitForRxMessagesWithTimeOut = (WAITFORRXMSGS) GetProcAddress(hAPIDLL,    "icsneoWaitForRxMessagesWithTimeOut");
    icsneoGetTimeStampForMsg = (GETTSFORMSG) GetProcAddress(hAPIDLL,                "icsneoGetTimeStampForMsg");
    icsneoEnableNetworkRXQueue = (ENABLERXQUEUE) GetProcAddress(hAPIDLL,            "icsneoEnableNetworkRXQueue");
    icsneoGetISO15765Status =  (GETISO15765STATUS) GetProcAddress(hAPIDLL,          "icsneoGetISO15765Status");
    icsneoSetISO15765RxParameters = (SETISO15765RXPARMS) GetProcAddress(hAPIDLL,    "icsneoSetISO15765RxParameters");
	icsneoDownloadISO15765_2_TXScript = (DOWNLOADISO15765_2_TXSCRIPT) GetProcAddress(hAPIDLL, "icsneoDownloadISO15765_2_TXScript");
	icsneoClearISO15765_2_TxScript = (CLEARISO15765_2_TXSCRIPT) GetProcAddress(hAPIDLL, "icsneoClearISO15765_2_TxScript");


	icsneoGetConfiguration =  (GETCONFIG) GetProcAddress(hAPIDLL,                   "icsneoGetConfiguration");
  	icsneoSendConfiguration = (SENDCONFIG) GetProcAddress(hAPIDLL,                  "icsneoSendConfiguration");
    icsneoGetFireSettings =   (GETFIRESETTINGS) GetProcAddress(hAPIDLL,             "icsneoGetFireSettings");
	icsneoSetFireSettings =   (SETFIRESETTINGS) GetProcAddress(hAPIDLL,             "icsneoSetFireSettings");
	icsneoGetVCAN3Settings =  (GETVCAN3SETTINGS) GetProcAddress(hAPIDLL,            "icsneoGetVCAN3Settings");
    icsneoSetVCAN3Settings =  (SETVCAN3SETTINGS) GetProcAddress(hAPIDLL,            "icsneoSetVCAN3Settings");
    icsneoSetBitRate =        (SETBITRATE)       GetProcAddress(hAPIDLL,            "icsneoSetBitRate");
	icsneoGetDeviceParameters = (GETDEVICEPARMS) GetProcAddress(hAPIDLL,            "icsneoGetDeviceParameters");
	icsneoSetDeviceParameters = (SETDEVICEPARMS) GetProcAddress(hAPIDLL,            "icsneoSetDeviceParameters");

	icsneoGetLastAPIError =   (GETLASTAPIERROR) GetProcAddress(hAPIDLL,             "icsneoGetLastAPIError");
    icsneoGetErrorMessages = (GETERRMSGS) GetProcAddress(hAPIDLL,                   "icsneoGetErrorMessages");
    icsneoGetErrorInfo =     (GETERRORINFO) GetProcAddress(hAPIDLL,                 "icsneoGetErrorInfo");

	icsneoScriptLoad =        (SCRIPTLOAD) GetProcAddress(hAPIDLL,                  "icsneoScriptLoad");
	icsneoScriptStart =       (SCRIPTSTART) GetProcAddress(hAPIDLL,                 "icsneoScriptStart");
	icsneoScriptStop =        (SCRIPTSTOP) GetProcAddress(hAPIDLL,                  "icsneoScriptStop");
	icsneoScriptClear =       (SCRIPTCLEAR) GetProcAddress(hAPIDLL,                 "icsneoScriptClear");
	icsneoScriptStartFBlock = (SCRIPTSTARTFBLOCK) GetProcAddress(hAPIDLL,           "icsneoScriptStartFBlock");
	icsneoScriptStopFBlock =  (SCRIPTSTOPFBLOCK) GetProcAddress(hAPIDLL,            "icsneoScriptStopFBlock");
	icsneoScriptGetFBlockStatus = (SCRIPTGETFBLOCKSTATUS) GetProcAddress(hAPIDLL,   "icsneoScriptGetFBlockStatus");
	icsneoScriptGetScriptStatus = (SCRIPTGETSCRIPTSTATUS) GetProcAddress(hAPIDLL, "icsneoScriptGetScriptStatus");
	icsneoScriptReadAppSignal = (SCRIPTREADAPPSIGNAL) GetProcAddress(hAPIDLL,       "icsneoScriptReadAppSignal");
    icsneoScriptWriteAppSignal = (SCRIPTWRITEAPPSIGNAL) GetProcAddress(hAPIDLL,     "icsneoScriptWriteAppSignal");

	icsneoScriptReadRxMessage = (SCRIPTREADRXMESSAGE) GetProcAddress(hAPIDLL,       "icsneoScriptReadRxMessage");
	icsneoScriptReadTxMessage = (SCRIPTREADTXMESSAGE) GetProcAddress(hAPIDLL,       "icsneoScriptReadTxMessage");
    icsneoScriptWriteRxMessage = (SCRIPTWRITERXMESSAGE)GetProcAddress(hAPIDLL,      "icsneoScriptWriteRxMessage");
	icsneoScriptWriteTxMessage = (SCRIPTWRITETXMESSAGE) GetProcAddress(hAPIDLL,     "icsneoScriptWriteTxMessage");

	

    if(!icsneoFindNeoDevices || !icsneoOpenNeoDevice || !icsneoClosePort || !icsneoFreeObject || !icsneoOpenPortEx ||  
	   !icsneoEnableNetworkCom || !icsneoTxMessages || !icsneoGetMessages || !icsneoWaitForRxMessagesWithTimeOut ||
       !icsneoGetTimeStampForMsg || !icsneoEnableNetworkRXQueue || !icsneoGetISO15765Status ||
       !icsneoSetISO15765RxParameters || !icsneoGetConfiguration || !icsneoSendConfiguration ||
       !icsneoGetFireSettings || !icsneoSetFireSettings || !icsneoGetVCAN3Settings ||
       !icsneoSetVCAN3Settings || !icsneoSetBitRate || !icsneoGetDeviceParameters ||
       !icsneoSetDeviceParameters || !icsneoGetLastAPIError || !icsneoGetErrorMessages ||
       !icsneoGetErrorInfo || !icsneoScriptLoad || !icsneoScriptStart || !icsneoScriptStop ||
       !icsneoScriptClear || !icsneoScriptStartFBlock || !icsneoScriptStopFBlock ||
       !icsneoScriptGetFBlockStatus || !icsneoScriptGetScriptStatus || !icsneoScriptReadAppSignal ||
       !icsneoScriptWriteAppSignal || !icsneoScriptReadRxMessage || !icsneoScriptReadTxMessage ||
       !icsneoScriptWriteRxMessage || !icsneoScriptWriteTxMessage || 
       !icsneoGetDLLVersion || !icsneoDownloadISO15765_2_TXScript ||
	   !icsneoClearISO15765_2_TxScript )
	{
		FreeLibrary(hAPIDLL);
		return false;
	}

   return true;
}	


void UnloadDLLAPI(HINSTANCE &hAPIDLL)
{
	if(hAPIDLL)
		FreeLibrary(hAPIDLL);
}