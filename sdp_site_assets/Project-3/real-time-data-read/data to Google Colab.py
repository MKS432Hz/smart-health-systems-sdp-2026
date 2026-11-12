import serial
from pycon import PyCon

# Initialize serial connection
arduino_port = "COM6"  # Replace with your Arduino port (e.g., "/dev/ttyUSB0" on Linux)
baud_rate = 9600
ser = serial.Serial(arduino_port, baud_rate, timeout=1)

# PyCon setup
pycon = PyCon(port=8000)  # Starts a PyCon server on port 8000

@pycon.expose
def get_sensor_data():
    """
    Read data from Arduino and return it.
    """
    if ser.in_waiting > 0:
        data = ser.readline().decode("utf-8").strip()
        return data
    return "No data available"

if __name__ == "__main__":
    print("Starting PyCon server...")
    pycon.start()
