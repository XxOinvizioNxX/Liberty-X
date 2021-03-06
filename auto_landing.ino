/*
 * Copyright (C) 2022 Fern Lane, Liberty-X Flight controller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * The Liberty-X project started as a fork of the YMFC-32 project by Joop Brokking
 *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * IT IS STRICTLY PROHIBITED TO USE THE PROJECT (OR PARTS OF THE PROJECT / CODE)
 * FOR MILITARY PURPOSES. ALSO, IT IS STRICTLY PROHIBITED TO USE THE PROJECT (OR PARTS OF THE PROJECT / CODE)
 * FOR ANY PURPOSE THAT MAY LEAD TO INJURY, HUMAN, ANIMAL OR ENVIRONMENTAL DAMAGE.
 * ALSO, IT IS PROHIBITED TO USE THE PROJECT (OR PARTS OF THE PROJECT / CODE) FOR ANY PURPOSE THAT
 * VIOLATES INTERNATIONAL HUMAN RIGHTS OR HUMAN FREEDOM.
 * BY USING THE PROJECT (OR PART OF THE PROJECT / CODE) YOU AGREE TO ALL OF THE ABOVE RULES.
 */

/// <summary>
/// Reduces the drone's altitude setpoint. As soon as the pressure stops changing, turns off the motors
/// Set auto_landing_step to 1 to begin auto-landing sequence
/// </summary>
/// <param name=""></param>
void auto_landing(void) {

    // Step 1. Set current gps position as setpoint
    if (auto_landing_step == 1) {
        // Set setpoints and waypoints
        l_lat_setpoint = l_lat_gps;
        l_lon_setpoint = l_lon_gps;
        l_lat_waypoint = l_lat_gps;
        l_lon_waypoint = l_lon_gps;

        // Reset GPS PID
        pid_gps_reset();

        // Switch to altitude reduction
        auto_landing_step = 2;
    }

    // Step 2. Altitude reduction for auto-landing
	if (auto_landing_step == 2) {
        // Turn off motors if current altitude stops decreasing
        if (pid_alt_setpoint > actual_pressure + 100)
            auto_landing_step = 3;

        // Set current flight mode to GPS or altitude stabilization
        if (link_allowed)
            flight_mode = 3;
        else if (flight_mode < 2)
            flight_mode = 2;

        // Increase pressure (decrease altitude)
        pid_alt_setpoint += AUTO_LANDING_ALTITUDE_TERM;
	}

    // Step 3. Turn off the motors
    else if (auto_landing_step == 3) {
        // Set current flight mode to GPS or altitude stabilization
        if (link_allowed)
            flight_mode = 3;
        else if (flight_mode < 2)
            flight_mode = 2;

        // Turn off the motors and clear variables
        start = 0;
        takeoff_detected = 0;
#ifdef LIBERTY_LINK
        link_clear_disarm();
#endif

        // Reset auto_landing_step
        if (start == 0)
            auto_landing_step = 0;
    }
}
