/**
 ******************************************************************************
 * @file    USB_Device/MSC_Standalone/Src/main.c
 * @author  MCD Application Team
 * @brief   USB device Mass storage demo main file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------ */
#include "main.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "app_ethernet.h"
#include <netif.h>
#include <dhcpServer.h>

#define UDP_PUSH_DATA 1

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
USBD_HandleTypeDef USBD_Device;

// global net interface structure
struct netif gNetif;

/* Private function prototypes ----------------------------------------------- */
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Netif_Config(void);

/* Private functions --------------------------------------------------------- */

void link_up() {
	netif_set_link_up(&gNetif);
}

void link_down() {
	netif_set_link_down(&gNetif);
}
//
//void data_received() {
//	ethernetif_input(&gNetif);
//}

int main(void) {
	/* STM32F412xx HAL library initialization */
	HAL_Init();

	/* Configure the System clock to have a frequency of 100 MHz */
	SystemClock_Config();

	HAL_SYSTICK_Config(SystemCoreClock / 1000U);

	/* Configure LED_RED and LED_BLUE */
	BSP_LED_Init(LED_GREEN);
	BSP_LED_Init(LED_ORANGE);
	BSP_LED_Init(LED_RED);
	BSP_LED_Init(LED_BLUE);



	/* Init Device Library */
	USBD_Init(&USBD_Device, &USBD_Class_Desc, 0);
	USBD_RegisterClass(&USBD_Device, USBD_CDC_RNDIS_CLASS);
	USBD_CDC_RNDIS_RegisterInterface(&USBD_Device, &USBD_CDC_RNDIS_fops);

	/* Start Device Process */
	USBD_Start(&USBD_Device);

	while(((USBD_CDC_RNDIS_HandleTypeDef *)((&USBD_Device)->pClassData))->State != CDC_RNDIS_STATE_INITIALIZED){

	}

	lwip_init();

	/* Configure the Network interface */
	Netif_Config();

	while (dhcpSrvInit() != ERR_OK) ;


#if UDP_PUSH_DATA
	struct udp_pcb *srx_pcb = NULL;
	err_t udpError;
	srx_pcb = udp_new();
	struct pbuf *p;
	uint8_t msg[6] = {'h','e','l','l','o','!'};
	if(srx_pcb != NULL) {
		udp_bind(srx_pcb,IP_ADDR_ANY,0xabcd);
	}


#endif

	/* Notify user about the network interface config */
	User_notification(&gNetif);


	uint32_t tick = HAL_GetTick();

	IPAddress_t dstAddress = {.addr_a={192,168,7,2}};

	/* Infinite loop */
	while (1) {

		/* Read a received packet from the Ethernet buffers and send it
		 to the lwIP for handling */
		ethernetif_input(&gNetif);

		/* Handle timeouts */
		sys_check_timeouts();

#if UDP_PUSH_DATA

		if((HAL_GetTick() - tick) > 1000) {
			tick = HAL_GetTick();
			if(srx_pcb != NULL) {
				p = pbuf_alloc(PBUF_TRANSPORT,sizeof(msg),PBUF_RAM);
				memcpy(p->payload,msg,sizeof(msg));
				//udpError = udp_sendto(srx_pcb,p,IP_ADDR_BROADCAST,0xabcd);

				udpError = udp_sendto(srx_pcb,p,(ip_addr_t *)&dstAddress,0xabcd);
				pbuf_free(p);

				UNUSED(udpError);
//				if(udpError != ERR_OK) {
//					BSP_LED_Toggle(LED_GREEN);
//				}
			}
		}
#endif


	}

}

/**
 * @brief  Configures the network interface
 * @param  None
 * @retval None
 */
static void Netif_Config(void) {
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
	IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
	IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);

	/* Add the network interface */
	netif_add(&gNetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

	/* Registers the default network interface. */
	netif_set_default(&gNetif);

	if (netif_is_link_up(&gNetif)) {
		/* When the netif is fully configured this function must be called */
		netif_set_up(&gNetif);
	} else {
		/* When the netif link is down this function must be called */
		netif_set_down(&gNetif);
	}

	/* Set the link callback function, this function is called on change of link status */
	netif_set_link_callback(&gNetif, ethernetif_update_config);
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 100000000
 *            HCLK(Hz)                       = 100000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 200
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 3
 *         The USB clock configuration from PLLI2S:
 *            PLLI2SM                        = 8
 *            PLLI2SN                        = 192
 *            PLLI2SQ                        = 4
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE()
	;

	/* The voltage scaling allows optimizing the power consumption when the
	 * device is clocked below the maximum system frequency, to update the
	 * voltage scaling value regarding system frequency refer to product
	 * datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 200;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	RCC_OscInitStruct.PLL.PLLR = 2;
	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);

	if (ret != HAL_OK) {
		Error_Handler();
	}

	/* Select PLLSAI output as USB clock source */
	PeriphClkInitStruct.PLLI2S.PLLI2SM = 8;
	PeriphClkInitStruct.PLLI2S.PLLI2SQ = 4;
	PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CK48;
	PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLI2SQ;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 * clocks dividers */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
	if (ret != HAL_OK) {
		Error_Handler();
	}
}


/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void) {
	/* User may add here some code to deal with this error */
	while (1) {
	}
}

/**
 * @brief This function provides accurate delay (in milliseconds) based
 *        on SysTick counter flag.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @param Delay: specifies the delay time length, in milliseconds.
 * @retval None
 */

void HAL_Delay(__IO uint32_t Delay) {
	while (Delay) {
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
			Delay--;
		}
	}
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t * file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line
	 * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
	 * line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
