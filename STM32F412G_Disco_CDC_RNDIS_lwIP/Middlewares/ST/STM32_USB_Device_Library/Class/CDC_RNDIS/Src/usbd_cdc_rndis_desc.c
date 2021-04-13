/*
 * usbd_cdc_rndis_desc.c
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

#include <usbd_def.h>
#include <usb_class_codes.h>
#include <usbd_cdc_desc.h>
#include <usb_descriptor_types.h>

USBDeviceDescriptor_t USBD_DeviceDesc = {
		sizeof(USBDeviceDescriptor_t),
		USB_DESC_TYPE_DEVICE,
		0x0200,
		USB_DEVICE_CLASS_CDC,
		USB_DEVICE_SUBCLASS_CDC,
		USB_DEVICE_PROTOCOL_CDC,
		USB_MAX_EP0_SIZE,
		USBD_VID,
		USBD_PID,
		0x0200,
		USBD_IDX_MFC_STR,
		USBD_IDX_PRODUCT_STR,
		USBD_IDX_SERIAL_STR,
		USBD_MAX_NUM_CONFIGURATION
};
