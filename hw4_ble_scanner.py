from bluepy.btle import Peripheral, UUID, Scanner, DefaultDelegate
import struct

TARGET_DEVICE_NAME = "Team02_BLE"
TARGET_DEVICE_ADDRESS = "ee:78:d5:9c:5d:eb"

HEARTRATE_SERVICE_UUID = 0x180D
HEARTRATE_CHARACTERISTIC_UUID = 0x2A37
MAGNETOMETER_SERVICE_UUID = 0x1812
MAGNETOMETER_CHARACTERISTIC_UUID = 0x1812

NOTIFY_SETUP = b"\x01\x00"


class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        # if isNewDev:
        #     print ("Discovered device", dev.addr)
        # elif isNewData:
        #     print ("Received new data from", dev.addr, )
        pass


print("Searching for device with name {} in 5 sec...".format(TARGET_DEVICE_NAME))
scanner = Scanner().withDelegate(ScanDelegate())
scanner.scan(5.0)
devices = list(scanner.getDevices())

target_devices = []
for dev in devices:
    if dev.addr == TARGET_DEVICE_ADDRESS or dev.getValueText(0x09) == TARGET_DEVICE_NAME:
        target_devices.append(dev)

if len(target_devices) == 0:
    print("No match for known device name or address. Showing unmatched candidates.")
    target_devices = devices

for idx, dev in enumerate(target_devices):
    print("{: <3d}: {} ({}), RSSI={} dB, {: 2d} services. NAME={}".format(
        idx, dev.addr, dev.addrType, dev.rssi, len(dev.getScanData()), dev.getValueText(0x09)))
    for (adtype, desc, value) in dev.getScanData():
        print("    {: <2d}, {} = {}".format(adtype, desc, value))

if len(target_devices) == 1:
    print("Selecting the only candidate automatically...", end="")
    dev = Peripheral(target_devices[0])
    print("Connected")
else:
    number = int(input('Enter your device number: '))
    print("Connecting device {}...".format(number), end="")
    dev = Peripheral(devices[num])
    print("Connected")


class DeviceDelegate(DefaultDelegate):
    def __init__(self, heartrateHandleNum, magnetometerHandleNum):
        DefaultDelegate.__init__(self)
        self.heartrateHandleNum = heartrateHandleNum
        self.magnetometerHandleNum = magnetometerHandleNum
        self.heartrate = 0
        self.magX = 0
        self.magY = 0
        self.magZ = 0

    def handleNotification(self, cHandle, data):
        if cHandle == self.heartrateHandleNum:
            # print("heart rate: {: 3d} bpm".format(int.from_bytes(data, byteorder='big')))
            self.heartrate = int.from_bytes(data, byteorder='big')
        elif cHandle == self.magnetometerHandleNum:
            # print("magX: {: 3d}, magY: {: 3d}, magZ: {: 3d}".format(data[0], data[1], data[2]))
            # buffer = struct.unpack("BBB", data[0, 3])
            self.magX = data[0]
            self.magY = data[1]
            self.magZ = data[2]


try:
    heartrateService = dev.getServiceByUUID(HEARTRATE_SERVICE_UUID)
    heartrateMeasureCh = heartrateService.getCharacteristics(
        HEARTRATE_CHARACTERISTIC_UUID)[0]
    heartrateHandle = heartrateMeasureCh.getHandle()
    heartrateCCCDHandle = heartrateHandle + 1
    dev.writeCharacteristic(heartrateCCCDHandle,
                            NOTIFY_SETUP, withResponse=True)

    magnetometerService = dev.getServiceByUUID(MAGNETOMETER_SERVICE_UUID)
    magnetometerMeasureCh = magnetometerService.getCharacteristics(
        MAGNETOMETER_CHARACTERISTIC_UUID)[0]
    magnetometerHandle = magnetometerMeasureCh.getHandle()
    magnetometerCCCDHandle = magnetometerHandle + 1
    dev.writeCharacteristic(magnetometerCCCDHandle,
                            NOTIFY_SETUP, withResponse=True)

    deviceDelegate = DeviceDelegate(heartrateHandle, magnetometerHandle)
    dev.withDelegate(deviceDelegate)
    print()

    while True:
        if dev.waitForNotifications(5.0):
            print("\rHeartrate: {: >3d} bpm, Magnetometer: ({: >3d}, {: >3d}, {: >3d})"
                  .format(deviceDelegate.heartrate, deviceDelegate.magX,
                          deviceDelegate.magY, deviceDelegate.magZ), end="")
        else:
            print("\nNo any notification in 5 sec. Quitting...")
            break

finally:
    dev.disconnect()
