@echo off

if exist "./recovery.bin" (InsertAttribute recovery.bin 0x00000400 0xAB018632 0x00000080)

