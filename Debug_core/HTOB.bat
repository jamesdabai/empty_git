@echo off

if exist "./core.bin" (InsertAttribute core.bin 0x00000400 0xAB018632 0x00000080)

