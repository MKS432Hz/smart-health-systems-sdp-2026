from flask import Flask, jsonify
import serial
import threading

# Flask app setup
app = Flask(__name__)

# Arduino serial port configuration
serial_port = 'COM9'  # Replace with your port
baud_rate = 9600       # Baud rate for Arduino communication

# Attempt to connect to the serial port
ser = None
try:
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    print(f"Connected to {serial_port}")
except Exception as e:
    print(f"Error connecting to {serial_port}: {e}")

# Shared variable to store the latest sensor data (as a list of floats)
latest_data = [0.0]  # Initialize with one value (most recent sensor reading)

# Home route
@app.route('/', methods=['GET'])
def home():
    return "Server is running. Use /data to fetch sensor data."

# Data route to fetch the latest sensor data as JSON
@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(latest_data)  # Return the latest data as JSON

# Function to read data from the Arduino
def read_serial():
    global latest_data
    if ser is None:
        print("Serial port not available. Exiting serial reader.")
        return

    while True:
        try:
            line = ser.readline().decode('utf-8').strip()
            if line:
                print("Raw data from Arduino:", line)  # Debugging output
                # Convert the value to a float
                try:
                    value = float(line)
                    latest_data[0] = value  # Update with the new value
                except ValueError:
                    print(f"Invalid data received: {line}")
        except serial.SerialException as e:
            print("Serial disconnected:", e)
            break  # Exit the loop if there's a serial exception
        except Exception as e:
            print("Error reading from serial:", e)

# Run Flask app and serial reader in parallel using threading
if __name__ == '__main__':
    # Start the serial reading in a separate thread
    threading.Thread(target=read_serial, daemon=True).start()
    # Run Flask server
    app.run(host='0.0.0.0', port=8080)  # Expose on all network interfaces
