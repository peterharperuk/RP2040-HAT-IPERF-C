set(TARGET_NAME w5x00_lwiperf_test)
set(LWIP_PATH ${PICO_EXTRAS_PATH}/lib/lwip)
set(TARGET_FILES    
    ${TARGET_NAME}.c
    ${LWIP_PATH}/src/apps/lwiperf/lwiperf.c
    )

add_executable(${TARGET_NAME} ${TARGET_FILES})

# This is required to include lwipopts.h
target_include_directories(lwip INTERFACE        
        ${PORT_DIR}/lwip
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        pico_stdlib
        hardware_spi
        hardware_dma
        ETHERNET_FILES
        IOLIBRARY_FILES
        lwip
        WIZ_LWIP_FILES
        pico_wiznet_spi_pio
        )
target_compile_definitions(${TARGET_NAME} PRIVATE
        PICO_USE_PIO=1
        )
pico_enable_stdio_usb(${TARGET_NAME} 1)
pico_enable_stdio_uart(${TARGET_NAME} 0)

pico_add_extra_outputs(${TARGET_NAME})
