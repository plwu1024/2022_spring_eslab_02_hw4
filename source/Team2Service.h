
/* MBED_DEPRECATED */
#warning "These services are deprecated and will be removed. Please see services.md for details about replacement services."

#ifndef MBED_BLE_TEAM_2_SERVICE_H__
#define MBED_BLE_TEAM_2_SERVICE_H__

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/GattServer.h"
#include "stm32l475e_iot01_magneto.h"

#if BLE_FEATURE_GATT_SERVER

class Team2Service {
public:
    Team2Service(BLE &_ble) :
        ble(_ble),
        // valueBytes(hrmCounter),
        hrmRate(
            GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE,
            pDataXYZ.getPointer(),
            pDataXYZ.getNumValueBytes(),
            pDataXYZ.getNumValueBytes(),
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
        )
    {
        setupService();
    }

    void updateTeam2() {
        pDataXYZ.updateTeam2();
        ble.gattServer().write(
            hrmRate.getValueHandle(),
            pDataXYZ.getPointer(),
            pDataXYZ.getNumValueBytes()
        );
    }

protected:
    void setupService() {
        GattCharacteristic *charTable[] = {
            &hrmRate,
        };
        GattService hrmService(
            // GattService::UUID_HEART_RATE_SERVICE,
            GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE,
            charTable,
            sizeof(charTable) / sizeof(charTable[0])
        );

        ble.gattServer().addService(hrmService);
    }

protected:
    struct Team2ValueBytes {
        


        Team2ValueBytes() : pDataXYZ()
        {
            updateTeam2();
        }

        void updateTeam2()
        {
            // if (hrmCounter <= 255) {
            //     valueBytes[FLAGS_BYTE_INDEX] &= ~VALUE_FORMAT_FLAG;
            //     valueBytes[FLAGS_BYTE_INDEX + 1] = hrmCounter;
            // } else {
            //     valueBytes[FLAGS_BYTE_INDEX] |= VALUE_FORMAT_FLAG;
            //     valueBytes[FLAGS_BYTE_INDEX + 1] = (uint8_t)(hrmCounter & 0xFF);
            //     valueBytes[FLAGS_BYTE_INDEX + 2] = (uint8_t)(hrmCounter >> 8);
            // }
            
            BSP_MAGNETO_Init();
            int16_t  pDataXYZ_temp[3];
            BSP_MAGNETO_GetXYZ(pDataXYZ_temp);
            pDataXYZ[0] = (uint8_t)pDataXYZ_temp[0];
            pDataXYZ[1] = (uint8_t)pDataXYZ_temp[1];
            pDataXYZ[2] = (uint8_t)pDataXYZ_temp[2];
            // printf("MAGNETO_X = %d\n", pDataXYZ[0]);
            // printf("MAGNETO_Y = %d\n", pDataXYZ[1]);
            // printf("MAGNETO_Z = %d\n", pDataXYZ[2]);
        }

        uint8_t *getPointer()
        {
            return pDataXYZ;
        }

        const uint8_t *getPointer() const
        {
            return pDataXYZ;
        }

        unsigned getNumValueBytes() const
        {
            return 1 + sizeof(int16_t)*3;
        }

    private:
        uint8_t pDataXYZ[3];
    };

protected:
    BLE &ble;
    Team2ValueBytes pDataXYZ;
    GattCharacteristic hrmRate;
};

#endif // BLE_FEATURE_GATT_SERVER

#endif /* #ifndef MBED_BLE_TEAM_2_SERVICE_H__*/
