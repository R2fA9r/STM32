/*
 * usbd_cdc_desc.h
 *
 *  Created on: 9 Apr 2021
 *      Author: Rolf S. Adelsberger
 *
 *
 * MIT License
 * -----------
 * Copyright 2021 Rolf S. Adelsberger
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef INC_USBD_CDC_DESC_H_
#define INC_USBD_CDC_DESC_H_

#define USBD_VID                      0x0483
#define USBD_PID                      0xa4a2  /* Replace '0xaaaa' with your device product ID */
#define USBD_LANGID_STRING            0x409  /* Replace '0xbbb' with your device language ID */
#define USBD_MANUFACTURER_STRING      "Sensoryx" /* Add your manufacturer string */
#define USBD_PRODUCT_HS_STRING        "Sensoryx RNDIS in HS Mode" /* Add your product High Speed string */
#define USBD_PRODUCT_FS_STRING        "Sensoryx RNDIS in FS Mode" /* Add your product Full Speed string */
#define USBD_CONFIGURATION_HS_STRING  "RNDIS Config" /* Add your configuration High Speed string */
#define USBD_INTERFACE_HS_STRING      "RNDIS Interface" /* Add your Interface High Speed string */
#define USBD_CONFIGURATION_FS_STRING  "RNDIS Config" /* Add your configuration Full Speed string */
#define USBD_INTERFACE_FS_STRING      "RNDIS Interface" /* Add your Interface Full Speed string */




#endif /* INC_USBD_CDC_DESC_H_ */
