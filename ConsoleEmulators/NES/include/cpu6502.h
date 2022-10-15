#pragma once
//#include <cstdint>

class NESBusSystem;

class CPU
{
public:
	CPU(NESBusSystem* nesBus);
	~CPU() = default;

	void writeData(uint16_t address, uint8_t data);
	uint8_t readData(uint16_t address);

	void reset();
	void clock();

public:
	// Registers of 6502 core (public to examine and debug later on)
	uint8_t A;
	uint8_t X;
	uint8_t Y;
	uint16_t PC; // Program Counter
	uint8_t SP; // Stack Pointer
	uint8_t status; // STATUS register

	enum StatusFlags
	{
		C = (1 << 0), // Carry bit
		Z = (1 << 1), // Zero bit
		I = (1 << 2), // Interrupt bit
		D = (1 << 3), // Decimal bit
		B = (1 << 4), // Break bit
		U = (1 << 5), // Unused bit 5, always set to 1
		V = (1 << 6), // Overflow bit
		N = (1 << 7)  // Negative bit
	};

private:
	NESBusSystem* bus;
};