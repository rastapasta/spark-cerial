/**
 * Copyright 2014  Michael Straßburger
 * http://github.com/rastapasta/spark-cerial
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef Cerial_h
#define Cerial_h
#include <math.h>
#include <spark_wiring_stream.h>

// import itoa
extern char* itoa(int a, char* buffer, unsigned char radix);

class Cerialize : public Stream
{
	private:
		// Our output + input buffers
		// Ouput: FIFO, circular
		// Input: FIFO, max sized
		uint8_t buffer[622];
		uint8_t bufferIn[620];

		// Circular position and input buffer counter
		uint16_t position = 0;
		uint16_t countIn = 0;

	public:
		Cerialize();

		// Overwrite the core Print function
		size_t write(uint8_t);
		
		// Overwrite all core Stream functions
		int available();
		int read();
		int peek();		
		void flush();

		// Emulate the Serial functions
		void begin();
		void end();

		// Offer an input backstream for a cloud call
		int input(String &input);
};
#endif