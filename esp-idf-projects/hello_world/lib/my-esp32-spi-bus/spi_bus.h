#if !defined(__SPI_BUS_H__)
#define __SPI_BUS_H__

#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t spi_init(void *strip);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
