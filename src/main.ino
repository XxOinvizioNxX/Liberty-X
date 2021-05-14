/*
 * Copyright (C) 2021 Frey Hertz (Pavel Neshumov), Liberty-X Flight controller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// Constants, variables and config
#include "constants.h"
#include "config.h"
#include "pid.h"
#include "datatypes.h"

// External libraries
// IMPORTANT NOTE: In the "WS2812B" library, SPI.setClockDivider() must be removed from void begin()
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <WS2812B.h>

// External library objects
TwoWire HWire(2, I2C_FAST_MODE);
WS2812B ws_leds = WS2812B(3);

void setup()
{
    // EEPROM setup
    EEPROM.PageBase0 = 0x801F000;
    EEPROM.PageBase1 = 0x801F800;
    EEPROM.PageSize = 0x400;

    // I2C setup
    HWire.begin();

    // Modules setup
    TELEMETRY_SERIAL.begin(TELEMETRY_BAUDRATE);
    delay(250);
    timers_setup();
    gps_setup();
    voltmeter_setup();
    leds_setup();
    imu_setup();
    compass_setup();
    imu_calibrate_gyro();
    barometer_setup();

    // Wait for the reciever
    while (channel_1 < 990 || channel_2 < 990 || channel_3 < 990 || channel_4 < 990) {
        // Stay in the loop because there is no valid signal from the receiver
        error = 4;
        // Show cuurent error
        leds_error_signal();
        // Simulate main loop
        delayMicroseconds(LOOP_PERIOD);
    }

    // Reset errors
    error = 0;

    // Flush serial buffer
    TELEMETRY_SERIAL.flush();

    // Set the initial compass heading
    angle_yaw = actual_compass_heading;

    // Reset loop timer
    loop_timer = micros();
}

void loop()
{
    // Pre-flight calibartions and programming mode
    receiver_pre_flight();

    // Select flight modes with the receiver
    receiver_modes();

    // Liberty-Link
#ifdef LIBERTY_LINK
    // Receive data and fly over GPS and altitude waypoints
    liberty_link();
#endif

    // LEDs
    leds_handler();

    // Read raw IMU data
    imu_read();

    // Read barometer pressure
    barometer_handler();
    // Execute altitude PID controllers
    pid_altitude();

    // Read compass heading
    compass_read();

    // Read raw UBLOX data from the GPS module
    gps_read();
    // Parse UBLOX data and perform manual corrections
    gps_handler();
    // Execute GPS PID controllers
    pid_gps();

    // Calculate angles with the help of acc and gyro
    calculate_angles();
    // Calculate vertical acceleration vector
    vertical_acceleration();
    // Combine all corrections for the PID controller
    channel_collector();
    // Process main PID controllers
    pid_roll_pitch_yaw();

    // Start stop and takeoff from the receiver
    receiver_start_stop();

    // Measure current voltage
    voltmeter();
    // Collect throttle value and ESC outputs
    throttle_and_motors();

    // Adjust variable with the channel_8
    in_flight_adjuster();

    // Camera gimbal
    gimbal();

    // Telemetry
#ifdef TELEMETRY
#ifdef LIBERTY_LINK
    // Send telemetry if Liberty-Link not allowed, or if link_command = 8
    if (!link_allowed || link_telemetry_allowed) {
        telemetry();
        link_telemetry_allowed = 0;
    }
#else
    telemetry();
#endif
#endif

    // Debugger
#ifdef DEBUGGER
    debugger();
#endif

    // Check loop time
    if (micros() - loop_timer > MAX_ALLOWED_LOOP_PERIOD)
        error = 2;
    while (micros() - loop_timer < LOOP_PERIOD);
    loop_timer = micros();
}
