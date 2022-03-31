# ble_scan_connect.py:
from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate
import struct
class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)
    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print ("Discovered device", dev.addr)
        elif isNewData:
            print ("Received new data from", dev.addr)
scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan(10.0)
n=0
addr = []
for dev in devices:
    if dev.addr == "ee:78:d5:9c:5d:eb":
        print ("%d: Device %s (%s), RSSI=%d dB" % (n, dev.addr, 
        dev.addrType, dev.rssi))
        for (adtype, desc, value) in dev.getScanData():
            print (" %s = %s" % (desc, value))
    addr.append(dev.addr)
    n += 1
    
number = input('Enter your device number: ')
print ('Device', number)
num = int(number)
print (addr[num])
#
print ("Connecting...")
dev = Peripheral(addr[num], 'random')
#
print ("Services...")
for svc in dev.services:
    print (str(svc))
#
import struct
from bluepy.btle import *


class MyDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        if cHandle == 13:
            print(f"heartrate: {int.from_bytes(data, byteorder='big')} bpm")
        elif cHandle == 19:
            # print(data)
            # print(data[0])
            print(f"magX: {data[0]}")
            print(f"magY: {data[1]}")
            print(f"magZ: {data[2]}")
            
            # print(len(data))
            # buffer = struct.unpack("BBB", data)
            # print(f"X: {buffer[0]}")
            # print(f"Y: {buffer[1]}")
            # print(f"Z: {buffer[2]}")




try:
    # testService = dev.getServiceByUUID(UUID(0xfff0))
    # for ch in testService.getCharacteristics():
    #     print (str(ch))

    # ch = dev.getCharacteristics(uuid=UUID(0xfff1))[0]
    # if (ch.supportsRead()):
    #     print (ch.read())

    dev.setDelegate(MyDelegate())

    # print("Connected")
    for idx, i in enumerate(dev.getDescriptors()):
        print(f"{idx} UUID: {i.uuid}")

    notify_setup = b"\x01\x00"
    # service1 = dev.getServiceByUUID(UUID(0x180D))
    ch1 = dev.getCharacteristics(uuid=UUID(0x2A37))[0]
    handle1 = ch1.getHandle()
    handle1 += 1
    print(f"handle: {handle1}")
    dev.writeCharacteristic(handle1, notify_setup, withResponse=True)
    # for ch in service1.getCharacteristics():
    #     print (str(ch), ch.supportsRead())
    #     if(ch.supportsRead()):
    #         handle = ch.getHandle()
    #         print(dev.readCharacteristic(handle))

    # service2 = dev.getServiceByUUID(UUID(0x1812))
    ch2 = dev.getCharacteristics(uuid=UUID(0x1812))[0]
    handle2 = ch2.getHandle()
    handle2 += 1
    dev.writeCharacteristic(handle2, notify_setup, withResponse=True)
    # for ch in service2.getCharacteristics():
    #     print (str(ch), ch.supportsRead())
    #     handle = ch.getHandle()
    #     dev.writeCharacteristic(handle, notify_setup, withResponse=True)
    # ch = ch + 2
    
    
    # handle += 2
    # print(f"the handle is: {handle}")
    # if (ch.supportsRead()):
    #     print (ch.read())
    
    # notify_setup = b"\x02\x00"
    # dev.writeCharacteristic(handle, notify_setup, withResponse=True)
    # print("writing done")
    while True:
        if dev.waitForNotifications(1.0):
            print("notifications")
    #         print("heart rate:")
    #         dev.readCharacteristic(handle1)
    #         print("magnetometer sensor:")
    #         dev.readCharacteristic(handle2)
    #         break
    
    # for i in dev.getDescriptors():
    #     print(f"UUID: {i.uuid}")


finally:
    dev.disconnect() 
#




