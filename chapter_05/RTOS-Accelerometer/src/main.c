#include "mpu6050.h"

#define I2C_MASTER_SCL_IO 22      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */


void i2c_bus_init(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    printf("ret = %d\n",ret);

    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    printf("ret = %d\n",ret);
}

mpu6050_handle_t i2c_sensor_mpu6050_init(void)
{
    esp_err_t ret;
    mpu6050_handle_t mpu6050 = NULL;
    i2c_bus_init();
    mpu6050 = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);
    printf("ret = %p\n",mpu6050);

    ret = mpu6050_config(mpu6050, ACCE_FS_16G, GYRO_FS_2000DPS);
    printf("ret = %d\n",ret);

    ret = mpu6050_wake_up(mpu6050);
    printf("ret = %d\n",ret);

    return mpu6050;
}

void accelerometer_task(void *pvParameter) {
    
    mpu6050_acce_value_t acce;
    mpu6050_handle_t mpu6050 = i2c_sensor_mpu6050_init();

    while(1)
    {
        mpu6050_get_acce(mpu6050, &acce);
        printf("acce_x = %f, acce_y = %f, acce_z = %f\n", acce.acce_x, acce.acce_y, acce.acce_z);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}

void app_main() {

    i2c_bus_init();
    xTaskCreate(&accelerometer_task, "accelerometer_task", 2*configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}