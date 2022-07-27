/*
 * Copyright 2016 <Admobilize>
 * MATRIX Labs  [http://creator.matrix.one]
 * This file is part of MATRIX Creator HAL
 *
 * MATRIX Creator HAL is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Last edited July 2022 by Talha Rao

// To COMPILE
// g++ -o combinedSensorsNFC combinedSensorsNFC.cpp -std=c++11 -DNXPBUILD__PH_RASPBERRY_PI -I/usr/local/include/matrix_nfc/nxp_nfc/NxpNfcRdLib/types -I/usr/local/include/matrix_nfc/nxp_nfc/NxpNfcRdLib/intfs -lmatrix_hal_nfc -lmatrix_creator_hal -I/usr/include/python3.7 -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -L/usr/lib -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions
// to finish program "ctrl z" then "ps" then kill "kill -9 PID#"

#include <unistd.h>
#include <cstdlib>
#include "/usr/include/python3.7/Python.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

// For using the Everloop
#include "matrix_hal/everloop.h"
#include "matrix_hal/everloop_image.h"
#include "matrix_hal/matrixio_bus.h"

// For using NFC
#include "matrix_nfc/nfc.h"
#include "matrix_nfc/nfc_data.h"

// For use of sensors
#include "../../cpp/driver/fw_data.h"
#include "../../cpp/driver/humidity_data.h"
#include "../../cpp/driver/humidity_sensor.h"
#include "../../cpp/driver/imu_data.h"
#include "../../cpp/driver/imu_sensor.h"
#include "../../cpp/driver/matrixio_bus.h"
#include "../../cpp/driver/mcu_firmware.h"
#include "../../cpp/driver/pressure_data.h"
#include "../../cpp/driver/pressure_sensor.h"
#include "../../cpp/driver/uv_data.h"
#include "../../cpp/driver/uv_sensor.h"

namespace hal = matrix_hal; // setting name space for compatibility
using namespace std;		// assuming std name space

int main()
{
	hal::MatrixIOBus bus; // Setting the matrix bus
	if (!bus.Init())
	{ // If matrix isnt detected return error
		return false;
	}
	hal::IMUSensor imu_sensor; // Sensor bus
	imu_sensor.Setup(&bus);

	hal::PressureSensor pressure_sensor; // Sensor bus
	pressure_sensor.Setup(&bus);

	hal::HumiditySensor humidity_sensor; // Sensor bus
	humidity_sensor.Setup(&bus);

	hal::UVSensor uv_sensor; // Sensor bus
	uv_sensor.Setup(&bus);

	hal::MCUFirmware mcu_firmware; // Sensor bus
	mcu_firmware.Setup(&bus);

	// Getting data
	hal::IMUData imu_data;
	hal::PressureData pressure_data;
	hal::HumidityData humidity_data;
	hal::UVData uv_data;
	hal::MCUData mcu_data;

	// Setting up Everloop
	hal::EverloopImage everloop_image(bus.MatrixLeds());
	hal::Everloop everloop;
	everloop.Setup(&bus);

	// Setting up NFC
	hal::NFC nfc;
	hal::NFCData nfc_data;

	/////////////////
	// MAIN CODE ///
	////////////////

	char filename[] = "combinedSensorsNFC.py"; // Setting up file reading
	FILE *fp;								   // Creating a file object
	Py_Initialize();						   // Intializing python support

	std::cout << "Program ready to scan a tag" << std::endl;

	while(true)
	{
		// Scan NFC tag and read info into nfc_data.info
		nfc.Activate();
		nfc.ReadInfo(&nfc_data.info); // Checking for nfc data to register an nfc as near
		nfc.Deactivate();

		// Output tag info and set everloop to green only if tag was detected,
		// else set everloop to off.
		if (nfc_data.info.recently_updated) // If a tag was recently detected
		{
			// std::cout << nfc_data.info.ToString() << std::endl
			//		  << std::endl;
			for (matrix_hal::LedValue &led : everloop_image.leds)
			{
				led.red = 2 0;
				led.green = 0;
				led.blue = 0;
				led.white = 0;
			}

			// Getting data
			mcu_firmware.Read(&mcu_data);
			imu_sensor.Read(&imu_data);
			pressure_sensor.Read(&pressure_data);
			humidity_sensor.Read(&humidity_data);
			uv_sensor.Read(&uv_data);

			// std::system("clear");

			std::cout << "\n"
					  << std::endl;
			std::cout << "The yaw is " << imu_data.yaw << "°\t";
			std::cout << "The roll is " << imu_data.roll << "°\t";
			std::cout << "The pitch is " << imu_data.pitch << "°" << std::endl;
			std::cout << "The x acceleration is " << imu_data.accel_x << ", the y acceleration is " << imu_data.accel_y
					  << ", the z acceleration is " << imu_data.accel_z << "" << std::endl
					  << std::endl;

			std::cout << "The humidity is " << humidity_data.humidity << " %" << std::endl;
			std::cout << "The temperature (from humidity sensor) is "
					  << humidity_data.temperature << " °C" << std::endl
					  << std::endl;

			std::cout << "The pressure is " << pressure_data.pressure << " kPa" << std::endl;
			std::cout << "The altitude is " << pressure_data.altitude << " meters"
					  << std::endl;
			std::cout << "The temperature (from altimeter) is " << pressure_data.temperature
					  << " °C" << std::endl
					  << std::endl;
			std::cout << "The UV index is " << uv_data.uv << std::endl
					  << std::endl;

			// OPENING AND PRINTING TO THE FILE
			ofstream myfile("TextForAudio.txt");
			if (myfile.is_open())
			{
				myfile << "yaw " << imu_data.yaw << " degrees" << endl;
				myfile << "roll " << imu_data.roll << " degrees" << endl;
				myfile << "pitch " << imu_data.pitch << " degrees" << endl;

				myfile << "x acceleration " << imu_data.accel_x << endl;
				myfile << "y acceleration " << imu_data.accel_y << endl;
				myfile << "z acceleration " << imu_data.accel_z << endl;

				myfile << "humidity " << humidity_data.humidity << " percent" << endl;
				myfile << "humidity sensor temperature " << humidity_data.temperature << " degrees celcius" << endl;

				myfile << "pressure " << pressure_data.pressure << " kilo pascals" << endl;
				myfile << "altitude " << pressure_data.altitude << " meters" << endl;
				myfile << "altimeter temperature " << pressure_data.temperature << " degrees celcius" << endl;
				myfile << "UV index" << uv_data.uv << endl;

				myfile.close(); // Close file to prevent leaks
			}
			else
			{
				cout << "Unable to open file";
			}
			//    std::cout << "MCU ID = 0x" << std::hex << mcu_data.ID << std::endl;
			//    std::cout << "MCU version = 0x" << mcu_data.version << std::endl
			//              << std::endl;

			//    usleep(1000000);
			// Update the Everloop
			everloop.Write(&everloop_image);

			fp = _Py_fopen(filename, "r");	// Open python file through python
			PyRun_SimpleFile(fp, filename); // Run the file
			rewind(fp);						// Reset file pointer back to beginning

			// Sleep for a reasonable amount of time
			std::this_thread::sleep_for(std::chrono::microseconds(1000000));
		}
		else
		{
			for (matrix_hal::LedValue &led : everloop_image.leds)
			{
				led.red = 0;
				led.green = 20;
				led.blue = 0;
				led.white = 0;
				everloop.Write(&everloop_image);
			}
		}

	}

	Py_Finalize(); // Close python. Will never get to here

	return 0;
}

// to finish program "ctrl z" then "ps" then kill "kill -9 PID#"

// A helper function for writing NDEF to tag
/*		int NDEF_WriteToNewTag(matrix_hal::NFC &nfc, matrix_hal::NFCData &nfc_data) {
			std::cout << "Press enter to write" << std::endl;

			// Loop until enter pressed
			char temp = 'x';
			while (temp != '\n') std::cin.get(temp);

			// Create an NDEFParser object and add a website NDEF record to the parser.
			matrix_hal::NDEFParser ndef_parser = matrix_hal::NDEFParser();
			ndef_parser.AddUriRecord("http://docs.matrix.one");
			std::cout << "Message Info:" << std::endl;
			// Output the NDEF message information from the NDEFParser.
			std::cout << ndef_parser.ToString() << std::endl;

			// Write the NDEF message from the NDEFParser to a NFC tag.
			nfc.Activate();
			nfc.ndef.Write(&ndef_parser);
			nfc.Deactivate();

			std::cout << "Wrote to new tag, try scanning tag with Android phone."
					<< std::endl;
			return 0;
		}
*/
/*while (true) {
	mcu_firmware.Read(&mcu_data);
	imu_sensor.Read(&imu_data);
	pressure_sensor.Read(&pressure_data);
	humidity_sensor.Read(&humidity_data);
	uv_sensor.Read(&uv_data);

	std::system("clear");

	std::cout << "\n" << std::endl;
	std::cout << "yaw = " << imu_data.yaw << "°\t";
	std::cout << "roll = " << imu_data.roll << "°\t";
	std::cout << "pitch = " << imu_data.pitch << "°" << std::endl;
	std::cout << "accel = {" << imu_data.accel_x << ", " << imu_data.accel_y
			  << "," << imu_data.accel_z << "}" << std::endl
			  << std::endl;

	std::cout << "humidity = " << humidity_data.humidity << " %" << std::endl;
	std::cout << "temperature (from humidity sensor) = "
			  << humidity_data.temperature << " °C" << std::endl
			  << std::endl;

	std::cout << "pressure = " << pressure_data.pressure << " kPa" << std::endl;
	std::cout << "altitude = " << pressure_data.altitude << " meters"
			  << std::endl;
	std::cout << "temperature (from altimeter) = " << pressure_data.temperature
			  << " °C" << std::endl
			  << std::endl;
	std::cout << "UV = " << uv_data.uv << std::endl << std::endl;

//    std::cout << "MCU ID = 0x" << std::hex << mcu_data.ID << std::endl;
//    std::cout << "MCU version = 0x" << mcu_data.version << std::endl
//              << std::endl;

//    usleep(1000000);


			// Scan NFC tag and read info into nfc_data.info
			nfc.Activate();
			nfc.ReadInfo(&nfc_data.info);
			nfc.Deactivate();

			// Output tag info and set everloop to green only if tag was detected,
			// else set everloop to off.
			if (nfc_data.info.recently_updated) {
					std::cout << nfc_data.info.ToString() << std::endl << std::endl;
					for (matrix_hal::LedValue &led : everloop_image.leds) {
						led.red = 0;
						led.green = 20;
						led.blue = 0;
						led.white = 0;
					}
			} else {
					for (matrix_hal::LedValue &led : everloop_image.leds) {
						led.red = 0;
						led.green = 0;
						led.blue = 0;
						led.white = 0;
				 }
			}

			// Update the Everloop
			everloop.Write(&everloop_image);

			// Sleep for a reasonable amount of time
			std::this_thread::sleep_for(std::chrono::microseconds(10000));

			usleep(1000000);

  }

  return 0;
}
*/
