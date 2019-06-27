#include "MSIP404.h"

//#define base_addr 0x200
#define BASEINDEX 0x800
#define SIZE 32
#define SIZEINDEX 8

EmbeddedDevice::MSIP404::MSIP404(EmbeddedOperations* eops, uint32_t base_addr)
{
	if (eops->ioperm(base_addr, SIZE, 1) != 0) {
		std::cout << "Permission Failed accessing MSI-404" << std::endl;
		exit(1);
	}

	this->base_addr = base_addr;
	this->eops = eops;
}

EmbeddedDevice::MSIP404::~MSIP404()
{
	this->base_addr = NULL;
	this->eops = NULL;
}

void EmbeddedDevice::MSIP404::resetChannel(uint8_t channel)//need to test
{

	eops->outb(1, base_addr + channel);
	return;
}

int32_t EmbeddedDevice::MSIP404::readChannel(uint8_t channel)//need to test
{

	int channelLSB = 4 * channel;
	int8_t msb = eops->inb(base_addr + channelLSB + 3);
	int8_t smsb = eops->inb(base_addr + channelLSB + 2);
	int8_t tmsb = eops->inb(base_addr + channelLSB + 1);
	int8_t lsb = eops->inb(base_addr + channelLSB);
	int32_t count = lsb + (tmsb << 8) + (smsb << 16) + (msb << 24);
	return count;
}

bool EmbeddedDevice::MSIP404::readIndex(uint8_t channel)//need to test
{

	int i = 0;
	int channelIndex = 0;
	int j = 0;

	for (i = 0, channelIndex = 0, j = 0; i < channel; i++, j++) {
		if (j == 1) {
			j = 0;
			channelIndex++;
		}
	}

	if ((eops->inb(BASEINDEX + channelIndex) & (1 << 7)) && (j == 0)) {
		return true;
	}
	else if ((eops->inb(BASEINDEX + channelIndex) & (1 << 5)) && (j == 1)) {
		return true;
	}
	return false;
}

bool EmbeddedDevice::MSIP404::operator!()
{

	for (int i = 0; i < 8; i++) {
		eops->outb(1, base_addr + i);
	}
	return true;
}
