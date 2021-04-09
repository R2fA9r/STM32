/**
 ******************************************************************************
 * @file    LwIP/LwIP_TCP_Echo_Server/Src/app_ethernet.c
 * @author  MCD Application Team
 * @brief   Ethernet specefic module
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/dhcp.h"
#include "app_ethernet.h"
#ifdef USE_LCD
#include "lcd_log.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Notify the User about the nework interface config status
 * @param  netif: the network interface
 * @retval None
 */
void User_notification(struct netif *netif) {
	if (netif_is_up(netif)) {
		User_signalNetifUp();
	} else {
		User_signalNetifDown();
	}

}

void User_signalNetifUp() {
	BSP_LED_On(LED_BLUE);
	BSP_LED_Off(LED_ORANGE);
}

void User_signalNetifDown() {
	BSP_LED_On(LED_ORANGE);
	BSP_LED_Off(LED_BLUE);
}

/**
 * @brief  This function notify user about link status changement.
 * @param  netif: the network interface
 * @retval None
 */
void ethernetif_notify_conn_changed(struct netif *netif) {

	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	if (netif_is_link_up(netif)) {

		IP_ADDR4(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
		IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
		IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

		netif_set_addr(netif, &ipaddr, &netmask, &gw);

		/* When the netif is fully configured this function must be called.*/
		netif_set_up(netif);
		User_signalNetifUp();
	} else {

		/*  When the netif link is down this function must be called.*/
		netif_set_down(netif);
		User_signalNetifDown();

	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
