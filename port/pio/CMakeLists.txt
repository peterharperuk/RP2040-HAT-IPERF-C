add_library(pico_wiznet_spi_pio INTERFACE)
pico_generate_pio_header(pico_wiznet_spi_pio ${CMAKE_CURRENT_LIST_DIR}/wiznet_spi_pio.pio)
target_link_libraries(pico_wiznet_spi_pio INTERFACE
    pico_stdlib
    hardware_pio
    hardware_dma
    hardware_gpio
)
target_sources(pico_wiznet_spi_pio INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/wiznet_spi_pio.c
)
target_include_directories(pico_wiznet_spi_pio INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)
