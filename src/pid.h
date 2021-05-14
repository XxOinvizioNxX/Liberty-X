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

#ifndef PID_H
#define PID_H

/******************************/
/*            Roll            */
/******************************/
// Roll P-controller (default = 1.3)
const float PID_ROLL_P PROGMEM = 3.9;

// Roll I-controller (default = 0.04)
const float PID_ROLL_I PROGMEM = 0.087;

// Roll D-controller (default = 18.0)
const float PID_ROLL_D PROGMEM = 47.0;

// Maximum output of the PID - controller (+ / -)
const float PID_ROLL_MAX PROGMEM = 400;


/*******************************/
/*            Pitch            */
/*******************************/
// Pitch P-controller (default = 1.3)
const float PID_PITCH_P PROGMEM = 3.9;

// Pitch I-controller (default = 0.04)
const float PID_PITCH_I PROGMEM = 0.087;

// Pitch D-controller (default = 18.0)
const float PID_PITCH_D PROGMEM = 47.0;

// Maximum output of the PID - controller (+ / -)
const float PID_PITCH_MAX PROGMEM = 400;


/*****************************/
/*            Yaw            */
/*****************************/
// Yaw P-controller (default = 4.0)
const float PID_YAW_P PROGMEM = 15.2;

// Yaw I-controller (default = 0.02)
const float PID_YAW_I PROGMEM = 0.2;

// Yaw D-controller (default = 0.0)
const float PID_YAW_D PROGMEM = 0.0;

// Maximum output of the PID - controller (+ / -)
const float PID_YAW_MAX PROGMEM = 400;


/**********************************/
/*            Altitude            */
/**********************************/
// Altitude P-controller (default = 1.4)
const float PID_ALT_P PROGMEM = 1.8;

// Altitude I-controller (default = 0.002)
const float PID_ALT_I PROGMEM = 0.0014;

// Altitude D-controller (default = 7.5)
const float PID_ALT_D PROGMEM = 14.0;

// Maximum output of the PID - controller (+ / -)
const float PID_ALT_MAX PROGMEM = 200;


/*****************************/
/*            GPS            */
/*****************************/
// GPS P-controller (default = 2.7)
const float PID_GPS_P PROGMEM = 1.8;

// GPS rotating-memory D-controller (default = 6.5)
const float PID_GPS_D PROGMEM = 12.0;

// Maximum output of the PID - controller (+ / -)
const float PID_GPS_MAX PROGMEM = 300;

#endif