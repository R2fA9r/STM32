/**
  ******************************************************************************
  * @file    usbd_cdc_rndis_if_template.h
  * @author  MCD Application Team
  * @brief   Header for usbd_cdc_rndis_if.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_RNDIS_IF_H
#define __USBD_CDC_RNDIS_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_rndis.h"
#include <stdbool.h>
#include <macros.h>

/* USB handler declaration */
extern USBD_HandleTypeDef  USBD_Device;

extern volatile uint8_t CDC_RNDISInitialized;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*
 * this is the MAC address of the Network adapter how it presents itself to the operating system
 * it is DIFFERENT from the MAC address
 */
#define CDC_RNDIS_MAC_STR_DESC                 (uint8_t *)"deadbeefabcd"
#define CDC_RNDIS_MAC_ADDR0                    0xdeU
#define CDC_RNDIS_MAC_ADDR1                    0xadU
#define CDC_RNDIS_MAC_ADDR2                    0xbeU
#define CDC_RNDIS_MAC_ADDR3                    0xefU
#define CDC_RNDIS_MAC_ADDR4                    0xabU
#define CDC_RNDIS_MAC_ADDR5                    0xcdU

#define USBD_CDC_RNDIS_VENDOR_DESC             "Sensoryx"
#define USBD_CDC_RNDIS_LINK_SPEED              100000U /* 10Mbps */
#define USBD_CDC_RNDIS_VID                     0x0483U

/* Max Number of Trials waiting for Tx ready */
#define CDC_RNDIS_MAX_TX_WAIT_TRIALS           1000000U

/* Ethernet Maximum Segment size, typically 1514 bytes */
#define CDC_RNDIS_ETH_MAX_SEGSZE                            1514U

#define CDC_USERBUFFER_SIZE									(CDC_RNDIS_ETH_MAX_SEGSZE + 100)

#define CDC_RNDIS_CONNECT_SPEED_UPSTREAM                    0x1E000000U
#define CDC_RNDIS_CONNECT_SPEED_DOWNSTREAM                  0x1E000000U


extern USBD_CDC_RNDIS_ItfTypeDef                    USBD_CDC_RNDIS_fops;


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __USBD_CDC_RNDIS_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
