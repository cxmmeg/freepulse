
#ifndef __SPI_h__
#define __SPI_h__

#include "System.h"
#include "stm32f4xx_spi.h"

enum SPI_Channel {
	spi_c1, spi_c2, spi_c3, spi_c4, spi_c5		
};

class SPI_Interface {
public:
	SPI_Channel SPIc;
	SPI_TypeDef* SPIx;

	SPI_Interface(SPI_Channel SPIc) {
		this->SPIc = SPIc;
		switch(SPIc){
			case spi_c1:
				this->SPIx = SPI1;
				break;
			case spi_c2:
				this->SPIx = SPI2;
				break;
			case spi_c3:
				this->SPIx = SPI3;
				break;
			case spi_c4:
				this->SPIx = SPI4;
				break;
			case spi_c5:
				this->SPIx = SPI5;
				break;
		}
	}

	void begin(){
		SPI_InitTypeDef SPI_InitStruct;

		// Initialize alternate function pins A5, A6, and A7
		configure_GPIO(PA5, NO_PU_PD, ALT); 
		configure_GPIO(PA6, NO_PU_PD, ALT); 
		configure_GPIO(PA7, NO_PU_PD, ALT); 
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); // SCLK
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); // MISO
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); // MOSI

		switch(this->SPIc){
			case spi_c1:
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
				break;
			case spi_c2:
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
				break;
			case spi_c3:
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
				break;
			case spi_c4:
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
				break;
			case spi_c5:
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
				break;
		}

		configure(&SPI_InitStruct);
		SPI_Init(this->SPIx, &SPI_InitStruct);
		SPI_Cmd(this->SPIx, ENABLE);
	};

	void configure(SPI_InitTypeDef* SPI_InitStruct){
		/* SPI default settings: 
		 * - Full duplex mode
		 * - Communicate as master
		 * - 8 bit wide communication
		 * - SPI Mode 0
		 * - Internal NSS management
		 * - 12.5 MHz Baud Rate TODO: check this again
		 * - MSB first
		 */
		SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
		SPI_InitStruct->SPI_Mode = SPI_Mode_Master;     
		SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b; 
		SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;        
		SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;      
		SPI_InitStruct->SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; 
		SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
		SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;
	};

	uint8_t transfer(uint8_t data){
		this->SPIx->DR = data; // write data to be transmitted to the SPI data register
		while( !(this->SPIx->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
		while( !(this->SPIx->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
		while( this->SPIx->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
		return this->SPIx->DR; // return received data from SPI data register
	};

	/* void setClockDivider(uint8_t clock_divider){}; */
	/* void setDataMode(){}; */
};
#endif
