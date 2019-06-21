// Assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
#include "../..//PCM3718.h"
#include "../..//DAC06.h"
#include "../..//MSIP404.h"
#include "../..//EmbeddedOperations.h"

int main() {
	EmbeddedOperations eops;
	EmbeddedDevice::DAC06 DAC(&eops, 0x320);
	EmbeddedDevice::MSIP404 MSI(&eops, 0x200);
	EmbeddedDevice::PCM3718 PCM(&eops, 0x300);

	DAC.analogOutputRaw(0, 0x0FFF);
	DAC.analogOutputVoltage(0, 5);
	DAC.analogOutputVoltage(0, -5);
	DAC.analogOutputVoltage(0, 0);
	MSI.operator!();


	return 0;
}