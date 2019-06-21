#include "MSIP404.h"

//#define base_addr 0x200
#define BASEINDEX 0x800
#define SIZE 32
#define SIZEINDEX 8

EmbeddedDevice::MSIP404::MSIP404(EmbeddedOperations* eops, uint32_t base_addr)
{
	this->base_addr = base_addr;
	this->eops = eops;
}

EmbeddedDevice::MSIP404::~MSIP404()
{
}

void EmbeddedDevice::MSIP404::resetChannel(uint8_t channel)
{
	if (eops->ioperm(base_addr, SIZE, 1) != 0) {
		std::cout << "Permission Failed accessing MSI-404" << std::endl;
		exit(1);
	}
	eops->outb(1, base_addr + channel);
	return;
}

int32_t EmbeddedDevice::MSIP404::readChannel(uint8_t channel)
{
	if (eops->ioperm(base_addr, SIZE, 1) != 0) {
		std::cout << "Permission Failed accessing MSI-404" << std::endl;
		exit(1);
	}

	int channelLSB = 4 * channel;
	int8_t msb = eops->inb(base_addr + channelLSB + 3);
	int8_t smsb = eops->inb(base_addr + channelLSB + 2);
	int8_t tmsb = eops->inb(base_addr + channelLSB + 1);
	int8_t lsb = eops->inb(base_addr + channelLSB);
	int32_t count = lsb + (tmsb << 8) + (smsb << 16) + (msb << 24);
	return count;
}

bool EmbeddedDevice::MSIP404::readIndex(uint8_t channel)
{
	if (eops->ioperm(BASEINDEX, SIZEINDEX, 1) != 0) {
		std::cout << "Permission failed to read index!" << std::endl;
		exit(1);
	}
	int i = 0;
	int channelIndex = 0;
	int j = 0;

	for (i = 0, channelIndex = 0, j = 0; i <= channel; i++, j++) {
		if (j == 2) {
			j = 0;
			channelIndex++;
		}
	}

	if ((eops->inb(BASEINDEX + channelIndex) & (1 << 7)) && (i == 1)) {
		return true;
	}
	else if ((eops->inb(BASEINDEX + channelIndex) & (1 << 5)) && (i == 0)) {
		return true;
	}
	return false;
}

bool EmbeddedDevice::MSIP404::operator!()
{
	if (eops->ioperm(base_addr, SIZEINDEX, 1) != 0) {
		std::cout << "Permission failed to read index!" << std::endl;
		exit(1);
	}

	for (int i = 0; i < 8; i++) {
		eops->outb(1, base_addr + i);
	}
	return true;
}
