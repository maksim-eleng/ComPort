#pragma once

#include "ComPort.h"
#include "TimeBase.h"
#include "SysConst.h"

/***********************	MACROS	***************************
************************************************************/

/************************************************************
 * @brief Checking if there is a command on a least 1 input 
************************************************************/
#define checkCmdTimeOut(cmd_Name)	(Cmd[cmd_Name].Bit.TimeOut)

/************************************************************
 * @brief
************************************************************/
class NMEA : public IObsTimeBase, public IObsComPort
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

public:

	/************************************************************
	* @brief	For detecting name of NMEA string.
	* Don't change position NO...RESET and CHECKSUMERR command 
	************************************************************/
	typedef enum CMD_NAME_ENUM
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
	}cmdName_t;

	/************************************************************
	* @brief For set period of transmit NMEA strings.
	* These values for TimeInterval
	************************************************************/
	typedef enum TIME_INTERVAL_ENUM:int8_t
	{
		T_0_0_1S,			//для временного интервала 0.01с
		T_0_1S	= 1,	//для временного интервала 0.1с
		T_0_5S	= 5,	//для временного интервала 0.5с
		T_1S		= 10,	//для временного интервала 1с
		T_2S		= 20,	//для временного интервала 2с
		T_NOT_CFG = NOT_AVAILABLE,
	}timeInterval_t;

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
	}cmdPermitsEEPROM_t;

	/************************************************************
	* @brief Sets parameters of NMEA command, 
	* UART for store sets in EEPROM or another storage only.
	************************************************************/
	typedef struct NMEA_CFG_EEPROM_STRUCT
	{
		struct CFG_CHANNEL_EEPROM_STRUCT
		{
			NMEA::cmdPermitsEEPROM_t cmdPermits[NMEA::MAX_NMEACmdStrIndex];
			ComPort::BAUD_ENUM BautRate;				// скорость работы UART.
			NMEA::timeInterval_t TimeInterval;	// интервал передачи сформированных команд
			uint8_t TIDPriorityPermis;					// 
																					// permis use in/out for UART.
																					// If prohibited - in/out may be used as io
			struct {
				uint8_t in	: 4;	// !=0 - io используется для UART. ==0 - как IO
				uint8_t out	: 4;	// !=0 - io используется для UART. ==0 - как IO
			}ioUsedForUART;
		}chCfg[SysConst::maxUARTChannel];
		uint8_t numOfTerminalChannel;			// channel number of UART for terminal 
	}nmeaCfgEEPROM_t;



	/**
	* @brief 
	* @param cfgExt 
	* @return 
	*/
	NMEA(nmeaCfgEEPROM_t& nmeaCfgEEPROM, std::vector<ComPort>& com, TimeBase& sysClk);

	
private:

	/************************************************************
	 * @brief	For detection name of NMEA string. Location of str must
	 * mutch with enum NMEA_STR_DETECT_ENUM
	************************************************************/
	const char cmdName[MAX_NMEACmdStrIndex][5] =
	{
	{ "NO"   },
	{ "HALT" },
	{ "PRG"  },
	{ "SET"  },
	{ "___:"},
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
	{ "___:"},
	{ "VDO:" },
	{ "VDM:" },
	{ "___:"},
	{ "HDT:" },
	{ "HDG:" },
	{ "HDM:" },
	{ "ROT:" },
	{ "___:"},
	{ "VBW:" },
	{ "VHW:" },
	{ "VLW:" },
	{ "VDR:" },
	{ "___:"},
	{ "DBK:" },
	{ "DBS:" },
	{ "DBT:" },
	{ "DPT:" },
	{ "___:"},
	{ "MDA:" },
	{ "MWD:" },
	{ "MWV:" },
	{ "MTW:" },
	{ "XDR:" },
	{ "VWR:" },
	{ "VWT:" },
	{ "___:"},
	{ "RSD:" },
	{ "TLL:" },
	{ "TTM:" },
	{ "___:"},
	{ "RSA:" },
	{ "___:"},
	{ "AAM:" },
	{ "APA:" },
	{ "APB:" },
	{ "___:"},
	{ "ALR:" },
	{ "EVE:" },
	{ "TXT:" },
	{ "OTH:" },
	{ "___:"}
	};

	/************************************************************
	 * @brief For strore NMEA data in c_stile string
	************************************************************/
	typedef struct DATA_STRUCT
	{
		char	Lat[11];		//долгота
		char	NorS;			//северная или южная
		char	Lon[12];		//широта
		char	EorW;		  	//восточная или западная
		char	GPSIndicator;	//индикатор
		char	Satellites[5];	//количество спутников
		char	Dilution[6];	//горизонтальное разбавление
		char	Altitude[6];	//высота антены
		char	GeoidalSep[6];	// GeoidalSeparation
		char	AgeDGPSData[6];	// Age of DGPS data
		char	DifStationID[6];// Differential Reference Station ID
		char	Status;			//статус
		char	GroundSpeedGPS[6];	   	//скорость относительно земли в узлах
		char	GroundSpeedLOG[6];	   	//скорость относительно земли в узлах
		char	GroundSpeedLOGTransverse[6];	//Поперечная скорость относительно земли в узлах
		char	GroundSpeedLOGStatus;	// статус скорости относительно грунта
		char	WaterSpeedLOG[6];	   	//скорость относительно воды в узлах
		char	WaterSpeedLOGTransverse[6];	   	//Поперечная скорость относительно воды в узлах
		char	WaterSpeedLOGStatus;	// статус скорости относительно воды
		char	HeadingTrueGPS[6]; 	//истинный курс с GPS
		char	HeadingTrueHDT[6]; 	//истинный курс с команды HDT
		char	MagVar[8];			// магнитное отклонение в формате"<знак>xxxx". Знак формируется в зависимости от направления отклонения
		short	MagVarSigned;		// чиловое значение магнитной вариации
		char	MagDev[8];			// девиация компаса  в формате"<знак>xxxx". Знак формируется в зависимости от направления отклонения
		short	MagDevSigned;		// чиловое значение магнитной девиации
		char	MagHeading[6];		// курс по магнитному компасу с учетом поправок
		char	MagSensor[6];		// показания сенсора магнитного компаса
		char	UTCTime[11];		// время
		char	TimeZone[5];		// временное смещение
		char	Date[4];			// дата
		char	Month[4];			// месяц
		char	Year[6];			// год
		// для DTM
		char	LocalDatum[4];
		char	LocalDatumCode;
		char	LatOffset[9];
		char	LatOffsetNorS;
		char	LonOffset[9];
		char	LonOffsetEorW;
		char	AltitudeOffset[9]; //всталяем всегда '0.0'
		char	ReferenceDattum[4];
	}nmeaData_t;

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
	}cmdPermits_t;

	/************************************************************
	* @brief Sets parameters of NMEA command and flags of NMEA,  
	* UART for anything 1 channel
	************************************************************/
	typedef struct FL_STRUCT
	{
		struct CMD_STRUCT {
			cmdPermits_t permits = (cmdPermits_t)0;
			char counter;
		}cmd[MAX_NMEACmdStrIndex];

		ComPort::BAUD_ENUM BautRate;	// скорость работы UART.
		timeInterval_t TimeInterval;	// интервал передачи сформированных команд
		bool TIDPriorityPermis;				// 
		// permis use in/out for UART.
		// If prohibited - in/out may be used as io
		struct IO_USED_FOR_UART_STRUCT{
			uint8_t in : 4;		// !=0 - io используется для UART. ==0 - как IO
			uint8_t out : 4;	// !=0 - io используется для UART. ==0 - как IO
		}ioUsedForUART;
	}chFlags_t;

	/************************************************************
	* @brief For automatically transmition NMEA string  
	*************************************************************/
	typedef struct CMD_CONV_STRUCT{
		// name of command for decoding data from NMEA string or forming NMEA string for transmit
		cmdName_t cmdName;
		// last TID, that was received for convertable command
		char lastTID[2];
		// pointer to function of forming NMEA string for transmit
		void (NMEA::*formingStr)(void);
	}convertCmd_t;

	void formingGGAStr();
	void formingGNSStr();
	void formingGLLStr();
	void formingRMCStr();
	void formingVTGStr();
	void formingZDAStr();
	void formingDTMStr();
	void formingHDTStr();
	void formingHDMStr();
	void formingVBWStr();
	void formingVHWStr();
	void formingStr();

	/************************************************************
	* @brief For automatically transmition NMEA string
	* Initialization name of NMEA command and forming function.
	* May be used as 		m_convertCmd[0].cmdName; (this->*m_convertCmd[0].formingStr)();
	*************************************************************/
	const convertCmd_t m_convertCmd[14] = {
		/* For GPS */
		GGA, "", &NMEA::formingGGAStr,
		GNS, "", &NMEA::formingGNSStr,
		GLL, "", &NMEA::formingGLLStr,
		RMC, "", &NMEA::formingRMCStr,
		VTG, "", &NMEA::formingVTGStr,
		ZDA, "", &NMEA::formingZDAStr,
		DTM, "", &NMEA::formingDTMStr,
		/* For GYRO */
		HDT, "", &NMEA::formingHDTStr,
		HDG, "", nullptr,
		HDM, "", &NMEA::formingHDMStr,
		/* For AIS */
		VDO, "", nullptr,
		/* for other*/
		ALR, "", nullptr
	};
	
	/**
	 * Any command with curent TID not processed if was received on channel with 
	 * lower priority and this TID was detected earlier in channel with highter priority. 
	*/
	struct TID {
		char name[2] = "";					// TID name as "GP"
		uint8_t channel = 0;				// num of channel where this TID was received
		uint8_t counter = TIMEOUT;	// time after last detect TID on this channel
	}m_TID[15];

	/************************************************************
	* @brief Sets parameters of NMEA command and flags of NMEA,  
	* UART for all used channel
	************************************************************/
	chFlags_t m_flags[SysConst::maxUARTChannel];
	//STATEFRAME_ENUM volatile StateFrame;	// состояние фрейма
	//cmdName_t m_cmd = NO;	// продетектированная команда
	//обнаружен конец строки в режиме программирования. Для ожидания проги действий оператора
	// устанавливается в AnalyzeNMEAFrame, сбрасывается после обработки
	bool volatile m_fNewStrInPrg = false;
	uint8_t m_numOfTerminalChannel = 0;			// channel number of UART for terminal 
	std::vector<ComPort>& m_com;						// external port of channels aray 






	/**
	 * @brief 
	 * @param cfgEEPROM 
	 * @return 
	*/
	void setCfgDefault(nmeaCfgEEPROM_t& cfgEEPROM, char ch);

	/**
	 * @brief 
	*/
	void setCfgDefault(nmeaCfgEEPROM_t& cfgEEPROM);

	/**
	 * @brief 
	 * @param chFl 
	 * @param cmd 
	 * @param permitMsk 
	 * @return 
	*/
	bool checkCmdPermits(chFlags_t& chFl, cmdName_t cmd, cmdPermits_t permitMsk);
	
	//void setPermits(const cmdName_t& cmd, const cmdPermit_t& permitMsk, const uint8_t ch = 0);

	//void resetPermits(const cmdName_t& cmd, const cmdPermit_t& permitMsk, const uint8_t ch = 0);

	//operator[]()

	/**
	* @brief Check config in EEPROM. Return false if all field in config
	* in unprog state
	* @param cfgEEPROM <nmeaCfgEEPROM_t> - mirror of NMEA sets in EEPROM
	* @param unprogValue <uint8_t> - may be like as 0xFF or 0
	* @return <bool> false  if all field in config in unprog state
	*/
	bool checkCfgEEPROM(nmeaCfgEEPROM_t& cfgEEPROM, uint8_t unprogValue);



	virtual void handleEvent(TimeBase& ref, TimeBase::tBaseEvtMsk_t) override;

	virtual void handleEvent(ComPort& com, comEvtMsk_t evtMask) override;


};



