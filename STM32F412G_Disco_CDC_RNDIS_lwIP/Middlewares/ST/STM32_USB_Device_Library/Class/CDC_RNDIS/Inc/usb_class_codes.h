/*
 * usb_class_codes.h
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

#ifndef INC_USB_CLASS_CODES_H_
#define INC_USB_CLASS_CODES_H_

//=================
//Device classes (see https://www.usb.org/defined-class-codes)
//=================

#define USB_DEVICE_CLASS_GEN		0x00
#define USB_DEVICE_CLASS_AUDIO		0x01
#define USB_DEVICE_CLASS_CDC    	0x02
#define USB_DEVICE_CLASS_HID		0x03
#define USB_DEVICE_CLASS_PHYS		0x05
#define USB_DEVICE_CLASS_IMG		0x06
#define USB_DEVICE_CLASS_PRNT		0x07
#define USB_DEVICE_CLASS_MSTOR		0x08
#define USB_DEVICE_CLASS_HUB		0x09
#define USB_DEVICE_CLASS_CDCDATA	0x0a
#define USB_DEVICE_CLASS_SMARTC		0x0b
#define USB_DEVICE_CLASS_CSEC		0x0d
#define USB_DEVICE_CLASS_VIDEO		0x0e
#define USB_DEVICE_CLASS_HEALTH		0x0f
#define USB_DEVICE_CLASS_AV			0x10
#define USB_DEVICE_CLASS_BBDEV		0x11
#define USB_DEVICE_CLASS_USBBRIDGE	0x12
#define USB_DEVICE_CLASS_DIAG		0xdc
#define USB_DEVICE_CLASS_WCTR		0xe0
#define USB_DEVICE_CLASS_MISC		0xef
#define USB_DEVICE_CLASS_APPSPEC	0xfe
#define USB_DEVICE_CLASS_VENDOR		0xff

//CDC communication interface class
// for Linux/Win7:
#define CDC_INTERFACE_CLASS_COMMUNICATION      0x02
// for Win10
#define CDC_INTERFACE_CLASS_RNDIS              0xEF

//CDC communication interface subclass
// for Linux/Win7:
#define CDC_INTERFACE_SUBCLASS_ACM             0x02
// for Win10
#define CDC_INTERFACE_SUBCLASS_RNDIS           0x04

//CDC communication interface protocol
// for Linux/Win7:
#define CDC_INTERFACE_PROTOCOL_VENDOR_SPECIFIC 0xFF
// for Win10
#define CDC_INTERFACE_PROTOCOL_RNDIS           0x01


//CDC descriptor types
#define CDC_CS_INTERFACE 0x24
#define CDC_CS_ENDPOINT  0x25

//CDC descriptor subtypes
#define CDC_HEADER_DESC_SUBTYPE          0x00
#define CDC_CALL_MANAGEMENT_DESC_SUBTYPE 0x01
#define CDC_ACM_DESC_SUBTYPE             0x02
#define CDC_UNION_DESC_SUBTYPE           0x06

//Endpoint types
#define USB_ENDPOINT_TYPE_MASK        0x03
#define USB_ENDPOINT_TYPE_CONTROL     0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS 0x01
#define USB_ENDPOINT_TYPE_BULK        0x02
#define USB_ENDPOINT_TYPE_INTERRUPT   0x03

//CDC data interface class
#define CDC_INTERFACE_CLASS_DATA    0x0A
//CDC data interface subclass
#define CDC_INTERFACE_SUBCLASS_DATA 0x00
//CDC data interface protocol
#define CDC_INTERFACE_PROTOCOL_DATA 0x00

//==================
//Device subclasses
//===================
#define USB_DEVICE_SUBCLASS_CDC 	0x00
#define USB_DEVICE_PROTOCOL_CDC 	0x00



#endif /* INC_USB_CLASS_CODES_H_ */
