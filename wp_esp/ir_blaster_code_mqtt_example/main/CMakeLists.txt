idf_component_register(SRCS "app_main.c" "gpio_adc.c"
                    INCLUDE_DIRS ".")
                    
 add_prebuilt_library(ir_func "./lib/libir_func.a" PRIV_REQUIRES esp_timer driver)
target_link_libraries(${COMPONENT_LIB} PUBLIC ir_func)