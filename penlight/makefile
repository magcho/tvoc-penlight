espUploader := ~/Library/Arduino15/packages/esp8266/hardware/esp8266/2.6.3/tools/upload.py
espOtaUploader := ~/Library/Arduino15/packages/esp8266/hardware/esp8266/2.6.3/tools/espota.py

binName := penlight

serialPort := /dev/cu.wchusbserial141130
# serialPort := /dev/cu.usbserial-DM01P2EB
hostIp := 192.168.11.4


build: *.ino
	arduino-cli compile -b 'esp8266:esp8266:generic' \
		-o $(binName) .

clean:
	rm $(binName).elf $(binName).bin

upload:
	python $(espUploader) \
		--chip esp8266 \
		--port  $(serialPort) \
		--baud 115200 \
		--before no_reset \
		--after soft_reset \
		write_flash 0x0 ./$(binName).bin

ota_upload:
	python $(espOtaUploader) \
		-i $(hostIp) \
		-p 8266 \
		--auth= \
		-f ./$(binName).bin
