#pragma once

#include "ComPort.h"
#include "TimeBase.h"
#include "SysConst.h"



/***********************	MACROS	***************************
************************************************************/

/************************************************************
 * @brief Checking if there is a command on a least 1 input 
************************************************************/
#define checkCmdTimeOut(cmdName)	(Cmd[cmdName].Bit.TimeOut)

/************************************************************
 * @brief
************************************************************/
class NMEA// : public IObsTimeBase, public IObsComPort
{
	static constexpr char StartSymbol = '$';			// symbol of start NMEA string
	static constexpr char AISStartSymbol = '!';		// symbol of start AIS NMEA string
	static constexpr char EndSymbol = '*';				// symbol end of data in NMEA string
	static constexpr int8_t NOT_AVAILABLE = -1;			// if not config or not available
	// time after which it is considered that there is no command, in sec
	static constexpr unsigned char TIMEOUT = 5;

	// for AIS. If in AIS string these data - data not available
	static constexpr auto AIS_LAT_NOT_AVAILABLE = 91;
	static constexpr auto AIS_LON_NOT_AVAILABLE	= 181;
	static constexpr auto AIS_SOG_NOT_AVAILABLE	= 1023;
	static constexpr auto AIS_COG_NOT_AVAILABLE	= 3600;
	static constexpr auto AIS_HDT_NOT_AVAILABLE	= 511;
	/*******************************************************/
//��� �������������� ������ NMEA (����� ������ � ������).
//����� ����� ������������ NMEA_STR_DETECT_ENUM
//��������� �������������� ������� � � ������� ����������������
//������� NO...RESET � CHECKSUMERR ������� �� ������
/*******************************************************/
	

	
	/************************************************************
	* @brief	For detecting name of NMEA string.
	* Don't change position NO...RESET and CHECKSUMERR command 
	************************************************************/
	typedef enum NMEA_STR_DETECT_ENUM
	{
		NO,
		HALT,
		PRG,
		RESET,
		START_DATA,
		// GPS command
		GGA,
		GNS,
		GLL,
		RMC,
		VTG,
		ZDA,
		DTM,
		BOD,
		BWC,
		BWR,
		BWW,
		GBS,
		GLC,
		GSA,
		GSV,
		RMA,
		RMB,
		RTE,
		Rnn,
		WCV,
		WNC,
		WNR,
		WPL,
		XTE,
		ZTG,
		END_GPS_DATA,
		VDO,
		VDM,
		END_AIS_DATA,
		// GYRO command
		HDT,
		HDG,
		HDM,
		ROT,
		END_GYRO_DATA,
		// LOG command
		VBW,
		VHW,
		VLW,
		VDR,
		END_LOG_DATA,
		// ECHO command
		DBK,
		DBS,
		DBT,
		DPT,
		END_ECHO_DATA,
		// Weather's command
		MDA,
		MWD,
		MWV,
		MTW,
		XDR,
		VWR,
		VWT,
		END_WEATHER_DATA,
		// Radar's command
		RSD,
		TLL,
		TTM,
		END_RADAR_DATA,
		// Rudder's command
		RSA,
		END_RUDDER_DATA,
		// Autopilot command
		AAM,
		APA,
		APB,
		END_AUTOPILOT_DATA,
		// Alarm & other command
		ALR,
		EVE,
		TXT,
		OTHER,
		END_OTHER_EQUIPMENT,
		MAX_NMEACmdStrIndex,
	}nmeaStrDetect_t;
	
	/************************************************************
	 * @brief	For detection name of NMEA string. Location of str must
	 * mutch with enum NMEA_STR_DETECT_ENUM
	************************************************************/
	static constexpr char cmdName[MAX_NMEACmdStrIndex][7] =
	{
	{ "NO"   },
	{ "HALT" },
	{ "PRG"  },
	{ "SET"  },
	{ "___"},
	{ "GGA:" },
	{ "GNS:" },
	{ "GLL:" },
	{ "RMC:" },
	{ "VTG:" },
	{ "ZDA:" },
	{ "DTM:" },
	{ "BOD:" },
	{ "BWC:" },
	{ "BWR:" },
	{ "BWW:" },
	{ "GBS:" },
	{ "GLC:" },
	{ "GSA:" },
	{ "GSV:" },
	{ "RMA:" },
	{ "RMB:" },
	{ "RTE:" },
	{ "Rnn:" },
	{ "WCV:" },
	{ "WNC:" },
	{ "WNR:" },
	{ "WPL:" },
	{ "XTE:" },
	{ "ZTG:" },
	{ "___"},
	{ "VDO:" },
	{ "VDM:" },
	{ "___"},
	{ "HDT:" },
	{ "HDG:" },
	{ "HDM:" },
	{ "ROT:" },
	{ "___"},
	{ "VBW:" },
	{ "VHW:" },
	{ "VLW:" },
	{ "VDR:" },
	{ "___"},
	{ "DBK:" },
	{ "DBS:" },
	{ "DBT:" },
	{ "DPT:" },
	{ "___"},
	{ "MDA:" },
	{ "MWD:" },
	{ "MWV:" },
	{ "MTW:" },
	{ "XDR:" },
	{ "VWR:" },
	{ "VWT:" },
	{ "___"},
	{ "RSD:" },
	{ "TLL:" },
	{ "TTM:" },
	{ "___"},
	{ "RSA:" },
	{ "___"},
	{ "AAM:" },
	{ "APA:" },
	{ "APB:" },
	{ "___"},
	{ "ALR:" },
	{ "EVE:" },
	{ "TXT:" },
	{ "OTHER:" },
	{ "___"},
	};

	/************************************************************
	 * @brief
	************************************************************/
	typedef enum CONVERT_COMMAND_ENUM
	{
		// For GPS
		GGA_CONV = GGA,
		GNS_CONV = GNS,
		GLL_CONV = GLL,
		RMC_CONV = RMC,
		VTG_CONV = VTG,
		ZDA_CONV = ZDA,
		DTM_CONV = DTM,
		// For GYRO
		HDT_CONV = HDT,
		HDG_CONV = HDG,
		HDM_CONV = HDM,
		// For LOG
		VBW_CONV = VBW,
		VHW_CONV = VHW,
		// For AIS
		VDO_CONV = VDO,
		// For other
		END_CONV,
	}convertCmd_t;

	/************************************************************
	 * @brief For strore NMEA data in c_stile string
	************************************************************/
	typedef struct NMEA_STRUCT
	{
		char Lat[11];		//�������
		char NorS;			//�������� ��� �����
		char Lon[12];		//������
		char EorW;		  	//��������� ��� ��������
		char GPSIndicator;	//���������
		char Satellites[5];	//���������� ���������
		char Dilution[6];	//�������������� �����������
		char Altitude[6];	//������ ������
		char GeoidalSep[6];	// GeoidalSeparation
		char AgeDGPSData[6];	// Age of DGPS data
		char DifStationID[6];// Differential Reference Station ID
		char Status;			//������
		char GroundSpeedGPS[6];	   	//�������� ������������ ����� � �����
		char GroundSpeedLOG[6];	   	//�������� ������������ ����� � �����
		char GroundSpeedLOGTransverse[6];	//���������� �������� ������������ ����� � �����
		char GroundSpeedLOGStatus;	// ������ �������� ������������ ������
		char WaterSpeedLOG[6];	   	//�������� ������������ ���� � �����
		char WaterSpeedLOGTransverse[6];	   	//���������� �������� ������������ ���� � �����
		char WaterSpeedLOGStatus;	// ������ �������� ������������ ����
		char HeadingTrueGPS[6]; 	//�������� ���� � GPS
		char HeadingTrueHDT[6]; 	//�������� ���� � ������� HDT
		char MagVar[8];			// ��������� ���������� � �������"<����>xxxx". ���� ����������� � ����������� �� ����������� ����������
		short MagVarSigned;		// ������� �������� ��������� ��������
		char MagDev[8];			// �������� �������  � �������"<����>xxxx". ���� ����������� � ����������� �� ����������� ����������
		short MagDevSigned;		// ������� �������� ��������� ��������
		char MagHeading[6];		// ���� �� ���������� ������� � ������ ��������
		char MagSensor[6];		// ��������� ������� ���������� �������
		char UTCTime[11];		// �����
		char TimeZone[5];		// ��������� ��������
		char Date[4];			// ����
		char Month[4];			// �����
		char Year[6];			// ���
		// ��� DTM
		char LocalDatum[4];
		char LocalDatumCode;
		char LatOffset[9];
		char LatOffsetNorS;
		char LonOffset[9];
		char LonOffsetEorW;
		char AltitudeOffset[9]; //�������� ������ '0.0'
		char ReferenceDattum[4];
	}nmea_t;

	/************************************************************
	 * @brief For strore AIS NMEA data in c_stile string
	************************************************************/
	typedef struct AIS_NMEA_STRUCT
	{
		unsigned int MMSI;
		float Lat;
		float Lon;
		//#pragma anon_unions
		union AIS_AKK_UNION
		{
			unsigned int All;
			struct AKK_STR
			{
				unsigned char A0;
				unsigned char A1;
				unsigned char A2;
				unsigned char A3;
			}Path;
		}Akk;
		uint16_t SOG;
		uint16_t COG;
		uint16_t HDT;
		uint8_t MesType;
		uint8_t Data[18];
	}nmeaAIS_t;

	/************************************************************
	 * @brief For set period of transmit NMEA strings.
	 * These values for TimeInterval
	************************************************************/
	typedef enum TIME_INTERVAL_ENUM:int8_t
	{
		T_0_0_1S,			//��� ���������� ��������� 0.01�
		T_0_1S	= 1,	//��� ���������� ��������� 0.1�
		T_0_5S	= 5,	//��� ���������� ��������� 0.5�
		T_1S		= 10,	//��� ���������� ��������� 1�
		T_2S		= 20,	//��� ���������� ��������� 2�
		T_NOT_CFG = NOT_AVAILABLE,
	}timeInterval_t;




	/**
	 * @brief 
	 * @param cfgExt 
	 * @return 
	*/
	NMEA(nmeaCfgEEPROM_t& cfgExt);




protected:

private:

	/************************************************************
	* @brief Mask for sets for every NMEA command 
	************************************************************/
	typedef enum : uint8_t
	{
		In				= (1 << 0),
		Out				= (1 << 1),
		Convert		= (1 << 2),
		Forced		= (1 << 3),
		Calc			= (1 << 4),
		TimeOut		= (1 << 5),
		Reserved1	= (1 << 6),
		Reserved2	= (1 << 7),
	}cmdCfgMsk_t;

	/************************************************************
	* @brief Sets parameters of NMEA command and flaf of NMEA,  
	* UART
	************************************************************/
	typedef struct NMEA_CFG_STRUCT
	{
		struct 
		{
			struct {
				cmdCfgMsk_t permit;
				void (*formingStr)(uint8_t);	// ��������� �� �-� ������������ ������ ��� �������� � ������ NMEA
			}cmd[MAX_NMEACmdStrIndex];

			ComPort::BAUD_ENUM BautRate;			// �������� ������ UART.
			timeInterval_t TimeInterval;			// �������� �������� �������������� ������
			uint8_t TIDPriorityPermis;				// 
			// permis use in/out for UART.
			// If prohibited - in/out may be used as io
			struct {
				uint8_t in : 4;		// !=0 - io ������������ ��� UART. ==0 - ��� IO
				uint8_t out : 4;	// !=0 - io ������������ ��� UART. ==0 - ��� IO
			}ioUsedForUART;
		}chCfg[SysConst::UARTChannel];

	uint8_t numOfTerminalChannel;			// channel number of UART for terminal 
	}nmeaCfg_t;


	/************************************************************
	* @brief Mask for sets for every NMEA command for store sets
	* in EEPROM or another storage
	************************************************************/
	typedef enum: uint8_t 
	{
		In_E			= 1 << 0,
		Out_E			= 1 << 1,
		Convert_E	= 1 << 2,
		Forced_E	= 1 << 3,
		Calc_E		= 1 << 4,
	}cmdCfgMskEEPROM_t;

	/************************************************************
	* @brief Sets parameters of NMEA command, 
	* UART for store sets in EEPROM or another storage only.
	************************************************************/
	typedef struct NMEA_CFG_EEPROM_STRUCT
	{
		struct
		{
			cmdCfgMskEEPROM_t cmd[MAX_NMEACmdStrIndex];

			ComPort::BAUD_ENUM BautRate;			// �������� ������ UART.
			timeInterval_t TimeInterval;			// �������� �������� �������������� ������
			uint8_t TIDPriorityPermis;				// 
			// permis use in/out for UART.
			// If prohibited - in/out may be used as io
			struct {
				uint8_t in : 4;		// !=0 - io ������������ ��� UART. ==0 - ��� IO
				uint8_t out : 4;	// !=0 - io ������������ ��� UART. ==0 - ��� IO
			}ioUsedForUART;
		}chCfg[SysConst::UARTChannel];

		uint8_t numOfTerminalChannel;			// channel number of UART for terminal 
	}nmeaCfgEEPROM_t;

	//STATEFRAME_ENUM volatile StateFrame;	// ��������� ������
	nmeaStrDetect_t volatile m_cmd = NO;	// ������������������ �������
	//��������� ����� ������ � ������ ����������������. ��� �������� ����� �������� ���������
	// ��������������� � AnalyzeNMEAFrame, ������������ ����� ���������
	bool volatile m_fNewStrInPrg;
	//// ��������� �� ��������������� ���������
	//CMD_UNION* CmdPoint;
	// 
	nmeaCfg_t m_nmeaFl;


	// for 1 channel
	typedef struct CHANNEL_VAR_STRUCT
	{
		timeInterval_t m_timeInterval;	// �������� �������� �������������� ������
		bool m_fPermisTx;							// =1 - ������ ����� ����������� ������� � nmeaFormingStr. ����������� ��������.
//		bool m_fChechSumErrFlag;				// =1 - � ������ ������ ���� ������ ����������� ����� ��� ���������� ���������
		// ���������� ������������� ����������� �� ���� Talker
		bool m_fTIDPriorityPermis;
	}nmeaFlCh_t;



	/**
	 * @brief 
	*/
	bool setCfgDefault(nmeaCfg_t& cfg, int ch, bool fSave = false);

	bool setCfgDefault(nmeaCfg_t& cfg, bool fSave = false);


};

using nmeaCfgExternal_t = Nmea::cfgExternal_t;
