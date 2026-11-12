from flask import Flask, jsonify
import serial
import threading

# Flask app setup
app = Flask(__name__)

# Arduino serial port configuration
serial_port = 'COM6'
baud_rate = 115200

try:
    ser = serial.Serial(serial_port, baud_rate)
    print(f"Connected to {serial_port}")
except Exception as e:
    print(f"Error connecting to {serial_port}: {e}")
    ser = None

# Shared variable to store the latest sensor data
latest_data = [0] * 8  # Initialize with 8 channels of zero

@app.route('/', methods=['GET'])
def home():
    return "Server is running. Use /data to fetch sensor data."

@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(latest_data)  # Return the latest data as JSON

def read_serial():
    global latest_data
    if ser is None:
        print("Serial port not available. Exiting serial reader.")
        return

    while True:
        try:
            line = ser.readline().decode('utf-8').strip()
            print("Raw data from Arduino:", line)  # Debugging output
            values = list(map(int, line.split(',')))
            if len(values) == 8:  # Ensure data integrity
                latest_data = values
        except Exception as e:
            print("Error reading from serial:", e)

# Run Flask app and serial reading in parallel
if __name__ == '__main__':
    threading.Thread(target=read_serial, daemon=True).start()
    app.run(host='127.0.0.1', port=8080)  # Expose on all interfaces