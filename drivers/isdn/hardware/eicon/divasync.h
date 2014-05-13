
/*
 *
 Copyright (c) Eicon Networks, 2002.
 *
 This source file is supplied for the use with
 Eicon Networks range of DIVA Server Adapters.
 *
 Eicon File Revision :    2.1
 *
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.
 *
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY OF ANY KIND WHATSOEVER INCLUDING ANY
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.
 *
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifndef __DIVA_SYNC__H
#define __DIVA_SYNC__H
#define IDI_SYNC_REQ_REMOVE             0x00
#define IDI_SYNC_REQ_GET_NAME           0x01
#define IDI_SYNC_REQ_GET_SERIAL         0x02
#define IDI_SYNC_REQ_SET_POSTCALL       0x03
#define IDI_SYNC_REQ_GET_XLOG           0x04
#define IDI_SYNC_REQ_GET_FEATURES       0x05
#define IDI_SYNC_REQ_USB_REGISTER       0x06
#define IDI_SYNC_REQ_USB_RELEASE        0x07
#define IDI_SYNC_REQ_USB_ADD_DEVICE     0x08
#define IDI_SYNC_REQ_USB_START_DEVICE   0x09
#define IDI_SYNC_REQ_USB_STOP_DEVICE    0x0A
#define IDI_SYNC_REQ_USB_REMOVE_DEVICE  0x0B
#define IDI_SYNC_REQ_GET_CARDTYPE       0x0C
#define IDI_SYNC_REQ_GET_DBG_XLOG       0x0D
#define DIVA_USB
#define DIVA_USB_REQ                    0xAC
#define DIVA_USB_TEST                   0xAB
#define DIVA_USB_ADD_ADAPTER            0xAC
#define DIVA_USB_REMOVE_ADAPTER         0xAD
#define IDI_SYNC_REQ_SERIAL_HOOK        0x80
#define IDI_SYNC_REQ_XCHANGE_STATUS     0x81
#define IDI_SYNC_REQ_USB_HOOK           0x82
#define IDI_SYNC_REQ_PORTDRV_HOOK       0x83
#define IDI_SYNC_REQ_SLI                0x84   
#define IDI_SYNC_REQ_RECONFIGURE        0x85
#define IDI_SYNC_REQ_RESET              0x86
#define IDI_SYNC_REQ_GET_85X_DEVICE_DATA     0x87
#define IDI_SYNC_REQ_LOCK_85X                   0x88
#define IDI_SYNC_REQ_DIVA_85X_USB_DATA_EXCHANGE 0x99
#define IDI_SYNC_REQ_DIPORT_EXCHANGE_REQ   0x98
#define IDI_SYNC_REQ_GET_85X_EXT_PORT_TYPE      0xA0
#define IDI_SYNC_REQ_XDI_GET_EXTENDED_FEATURES  0x92
typedef struct _diva_xdi_get_extended_xdi_features {
	dword buffer_length_in_bytes;
	byte  *features;
} diva_xdi_get_extended_xdi_features_t;
#define DIVA_XDI_EXTENDED_FEATURES_VALID          0x01
#define DIVA_XDI_EXTENDED_FEATURE_CMA             0x02
#define DIVA_XDI_EXTENDED_FEATURE_SDRAM_BAR       0x04
#define DIVA_XDI_EXTENDED_FEATURE_CAPI_PRMS       0x08
#define DIVA_XDI_EXTENDED_FEATURE_NO_CANCEL_RC    0x10
#define DIVA_XDI_EXTENDED_FEATURE_RX_DMA          0x20
#define DIVA_XDI_EXTENDED_FEATURE_MANAGEMENT_DMA  0x40
#define DIVA_XDI_EXTENDED_FEATURE_WIDE_ID         0x80
#define DIVA_XDI_EXTENDED_FEATURES_MAX_SZ    1
#define IDI_SYNC_REQ_XDI_GET_ADAPTER_SDRAM_BAR   0x93
typedef struct _diva_xdi_get_adapter_sdram_bar {
	dword bar;
} diva_xdi_get_adapter_sdram_bar_t;
#define IDI_SYNC_REQ_XDI_GET_CAPI_PARAMS   0x94
/*
  CAPI Parameters will be written in the caller's buffer
*/
typedef struct _diva_xdi_get_capi_parameters {
	dword structure_length;
	byte flag_dynamic_l1_down;
	byte group_optimization_enabled;
} diva_xdi_get_capi_parameters_t;
#define IDI_SYNC_REQ_XDI_GET_LOGICAL_ADAPTER_NUMBER   0x95
typedef struct _diva_xdi_get_logical_adapter_number {
	dword logical_adapter_number;
	dword controller;
	dword total_controllers;
} diva_xdi_get_logical_adapter_number_s_t;
#define IDI_SYNC_REQ_UP1DM_OPERATION   0x96
#define IDI_SYNC_REQ_DMA_DESCRIPTOR_OPERATION 0x97
#define IDI_SYNC_REQ_DMA_DESCRIPTOR_ALLOC     0x01
#define IDI_SYNC_REQ_DMA_DESCRIPTOR_FREE      0x02
typedef struct _diva_xdi_dma_descriptor_operation {
	int operation;
	int descriptor_number;
	void *descriptor_address;
	dword descriptor_magic;
} diva_xdi_dma_descriptor_operation_t;
#define IDI_SYNC_REQ_DIDD_REGISTER_ADAPTER_NOTIFY   0x01
#define IDI_SYNC_REQ_DIDD_REMOVE_ADAPTER_NOTIFY     0x02
#define IDI_SYNC_REQ_DIDD_ADD_ADAPTER               0x03
#define IDI_SYNC_REQ_DIDD_REMOVE_ADAPTER            0x04
#define IDI_SYNC_REQ_DIDD_READ_ADAPTER_ARRAY        0x05
#define IDI_SYNC_REQ_DIDD_GET_CFG_LIB_IFC           0x10
typedef struct _diva_didd_adapter_notify {
	dword handle; 
	void *callback;
	void *context;
} diva_didd_adapter_notify_t;
typedef struct _diva_didd_add_adapter {
	void *descriptor;
} diva_didd_add_adapter_t;
typedef struct _diva_didd_remove_adapter {
	IDI_CALL p_request;
} diva_didd_remove_adapter_t;
typedef struct _diva_didd_read_adapter_array {
	void *buffer;
	dword length;
} diva_didd_read_adapter_array_t;
typedef struct _diva_didd_get_cfg_lib_ifc {
	void *ifc;
} diva_didd_get_cfg_lib_ifc_t;
#define IDI_SYNC_REQ_XDI_GET_STREAM    0x91
#define DIVA_XDI_SYNCHRONOUS_SERVICE   0x01
#define DIVA_XDI_DMA_SERVICE           0x02
#define DIVA_XDI_AUTO_SERVICE          0x03
#define DIVA_ISTREAM_COMPLETE_NOTIFY   0
#define DIVA_ISTREAM_COMPLETE_READ     1
#define DIVA_ISTREAM_COMPLETE_WRITE    2
typedef struct _diva_xdi_stream_interface {
	unsigned char  Id;                 
	unsigned char provided_service;    
	unsigned char requested_service;   
	void *xdi_context;    
	void *client_context;   
	int (*write)(void *context,
		     int Id,
		     void *data,
		     int length,
		     int final,
		     byte usr1,
		     byte usr2);
	int (*read)(void *context,
		    int Id,
		    void *data,
		    int max_length,
		    int *final,
		    byte *usr1,
		    byte *usr2);
	int (*complete)(void *client_context,
			int Id,
			int what,
			void *data,
			int length,
			int *final);
} diva_xdi_stream_interface_t;
typedef struct
{ unsigned char LineState;         
#define SERIAL_GSM_CELL 0x01   
	unsigned char CardState;          
	unsigned char IsdnState;          
	unsigned char HookState;          
#define SERIAL_ON_HOOK 0x02   
} SERIAL_STATE;
typedef int (*SERIAL_INT_CB)(void *Context);
typedef int (*SERIAL_DPC_CB)(void *Context);
typedef unsigned char (*SERIAL_I_SYNC)(void *Context);
typedef struct
{ 
	unsigned char Req;             
	unsigned char Rc;              
	unsigned char Function;           
#define SERIAL_HOOK_ATTACH 0x81
#define SERIAL_HOOK_STATUS 0x82
#define SERIAL_HOOK_I_SYNC 0x83
#define SERIAL_HOOK_NOECHO 0x84
#define SERIAL_HOOK_RING 0x85
#define SERIAL_HOOK_DETACH 0x8f
	unsigned char Flags;           
	
	SERIAL_INT_CB InterruptHandler; 
	SERIAL_DPC_CB DeferredHandler; 
	void   *HandlerContext; 
	
	unsigned long IoBase;    
	SERIAL_STATE State;
	
	SERIAL_I_SYNC SyncFunction;  
	void   *SyncContext;  
	unsigned char SyncResult;   
} SERIAL_HOOK;
typedef struct
{ 
	unsigned char Req;             
	unsigned char Rc;              
#define DRIVER_STATUS_BOOT  0xA1
#define DRIVER_STATUS_INIT_DEV 0xA2
#define DRIVER_STATUS_RUNNING 0xA3
#define DRIVER_STATUS_SHUTDOWN 0xAF
#define DRIVER_STATUS_TRAPPED 0xAE
	unsigned char wmpStatus;          
	unsigned char idiStatus;   
	unsigned long wizProto;   
	
	unsigned long cardType;   
	unsigned long nt2;    
	unsigned long permanent;   
	unsigned long stableL2;   
	unsigned long tei;    
#define CRC4_MASK   0x00000003
#define L1_TRISTATE_MASK 0x00000004
#define WATCHDOG_MASK  0x00000008
#define NO_ORDER_CHECK_MASK 0x00000010
#define LOW_CHANNEL_MASK 0x00000020
#define NO_HSCX30_MASK  0x00000040
#define SET_BOARD   0x00001000
#define SET_CRC4   0x00030000
#define SET_L1_TRISTATE  0x00040000
#define SET_WATCHDOG  0x00080000
#define SET_NO_ORDER_CHECK 0x00100000
#define SET_LOW_CHANNEL  0x00200000
#define SET_NO_HSCX30  0x00400000
#define SET_MODE   0x00800000
#define SET_PROTO   0x02000000
#define SET_CARDTYPE  0x04000000
#define SET_NT2    0x08000000
#define SET_PERMANENT  0x10000000
#define SET_STABLEL2  0x20000000
#define SET_TEI    0x40000000
#define SET_NUMBERLEN  0x80000000
	unsigned long Flag;  
	unsigned long NumberLen; 
	union {
		struct {    
			unsigned long SerialNumber;
			char     *pCardname; 
		} board;
		struct {      
			void *pRawResources;
			void *pXlatResources;
		} res;
		struct { 
#define GLARE_RESOLVE_MASK 0x00000001
#define DID_MASK   0x00000002
#define BEARER_CAP_MASK  0x0000000c
#define SET_GLARE_RESOLVE 0x00010000
#define SET_DID    0x00020000
#define SET_BEARER_CAP  0x000c0000
			unsigned long Flag;  
			unsigned short DigitTimeout;
			unsigned short AnswerDelay;
		} rbs;
		struct { 
#define CALL_REF_LENGTH1_MASK 0x00000001
#define BRI_CHANNEL_ID_MASK  0x00000002
#define SET_CALL_REF_LENGTH  0x00010000
#define SET_BRI_CHANNEL_ID  0x00020000
			unsigned long Flag;  
		} qsig;
		struct { 
#define SET_SPID1   0x00010000
#define SET_NUMBER1   0x00020000
#define SET_SUBADDRESS1  0x00040000
#define SET_SPID2   0x00100000
#define SET_NUMBER2   0x00200000
#define SET_SUBADDRESS2  0x00400000
#define MASK_SET   0xffff0000
			unsigned long Flag;   
			unsigned char *pBuffer; 
		} isdnNo;
	}
		parms
		;
} isdnProps;
typedef void (*PORTDRV_HOOK_CB)(void *Context, int Plug);
typedef struct
{ 
	unsigned char Req;             
	unsigned char Rc;              
	unsigned char Function;           
	unsigned char Flags;           
	PORTDRV_HOOK_CB Callback;   
	void   *Context;   
	unsigned long Info;    
} PORTDRV_HOOK;
#define SLI_INSTALL     (0xA1)
#define SLI_UNINSTALL   (0xA2)
typedef int (*SLIENTRYPOINT)(void *p3SignalAPI, void *pContext);
typedef struct
{   
	unsigned char   Req;                
	unsigned char   Rc;                 
	unsigned char   Function;           
	unsigned char   Flags;              
	SLIENTRYPOINT   Callback;           
	void            *Context;           
	unsigned long   Info;               
} SLIENTRYPOINT_REQ;
typedef int (*USB_SEND_REQ)(unsigned char PipeIndex, unsigned char Type, void *Data, int sizeData);
typedef int (*USB_START_DEV)(void *Adapter, void *Ipac);
typedef void (*USB_RECV_NOTIFY)(void *Ipac, void *msg);
typedef void (*USB_XMIT_NOTIFY)(void *Ipac, unsigned char PipeIndex);
typedef union
{ ENTITY Entity;
	struct
	{ 
		unsigned char   Req; 
		unsigned char   Rc;  
	}   Request;
	struct
	{ unsigned char   Req; 
		unsigned char   Rc;  
		unsigned char   name[BOARD_NAME_LENGTH];
	}   GetName;
	struct
	{ unsigned char   Req; 
		unsigned char   Rc;  
		unsigned long   serial; 
	}   GetSerial;
	struct
	{ unsigned char   Req; 
		unsigned char   Rc;  
		unsigned long   lineIdx;
	}   GetLineIdx;
	struct
	{ unsigned char  Req;     
		unsigned char  Rc;      
		unsigned long  cardtype;
	}   GetCardType;
	struct
	{ unsigned short command;
		unsigned short dummy; 
		IDI_CALL       callback;
		ENTITY      *contxt; 
	}   PostCall;
	struct
	{ unsigned char  Req;  
		unsigned char  Rc;   
		unsigned char  pcm[1]; 
	}   GetXlog;
	struct
	{ unsigned char  Req;  
		unsigned char  Rc;   
		unsigned short features;
	}   GetFeatures;
	SERIAL_HOOK  SerialHook;
	struct
	{ unsigned char   Req;
		unsigned char   Rc;
		USB_SEND_REQ    UsbSendRequest; 
		
		
		USB_RECV_NOTIFY usb_recv;       
		
		USB_XMIT_NOTIFY usb_xmit;       
		
		USB_START_DEV   UsbStartDevice; 
		IDI_CALL        callback;       
		ENTITY          *contxt;     
		void **ipac_ptr;    
	} Usb_Msg_old;
	struct
	{ unsigned char Req;
		unsigned char Rc;
		USB_SEND_REQ    pUsbSendRequest;
		
		
		USB_RECV_NOTIFY p_usb_recv;     
		
		USB_XMIT_NOTIFY p_usb_xmit;     
		
		void            *ipac_ptr;      
	} Usb_Msg;
	PORTDRV_HOOK PortdrvHook;
	SLIENTRYPOINT_REQ   sliEntryPointReq;
	struct {
		unsigned char Req;
		unsigned char Rc;
		diva_xdi_stream_interface_t info;
	} xdi_stream_info;
	struct {
		unsigned char Req;
		unsigned char Rc;
		diva_xdi_get_extended_xdi_features_t info;
	} xdi_extended_features;
	struct {
		unsigned char Req;
		unsigned char Rc;
		diva_xdi_get_adapter_sdram_bar_t info;
	} xdi_sdram_bar;
	struct {
		unsigned char Req;
		unsigned char Rc;
		diva_xdi_get_capi_parameters_t info;
	} xdi_capi_prms;
	struct {
		ENTITY           e;
		diva_didd_adapter_notify_t info;
	} didd_notify;
	struct {
		ENTITY           e;
		diva_didd_add_adapter_t   info;
	} didd_add_adapter;
	struct {
		ENTITY           e;
		diva_didd_remove_adapter_t info;
	} didd_remove_adapter;
	struct {
		ENTITY             e;
		diva_didd_read_adapter_array_t info;
	} didd_read_adapter_array;
	struct {
		ENTITY             e;
		diva_didd_get_cfg_lib_ifc_t     info;
	} didd_get_cfg_lib_ifc;
	struct {
		unsigned char Req;
		unsigned char Rc;
		diva_xdi_get_logical_adapter_number_s_t info;
	} xdi_logical_adapter_number;
	struct {
		unsigned char Req;
		unsigned char Rc;
		diva_xdi_dma_descriptor_operation_t info;
	} xdi_dma_descriptor_operation;
} IDI_SYNC_REQ;
#endif 
