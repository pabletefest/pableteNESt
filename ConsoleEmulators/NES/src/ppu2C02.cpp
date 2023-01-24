#include "ppu2C02.h"

namespace nes
{
	constexpr uint16_t baseNameTableAddresses[4] = { 0x2000, 0x2400, 0x2800, 0x2C00 };

	enum BG_FETCHING_STEP
	{
		FETCH_NT_BYTE = 0,
		FETCH_AT_BYTE = 2,
		FETCH_PT_LOW_BYTE = 4,
		FETCH_PT_HIGH_BYTE = 6,
		INC_X_SCROLL_V = 7
	};

	PPU::PPU()
		: pixelsFrameBuffer(std::vector<Pixel>(256 * 240, Pixel()))
	{
		init();

		nesPalToRGBAPalArray[0x00] = Pixel{ 255, 84, 84, 84 };
		nesPalToRGBAPalArray[0x01] = Pixel{ 255, 0, 30, 116 };
		nesPalToRGBAPalArray[0x02] = Pixel{ 255, 8, 16, 144 };
		nesPalToRGBAPalArray[0x03] = Pixel{ 255, 48, 0, 136 };
		nesPalToRGBAPalArray[0x04] = Pixel{ 255, 68, 0, 100 };
		nesPalToRGBAPalArray[0x05] = Pixel{ 255, 92, 0, 48 };
		nesPalToRGBAPalArray[0x06] = Pixel{ 255, 84, 4, 0 };
		nesPalToRGBAPalArray[0x07] = Pixel{ 255, 60, 24, 0 };
		nesPalToRGBAPalArray[0x08] = Pixel{ 255, 32, 42, 0 };
		nesPalToRGBAPalArray[0x09] = Pixel{ 255, 8, 58, 0 };
		nesPalToRGBAPalArray[0x0A] = Pixel{ 255, 0, 64, 0 };
		nesPalToRGBAPalArray[0x0B] = Pixel{ 255, 0, 60, 0 };
		nesPalToRGBAPalArray[0x0C] = Pixel{ 255, 0, 50, 60 };
		nesPalToRGBAPalArray[0x0D] = Pixel{ 255, 0, 0, 0 };
		nesPalToRGBAPalArray[0x0E] = Pixel{ 255, 0, 0, 0 };
		nesPalToRGBAPalArray[0x0F] = Pixel{ 255, 0, 0, 0 };

		nesPalToRGBAPalArray[0x10] = Pixel{ 255, 152, 150, 152 };
		nesPalToRGBAPalArray[0x11] = Pixel{ 255, 8, 76, 196 };
		nesPalToRGBAPalArray[0x12] = Pixel{ 255, 48, 50, 236 };
		nesPalToRGBAPalArray[0x13] = Pixel{ 255, 92, 30, 228 };
		nesPalToRGBAPalArray[0x14] = Pixel{ 255, 136, 20, 176 };
		nesPalToRGBAPalArray[0x15] = Pixel{ 255, 160, 20, 100 };
		nesPalToRGBAPalArray[0x16] = Pixel{ 255, 152, 34, 32 };
		nesPalToRGBAPalArray[0x17] = Pixel{ 255, 120, 60, 0 };
		nesPalToRGBAPalArray[0x18] = Pixel{ 255, 84, 90, 0 };
		nesPalToRGBAPalArray[0x19] = Pixel{ 255, 40, 114, 0 };
		nesPalToRGBAPalArray[0x1A] = Pixel{ 255, 8, 124, 0 };
		nesPalToRGBAPalArray[0x1B] = Pixel{ 255, 0, 118, 40 };
		nesPalToRGBAPalArray[0x1C] = Pixel{ 255, 0, 102, 120 };
		nesPalToRGBAPalArray[0x1D] = Pixel{ 255, 0, 0, 0 };
		nesPalToRGBAPalArray[0x1E] = Pixel{ 255, 0, 0, 0 };
		nesPalToRGBAPalArray[0x1F] = Pixel{ 255, 0, 0, 0 };

		nesPalToRGBAPalArray[0x20] = Pixel{ 255, 236, 238, 236 };
		nesPalToRGBAPalArray[0x21] = Pixel{ 255, 76, 154, 236 };
		nesPalToRGBAPalArray[0x22] = Pixel{ 255, 120, 124, 236 };
		nesPalToRGBAPalArray[0x23] = Pixel{ 255, 176, 98, 236 };
		nesPalToRGBAPalArray[0x24] = Pixel{ 255, 228, 84, 236 };
		nesPalToRGBAPalArray[0x25] = Pixel{ 255, 236, 88, 180 };
		nesPalToRGBAPalArray[0x26] = Pixel{ 255, 236, 106, 100 };
		nesPalToRGBAPalArray[0x27] = Pixel{ 255, 212, 136, 32 };
		nesPalToRGBAPalArray[0x28] = Pixel{ 255, 160, 170, 0 };
		nesPalToRGBAPalArray[0x29] = Pixel{ 255, 116, 196, 0 };
		nesPalToRGBAPalArray[0x2A] = Pixel{ 255, 76, 208, 32 };
		nesPalToRGBAPalArray[0x2B] = Pixel{ 255, 56, 204, 108 };
		nesPalToRGBAPalArray[0x2C] = Pixel{ 255, 56, 180, 204 };
		nesPalToRGBAPalArray[0x2D] = Pixel{ 255, 60, 60, 60 };
		nesPalToRGBAPalArray[0x2E] = Pixel{ 255, 0, 0, 0 };
		nesPalToRGBAPalArray[0x2F] = Pixel{ 255, 0, 0, 0 };

		nesPalToRGBAPalArray[0x30] = Pixel{ 255, 236, 238, 236 };
		nesPalToRGBAPalArray[0x31] = Pixel{ 255, 168, 204, 236 };
		nesPalToRGBAPalArray[0x32] = Pixel{ 255, 188, 188, 236 };
		nesPalToRGBAPalArray[0x33] = Pixel{ 255, 212, 178, 236 };
		nesPalToRGBAPalArray[0x34] = Pixel{ 255, 236, 174, 236 };
		nesPalToRGBAPalArray[0x35] = Pixel{ 255, 236, 174, 212 };
		nesPalToRGBAPalArray[0x36] = Pixel{ 255, 236, 180, 176 };
		nesPalToRGBAPalArray[0x37] = Pixel{ 255, 228, 196, 144 };
		nesPalToRGBAPalArray[0x38] = Pixel{ 255, 204, 210, 120 };
		nesPalToRGBAPalArray[0x39] = Pixel{ 255, 180, 222, 120 };
		nesPalToRGBAPalArray[0x3A] = Pixel{ 255, 168, 226, 144 };
		nesPalToRGBAPalArray[0x3B] = Pixel{ 255, 152, 226, 180 };
		nesPalToRGBAPalArray[0x3C] = Pixel{ 255, 160, 214, 228 };
		nesPalToRGBAPalArray[0x3D] = Pixel{ 255, 160, 162, 160 };
		nesPalToRGBAPalArray[0x3E] = Pixel{ 255, 0, 0, 0 };
		nesPalToRGBAPalArray[0x3F] = Pixel{ 255, 0, 0, 0 };
	}

	void PPU::connectCartridge(const std::shared_ptr<Cartridge>& cart)
	{
		this->cartridge = cart;
	}

	void PPU::reset()
	{
		PPUCTRL.controlReg = 0x00;
		PPUMASK.maskReg = 0x00;
	}

	void PPU::clock()
	{

		auto updateDataShiftRegisters = [&]() -> void {
			// Load to lower part of the reg so that fetching is done from the upper part
			low_pattern_shifter = (low_pattern_shifter & 0xFF00) | fetchedLowBytePT;
			high_pattern_shifter = (high_pattern_shifter & 0xFF00) | fetchedHighBytePT;

			// If bit is 1 then all 1s, 0s otherwise
			low_attribute_shifter = (low_attribute_shifter & 0xFF00) | ((fetchedByteAT & 0b01) ? 0xFF : 0x00); // Low bit
			high_attribute_shifter = (high_attribute_shifter & 0xFF00) | ((fetchedByteAT & 0b10) ? 0xFF : 0x00); // High bit
		};
		//if (!PPUMASK.showBackground && !PPUMASK.showSprites) // Rendering disabled
		//    return;

		//// Odd frame skipped cycle (if rendering enabled)
		//if ((PPUMASK.showBackground || PPUMASK.showSprites))
		//{
		//    if (cycle == 0 && scanline == 0 && framesElapsed % 2 == 1)
		//        cycle = 1;
		//}

		// Everything that happens on visible scanlines and pre-render scanline goes here
		if (scanline >= -1 && scanline < 240)
		{
			// Odd frame skipped cycle (if rendering enabled)
			if ((PPUMASK.showBackground || PPUMASK.showSprites))
			{
				if (cycle == 0 && scanline == 0 && framesElapsed % 2 == 1)
				{
					cycle = 1;
					//return;
				}

				/*if (cycle == 339 && scanline == -1 && framesElapsed % 2 == 1)
				{
					cycle = 0;
					scanline = 0;
					return;
				}*/
			}

			if (cycle == 1 && scanline == -1)
			{
				PPUSTATUS.verticalBlank = 0;
				PPUSTATUS.spriteZeroHit = 0;
				PPUSTATUS.spriteOverflow = 0;
			}

			if (scanline >= 0 && ((cycle >= 1 && cycle <= 257) || (cycle >= 321 && cycle <= 336)))
			{
				// Shift registers every cycle
				low_pattern_shifter <<= 1;
				high_pattern_shifter <<= 1;
				low_attribute_shifter <<= 1;
				high_attribute_shifter <<= 1;

				switch ((cycle - 1) % 8) // Every 8 cycles we repeat this
				{
				case FETCH_NT_BYTE: // 0
					// Every 8 cyles (case 0), the shifter register get updated (1 to 257 on odd cycles)
					updateDataShiftRegisters();

					fetchedByteNT = ppuRead(0x2000 | (loopyV.vramAddrPtr & 0x0FFF));
					break;
				case FETCH_AT_BYTE: // 2
				{
					/*uint16_t attribOffset = (loopyV.vramAddrPtr & 0x0C00)
						| ((loopyV.vramAddrPtr >> 4) & 0x38)
						| ((loopyV.vramAddrPtr >> 2) & 0x07);*/

					uint16_t attribOffset = ((loopyV.nametableSelectY << 11)
						| (loopyV.nametableSelectX << 10) | ((loopyV.coarseYScroll >> 2) << 3)
						| (loopyV.coarseXScroll >> 2));

					fetchedByteAT = ppuRead(0x23C0 | attribOffset);

					if (loopyV.coarseYScroll & 0x02) // Bottom half else top half
						fetchedByteAT >>= 4;
					if (loopyV.coarseXScroll & 0x02) // Right half else left half
						fetchedByteAT >>= 2;

					fetchedByteAT &= 0x03; // After all we only get the low 2 bits for the tile
				}
				break;
				case FETCH_PT_LOW_BYTE: // 4
					/*
						0HRRRR CCCCPTTT
						|||||| |||||+++- T: Fine Y offset, the row number within a tile
						|||||| ||||+---- P: Bit plane (0: "lower"; 1: "upper")
						|||||| ++++----- C: Tile column
						||++++---------- R: Tile row
						|+-------------- H: Half of pattern table (0: "left"; 1: "right")
						+--------------- 0: Pattern table is at $0000-$1FFF
					*/
				{
					uint16_t lowByteAddr = PPUCTRL.backgroundPatternTabAddr << 12 | fetchedByteNT << 4
						| 0 << 3 | loopyV.fineYScroll; // 0 << 3 == + 0 (low tile byte)

					fetchedLowBytePT = ppuRead(lowByteAddr);
				}
				break;
				case FETCH_PT_HIGH_BYTE: // 6
					/*
						0HRRRR CCCCPTTT
						|||||| |||||+++- T: Fine Y offset, the row number within a tile
						|||||| ||||+---- P: Bit plane (0: "lower"; 1: "upper")
						|||||| ++++----- C: Tile column
						||++++---------- R: Tile row
						|+-------------- H: Half of pattern table (0: "left"; 1: "right")
						+--------------- 0: Pattern table is at $0000-$1FFF
					*/
				{
					uint16_t highByteAddr = PPUCTRL.backgroundPatternTabAddr << 12 | fetchedByteNT << 4
						| 1 << 3 | loopyV.fineYScroll; // 1 << 3 == + 8 (high tile byte)

					fetchedHighBytePT = ppuRead(highByteAddr);
				}
				break;
				case INC_X_SCROLL_V: // 7
					if (PPUMASK.showBackground || PPUMASK.showSprites)
						incrementScrollXloopyV();
					break;
				}

				if (cycle == 256)
				{
					if (PPUMASK.showBackground || PPUMASK.showSprites)
						incrementScrollYloopyV();
				}

				if (cycle == 257)
				{
					if (PPUMASK.showBackground || PPUMASK.showSprites)
						copyXvaluesFromTtoVloopyRegs();
				}
			}

			if (scanline == -1 && cycle >= 280 && cycle <= 304)
			{
				if (PPUMASK.showBackground || PPUMASK.showSprites)
					copyYvaluesFromTtoVloopyRegs();
			}

			if (scanline >= 0 && scanline <= 239)
			{
				// ----- SPRITE REGION ---------

				// Sprite evaluation can be refactored in a future to be more accurate with how the real hardware does it

				if (cycle >= 1 && cycle <= 64)
				{
					// Refactor later on to be more accurate, we clear secondary OAM in last cycle for now
					if (cycle == 64)
						std::memset(scanlineSecondaryOAM, 0xFF, sizeof(scanlineSecondaryOAM));
				}

				// Sprites evaluation
				if (cycle >= 65 && cycle <= 256)
				{
					// Later on this could be refactored to be more accurate, for now we perform the evaluation all at oncec in the last cycle
					if (cycle == 256)
					{
						numSprFound = 0;
						//uint8_t* secondOAMPtr = reinterpret_cast<uint8_t*>(scanlineSecondaryOAM);

						// Check for 2nd OAM full or main OAM wrapped around
						for (uint8_t oamIndex = 0, secondOamIndex = 0; oamIndex < 64 && secondOamIndex < 8; oamIndex++)
						{
							//uint8_t spriteY = OAMptr[oamIndex * SIZE_OAM_SPR + 0];
							uint8_t spriteY = OAMbuffer[oamIndex].posY;

							//secondOAMPtr[secondOamIndex * SIZE_OAM_SPR + 0] = spriteY;
							scanlineSecondaryOAM[secondOamIndex].posY = spriteY;

							uint8_t spriteSize = PPUCTRL.sprSize ? 16 : 8;

							// Used current scanline instead of next scanline as PPU uses current one to deal with offset of 1 in Y coordinate
							// It compares Y to current scanline for next scanline sprite
							if (scanline >= spriteY && scanline < spriteY + spriteSize)
							{
								// Copy the rest of the OAM Sprite fields (tileIndex, Attributes and X position)
								//std::memcpy(secondOAMPtr + secondOamIndex * SIZE_OAM_SPR + 1, OAMptr + oamIndex * SIZE_OAM_SPR + 1, 3);
								scanlineSecondaryOAM[secondOamIndex].tileIndex = OAMbuffer[oamIndex].tileIndex;
								scanlineSecondaryOAM[secondOamIndex].attributes = OAMbuffer[oamIndex].attributes;
								scanlineSecondaryOAM[secondOamIndex].posX = OAMbuffer[oamIndex].posX;

								numSprFound++;
								secondOamIndex++;
							}
						}

						for (int remainingSlotStart = numSprFound; remainingSlotStart < 8; remainingSlotStart++)
							std::memset(&scanlineSecondaryOAM[numSprFound], 0xFF, (8 - numSprFound) * SIZE_OAM_SPR);
                    
					}
				}

				// Pretty famous bit-flipper: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
				auto FlipBits = [](uint8_t byte) {
					byte = ((byte & 0xF0) >> 4) | ((byte & 0x0F) << 4);
					byte = ((byte & 0xCC) >> 2) | ((byte & 0x33) << 2);
					byte = ((byte & 0xAA) >> 1) | ((byte & 0x55) << 1);
					return byte;

				};

				// Sprites rendering
				if (cycle >= 257 && cycle <= 320)
				{
					uint8_t spriteIndex = ((cycle - 1) % 64) >> 3; // cycle / 8 to get current corresponding sprite

					switch ((cycle - 1) % 8)
					{
					case 0: // Garbage NT fetch and sprite Y coordinate fetch
						(void)ppuRead(0x2000 | (loopyV.vramAddrPtr & 0x0FFF));
						fetchedSprY = scanlineSecondaryOAM[spriteIndex].posY;
						break;
					case 1: // Sprite tileIndex fetch
						fetchedSprTileIndex = scanlineSecondaryOAM[spriteIndex].tileIndex;
						break;
					case 2: // Garbage NT fetch and sprite attribute byte loading (from 2nd OAM to respective latch)                    
						(void)ppuRead(0x2000 | (loopyV.vramAddrPtr & 0x0FFF));
						spritesAttributesLatches[spriteIndex] = scanlineSecondaryOAM[spriteIndex].attributes;
						break;
					case 3: // Sprite X position byte loading (from 2nd OAM to respective counter)
						spritesXpositionCounters[spriteIndex] = scanlineSecondaryOAM[spriteIndex].posX;
						break;
					case 4: // Sprite pattern table tile low byte
						{
							uint16_t lowByteAddr = 0x0000;
							uint8_t spriteHeight = PPUCTRL.sprSize ? 16 : 8;

							if (spriteHeight == 8)
							{
								if (spritesAttributesLatches[spriteIndex] & 0x80) // Flip vertically)
								{
									lowByteAddr = PPUCTRL.sprPatternTabAddr << 12 | fetchedSprTileIndex << 4
										| 0 << 3 | ((7 - (scanline - fetchedSprY)) & 0x07); // 0 << 3 == + 0 (low tile byte)
								}
								else
								{
									lowByteAddr = PPUCTRL.sprPatternTabAddr << 12 | fetchedSprTileIndex << 4
										| 0 << 3 | ((scanline - fetchedSprY) & 0x07); // 0 << 3 == + 0 (low tile byte)
								}
							}
							else // 8 x 16
							{
								uint8_t tableSelected = (fetchedSprTileIndex & 0x01);
								uint8_t tileIndex = (fetchedSprTileIndex & 0xFE);
								uint8_t patternLine = (scanline - fetchedSprY);

								if (spritesAttributesLatches[spriteIndex] & 0x80) // Flip vertically)
								{
									if (patternLine < 8)
									{
										lowByteAddr = tableSelected * 0x1000 | (tileIndex + 1) << 4
											| 0 << 3 | ((7 - patternLine) & 0x07); // 0 << 3 == + 0 (low tile byte)
									}
									else
									{
										lowByteAddr = tableSelected * 0x1000 | tileIndex << 4
											| 0 << 3 | ((7 - patternLine) & 0x07); // 0 << 3 == + 0 (low tile byte)
									}
								}
								else
								{
									if (patternLine < 8)
									{
										lowByteAddr = tableSelected * 0x1000 | tileIndex << 4
											| 0 << 3 | (patternLine & 0x07); // 0 << 3 == + 0 (low tile byte)
									}
									else
									{
										lowByteAddr = tableSelected * 0x1000 | (tileIndex + 1) << 4
											| 0 << 3 | (patternLine & 0x07); // 0 << 3 == + 0 (low tile byte)
									}
								}
							}

							uint8_t lowByte = ppuRead(lowByteAddr);

							if (spritesAttributesLatches[spriteIndex] & 0x40) // Flip horizontally
								lowByte = FlipBits(lowByte);

							spritesLowBytePatternShifters[spriteIndex] = lowByte;

							if (spriteIndex >= numSprFound)
								spritesLowBytePatternShifters[spriteIndex] = 0x00;
						}
						break;
					case 6: // Sprite Pattern table tile high byte
						{
							uint16_t highByteAddr = 0x0000;
							uint8_t spriteHeight = PPUCTRL.sprSize ? 16 : 8;

							if (spriteHeight == 8)
							{
								if (spritesAttributesLatches[spriteIndex] & 0x80) // Flip vertically)
								{
									highByteAddr = PPUCTRL.sprPatternTabAddr << 12 | fetchedSprTileIndex << 4
										| 1 << 3 | ((7 - (scanline - fetchedSprY)) & 0x07); // 0 << 3 == + 0 (high tile byte)
								}
								else
								{
									highByteAddr = PPUCTRL.sprPatternTabAddr << 12 | fetchedSprTileIndex << 4
										| 1 << 3 | ((scanline - fetchedSprY) & 0x07); // 0 << 3 == + 0 (high tile byte)
								}
							}
							else // 8 x 16
							{
								uint8_t tableSelected = (fetchedSprTileIndex & 0x01);
								uint8_t tileIndex = (fetchedSprTileIndex & 0xFE);
								uint8_t patternLine = (scanline - fetchedSprY);

								if (spritesAttributesLatches[spriteIndex] & 0x80) // Flip vertically)
								{
									if (patternLine < 8)
									{
										highByteAddr = tableSelected * 0x1000 | (tileIndex + 1) << 4
											| 1 << 3 | ((7 - patternLine) & 0x07); // 0 << 3 == + 0 (low tile byte)
									}
									else
									{
										highByteAddr = tableSelected * 0x1000 | tileIndex << 4
											| 1 << 3 | ((7 - patternLine) & 0x07); // 0 << 3 == + 0 (low tile byte)
									}
								}
								else
								{
									if (patternLine < 8)
									{
										highByteAddr = tableSelected * 0x1000 | tileIndex << 4
											| 1 << 3 | (patternLine & 0x07); // 0 << 3 == + 0 (low tile byte)
									}
									else
									{
										highByteAddr = tableSelected * 0x1000 | (tileIndex + 1) << 4
											| 1 << 3 | (patternLine & 0x07); // 0 << 3 == + 0 (low tile byte)
									}
								}
							}

							uint8_t highByte = ppuRead(highByteAddr);

							if (spritesAttributesLatches[spriteIndex] & 0x40) // Flip horizontally
								highByte = FlipBits(highByte);

							spritesHighBytePatternShifters[spriteIndex] = highByte;

							if (spriteIndex >= numSprFound)
								spritesHighBytePatternShifters[spriteIndex] = 0x00;
						}
						break;
					case 7: // Fetch again X
						spritesXpositionCounters[spriteIndex] = scanlineSecondaryOAM[spriteIndex].posX;
						break;
					}
				}

				// ----- END SPRITE REGION -----

				if (cycle >= 337 && cycle <= 340)
				{

				}
			}
		}

		if (cycle == 1 && scanline == 241)
		{
			PPUSTATUS.verticalBlank = 1;

			if (PPUCTRL.enableNMI)
				nmi = true;
		}

		if (scanline == 240)
		{
			// Post-render scanline, PPU idles
		}

		//// ----- TESTING -------------
		//pixelsFrameBuffer[scanline * 341 + cycle] = nesPalToRGBAPalArray[rand() % 0x3F];
		//// ----- END OF TESTING ------

		// RENDERING A PIXEL
		uint8_t bgPixel = 0x00;
		uint8_t bgPalette = 0x00;

		uint8_t sprPixel = 0x00;
		uint8_t sprPalette = 0x00;
		bool isForegroundPriority = false;

		uint8_t finalPixel = 0x00;
		uint8_t finalPalette = 0x00;

		if (PPUMASK.showBackground)
		{
			uint16_t multiplexerBitSelector = 0x8000 >> fineXScroll;

			uint8_t bgPixelLow = (low_pattern_shifter & multiplexerBitSelector) > 0;
			uint8_t bgPixelHigh = (high_pattern_shifter & multiplexerBitSelector) > 0;
			bgPixel = (bgPixelHigh << 1) | bgPixelLow;

			uint8_t bgPaletteLow = (low_attribute_shifter & multiplexerBitSelector) > 0;
			uint8_t bgPaletteHigh = (high_attribute_shifter & multiplexerBitSelector) > 0;
			bgPalette = (bgPaletteHigh << 1) | bgPaletteLow;
		}

		if (PPUMASK.showSprites && scanline >= 1 && scanline < 240 && cycle >= 1 && cycle < 257)
		{
			for (int i = 7; i >= 0; i--)
			{
				if (spritesXpositionCounters[i] > 0)
				{
					spritesXpositionCounters[i]--;
				}
				else
				{
					if (spriteRenderingCounters[i] <= 7 && spriteRenderingCounters[i] >= 0)
					{
						uint8_t sprPixelLow = (spritesLowBytePatternShifters[i] & 0x80) > 0;
						uint8_t sprPixelHigh = (spritesHighBytePatternShifters[i] & 0x80) > 0;
						sprPixel = (sprPixelHigh << 1) | sprPixelLow;

						sprPalette = (spritesAttributesLatches[i] & 0x03) + 4;

						isForegroundPriority = (spritesAttributesLatches[i] & 0x20) == 0; // Sprite pixel has prio
						
						spriteRenderingCounters[i]--;
					}

					spritesLowBytePatternShifters[i] <<= 1;
					spritesHighBytePatternShifters[i] <<= 1;
				}

				//if (spritesXpositionCounters[i] == 0 && spriteRenderingCounters[i] <= 7 && spriteRenderingCounters[i] >= 0) // Render 8 pixels
				//{
				//	uint8_t sprPixelLow = (spritesLowBytePatternShifters[i] & 0x80) > 0;
				//	uint8_t sprPixelHigh = (spritesHighBytePatternShifters[i] & 0x80) > 0;
				//	sprPixel = (sprPixelHigh << 1) | sprPixelLow;

				//	sprPalette = (spritesAttributesLatches[i] & 0x03) + 4;

				//	isForegroundPriority = (spritesAttributesLatches[i] & 0x20) == 0; // Sprite pixel has prio

				//	spriteRenderingCounters[i]--;
				//}
			}
		}

		if (!bgPixel && !sprPixel)
		{
			finalPixel = 0x00;
			finalPalette = 0x00;
		}
		else if (bgPixel && !sprPixel)
		{
			finalPixel = bgPixel;
			finalPalette = bgPalette;
		}
		else if (!bgPixel && sprPixel)
		{
			finalPixel = sprPixel;
			finalPalette = sprPalette;
		}
		else
		{
			if (isForegroundPriority)
			{
				finalPixel = sprPixel;
				finalPalette = sprPalette;
			}
			else
			{
				finalPixel = bgPixel;
				finalPalette = bgPalette;
			}
		}

		if (PPUMASK.showBackground || PPUMASK.showSprites)
		{
			if (scanline >= 0 && scanline <= 239 && cycle >= 1 && cycle <= 256)
				pixelsFrameBuffer[scanline * 256 + (cycle - 1)] =/* isSprPixel ? nesPalToRGBAPalArray[0x30] : */getRGBAFromNesPalette(finalPalette, finalPixel);
		}

		/*if (cycle == 0)
			currentXpixelBG = 0;
		else if (cycle > 0 && cycle <= 256)
			currentXpixelBG++;*/

		cycle++;

		if (cycle >= 341)
		{
			cycle = 0;
			scanline++;

			std::memset(spriteRenderingCounters, 7, 8);

			if (scanline >= 261)
			{
				scanline = -1;
				frameCompleted = true;
				framesElapsed++;
			}
		}

		totalCycles++;
	}

	uint8_t PPU::cpuRead(uint16_t address)
	{
		uint8_t dataRead = 0x00;

		switch (address)
		{
		case 0x0000: // Control
			break;
		case 0x0001: // Mask
			break;
		case 0x0002: // Status
			dataRead = PPUSTATUS.statusReg;
			//dataRedd = (PPUSTATUS.statusReg & 0xE0) | (internalReadBuffer & 0x1F); Put lower 5 bits of noise into the read data, check if needed later
			PPUSTATUS.verticalBlank = 0;
			addressLatchToggle = false; //Reset -> w = 0
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			dataRead = OAMptr[OAMaddr];

			if (!PPUSTATUS.verticalBlank)
				OAMaddr++;

			if (scanline >= 0 && scanline <= 239 && cycle >= 1 && cycle <= 64)
				dataRead = 0xFF;
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
			if (loopyV.vramAddrPtr <= 0x3EFF)
			{
				dataRead = internalReadBuffer;
				internalReadBuffer = ppuRead(loopyV.vramAddrPtr);
			}
			else if (loopyV.vramAddrPtr >= 0x3F00)
			{
				dataRead = ppuRead(loopyV.vramAddrPtr);
			}

			loopyV.vramAddrPtr += PPUCTRL.vramAddrInc ? 32 : 1;
			break;
		}

		return dataRead;
	}

	void PPU::cpuWrite(uint16_t address, uint8_t data)
	{
		switch (address)
		{
		case 0x0000: // Control
			PPUCTRL.controlReg = data;
			loopyT.nametableSelectX = PPUCTRL.nametableX;
			loopyT.nametableSelectY = PPUCTRL.nametableY;
			break;
		case 0x0001: // Mask
			PPUMASK.maskReg = data;
			break;
		case 0x0002: // Status
			break;
		case 0x0003: // OAM Address
			OAMaddr = data;
			break;
		case 0x0004: // OAM Data
			OAMptr[OAMaddr++] = data;
			break;
		case 0x0005: // Scroll
			if (!addressLatchToggle) // XXXXXxxx, Top 5 bits become XXXXX of "T", low 3 bits become the fine X scroll(updated immediately).
			{
				loopyT.coarseXScroll = ((data & 0xF8) >> 3) & 0x001F; // Top 5 bits of data become the lower 5 bits of T
				fineXScroll = data & 0x07; // Low 3 bits of data become fine X scroll
			}
			else // YYYYYyyy, Top 5 bits become YYYYY of "T", low 3 bits become yyy of "T" (fine Y scroll)
			{
				loopyT.coarseYScroll = ((data & 0xF8) >> 3) & 0x001F;
				loopyT.fineYScroll = data & 0x07;
			}

			addressLatchToggle = !addressLatchToggle;
			break;
		case 0x0006: // PPU Address
			if (!addressLatchToggle) // 00yyNNYY, sets the top 8 bits in "T".The top bit of fine Y scroll gets corrupted to 0 here, so we do a $2005 second write to make up for that
			{
				// We treat the addr as big endian (i.e. hi_byte is provided first)
				loopyT.vramAddrPtr = (data << 8) & 0x3F00; // High bit of fine Y scroll is corrupted to 0. We emulate this hardware behaviour
			}
			else // YYYXXXXX, Sets the low 8 bits of "T", which includes the coarse X, and low 3 bits of coarse y. After doing this, "T" is copied to "V", allowing mid-screen vertical scrolling
			{
				loopyT.vramAddrPtr |= data & 0x00FF;
				loopyV = loopyT;
			}

			addressLatchToggle = !addressLatchToggle; // Invert latch
			break;
		case 0x0007: // PPU Data
			ppuWrite(loopyV.vramAddrPtr, data);
			loopyV.vramAddrPtr += PPUCTRL.vramAddrInc ? 32 : 1;
			break;
		}
	}

	uint8_t PPU::ppuRead(uint16_t address)
	{
		uint8_t dataRead = 0x00;
		address &= 0x3FFF; // Masking address to avoid going beyond the PPU addressable range

		if (cartridge->ppuRead(address, dataRead))
		{

		}
		else if (address >= 0x2000 && address <= 0x2FFF)
		{
			if (cartridge->getNTMirroring() == Cartridge::Mirroring::VERTICAL)
			{
				if (address >= 0x2800 && address <= 0x2BFF)
					address &= 0x23FF;
				else if (address >= 0x2C00 && address <= 0x2FFF)
					address &= 0x27FF;

			}
			else if (cartridge->getNTMirroring() == Cartridge::Mirroring::HORIZONTAL)
			{
				if (address <= 0x27FF)
					address &= 0x23FF;
				else if (address >= 0x2C00)
					address &= 0x2BFF;
			}

			dataRead = nameTables[(address & 0x0F00) >= 0x0400][address & 0x03FF];
		}
		else if (address >= 0x3F00)
		{
			address &= 0x001F;
			dataRead = paletteRam[address];
		}

		return dataRead;
	}

	void PPU::ppuWrite(uint16_t address, uint8_t data)
	{
		address &= 0x3FFF; // Masking address to avoid going beyond the PPU addressable range

		if (cartridge->ppuWrite(address, data))
		{

		}
		else if (address >= 0x2000 && address <= 0x2FFF)
		{
			if (cartridge->getNTMirroring() == Cartridge::Mirroring::VERTICAL)
			{
				if (address >= 0x2800 && address <= 0x2BFF)
					address &= 0x23FF;
				else if (address >= 0x2C00 && address <= 0x2FFF)
					address &= 0x27FF;

			}
			else if (cartridge->getNTMirroring() == Cartridge::Mirroring::HORIZONTAL)
			{
				if (address <= 0x27FF)
					address &= 0x23FF;
				else if (address >= 0x2C00)
					address &= 0x2BFF;
			}

			nameTables[(address & 0x0F00) >= 0x0400][address & 0x03FF] = data;
		}
		else if (address >= 0x3F00) // Palette RAM indexes
		{
			address &= 0x001F;

			if ((address & 0x000F) % 0x04 == 0) // Multiples of 4 are mirrors of 0x3F00 (last colour of a palette mirrors to background colour at 0x3F00)
				address &= 0x000F; // Addresses with 0x3F1X multiples of 4 are mirrors of 0x3F0X equivalents (i.e. 10, 14, 18, 1C)

			paletteRam[address] = data;
		}
	}

	PPU::Pixel PPU::getRGBAFromNesPalette(uint8_t paletteIndex, uint8_t pixelIndex)
	{
		uint8_t nesColour = ppuRead(0x3F00 + (paletteIndex * 4 + pixelIndex));
		return nesPalToRGBAPalArray[nesColour];
	}

	std::vector<PPU::Pixel> PPU::getPatternTableBuffer(uint8_t patternIndex, uint8_t paletteIndex)
	{
		std::vector<PPU::Pixel> buffer = std::vector<PPU::Pixel>(128 * 128); // 128 by 128 pixels wide

		// Iterate over sprites indeces
		for (int indexTileY = 0; indexTileY < 16; indexTileY++)
		{
			for (int indexTileX = 0; indexTileX < 16; indexTileX++)
			{
				int tileOffset = indexTileY * 256 + indexTileX * 16;

				// Iterate over sprite rows
				for (int sprRow = 0; sprRow < 8; sprRow++)
				{
					//Each tile is 16 bytes, each row is 2 bytes separated each one in two blocks of 8 bytes (LSB and MSB, each one separated 8 bytes)
					uint8_t LSB = ppuRead(patternIndex * 0x1000 + tileOffset + sprRow + 0x0000);
					uint8_t MSB = ppuRead(patternIndex * 0x1000 + tileOffset + sprRow + 0x0008);

					// Iterate over sprite column from a row using LSB and MSB
					for (int sprCol = 0; sprCol < 8; sprCol++)
					{
						uint8_t lsb = (LSB & 0x80) >> 7;
						uint8_t msb = (MSB & 0x80) >> 7;

						uint8_t pixel = (msb << 1) | lsb;

						uint16_t x = indexTileX * 8 + (sprCol);
						uint16_t y = indexTileY * 8 + sprRow;

						buffer[y * 128 + x] = getRGBAFromNesPalette(0, pixel);

						MSB <<= 1;
						LSB <<= 1;
					}
				}
			}
		}

		return buffer;
	}

	void* PPU::getNametable(int8_t selectNT)
	{
		return nameTables[selectNT];
	}

	void PPU::init()
	{
		PPUCTRL.controlReg = 0x00;
		PPUMASK.maskReg = 0x00;
		PPUSTATUS.statusReg = 0xA0;

		OAMptr = reinterpret_cast<uint8_t*>(OAMbuffer);
	}

	void PPU::incrementScrollXloopyV()
	{
		if (loopyV.coarseXScroll == 31)
		{
			loopyV.nametableSelectX = ~loopyV.nametableSelectX;
		}

		loopyV.coarseXScroll++;
	}

	void PPU::incrementScrollYloopyV()
	{
		if (loopyV.fineYScroll < 7)
		{
			loopyV.fineYScroll++;
		}
		else
		{
			loopyV.fineYScroll = 0;

			if (loopyV.coarseYScroll == 29)
			{
				loopyV.coarseYScroll = 0;
				loopyV.nametableSelectY = ~loopyV.nametableSelectY;
			}
			//else if (loopyV.coarseYScroll == 31)
			//{
			//    loopyV.coarseYScroll = 0;
			//}
			else
			{
				loopyV.coarseYScroll++; // Case Y scroll == 31 handled here as incrementing wraps around 0
			}
		}
	}

	void PPU::copyXvaluesFromTtoVloopyRegs()
	{
		loopyV.coarseXScroll = loopyT.coarseXScroll;
		loopyV.nametableSelectX = loopyT.nametableSelectX;
	}

	void PPU::copyYvaluesFromTtoVloopyRegs()
	{
		loopyV.coarseYScroll = loopyT.coarseYScroll;
		loopyV.nametableSelectY = loopyT.nametableSelectY;
		loopyV.fineYScroll = loopyT.fineYScroll;
	}
}