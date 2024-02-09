application=app

main:
	mkdir -p build

	arm-none-eabi-gcc \
		-mthumb \
		-mcpu=cortex-m4 \
		-mfpu=fpv4-sp-d16 \
		-mfloat-abi=softfp \
		-ffunction-sections \
		-fdata-sections \
		-std=c99 -Wall -pedantic \
		-c -Os -O0 -g \
		-DPART_TM4C123GH6PM -DTARGET_IS_BLIZZARD_RA1 -Dgcc \
		-o build/main.o \
		-MD -MF \
		build/main.d main.c

	arm-none-eabi-gcc \
		-mthumb \
		-mcpu=cortex-m4 \
		-mfpu=fpv4-sp-d16 \
		-mfloat-abi=softfp \
		-ffunction-sections \
		-fdata-sections \
		-std=c99 -Wall -pedantic -c \
		-Os -O0 \
		-Dgcc -DPART_TM4C123GH6PM -DTARGET_IS_BLIZZARD_RA1 \
		-g \
		-MD -MF \
		build/startup_gcc.d startup_gcc.c \
		-o build/startup_gcc.o

	arm-none-eabi-ld build/main.o build/startup_gcc.o -T tm4c123.ld --entry ResetISR --gc-sections -o build/main.axf -Map=build/main.map
	arm-none-eabi-objcopy -O binary build/main.axf build/main.bin
	arm-none-eabi-objdump -Sd -W build/main.axf > build/main.lss
	arm-none-eabi-size build/main.axf

flash: main
	sudo lm4flash build/main.bin

clean:
	rm -rf build
