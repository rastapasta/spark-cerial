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
var spark = require('sparknode'),
	fs = require('fs');

var config = process.env['HOME']+'/.spark/spark.config.json';
var core;

// Try to read from spark-cli configuration file and check ARGV
if (fs.existsSync(config)) {
	var json = JSON.parse(fs.readFileSync(config, 'utf8'));
	var accessToken = json.access_token;

	if (process.argv.length < 3) {
		console.log('Usage: '+process.argv[1]+' [coreId]');
		process.exit(0);		
	}
} else {
	if (process.argv.length != 4) {
		console.log('Usage: '+process.argv[1]+' [coreId] [accessToken]');
		process.exit(0);
	}
	var accessToken = process.argv[3];
}
var coreId = process.argv[2];

// Try to authenticate with the Spark Cloud
try {
	core = new spark.Core({
		accessToken: accessToken,
		id: coreId
	});
} catch(e) {
	console.log("Cloud connection failed... check your access token!");
	process.exit(0);
}

// Wait for the core to be connected..
core.on('connect', function(){
	console.log('Succesfully connected to the Spark Cloud. Starting cerial monitor...');
	requestLoop();
});

// And start our pull loop!
var lastPointer = 0;
function requestLoop() {
	core.cerialBuffer(function(err, data) {
		if (err)
			return console.log(err);

		if (data) {
			var pointer = parseInt(data.substr(0,3));
			var serial = data.substr(4);
			
			// Print what we got!
			process.stdout.write( pointer > lastPointer ?
				serial.substr(lastPointer, pointer-lastPointer) :
				serial.substr(lastPointer) + serial.substr(0, pointer)
			);

			lastPointer = pointer;
		}
		requestLoop();
	});	
}