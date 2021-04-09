/**
  ******************************************************************************
  * @file    usbd_cdc_rndis_if_template.c
  * @author  MCD Application Team
  * @brief   Source file for USBD CDC_RNDIS interface template
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

/* Includes ------------------------------------------------------------------*/


#include "main.h"
#include <stm32412g_discovery.h>
#include <usbd_cdc_rndis_if.h>
#include <usbd_rxBuffer.h>


/* Private variables */
/**< Internal flag indicating if a RNDIS message is waiting to be returned to the host. */
USBD_CDC_RNDIS_StateTypeDef CurrRNDISState = CDC_RNDIS_STATE_UNINITIALIZED;
/**< Current RNDIS state of the adapter, a value from the \ref RNDIS_States_t enum. */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t UserRxBuffer[CDC_USERBUFFER_SIZE] __ALIGN_END; /* Received Data over USB are stored in this buffer */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN static uint8_t UserTxBuffer[CDC_USERBUFFER_SIZE] __ALIGN_END; /* Received Data over CDC_RNDIS (CDC_RNDIS interface) are stored in this buffer */


volatile uint8_t CDC_RNDISInitialized = 0U;

/* Private function prototypes -----------------------------------------------*/
static int8_t CDC_RNDIS_Itf_Init(void);
static int8_t CDC_RNDIS_Itf_DeInit(void);
static int8_t CDC_RNDIS_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t CDC_RNDIS_Itf_Receive(uint8_t *pbuf, uint32_t *Len);
static int8_t CDC_RNDIS_Itf_TransmitCplt(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);
static int8_t CDC_RNDIS_Itf_Process(USBD_HandleTypeDef *pdev);

USBD_CDC_RNDIS_ItfTypeDef USBD_CDC_RNDIS_fops = {
  CDC_RNDIS_Itf_Init,
  CDC_RNDIS_Itf_DeInit,
  CDC_RNDIS_Itf_Control,
  CDC_RNDIS_Itf_Receive,
  CDC_RNDIS_Itf_TransmitCplt,
  CDC_RNDIS_Itf_Process,
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  CDC_RNDIS_Itf_Init
  *         Initializes the CDC_RNDIS media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */

static int8_t CDC_RNDIS_Itf_Init(void)
{
  if (CDC_RNDISInitialized == 0U)
  {
    /*
      Initialize the LwIP stack
      Add your code here
    */

	  USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *)(USBD_Device.pClassData);
	  hcdc_cdc_rndis->LinkStatus = 1U;
	  link_up();

	  usbdRxBufferInit();

	  CDC_RNDISInitialized = 1U;
  }

  /* Set Application Buffers */
  (void)USBD_CDC_RNDIS_SetTxBuffer(&USBD_Device, UserTxBuffer, 0U);
  (void)USBD_CDC_RNDIS_SetRxBuffer(&USBD_Device, UserRxBuffer);

  return (0);
}

/**
  * @brief  CDC_RNDIS_Itf_DeInit
  *         DeInitializes the CDC_RNDIS media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_DeInit(void)
{
  USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *)(USBD_Device.pClassData);

  /*
     Add your code here
  */

  CDC_RNDISInitialized = 0;

  /* Notify application layer that link is down */
  hcdc_cdc_rndis->LinkStatus = 0U;
  link_down();

  return (0);
}

/**
  * @brief  CDC_RNDIS_Itf_Control
  *         Manage the CDC_RNDIS class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
  USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *)(USBD_Device.pClassData);

  switch (cmd)
  {
    case CDC_RNDIS_SEND_ENCAPSULATED_COMMAND:
      /* Add your code here */
    	// nothing needs to be done AFAIK
      break;

    case CDC_RNDIS_GET_ENCAPSULATED_RESPONSE:
      /* Check if this is the first time we enter */
      if (hcdc_cdc_rndis->LinkStatus == 0U)
      {
        /* Setup the Link up at TCP/IP stack level */
        /*
          Add your code here
        */
      }
      /* Add your code here */
      break;

    default:
      /* Add your code here */
      break;
  }

  UNUSED(length);
  UNUSED(pbuf);

  return (0);
}

/**
  * @brief  CDC_RNDIS_Itf_Receive
  *         Data received over USB OUT endpoint are sent over CDC_RNDIS interface
  *         through this function.
  * @param  Buf: Buffer of data to be transmitted
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */

static int8_t CDC_RNDIS_Itf_Receive(uint8_t *Buf, uint32_t *Len) {
	/* Get the CDC_RNDIS handler pointer */
	USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *) (USBD_Device.pClassData);

	/* Call Eth buffer processing */

	hcdc_cdc_rndis->RxState = 1U;

	usbdRxBufferPush(Buf,*Len);

	// clear the receive buffer
	// todo: most likely not needed...
	memset(UserRxBuffer, 0, CDC_USERBUFFER_SIZE);

	return USBD_OK;
}

/**
  * @brief  CDC_RNDIS_Itf_TransmitCplt
  *         Data transmitted callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @param  epnum: EP number
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);
  return (0);
}

/**
  * @brief  CDC_RNDIS_Itf_Process
  *         Data received over USB OUT endpoint are sent over CDC_RNDIS interface
  *         through this function.
  * @param  pdef: pointer to the USB Device Handle
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_Process(USBD_HandleTypeDef *pdev)
{
  /* Get the CDC_RNDIS handler pointer */
  USBD_CDC_RNDIS_HandleTypeDef   *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *)(pdev->pClassData);

  if ((hcdc_cdc_rndis != NULL) && (hcdc_cdc_rndis->LinkStatus != 0U))
  {
    /*
       Add your code here
       Read a received packet from the Ethernet buffers and send it
       to the lwIP for handling
    */
  }

  return (0);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
