
# ioLibrary_Driver
add_library(IOLIBRARY_FILES STATIC)

target_sources(IOLIBRARY_FILES PUBLIC
        ${PORT_DIR}/ioLibrary_Driver/src/w5x00_spi.c
        )

target_include_directories(IOLIBRARY_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${PORT_DIR}/ioLibrary_Driver/inc
        ${PORT_DIR}
        )

target_link_libraries(IOLIBRARY_FILES PRIVATE
        ETHERNET_FILES        
        pico_stdlib
        hardware_spi
        hardware_dma
        hardware_clocks
        lwip
        )

# wiznet lwip
add_library(WIZ_LWIP_FILES STATIC)

target_sources(WIZ_LWIP_FILES PUBLIC
        ${PORT_DIR}/lwip/src/w5x00_lwip.c
        )

target_include_directories(WIZ_LWIP_FILES PUBLIC
        ${PICO_EXTRAS_LWIP_SRC_DIR}
        ${WIZNET_DIR}/Ethernet
        ${PORT_DIR}
        ${PORT_DIR}/lwip/inc
        )

target_link_libraries(WIZ_LWIP_FILES PRIVATE
        ETHERNET_FILES
        lwip
        )

add_subdirectory(pio)
