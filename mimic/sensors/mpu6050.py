import logging
from typing import List
from ..bridge import I2CSlave, MimicBridge

logger = logging.getLogger("Mimic")

class MPU6050Simulator:
    """Simulates an MPU6050 Accelerometer/Gyroscope."""
    
    WHO_AM_I_REG = 0x75
    PWR_MGMT_1_REG = 0x6B
    ACCEL_XOUT_H = 0x3B
    GYRO_XOUT_H = 0x43
    TEMP_OUT_H = 0x41
    
    def __init__(self, bridge: MimicBridge, address: int = 0x68):
        self.i2c = I2CSlave(bridge)
        self.address = address
        self.registers = {
            self.WHO_AM_I_REG: [0x68],
            self.PWR_MGMT_1_REG: [0x40],
            0x1A: [0x00],
            0x1B: [0x00],
            0x1C: [0x00],
        }
        self.accel_data = [0, 0, 16384]
        self.gyro_data = [0, 0, 0]
        self.temp_raw = 3650

    def start(self):
        logger.info(f"Starting MPU6050 Simulator at address 0x{self.address:02X}...")
        if not self.i2c.init(self.address):
            logger.error("Failed to initialize I2C Slave")
            return

        active_reg = 0x00
        while True:
            try:
                received = self.i2c.wait_for_write(1)
                if received:
                    active_reg = received[0]
                    if len(received) > 1:
                        self.registers[active_reg] = received[1:]
                        logger.info(f"Master wrote to Reg 0x{active_reg:02X}")
                        continue
                    
                    data_to_send = self._get_reg_data(active_reg)
                    if data_to_send:
                        self.i2c.respond(data_to_send)
            except KeyboardInterrupt:
                break

    def _get_reg_data(self, reg: int) -> List[int]:
        if reg == self.WHO_AM_I_REG:
            return [0x68]
        elif reg == self.ACCEL_XOUT_H:
            return self._to_16bit_list(self.accel_data)
        elif reg == self.GYRO_XOUT_H:
            return self._to_16bit_list(self.gyro_data)
        elif reg == self.TEMP_OUT_H:
            return [(self.temp_raw >> 8) & 0xFF, self.temp_raw & 0xFF]
        return self.registers.get(reg, [0x00])

    def _to_16bit_list(self, values: List[int]) -> List[int]:
        res = []
        for v in values:
            v &= 0xFFFF
            res.append((v >> 8) & 0xFF)
            res.append(v & 0xFF)
        return res
