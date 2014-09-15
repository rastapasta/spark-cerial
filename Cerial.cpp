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

#include "Cerial.h"

Cerialize::Cerialize() {
	// Maker sure our buffer is empty
	memset(&buffer, 0, sizeof(buffer));
}

// Write a byte to the buffer - overflows after set length
size_t Cerialize::write(uint8_t character) {
	// Set the current circular position and write the byte
	buffer[0] = position>>4;
	buffer[1] = position&0xFF;
	buffer[position+2] = character;

	// Increment our circular buffer pointer
	position = ++position % (sizeof(buffer)-2);
	return 1;
}

// Return the next byte of the input buffer
int Cerialize::read() {
	if (!available())
		return -1;

	// Store the byte
	int value = bufferIn[0];

	// FIFO buffer - shift the whole memory one position to the left
	// TODO: maybe more performant to implement rotating index access
	if (--countIn)
		memcpy(&bufferIn[0], &bufferIn[1], countIn);

	return value;
}

// Return the next bye in the input buffer
int Cerialize::peek() {
	return available() ? bufferIn[0] : -1;
}

// How many bytes are waiting in the input buffer?
int Cerialize::available() {
	return countIn;
}

// Handle input the user sends to the Cerial device
int Cerialize::input(String &input) {
	if (input.length()+countIn > sizeof(bufferIn))
		return -1;

	for (byte i=0; i<input.length(); i++)
		bufferIn[countIn++] = input[i];

	return countIn;
}

// Setup the cloud buffer access
void Cerialize::begin() {
	Spark.variable("cerial", buffer, STRING);
	// TODO: would <3 to do a Spark.function("cerial", input) here
	// But terribly fails with following error:
	//		static void function(const char *funcKey, int (*pFunc)(String paramString));
	//		no known conversion for argument 2 from '<unresolved overloaded function type>' to 'int (*)(String)'
}

// No need to support end & flush, here for full Serial compatibility reasons
void Cerialize::end() {}
void Cerialize::flush(){}
