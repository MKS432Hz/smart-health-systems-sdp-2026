from flask import Flask, jsonify
import serial
import threading

# Flask app setup
app = Flask(__name__)

# Arduino serial port configuration
serial_port = 'COM6'  # Change as needed
baud_rate = 9600

# Initialize serial connection
try:
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    print(f"Connected to {serial_port}")
except Exception as e:
    print(f"Error connecting to {serial_port}: {e}")
    ser = None

# Lock for thread safety
data_lock = threading.Lock()

# Shared variable to store the latest sensor data
latest_data = [0.0] * 8  # Initialize with 8 zero values

@app.route('/', methods=['GET'])
def home():
    return "Server is running. Use /data to fetch sensor data."

@app.route('/data', methods=['GET'])
def get_data():
    with data_lock:
        print("Serving data:", latest_data)  # Debugging output
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
            values = list(map(float, line.split(',')))  # Convert to float

            if len(values) == 8:  # Ensure correct number of values
                with data_lock:
                    latest_data = values  # Safely update shared data
                    print("Updated latest_data:", latest_data)

        except ValueError as e:
            print(f"Invalid data format: {line} - Error: {e}")
        except Exception as e:
            print("Error reading from serial:", e)

# Run Flask app and serial reading in parallel
if __name__ == '__main__':
    threading.Thread(target=read_serial, daemon=True).start()
    app.run(host='0.0.0.0', port=8080)  # Allow external access
