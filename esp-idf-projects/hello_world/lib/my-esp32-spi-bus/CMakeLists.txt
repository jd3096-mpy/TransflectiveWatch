idf_component_register(
    SRCS spi_bus.c esp32-hal-spi.c esp32-hal-matrix.c esp32-hal-cpu.c Adafruit_SPIDevice.cpp SPI.cpp esp32-hal-gpio.c esp32-hal-misc.c esp32-hal-uart.c
    INCLUDE_DIRS .
    REQUIRES driver log
)
