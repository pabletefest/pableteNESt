#pragma once
//#include <cstdint>
#include <string>
#include <vector>

class NESBusSystem;

// CPU 2A03/2A07 (6502 NES console processor)

class CPU
{
public:
	CPU(NESBusSystem* nesBus);
	~CPU() = default;

	void reset();
	void irq();
	void nmi();
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

	bool isLastCompleted() const;

private:

	void writeData(uint16_t address, uint8_t data);
	uint8_t readData(uint16_t address);

	uint8_t getStatusFlag(StatusFlags flag);
	void setStatusFlag(StatusFlags flag, bool isSet);

	// ADDRESSING MODES
	uint8_t IMP();	uint8_t IMM();
	uint8_t ZP0();	uint8_t ZPX();
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();
	uint8_t ABY();	uint8_t IND();
	uint8_t IZX();	uint8_t IZY();

	// INSTRUCTIONS
	uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

	// Default method for illegal opcodes (for now)
	// Acts as a NOP instruction
	uint8_t XXX();

private:
	NESBusSystem* bus = nullptr;

	uint8_t fetched = 0x00; // Operand value for the ALU
	uint16_t effectiveAddr = 0x0000; // Address of operand
	uint16_t relativeAddr = 0x0000; // Relative address for branch instructions
	uint8_t currentOpcode = 0x00; // Last fetched opcode
	uint8_t instructionCycles = 0;
	uint64_t debugTotalCyclesElapsed = 0;

	// Fetch data from the effective memory addres
	uint8_t fetch();

	struct Instruction
	{
		std::string name;
		uint8_t (CPU::* instruction)(void) = nullptr;
		uint8_t (CPU::* addressMode)(void) = nullptr;
		uint8_t cyclesRequired = 0;
	};

	std::vector<Instruction> instructionsTable;
};