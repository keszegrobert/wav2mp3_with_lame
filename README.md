# wav2mp3_with_lame
This command line tool converts different types of PCM coded wav files into mp3 using the lame library

# How to build it

In your working directory open a Terminal and write these commands:
```
git clone https://github.com/keszegrobert/wav2mp3_with_lame
cd wav2mp3_with_lame
mkdir build
cd build
cmake ..
cmake --build .
```

# Usage

This is a command line tool, so a Terminal will be needed to see how it works
```
wav2mp3 <folder>
```
where `<folder>` can be any folder in the  file system

# Abilities

The converter can deal only with the PCM format, 
8-bit, 16-bit, 24-bit data
and with 1 or 2 channels
and converts only one file at a time (no multithreading)

# Resources

[Everything about the WAV format](https://en.wikipedia.org/wiki/WAV)

[Lame project](https://lame.sourceforge.io/index.php)
