# eepparse
Parser for Pokémon Mini EEPROM dump files

# About
eepparse is a pure C, command-line utility to extract individual save files from Pokémon Mini EEPROM dumps (usually with the extention .eep),
generated by the PokeMini emulator or dumped from a real Pokémon Mini.

As of this version, it can parse dumps and write out the save files as [GameID][GameName].bin.

The auxiliary data (inc. RTC configuration) is not yet extracted. There is no support for rebuilding an EEPROM image from the dumped files.

# Compiling
```
cd eepparse
make
```

# Usage
```
eepparse infile [outdir]
```
