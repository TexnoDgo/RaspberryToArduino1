#!/usr/bin/env python3

"""
Get code:
T1 -
T2 -
T3 -
O1 -
02 -
V1 -
V2 -
M1 -
M2 -
M3 -
M4 -
"""

import serial
import sys
import time

# Input arguments
step_count = int(sys.argv[1])
electrical_test = sys.argv[2]
load_time = int(sys.argv[3])
idle_mode = sys.argv[4]


# Arguments check
def input_arg_check(step_count, electrical_test, load_time, idle_mode):
    try:
        if 0 < step_count < 1000:
            print("step_count argument is OK")
        else:
            print("step_count argument is not OK")

        if electrical_test == 'yes' or electrical_test == 'Yes' or electrical_test == 'YES' \
                or electrical_test == 'no' or electrical_test == 'No' or electrical_test == 'NO' \
                or electrical_test == 'not' or electrical_test == 'Not' or electrical_test == 'NOT':
            print("electrical_test argument is OK")
        else:
            print("electrical_test argument is not OK")

        if 0 < load_time < 2000:
            print("load_time argument is OK")
        else:
            print("load_time argument is not OK")

        if idle_mode == 'yes' or idle_mode == 'Yes' or idle_mode == 'YES' \
                or idle_mode == 'no' or idle_mode == 'No' or idle_mode == 'NO' \
                or idle_mode == 'not' or idle_mode == 'Not' or idle_mode == 'NOT':
            print("electrical_test argument is OK")
        else:
            print("electrical_test argument is not OK")
    except KeyboardInterrupt:
        print("Error")


input_arg_check(step_count, electrical_test, load_time, idle_mode)

step_count_line = "A1-{0}".format(step_count)
step_count_line = step_count_line.encode()
print(step_count_line)

electrical_test_line = "B1-{0}".format(electrical_test)
electrical_test_line = electrical_test_line.encode()
print(electrical_test_line)

load_time_line = "C1-{0}".format(load_time)
load_time_line = load_time_line.encode()
print(load_time_line)

idle_mode_line = "D1-{0}".format(idle_mode)
idle_mode_line = idle_mode_line.encode()
print(idle_mode_line)

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()

    ser.write(step_count_line)
    ser.write(electrical_test_line)
    ser.write(load_time_line)
    ser.write(idle_mode_line)
    while True:
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        time.sleep(1)

