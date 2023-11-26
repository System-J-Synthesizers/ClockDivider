/*
	startup - POR
		- RCC_CRRCR, enable HSI48 and wait for it to be ready.
		- set up the clock using the CLK48 mux (HSI48 is default)
		- enable the usb peripheral
		- switch peripheral on with the PDWN bit in the CNTR register
		- wait for startup time
		- clear the FRES bit in CNTR
		- clear the ISTR register to remove any pending interrupts
		- init registers and packet buffer description table

	USB reset
		- enable usb function with address - and endpoint 0
		- set the EF bit in USB_DADDR and initialise the EP0R register and buffers.
		- During enumeration, the host assigns an address which must be put into ADD[6:0] of USB_DADDR and 
			configure endpoint

	
*/