#ifndef __INC_BEAMFORMING_H
#define __INC_BEAMFORMING_H

#ifndef BEAMFORMING_SUPPORT
#define	BEAMFORMING_SUPPORT		0
#endif

#if (BEAMFORMING_SUPPORT == 1)

/*Beamforming Related*/
#include "TXBF/HalComTxbf.h"
#include "TXBF/HalTxbfJaguar.h"
#include "TXBF/HalTxbf8192E.h"
#include "TXBF/HalTxbf8814A.h"
#include "TXBF/HalTxbf8821B.h"

#define BEAMFORMEE_ENTRY_NUM		2
#define BEAMFORMER_ENTRY_NUM			2
#define GET_BEAMFORM_INFO(_pAdapter)	((PRT_BEAMFORMING_INFO)(&(((HAL_DATA_TYPE *)((_pAdapter)->HalData))->DM_OutSrc.BeamformingInfo)))

typedef enum _BEAMFORMING_ENTRY_STATE
{
	BEAMFORMING_ENTRY_STATE_UNINITIALIZE, 
	BEAMFORMING_ENTRY_STATE_INITIALIZEING, 
	BEAMFORMING_ENTRY_STATE_INITIALIZED, 
	BEAMFORMING_ENTRY_STATE_PROGRESSING, 
	BEAMFORMING_ENTRY_STATE_PROGRESSED, 
}BEAMFORMING_ENTRY_STATE, *PBEAMFORMING_ENTRY_STATE;


typedef enum _BEAMFORMING_STATE
{
	BEAMFORMING_STATE_END,
	BEAMFORMING_STATE_START_1BFee, 
	BEAMFORMING_STATE_START_2BFee
}BEAMFORMING_STATE, *PBEAMFORMING_STATE;

typedef enum _BEAMFORMING_NOTIFY_STATE
{
	BEAMFORMING_NOTIFY_NONE,
	BEAMFORMING_NOTIFY_ADD,
	BEAMFORMING_NOTIFY_DELETE,
	BEAMFORMING_NOTIFY_RESET
} BEAMFORMING_NOTIFY_STATE, *PBEAMFORMING_NOTIFY_STATE;

typedef enum _BEAMFORMING_CAP
{
	BEAMFORMING_CAP_NONE = 0x0,
	BEAMFORMER_CAP_HT_EXPLICIT = 0x1, 
	BEAMFORMEE_CAP_HT_EXPLICIT = 0x2, 
	BEAMFORMER_CAP_VHT_SU = 0x4,			// Self has er Cap, because Reg er  & peer ee
	BEAMFORMEE_CAP_VHT_SU = 0x8, 			// Self has ee Cap, because Reg ee & peer er 
	BEAMFORMER_CAP = 0x10,
	BEAMFORMEE_CAP = 0x20,
}BEAMFORMING_CAP, *PBEAMFORMING_CAP;


typedef enum _SOUNDING_MODE
{
	SOUNDING_SW_VHT_TIMER = 0x0,
	SOUNDING_SW_HT_TIMER = 0x1, 
	SOUNDING_STOP_All_TIMER = 0x2, 
	SOUNDING_HW_VHT_TIMER = 0x3,			
	SOUNDING_HW_HT_TIMER = 0x4,
	SOUNDING_STOP_OID_TIMER = 0x5, 
	SOUNDING_AUTO_VHT_TIMER = 0x6,
	SOUNDING_AUTO_HT_TIMER = 0x7,
	SOUNDING_FW_VHT_TIMER = 0x8,
	SOUNDING_FW_HT_TIMER = 0x9,
}SOUNDING_MODE, *PSOUNDING_MODE;


typedef struct _RT_BEAMFORMING_ENTRY {
	BOOLEAN bUsed;
	BOOLEAN	bTxBF;
	BOOLEAN bSound;
	u2Byte	AID;				// Used to construct AID field of NDPA packet.
	u2Byte	MacId;				// Used to Set Reg42C in IBSS mode. 
	u2Byte	P_AID;				// Used to fill Reg42C & Reg714 to compare with P_AID of Tx DESC. 
	u1Byte	MacAddr[6];			// Used to fill Reg6E4 to fill Mac address of CSI report frame.
	CHANNEL_WIDTH			SoundBW;	// Sounding BandWidth
	u2Byte					SoundPeriod;
	BEAMFORMING_CAP			BeamformEntryCap;
	BEAMFORMING_ENTRY_STATE	BeamformEntryState;	
	BOOLEAN						bBeamformingInProgress;
//	u1Byte	LogSeq;									// Move to _RT_BEAMFORMER_ENTRY
//	u2Byte	LogRetryCnt:3; 		// 0~4				// Move to _RT_BEAMFORMER_ENTRY
//	u2Byte	LogSuccessCnt:2;		// 0~2				// Move to _RT_BEAMFORMER_ENTRY
	u2Byte	LogStatusFailCnt:5;	// 0~21
	u2Byte	DefaultCSICnt:5;		// 0~21
	u1Byte	CSIMatrix[327];
	u2Byte	CSIMatrixLen;
	u1Byte	NumofSoundingDim;
	u1Byte	CompSteeringNumofBFer;
} RT_BEAMFORMING_ENTRY, *PRT_BEAMFORMING_ENTRY;

typedef struct _RT_BEAMFORMER_ENTRY {
	BOOLEAN 			bUsed;
	// P_AID of BFer entry is probably not used
	u2Byte				P_AID;				// Used to fill Reg42C & Reg714 to compare with P_AID of Tx DESC. 
	u1Byte				MacAddr[6];
	BEAMFORMING_CAP		BeamformEntryCap;
	u1Byte				NumofSoundingDim;
	u1Byte				LogSeq;							// Modified by Jeffery @2014-10-29
	u2Byte				LogRetryCnt:3; 		// 0~4		// Modified by Jeffery @2014-10-29	
	u2Byte				LogSuccessCnt:2;	// 0~2		// Modified by Jeffery @2014-10-29
} RT_BEAMFORMER_ENTRY, *PRT_BEAMFORMER_ENTRY;

typedef struct _RT_SOUNDING_INFO {
	u1Byte			SoundIdx;
	CHANNEL_WIDTH	SoundBW;
	SOUNDING_MODE	SoundMode; 
	u2Byte			SoundPeriod;
} RT_SOUNDING_INFO, *PRT_SOUNDING_INFO;



typedef struct _RT_BEAMFORMING_OID_INFO {
	u1Byte			SoundOidIdx;
	CHANNEL_WIDTH	SoundOidBW;	
	SOUNDING_MODE	SoundOidMode;
	u2Byte			SoundOidPeriod;
} RT_BEAMFORMING_OID_INFO, *PRT_BEAMFORMING_OID_INFO;


typedef struct _RT_BEAMFORMING_INFO {
	BEAMFORMING_CAP				BeamformCap;
	BEAMFORMING_STATE			BeamformState;
	RT_BEAMFORMING_ENTRY		BeamformeeEntry[BEAMFORMEE_ENTRY_NUM];
	RT_BEAMFORMER_ENTRY			BeamformerEntry[BEAMFORMER_ENTRY_NUM];
	u1Byte						BeamformeeCurIdx;
	RT_TIMER					BeamformingTimer;
	RT_SOUNDING_INFO			SoundingInfo;
	RT_BEAMFORMING_OID_INFO		BeamformingOidInfo;
	HAL_TXBF_INFO				TxbfInfo;
} RT_BEAMFORMING_INFO, *PRT_BEAMFORMING_INFO;


typedef struct _RT_NDPA_STA_INFO {
	u2Byte	AID:12;	
	u2Byte	FeedbackType:1;
	u2Byte	NcIndex:3;	
} RT_NDPA_STA_INFO, *PRT_NDPA_STA_INFO;


BEAMFORMING_CAP
phydm_Beamforming_GetEntryBeamCapByMacId(
	IN	PVOID	pDM_VOID,
	IN 	u1Byte 	MacId
 );


VOID
Beamforming_GidPAid(
	PADAPTER	Adapter,
	PRT_TCB		pTcb
	);


BOOLEAN
Beamforming_SendHTNDPAPacket(
	IN	PADAPTER		Adapter,
	IN	pu1Byte			RA,
	IN	CHANNEL_WIDTH	BW, 
	IN	u1Byte			QIdx
	);


BOOLEAN
Beamforming_SendVHTNDPAPacket(
	IN	PADAPTER		Adapter,
	IN	pu1Byte			RA,
	IN	u2Byte			AID,
	IN	CHANNEL_WIDTH	BW,
	IN	u1Byte			QIdx
	);


VOID
phydm_Beamforming_Notify(
	IN	PVOID	pDM_VOID
	);


VOID
Beamforming_Enter(
	PADAPTER		Adapter,
	PRT_WLAN_STA	pSTA
	);

VOID
Beamforming_Leave(
	PADAPTER		Adapter,
	pu1Byte			RA
	);

BOOLEAN
BeamformingStart_FW(
	PADAPTER		Adapter,
	u1Byte			Idx
	);

VOID
Beamforming_CheckSoundingSuccess(
	PADAPTER		Adapter,
	BOOLEAN			Status	
);

VOID
phydm_Beamforming_End_SW(
	IN	PVOID	pDM_VOID,
	BOOLEAN			Status	
	);

VOID
phydm_Beamforming_End_FW(
	IN	PVOID	pDM_VOID
	);

VOID
Beamforming_TimerCallback(
	PADAPTER			Adapter
	);

VOID
phydm_Beamforming_Init(
	IN	PVOID		pDM_VOID
	);



BEAMFORMING_CAP
phydm_Beamforming_GetBeamCap(
	IN	PVOID	pDM_VOID,
	IN PRT_BEAMFORMING_INFO 	pBeamInfo
	);


BOOLEAN
BeamformingControl_V1(
	PADAPTER		Adapter,
	pu1Byte			RA,
	u1Byte			AID,
	u1Byte			Mode,
	CHANNEL_WIDTH	BW,
	u1Byte			Rate
	);


BOOLEAN
phydm_BeamformingControl_V2(
	IN	PVOID		pDM_VOID,
	u1Byte			Idx,
	u1Byte			Mode, 
	CHANNEL_WIDTH	BW,
	u2Byte			Period
	);


BOOLEAN
BeamformingControl_V3(
	PADAPTER		Adapter,
	u1Byte			Threshold,
	u2Byte			Period
	);

VOID
phydm_Beamforming_Watchdog(
	IN	PVOID		pDM_VOID
	);

VOID
Beamforming_SWTimerCallback(
	PRT_TIMER		pTimer
	);

#else
#define Beamforming_GidPAid(Adapter, pTcb)
#define Beamforming_Enter(Adapter, pSTA)
#define Beamforming_Leave(Adapter, RA)
#define Beamforming_End_SW(Adapter, Status)
#define Beamforming_End_FW(Adapter)
#define Beamforming_Init(Adapter)
#define Beamforming_Release(Adapter)
#define BeamformingControl_V1(Adapter, RA, AID, Mode, BW, Rate) 		(TRUE)
#define BeamformingControl_V2(Adapter, Idx, Mode, BW, Period)			(TRUE)
#define Beamforming_TimerCallback(Adapter)
#define Beamforming_Watchdog(Adapter)
#endif


RT_STATUS
Beamforming_GetReportFrame(
	IN	PADAPTER		Adapter,
	IN	PRT_RFD			pRfd,
	IN	POCTET_STRING	pPduOS
	);

VOID
Beamforming_GetNDPAFrame(
	IN	PADAPTER		Adapter,
	IN	OCTET_STRING	pduOS
	);

#endif