/*
 * usbd_rxBuffer.c
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

#include <usbd_rxBuffer.h>

rxBufferElem_t m_rxBufQueue[RXBUF_NUMELEM];
uint32_t m_rxBufQHead;
uint32_t m_rxBufQTail;

// initialize the structures
void usbdRxBufferInit(void) {
	m_rxBufQHead = 0;
	m_rxBufQTail = 0;
}

// add an item to the buffer
// return true if successful, false if an error occurred (buffers full, buffer array smaller than len etc.)
bool usbdRxBufferPush(uint8_t* data, uint32_t len) {
	bool success = false;

	if(usbdRxBufferCount() < RXBUF_NUMELEM) {

		memcpy(m_rxBufQueue[m_rxBufQHead].buf,data,len);
		m_rxBufQueue[m_rxBufQHead].len = len;
		m_rxBufQHead = (m_rxBufQHead+1) % RXBUF_NUMELEM;
		success = true;
	}

	return success;
}


// returns true if there are no elements available
bool usbdRxBufferEmpty(void) {
	return (m_rxBufQHead==m_rxBufQTail)? true: false;
}


// returns the number of elements
uint32_t usbdRxBufferCount(void) {
	if(usbdRxBufferEmpty()) {
		return 0;
	}

	if(m_rxBufQTail < m_rxBufQHead) {
		return (m_rxBufQHead - m_rxBufQTail);
	} else {
		return (RXBUF_NUMELEM - m_rxBufQTail + m_rxBufQHead);
	}
}


// return the next element on the queue or NULL if there is none in the queue.
rxBufferElem_t* usbdRxBufferPop() {
	rxBufferElem_t* ret = NULL;

	if(!usbdRxBufferEmpty()) {
		ret = m_rxBufQueue + m_rxBufQTail;
		m_rxBufQTail = (m_rxBufQTail+1) % RXBUF_NUMELEM;
	}

	return ret;
}
