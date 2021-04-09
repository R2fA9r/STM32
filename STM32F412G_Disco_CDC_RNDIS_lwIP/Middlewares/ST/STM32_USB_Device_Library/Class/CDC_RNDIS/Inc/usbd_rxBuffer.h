/*
 * usbd_rxBuffer.h
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

#ifndef INC_USBD_RXBUFFER_H_
#define INC_USBD_RXBUFFER_H_
#include <stdbool.h>
#include <macros.h>
#include <usbd_cdc_rndis_if.h>

typedef struct {
	uint32_t 	len;
	uint8_t 	buf[CDC_USERBUFFER_SIZE];
} _ATTR_PACKED rxBufferElem_t;


#define RXBUF_NUMELEM		5


// initialize the structures
void usbdRxBufferInit(void);
// add an item to the buffer
// return true if successful, false if an error occurred (buffers full, buffer array smaller than len etc.)
bool usbdRxBufferPush(uint8_t* data, uint32_t len);
// returns true if there are no elements available
bool usbdRxBufferEmpty(void);
// returns the number of elements
uint32_t usbdRxBufferCount(void);
// return the next element on the queue or NULL if there is none in the queue.
rxBufferElem_t* usbdRxBufferPop();


#endif /* INC_USBD_RXBUFFER_H_ */
