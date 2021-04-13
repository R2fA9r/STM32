/*
 * usb_descriptor_types.h
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

#ifndef INC_USB_DESCRIPTOR_TYPES_H_
#define INC_USB_DESCRIPTOR_TYPES_H_

#include <macros.h>
#include <usb_class_codes.h>
#include <stdint.h>

typedef struct {
	uint8_t				bLength;
	uint8_t				bDescriptorType;
	uint16_t			bcdUSB;
	uint8_t				bDeviceClass;
	uint8_t				bDeviceSubClass;
	uint8_t				bDeviceProtocol;
	uint8_t				bMaxPacketSize0;
	uint16_t			idVendor;
	uint16_t			idProduct;
	uint16_t			bcdDevice;
	uint8_t				iManufacturer;
	uint8_t				iProduct;
	uint8_t				iSerialNumber;
	uint8_t				bNumConfigurations;
}_ATTR_PACKED USBDeviceDescriptor_t;

// inspired from https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/usbspec/ns-usbspec-_usb_device_qualifier_descriptor
typedef struct {
	uint8_t 			bLength;
	uint8_t 			bDescriptorType;
	uint16_t 			bcdUSB;
	uint8_t 			bDeviceClass;
	uint8_t 			bDeviceSubClass;
	uint8_t 			bDeviceProtocol;
	uint8_t 			bMaxPacketSize0;
	uint8_t 			bNumConfigurations;
	uint8_t 			bReserved;
}_ATTR_PACKED USBDeviceQualifierDescriptor_t;

// see: https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/usbspec/ns-usbspec-_usb_configuration_descriptor
typedef struct {
	uint8_t 			bLength;
	uint8_t 			bDescriptorType;
	uint16_t 			wTotalLength;
	uint8_t 			bNumInterfaces;
	uint8_t 			bConfigurationValue;
	uint8_t 			iConfiguration;
	uint8_t 			bmAttributes;
	uint8_t 			MaxPower;
}_ATTR_PACKED USBConfigurationDescriptor_t;

typedef struct {
	uint8_t  			bLength;
	uint8_t  			bDescriptorType;
	uint8_t  			bInterfaceNumber;
	uint8_t  			bAlternateSetting;
	uint8_t  			bNumEndpoints;
	uint8_t  			bInterfaceClass;
	uint8_t  			bInterfaceSubClass;
	uint8_t  			bInterfaceProtocol;
	uint8_t  			iInterface;
}_ATTR_PACKED USBInterfaceDescriptor_t;

typedef struct {
	uint8_t				bLength;
	uint8_t				bHIDDescriptorType;
	uint16_t			bcdHID;
	uint8_t				bCountryCode;
	uint8_t				bNumDescriptors;
	uint8_t				bClassDescriptorType;
	uint16_t			wDescriptorLength;
}_ATTR_PACKED USBHIDDescriptor_t;


// see: https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/usbspec/ns-usbspec-_usb_endpoint_descriptor
typedef struct  {
	uint8_t  			bLength;
	uint8_t  			bDescriptorType;
	uint8_t  			bEndpointAddress;
	uint8_t  			bmAttributes;
	uint16_t 			wMaxPacketSize;
	uint8_t 			bInterval;
} _ATTR_PACKED USBEndpointDescriptor;

// see https://www.usb.org/documents?search=&type%5B0%5D=55&items_per_page=50
typedef struct  {
   uint8_t 				bFunctionLength;
   uint8_t 				bDescriptorType;
   uint8_t 				bDescriptorSubtype;
   uint16_t 			bcdCDC;
} _ATTR_PACKED USBCDCHeaderDescriptor_t;

typedef struct  {
	uint8_t 			bFunctionLength;
	uint8_t 			bDescriptorType;
	uint8_t 			bDescriptorSubtype;
	uint8_t 			bmCapabilities;
	uint8_t 			bDataInterface;
}_ATTR_PACKED USBCDCCallManagementDescriptor_t;

typedef struct {
	uint8_t 			bFunctionLength;
	uint8_t 			bDescriptorType;
	uint8_t 			bDescriptorSubtype;
	uint8_t 			bmCapabilities;
}_ATTR_PACKED USBCDCACMDescriptor_t;

typedef struct {
	uint8_t 			bFunctionLength;
	uint8_t 			bDescriptorType;
	uint8_t 			bDescriptorSubtype;
	uint8_t 			bControlInterface;
	uint8_t 			bSubordinateInterface;
}_ATTR_PACKED USBCDCUnionDescriptor_t;


typedef struct {
	// regular configuration descriptor
	USBConfigurationDescriptor_t 		configDescriptor;

	// communication class itf
	USBInterfaceDescriptor_t			commInterfaceDescriptor;

	// CDC header functional descriptor
	USBCDCHeaderDescriptor_t			cdcHeaderDescriptor;

	// CDC call mgmt functional descriptor
	USBCDCCallManagementDescriptor_t	cdcCallManagementDescriptor;

	// CDC abstract control mgmt func descr
	USBCDCACMDescriptor_t				cdcACMDescriptor;

	// CDC union funct descr
	USBCDCUnionDescriptor_t				cdcUnionDescriptor;

	// descriptor for notification endpoint
	USBEndpointDescriptor				notificationEndpointDescriptor;

	// data class interface descriptor
	USBInterfaceDescriptor_t			dataInterfaceDescriptor;

	// data IN endpoint desc
	USBEndpointDescriptor				dataINEndpointDescriptor;

	// data OUT endpint
	USBEndpointDescriptor				dataOUTEndpointDescriptor;

}_ATTR_PACKED USBDeviceConfigurationDescriptor_t;

#endif /* INC_USB_DESCRIPTOR_TYPES_H_ */
