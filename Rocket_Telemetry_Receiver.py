import numpy as np
import matplotlib.pyplot as plt
import serial

# Graphs incoming serial data from Rocket_Telemetry_Transmitter.ino
# port = Arduino UNO port
# baud_rate = Arduino baud_rate
def plotSerial(port, baud_rate):
    # Serial initialization
    try:
        device = serial.Serial(port, baud_rate, timeout=.1)
    except Exception as error:
        print(error)
        return 0

    # Data initialization
    pitot = ["", -1]
    alt = ["", -1]
    acc = ["", -1]
    pitot_y = np.array([0])
    alt_y = np.array([0])
    acc_y = np.array([0])
    x = np.array([0])
    timer = 0

    # Graph initialization
    plt.ion()
    fig = plt.figure(figsize=(12, 12))
    pitot_plot = fig.add_subplot(2, 2, 1)
    plt.title("Pitot tube")
    plt.xlabel("Time(s)")
    plt.ylabel("Airspeed (ms)")

    alt_plot = fig.add_subplot(2, 2, (3, 4))
    plt.title("Altitude")
    plt.xlabel("Time(s)")
    plt.ylabel("Height (ft)")

    acc_plot = fig.add_subplot(2, 2, 2)
    plt.title("Accelerometer")
    plt.xlabel("Time(s)")
    plt.ylabel("Acc (m/s^2)")

    try:
        while True:
            if device.in_waiting > 18:
                # Read data until newline
                data = device.readline().decode('utf-8').split(" ")

                # Verify starting character
                if data[0][0] == "P":
                    pitot = data[0].split(":")
                    alt = data[1].split(":")
                    acc = data[2].split(":")

                # Check sensor data
                # index 0 = label, index 1 = value
                if pitot[0] == "Pi":
                    pitot_y = np.append(pitot_y, float(pitot[1]))
                else:
                    print("Pitot value not found", pitot)
                    pitot_y = np.append(pitot_y, pitot_y[-1])

                if alt[0] == "Al":
                    alt_y = np.append(alt_y, float(alt[1]))
                else:
                    print("Alt value not found", alt)
                    alt_y = np.append(alt_y, alt_y[-1])

                if acc[0] == "Ac":
                    acc_y = np.append(acc_y, float(acc[1]))
                else:
                    print("Acc value not found", acc)
                    acc_y = np.append(acc_y, acc_y[-1])

                # Sensor values are transmitted every second
                timer += 1
                x = np.append(x, timer)

            pitot_plot.plot(x, pitot_y, color="blue")
            alt_plot.plot(x, alt_y, color="green")
            acc_plot.plot(x, acc_y, color="red")

            fig.canvas.draw()
            fig.canvas.flush_events()

    except KeyboardInterrupt:
        return 0

def getData(port, baud_rate):
    # serial initialization
    try:
        device = serial.Serial(port, baud_rate, timeout=.1)
    except Exception as error:
        print(error)
        return 0

    timer = 0
    try:
        while True:
            if device.in_waiting > 20:
                data = device.readall().decode('utf-8').split(" ")
                pitot = data[0].split(":")
                alt = data[1].split(":")
                acc = data[2].split(":")

                if pitot[0] == "Pitot":
                    print(pitot)
                if alt[0] == "Alt":
                    print(alt)
                if acc[0] == "Acc":
                    print(acc)
                timer += 1
                print(timer)

    except KeyboardInterrupt:
        return 0

plotSerial("COM3", 9600)
